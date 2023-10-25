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
#include <libMetalRenderer/driver/driver.hpp>
#include <libMetalRenderer/cocoa/application.h>
#include <libMetalRenderer/driver/shader_program.h>
#include <libMacOS/cocoa/string.h>
#include <MetalKit/MetalKit.h>
#include <libMetalRenderer/driver/texture.h>
#include <libMetalRenderer/render/swapchain.h>
#include <libFoundation/profile/profiler.hpp>
#include <libMetalRenderer/render/render_operation.h>

// MARK: - Metal Driver Context

namespace renderer::metal
{
    struct context
    {
        struct {
            id<MTLDevice> device { nil };
            id<MTLCommandQueue> command_queue { nil };
            CAMetalLayer *output_layer { nil };
            math::vec2 viewport_size;

            struct {
                std::function<auto()->void> frame_request;
                std::thread runner;
                render_operation operation;
                bool should_terminate { false };
                renderer::callback completion;
            } render;

            struct {
                std::uint32_t index { 0 };
                std::vector<swapchain> all;
                dispatch_semaphore_t sema;
                swapchain *current { nullptr };
            } swap;

            struct {
                std::uint32_t next_id { 0 };
                std::uint32_t default_id { 0 };
                std::unordered_map<std::string, id<MTLLibrary>> libraries;
                std::unordered_map<std::uint32_t, metal::shader_program> programs;
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
    bindings.initialize = [&] { initialize(); };
    bindings.start = [&] (auto callback) { start(std::move(callback)); };

    bindings.set_viewport_title = [&] (auto title) { set_viewport_title(title); };
    bindings.set_viewport_size = [&] (auto width, auto height) { set_viewport_size(width, height); };
    bindings.viewport_title = [&] { return viewport_title(); };
    bindings.viewport_size = [&] { return viewport_size(); };

    bindings.start_frame = [&] (const auto& frame) { start_frame(frame); };
    bindings.end_frame = [&] (auto callback) { end_frame(std::move(callback)); };
    bindings.submit_draw_buffer = [&] (const auto& buffer) { draw(buffer); };

