// Copyright (c) 2023 Tom Hancocks
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <iostream>
#include <vector>
#include <thread>
#include <MetalKit/MetalKit.h>
#include <libFoundation/availability.hpp>
#include <libMacOS/cocoa/string.h>
#include <libFoundation/profile/profiler.hpp>
#include <libMetalRenderer/driver/driver.hpp>
#include <libMetalRenderer/cocoa/application.h>
#include <libMetalRenderer/cocoa/view.h>
#include <libMetalRenderer/driver/frame_generator.h>
#include <libMetalRenderer/driver/layer_output.h>
#include <libMetalRenderer/resource/shader/compiler.h>
#include <libMetalRenderer/resource/shader/shader_program.h>
#include <libMetalRenderer/resource/texture.h>
#include <libMetalRenderer/resource/shader/default_shader.hpp>

// MARK: - Metal Driver Context

namespace renderer::metal
{
    struct context
    {
        struct {
            id<MTLDevice> device { nil };
            id<MTLCommandQueue> command_queue { nil };
            math::vec2 viewport_size;
            layer_output layer;
            MTLPixelFormat pixel_format { 0 };

            struct {
                event::receiver *event_receiver { nullptr };
            } tempoary;

            struct {
                renderer::callback frame_request;
                renderer::callback completion;
                std::thread runner;
                bool should_terminate { false };
                frame_generator generator;
            } render;

            struct {
                resource::shader::compiler compiler;
                std::uint64_t default_id { 0 };
                std::unordered_map<std::string, id<MTLLibrary>> libraries;
                std::unordered_map<std::uint32_t, resource::shader::program> programs;
            } shader;

            struct {
                std::uint64_t next_id { 0 };
                std::unordered_map<std::uint64_t, texture> map;
            } textures;
        } metal;

        struct {
            MetalRendererApplication *app { nil };
            NSWindow *default_window { nil };
            NSTimer *runloop_timer { nil };
        } cocoa;

        struct {
            dispatch_source_t source;
            CVDisplayLinkRef link { nil };
            bool tick_received { true };
        } display;
    };
}

// MARK: - Display Link

static auto dispatch_display_render_request(
    CVDisplayLinkRef displayLink,
    const CVTimeStamp *now,
    const CVTimeStamp *outputTime,
    CVOptionFlags flagsIn,
    CVOptionFlags *flagsOut,
    void *displayLinkContext
) -> CVReturn {
    auto source = (__bridge dispatch_source_t)displayLinkContext;
    dispatch_source_merge_data(source, 1);
    return kCVReturnSuccess;
}

// MARK: - Construction

renderer::metal::driver::driver(renderer::display_configuration cfg)
    : m_config(cfg)
{}

// MARK: - API

auto renderer::metal::driver::api_info() -> renderer::api_info
{
    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    if (@available(macOS 10.15, *)) {
        if (![device supportsFamily:MTLGPUFamilyMac2]) {
            return renderer::api_info::unsupported();
        }
    }
    else {
        return renderer::api_info::unsupported();
    }

    return { "Metal", macos::cocoa::string::from(device.name) };
}

auto renderer::metal::driver::api_bindings() -> renderer::api::bindings
{
    api::bindings bindings;
    // Core
    bindings.core.initialize = [&] { initialize(); };
    bindings.core.start = [&] (auto frame_request) { start(std::move(frame_request)); };

    // Configuration
    bindings.configuration.set_viewport_title = [&] (auto title) { set_viewport_title(title); };
    bindings.configuration.set_viewport_size = [&] (auto width, auto height) { set_viewport_size(width, height); };
    bindings.configuration.viewport_title = [&] { return viewport_title(); };
    bindings.configuration.viewport_size = [&] { return viewport_size(); };

    // Frame Generation
    bindings.frame_generation.finish = [&] (auto callback) { end_frame(std::move(callback)); };
    bindings.frame_generation.submit_draw_buffer = [&] (const auto& buffer) { draw(buffer); };

    // Textures
    bindings.texture.create = [&] (const auto& data, auto size) { return create_texture(data, size); };
    bindings.texture.update = [&] (auto id, const auto& data) { update_texture(id, data); };
    bindings.texture.destroy = [&] (auto id) { destroy_texture(id); };

    // Delegation
    bindings.delegate.attach_event_receiver = [&] (auto *receiver) {
        MetalRendererView *view = m_context->cocoa.default_window.contentView;
        if (view) {
            [view attachEventReceiver:receiver];
        }
        m_context->metal.tempoary.event_receiver = receiver;
    };

    return bindings;
}

// MARK: - Application Management

auto renderer::metal::driver::start(renderer::callback frame_request_callback) -> void
{
    m_context->metal.render.frame_request = std::move(frame_request_callback);
    m_context->cocoa.app = [[MetalRendererApplication alloc] init];

    [m_context->cocoa.app runWithContinuation:[&] (MetalRendererApplication *app) {
        // Create a default window for the render output to be displayed into.
        m_context->metal.viewport_size = { (float)m_config.output_width(), (float)m_config.output_height() };
        m_context->cocoa.default_window = [app createWindowWithTitle:@"Metal" size:CGSizeMake(
            m_config.output_width(), m_config.output_height()
        )];
        MetalRendererView *view = m_context->cocoa.default_window.contentView;
        [view attachEventReceiver:m_context->metal.tempoary.event_receiver];

        // Configure the device
        configure_device();

        // Spin up a thread to run the render job.
        m_context->metal.render.runner = std::thread([&] {
            render_job();
        });

        sched_param sch { 0 };
        sch.sched_priority = 45;
        pthread_setschedparam(m_context->metal.render.runner.native_handle(), SCHED_RR, &sch);

        m_context->metal.render.runner.detach();

        return 0;
    }];
}


// MARK: - Metal Device Management

auto renderer::metal::driver::initialize() -> void
{
    m_context = new context();
    m_context->metal.device = MTLCreateSystemDefaultDevice();
}

auto renderer::metal::driver::configure_device() -> void
{
    m_context->metal.command_queue = [m_context->metal.device newCommandQueue];

    // Shaders
    m_context->metal.pixel_format = MTLPixelFormatBGRA8Unorm;
    m_context->metal.shader.compiler = resource::shader::compiler(m_context->metal.device);
    install_default_shader();

    // Configure the output layer
    CAMetalLayer *output_layer = (CAMetalLayer *)m_context->cocoa.default_window.contentView.layer;
    output_layer.device = m_context->metal.device;
    output_layer.pixelFormat = m_context->metal.pixel_format;
    output_layer.frame = (CGRect)m_context->cocoa.default_window.contentView.bounds;
    output_layer.displaySyncEnabled = m_config.vsync();
    m_context->metal.layer.initialize(
        output_layer,
        m_context->metal.viewport_size,
        m_context->metal.shader.programs[m_context->metal.shader.default_id]
    );

    // Configure the frame generator
    m_context->metal.render.generator = frame_generator(
        m_context->metal.device,
        (std::uint32_t)m_context->metal.viewport_size.x(),
        (std::uint32_t)m_context->metal.viewport_size.y(),
        m_config.swap_chain_count(),
        output_layer.pixelFormat
    );

    // Configure the display link
    m_context->display.source = dispatch_source_create(DISPATCH_SOURCE_TYPE_DATA_ADD, 0, 0, dispatch_get_main_queue());
    dispatch_source_set_event_handler(m_context->display.source, ^(){
        m_context->metal.layer.push_frame(
            m_context->metal.command_queue,
            m_context->metal.render.generator.latest_frame_texture()
        );
    });
    dispatch_resume(m_context->display.source);

    CVReturn r = CVDisplayLinkCreateWithActiveCGDisplays(&m_context->display.link);
    if (r != kCVReturnSuccess) {
        exit(1);
    }

    r = CVDisplayLinkSetOutputCallback(m_context->display.link, &dispatch_display_render_request, (__bridge void *)m_context->display.source);
    if (r != kCVReturnSuccess) {
        exit(1);
    }

    auto viewDisplayID = (CGDirectDisplayID)[m_context->cocoa.default_window.screen.deviceDescription[@"NSScreenNumber"] unsignedIntegerValue];
    r = CVDisplayLinkSetCurrentCGDisplay(m_context->display.link, viewDisplayID);
    if (r != kCVReturnSuccess) {
        exit(1);
    }

    CVDisplayLinkStart(m_context->display.link);
}


// MARK: - Viewport Management

auto renderer::metal::driver::set_viewport_title(const std::string& title) -> void
{
    m_context->cocoa.default_window.title = macos::cocoa::string::to(title);
}

auto renderer::metal::driver::set_viewport_size(std::uint32_t width, std::uint32_t height) -> void
{
    // Change the frame of the window to reflect the new viewport.
    auto titlebar_height = m_context->cocoa.default_window.frame.size.height - m_context->cocoa.default_window.contentView.frame.size.height;
    [m_context->cocoa.default_window.contentView setFrame:NSMakeRect(0, 0, width, height)];
    [m_context->cocoa.default_window setFrame:NSMakeRect(0, 0, width, height) display:YES animate:NO];
    [m_context->cocoa.default_window center];

    m_context->metal.viewport_size = { (float)width, (float)height };
    m_context->metal.layer.update_viewport_size(width, height);
}

auto renderer::metal::driver::viewport_title() const -> std::string
{
    return macos::cocoa::string::from(m_context->cocoa.default_window.title);
}

auto renderer::metal::driver::viewport_size() const -> math::vec2
{
    return m_context->metal.viewport_size;
}

// MARK: - Runner