    return bindings;
}

// MARK: - Application Management

auto renderer::metal::driver::start(renderer::callback frame_request_callback) -> void
{
    m_context = new context();
    m_context->metal.swap.sema = dispatch_semaphore_create(m_config.swap_chain_count());
    m_context->metal.render.frame_request = std::move(frame_request_callback);
    m_context->cocoa.app = [[MetalRendererApplication alloc] init];

    [m_context->cocoa.app runWithContinuation:[&] (MetalRendererApplication *app) {
        // Create a default window for the render output to be displayed into.
        m_context->metal.viewport_size = { (float)m_config.output_width(), (float)m_config.output_height() };
        m_context->cocoa.default_window = [app createWindowWithTitle:@"Metal" size:CGSizeMake(
            m_config.output_width(), m_config.output_height()
        )];

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

}

auto renderer::metal::driver::configure_device() -> void
{
    m_context->metal.device = MTLCreateSystemDefaultDevice();
    m_context->metal.output_layer = (CAMetalLayer *)m_context->cocoa.default_window.contentView.layer;
    m_context->metal.command_queue = [m_context->metal.device newCommandQueue];

    // Configure the render operation
    m_context->metal.render.operation.initialize(m_context->metal.device);

    // Configure the output layer
    m_context->metal.output_layer.device = m_context->metal.device;
    m_context->metal.output_layer.pixelFormat = MTLPixelFormatBGRA8Unorm;
    m_context->metal.output_layer.frame = (CGRect)m_context->cocoa.default_window.contentView.bounds;
    m_context->metal.output_layer.displaySyncEnabled = m_config.vsync();

    // Shaders
    install_default_shader();

    // Setup any required swap chains to use when rendering.
    for (auto i = 0; i < m_config.swap_chain_count(); ++i) {
        m_context->metal.swap.all.emplace_back();
    }

    // Configure the display link
    m_context->display.source = dispatch_source_create(DISPATCH_SOURCE_TYPE_DATA_ADD, 0, 0, dispatch_get_main_queue());
    dispatch_source_set_event_handler(m_context->display.source, ^(){
        m_context->display.tick_received = true;
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
    m_context->metal.output_layer.drawableSize = CGSizeMake(width, height);
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

auto renderer::metal::driver::should_request_frame() const -> bool
{
    // There are two methods by which we should check for frame requests.
    // If VSync is enabled, then we should check to see if we have received
    // a display tick.
    // We also need to wait for a swapchain instance is available.
    if (m_config.vsync() && !m_context->display.tick_received) {
        return false;
    }
    m_context->display.tick_received = false;

    // Wait for a swapchain to become available.
    dispatch_semaphore_wait(m_context->metal.swap.sema, DISPATCH_TIME_FOREVER);

    // We are now ready for a frame.
    return true;
}

auto renderer::metal::driver::render_job() -> void
{
    using namespace std::chrono_literals;
    while (!m_context->metal.render.should_terminate) {
        if (!should_request_frame()) {
            // Briefly sleep to avoid hammering the CPU, whilst we wait for an update.
            std::this_thread::sleep_for(250ns);
            continue;
        }

        // We are ready for a new frame to be produced! Request a new frame.
        // To do this we need to setup the swapchain associated with this frame.
        auto swap = m_context->metal.swap.index;
        m_context->metal.swap.index = (swap + 1) % m_config.swap_chain_count();
        m_context->metal.swap.current = &m_context->metal.swap.all[swap];
        m_context->metal.render.frame_request();
    }
}

// MARK: - Shader Management

#pragma region Shader Library Template Code
    static constexpr const char *s_shader_code_template {R"(
        #include <metal_stdlib>
        #include <simd/simd.h>

        using namespace metal;

        namespace kestrel
        {
            float rand(float3 v)
            {
                float2 K1 = float2(
                    23.14069263277926, // e^pi (Gelfond's constant)
                    2.665144142690225 // 2^sqrt(2) (Gelfond Schneider constant)
                );
                return fract( cos( dot(float2(v.x, v.y),K1) ) * 12345.6789 );
            }
        };

        typedef enum
        {
        	vertices = 0,
        	viewport_size = 1,
        } vertex_input_index;

        typedef enum
        {
        	base_color = 0,
        } texture_index;

        typedef struct
        {
        	float4 position;
        	float4 color;
        	float2 tex_coord;
        	float texture;
        } vertex_descriptor;

        typedef struct
        {
        	float4 position [[position]];
        	float4 color;
        	float2 tex_coord;
        	float texture;
        } raster_data;

        // Vertex Function
        vertex raster_data vertex_shader(
        	uint vertex_id [[vertex_id]],
        	constant vertex_descriptor *vertex_array [[buffer(vertex_input_index::vertices)]],
        	constant vector_uint2 *viewport_size_ptr [[buffer(vertex_input_index::viewport_size)]]
        ) {
        	raster_data out;

        	auto position = vertex_array[vertex_id].position;
        	auto tex_coord = vertex_array[vertex_id].tex_coord;
        	auto color = vertex_array[vertex_id].color;
        	auto texture = floor(vertex_array[vertex_id].texture);
            auto scale = 1.f;

        	float2 viewport_size = float2(*viewport_size_ptr);
            float2 pixel_space_position = floor(position.xy * scale);
            float2 inverse_size(1.0f / viewport_size.x, 1.0f / viewport_size.y);
            float clip_x = (2.0f * pixel_space_position.x * inverse_size.x) - 1.0f;
            float clip_y = (2.0f * -pixel_space_position.y * inverse_size.y) + 1.0f;

            @@VERTEX_FUNCTION@@
        }

        // Fragment Function
        fragment float4 fragment_shader(
        	raster_data in [[stage_in]],
        	array<texture2d<half>, 31> textures [[texture(0)]]
        ) {
            @@FRAGMENT_FUNCTION@@
        }
    )"};
#pragma endregion

#pragma region Default Shader Code
static constexpr const char *s_default_vertex_function {R"(
        out.position = float4(clip_x, clip_y, position.z, 1.0);
        out.tex_coord = tex_coord;
        out.color = color;
        out.texture = texture;
        return out;
    )"};

static constexpr const char *s_default_fragment_function {R"(
//        constexpr sampler texture_sampler (mag_filter::linear, min_filter::linear);
//        const float4 color_sample = in.color * float4(textures[in.texture].sample(texture_sampler, in.tex_coord));
        return in.color;
)"};
#pragma endregion

auto renderer::metal::driver::compile_shader_library(const shader::library& lib) -> shader::program
{
    auto vertex = lib.vertex_function();
    auto fragment = lib.fragment_function();

    // The first job is to actually assemble the library into a single string that can be passed to the
    // Metal device for compilation.
    std::string source(s_shader_code_template);

    std::string vertex_function_token("@@VERTEX_FUNCTION@@");
    auto vertex_function_it = source.find(vertex_function_token);
    if (vertex_function_it != std::string::npos) {
        source.replace(vertex_function_it, vertex_function_token.length(), vertex.code());
    }

    std::string fragment_function_token("@@FRAGMENT_FUNCTION@@");
    auto fragment_function_it = source.find(fragment_function_token);
    if (fragment_function_it != std::string::npos) {
        source.replace(fragment_function_it, fragment_function_token.length(), fragment.code());
    }

    // Now that the library source is produced, proceed to actually compiling. This is a potentially blocking
    // process.
    dispatch_group_t group = dispatch_group_create();
    dispatch_group_enter(group);

    NSString *library_source = macos::cocoa::string::to(source);
    __block id<MTLLibrary> new_library = nil;
    [m_context->metal.device newLibraryWithSource:library_source options:nil completionHandler:^(id<MTLLibrary> metal_library, NSError *error) {
        if (metal_library && !error) {
            m_context->metal.shader.libraries.insert({ lib.name(), metal_library });
            new_library = metal_library;
        }
        else {
            // TODO: Handle errors gracefully.
            std::cerr << error.description.UTF8String << std::endl;
        }
        dispatch_group_leave(group);
    }];

    dispatch_group_wait(group, DISPATCH_TIME_FOREVER);

    if (!new_library) {
        // Failed to compile, so return a null program.
        return {};
    }

    // Now that the library is compiled, produce a shader program.
    id<MTLFunction> metal_vertex_function = [new_library newFunctionWithName:macos::cocoa::string::to(vertex.name())];
    id<MTLFunction> metal_fragment_function = [new_library newFunctionWithName:macos::cocoa::string::to(fragment.name())];

    // TODO: Check for missing functions

    auto program_id = m_context->metal.shader.next_id++;
    metal::shader_program shader_program(
        m_context->metal.device,
        lib.name(),
        metal_vertex_function,
        metal_fragment_function,
        m_context->metal.output_layer.pixelFormat
    );
    m_context->metal.shader.programs.insert({ program_id, std::move(shader_program) });

    // Finally return a program reference for the shader...
    return shader::program(program_id);
}

auto renderer::metal::driver::shader_program_named(const std::string &name) const -> shader::program
{
    for (const auto& it : m_context->metal.shader.programs) {
        if (it.second.name() == name) {
            return shader::program(it.first);
        }
    }
    throw std::runtime_error("Failed to find shader program '" + name + ".");
}

auto renderer::metal::driver::install_default_shader() -> shader::program
{
    shader::library lib(
        "default",
        shader::function(s_default_vertex_function, shader::function::type::VERTEX, "vertex_shader"),
        shader::function(s_default_fragment_function, shader::function::type::FRAGMENT, "fragment_shader")
    );
    auto program = compile_shader_library(lib);
    m_context->metal.shader.default_id = program.id();
    return program;
}

// MARK: - Frame Management

auto renderer::metal::driver::start_frame(const renderer::frame &frame) -> void
{
    KESTREL_PROFILE_FUNCTION();
    m_context->metal.render.operation.clear();
}

auto renderer::metal::driver::end_frame(renderer::callback completion) -> void
{
    KESTREL_PROFILE_FUNCTION();
    auto swap = m_context->metal.swap.current;
    m_context->metal.render.completion = std::move(completion);

    // Prepare the swapchain for this frame.
    swap->prepare(
        m_context->metal.output_layer.nextDrawable,
        m_context->metal.command_queue.commandBuffer,
        (std::uint32_t)m_context->metal.viewport_size.x(),
        (std::uint32_t)m_context->metal.viewport_size.y()
    );

    // Commit the operation for processing...
    swap->commit(m_context->metal.render.operation, [&] {
        dispatch_semaphore_signal(m_context->metal.swap.sema);
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
    m_context->metal.render.operation.submit(buffer, textures, shader);
}

// MARK: - Texture Management

auto renderer::metal::driver::create_texture(const data::block &data, math::vec2 size) -> renderer::texture::device_id
{
    metal::texture tx;
    tx.device_id = m_context->metal.textures.next_id++;

    // Upload the texture to the GPU.
    tx.descriptor = [[MTLTextureDescriptor alloc] init];
    tx.descriptor.pixelFormat = MTLPixelFormatRGBA8Unorm;
    tx.descriptor.mipmapLevelCount = 1;
    tx.descriptor.width = (NSUInteger)size.x();
    tx.descriptor.height = (NSUInteger)size.y();

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