auto renderer::metal::driver::render_job() -> void
{
    while (!m_context->metal.render.should_terminate) {
        @autoreleasepool {
            m_context->metal.render.generator.wait_for_ready();

            // We are ready for a new frame to be produced! Request that a new frame be rendered.
            m_context->metal.render.frame_request();
        }
    }
}

// MARK: - Shader Management

auto renderer::metal::driver::compile_shader_library(const shader::library& lib) -> renderer::shader::program
{
    auto program = m_context->metal.shader.compiler.compile(lib, m_context->metal.pixel_format);
    auto program_uid = program.uid();
    m_context->metal.shader.programs.insert({ program_uid, std::move(program) });

    // Finally return a program reference for the shader...
    return shader::program(program_uid);
}

auto renderer::metal::driver::shader_program_named(const std::string &name) const -> renderer::shader::program
{
    for (const auto& it : m_context->metal.shader.programs) {
        if (it.second.name() == name) {
            return shader::program(it.first);
        }
    }
    throw std::runtime_error("Failed to find shader program '" + name + ".");
}

auto renderer::metal::driver::install_default_shader() -> renderer::shader::program
{
    shader::library lib(
        "default",
        shader::function(resource::shader::default_vertex_function, shader::function::type::VERTEX, "vertex_shader"),
        shader::function(resource::shader::default_fragment_function, shader::function::type::FRAGMENT, "fragment_shader")
    );
    auto program = compile_shader_library(lib);
    m_context->metal.shader.default_id = program.id();
    return program;
}

// MARK: - Frame Management

auto renderer::metal::driver::end_frame(renderer::callback completion) -> void
{
    m_context->metal.render.completion = std::move(completion);
    m_context->metal.render.generator.produce_new_frame(m_context->metal.command_queue, [&] {
        m_context->metal.render.completion();
    });
}

auto renderer::metal::driver::draw(const buffer& buffer) -> void
{
    // The first action to take is constructing a vector of textures so that we can correctly
    // bind the correct texture handles to each vertex.
    std::vector<id<MTLTexture>> textures(buffer.texture_count());
    for (auto i = 0; i < buffer.texture_count(); ++i) {
        auto it = m_context->metal.textures.map.find(buffer.device_texture_for_slot(i));
        if (it != m_context->metal.textures.map.end()) {
            textures[i] = it->second.handle;
        }
        else {
            // TODO: Throw an error here??
            textures[i] = nil;
        }
    }

    // Next find the correct shader.
    const auto& shader = m_context->metal.shader.programs.at(m_context->metal.shader.default_id);

    // Forward to the swapchain.
    m_context->metal.render.generator.current_operation().submit(buffer, textures, shader);
}

// MARK: - Texture Management

auto renderer::metal::driver::create_texture(const data::block &data, math::vec2 size) -> renderer::texture::device_id
{
    KESTREL_PROFILE_FUNCTION();
    metal::texture tx;
    tx.device_id = m_context->metal.textures.next_id++;

    // Upload the texture to the GPU.
    tx.descriptor = [[MTLTextureDescriptor alloc] init];
    tx.descriptor.pixelFormat = MTLPixelFormatRGBA8Unorm;
    tx.descriptor.mipmapLevelCount = 1;
    tx.descriptor.width = (NSUInteger)size.x();
    tx.descriptor.height = (NSUInteger)size.y();
#if TARGET_MACOS_APPLE_SILICON
    tx.descriptor.storageMode = MTLStorageModeShared;
#endif

    tx.region = MTLRegionMake2D(0, 0, tx.descriptor.width, tx.descriptor.height);
    tx.region.origin.z = 0;
    tx.region.size.depth = 1;

    tx.handle = [m_context->metal.device newTextureWithDescriptor:tx.descriptor];
    [tx.handle replaceRegion:tx.region mipmapLevel:0 withBytes:data.get<void *>() bytesPerRow:tx.handle.width << 2];

    // Store the texture, and return the id to the caller.
    auto device_id = tx.device_id;
    m_context->metal.textures.map.insert({ device_id, std::move(tx) });
    return device_id;
}

auto renderer::metal::driver::update_texture(renderer::texture::device_id id, const data::block &data) -> void
{
    auto it = m_context->metal.textures.map.find(id);
    if (it != m_context->metal.textures.map.end()) {
        const auto& tx = it->second;
        [tx.handle replaceRegion:tx.region
                     mipmapLevel:0
                       withBytes:data.get<void *>()
                     bytesPerRow:tx.handle.width << 2];
    }
}

auto renderer::metal::driver::destroy_texture(renderer::texture::device_id id) -> void
{
    // Find the texture itself.
    auto it = m_context->metal.textures.map.find(id);
    if (it != m_context->metal.textures.map.end()) {
        it->second.handle = nil;
        it->second.descriptor = nil;
        m_context->metal.textures.map.erase(it);
    }
}