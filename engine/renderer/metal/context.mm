// Copyright (c) 2022 Tom Hancocks
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

#include <Cocoa/Cocoa.h>

// LuaBridge and Objective-C are not really on speaking terms.
// Make sure that the two don't fight by removing the following
// definition.
#if defined(Nil)
#   undef Nil
#endif

#include "renderer/common/vertex.hpp"
#include "renderer/metal/context.h"
#include "renderer/metal/macos.h"
#include "renderer/metal/default_shader.hpp"
#include "renderer/metal/framebuffer.h"
#include "renderer/metal/swap_chain.h"
#include "renderer/metal/texture.h"
#include "util/uuid.hpp"

#include "imgui/backends/imgui_impl_metal.h"

// MARK: - Forward Definitions

static auto dispatch_display_render_request(
    CVDisplayLinkRef displayLink,
    const CVTimeStamp *now,
    const CVTimeStamp *outputTime,
    CVOptionFlags flagsIn,
    CVOptionFlags *flagsOut,
    void *displayLinkContext
) -> CVReturn;

// MARK: - Cocoa Application

auto renderer::metal::context::start_application(const std::function<auto(metal::context *)->void> &callback) -> void
{
    cocoa::start_application([&, callback] (KestrelApplication *app) {
        auto context = new renderer::metal::context([] {});

        context->m_window = [app createWindowWithTitle:@"Cosmic Frontier: Override" withSize: {
            static_cast<float>(context->m_metal.viewport_width),
            static_cast<float>(context->m_metal.viewport_height)
        }];

        context->configure_device();
        context->create_shader_library(s_default_metal_shader_code);
        context->add_shader_program("basic", "vertex_shader", "fragment_shader");
        context->add_shader_program("light", "vertex_shader", "fragment_shader");

        callback(context);
    });
}

// MARK: - Support

auto renderer::metal::context::supports_metal() -> bool
{
    id<MTLDevice> device = MTLCreateSystemDefaultDevice();
    if (@available(macOS 10.15, *)) {
        return [device supportsFamily:MTLGPUFamilyMac2];
    }
    else {
        return false;
    }
}

// MARK: - Construction

renderer::metal::context::context(const std::function<auto() -> void> &callback)
{
}

renderer::metal::context::~context()
{
}

// MARK: - Configuration

auto renderer::metal::context::configure_device() -> void
{
    m_metal.device = MTLCreateSystemDefaultDevice();
    m_output_layer = reinterpret_cast<CAMetalLayer *>([[m_window contentView] layer]);

    m_metal.command_queue = [m_metal.device newCommandQueue];

    [m_output_layer setDevice:m_metal.device];
    [m_output_layer setPixelFormat:MTLPixelFormatBGRA8Unorm];
    [m_output_layer setFrame:(CGRect)[[m_window contentView] bounds]];

    for (auto i = 0; i < constants::swap_count; ++i) {
        m_swap.passes[i] = new metal::swap_chain();
    }
    m_swap.ptr = m_swap.passes[0];

    m_display.source = dispatch_source_create(DISPATCH_SOURCE_TYPE_DATA_ADD, 0, 0, dispatch_get_main_queue());
    dispatch_source_set_event_handler(m_display.source, ^(){
        @autoreleasepool {
            tick();
        }
    });
    dispatch_resume(m_display.source);

    CVReturn r = CVDisplayLinkCreateWithActiveCGDisplays(&m_display.link_out);
    if (r != kCVReturnSuccess) {
        // TODO: Graceful error handling...
        exit(1);
    }

    r = CVDisplayLinkSetOutputCallback(m_display.link_out, &dispatch_display_render_request, (__bridge void *)m_display.source);
    if (r != kCVReturnSuccess) {
        exit(1);
    }

    CGDirectDisplayID viewDisplayID = (CGDirectDisplayID)[m_window.screen.deviceDescription[@"NSScreenNumber"] unsignedIntegerValue];
    r = CVDisplayLinkSetCurrentCGDisplay(m_display.link_out, viewDisplayID);
    if (r != kCVReturnSuccess) {
        exit(1);
    }

    CVDisplayLinkStart(m_display.link_out);

    // TODO: Make sure we're notified about the closing of the window, and clean up everything.
}

// MARK: - Display Link Render Request

static auto dispatch_display_render_request(
    CVDisplayLinkRef displayLink,
    const CVTimeStamp *now,
    const CVTimeStamp *outputTime,
    CVOptionFlags flagsIn,
    CVOptionFlags *flagsOut,
    void *displayLinkContext
) -> CVReturn {
    dispatch_source_t source = (__bridge dispatch_source_t)displayLinkContext;
    dispatch_source_merge_data(source, 1);
    return kCVReturnSuccess;
}

auto renderer::metal::context::tick() -> void
{
    m_metal.viewport_width = static_cast<uint32_t>(m_window.contentView.bounds.size.width);
    m_metal.viewport_height = static_cast<uint32_t>(m_window.contentView.bounds.size.height);

    m_display.tick();
}

// MARK: - Window Accessors

auto renderer::metal::context::set_viewport_size(const math::size &viewport_size) -> void
{
    [m_window setFrame:NSMakeRect(0, 0, viewport_size.width, viewport_size.height) display:YES animate:NO];
    [m_window center];

    m_metal.viewport_width = static_cast<uint32_t>(viewport_size.width);
    m_metal.viewport_height = static_cast<uint32_t>(viewport_size.height);
}

auto renderer::metal::context::viewport_size() const -> math::size
{
    return { m_metal.viewport_width, m_metal.viewport_height };
}

auto renderer::metal::context::set_viewport_title(const std::string &title) -> void
{
    [m_window setTitle:[NSString stringWithUTF8String:title.c_str()]];
}

auto renderer::metal::context::viewport_title() const -> std::string
{
    std::string title { [[m_window title] UTF8String] };
    return title;
}


// MARK: - Shader Management

auto renderer::metal::context::create_shader_library(const std::string &source) -> void
{
    dispatch_group_t group = dispatch_group_create();
    dispatch_group_enter(group);

    NSString *sourceString = [NSString stringWithUTF8String:source.c_str()];
    [m_metal.device newLibraryWithSource:sourceString options:nullptr completionHandler:^(id<MTLLibrary> lib, NSError *err) {
        if (lib && !err) {
            m_metal.library = [lib retain];
        }
        else {
            // TODO: Handle errors gracefully
        }
        dispatch_group_leave(group);
    }];

    dispatch_group_wait(group, DISPATCH_TIME_FOREVER);
    dispatch_release(group);
}

auto renderer::metal::context::add_shader_program(const std::string &name, const std::string &vertex_function_name, const std::string &fragment_function_name) -> std::shared_ptr<shader::program>
{
    id<MTLFunction> vertex_function = [m_metal.library newFunctionWithName:[NSString stringWithUTF8String:vertex_function_name.c_str()]];
    id<MTLFunction> fragment_function = [m_metal.library newFunctionWithName:[NSString stringWithUTF8String:fragment_function_name.c_str()]];

    if (!vertex_function) {
        // TODO: Handle missing vertex function
    }

    if (!fragment_function) {
        // TODO: Handle missing fragment function
    }

    // Compile the functions into a pipeline that can be used.
    MTLRenderPipelineDescriptor *pipeline = [[MTLRenderPipelineDescriptor alloc] init];
    pipeline.label = [NSString stringWithUTF8String:name.c_str()];
    pipeline.vertexFunction = vertex_function;
    pipeline.fragmentFunction = fragment_function;
    pipeline.colorAttachments[0].pixelFormat = m_output_layer.pixelFormat;
    pipeline.colorAttachments[0].blendingEnabled = YES;

    if (name == "light") {
        pipeline.colorAttachments[0].rgbBlendOperation = MTLBlendOperationAdd;
        pipeline.colorAttachments[0].alphaBlendOperation = MTLBlendOperationAdd;
        pipeline.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
        pipeline.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorSourceAlpha;
        pipeline.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOne;
        pipeline.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    }
    else {
        pipeline.colorAttachments[0].rgbBlendOperation = MTLBlendOperationAdd;
        pipeline.colorAttachments[0].alphaBlendOperation = MTLBlendOperationAdd;
        pipeline.colorAttachments[0].sourceRGBBlendFactor = MTLBlendFactorSourceAlpha;
        pipeline.colorAttachments[0].sourceAlphaBlendFactor = MTLBlendFactorSourceAlpha;
        pipeline.colorAttachments[0].destinationRGBBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
        pipeline.colorAttachments[0].destinationAlphaBlendFactor = MTLBlendFactorOneMinusSourceAlpha;
    }

    NSError *error;
    id<MTLRenderPipelineState> pipelineState = [[m_metal.device newRenderPipelineStateWithDescriptor:pipeline error:&error] retain];

    util::uuid id { name };
    auto program = std::make_shared<shader::program>(pipelineState);
    m_metal.shader_programs.insert(std::pair(id, program));

    return shader_program(name);
}

auto renderer::metal::context::shader_program(const std::string &name) -> std::shared_ptr<shader::program>
{
    util::uuid id { name };
    auto it = m_metal.shader_programs.find(id);
    if (it == m_metal.shader_programs.end()) {
        // TODO: Handle this error
        abort();
    }
    return it->second;
}

// MARK: - ImGui

auto renderer::metal::context::enable_imgui() -> void
{
    m_imgui.enabled = ImGui_ImplMetal_Init(m_metal.device);
}

auto renderer::metal::context::disable_imgui() -> void
{
    m_imgui.enabled = false;
    ImGui_ImplMetal_Shutdown();
}

// MARK: - Frames

auto renderer::metal::context::start_frame(const render_pass *pass, bool imgui) -> void
{
    auto active_pass = const_cast<render_pass *>(pass);

    if (!pass) {
        dispatch_semaphore_wait(m_swap.sema, DISPATCH_TIME_FOREVER);
        auto pass_index = m_swap.index;
        m_swap.index = (pass_index + 1) % constants::swap_count;

        auto render_pass = reinterpret_cast<metal::swap_chain *>(m_swap.passes[pass_index]);
        render_pass->start([m_output_layer nextDrawable], m_metal.command_queue.commandBuffer, m_metal.viewport_width, m_metal.viewport_height);

        m_swap.ptr = render_pass;
        active_pass = const_cast<renderer::render_pass *>(reinterpret_cast<renderer::render_pass *>(render_pass));
    }

    if (imgui) {
        active_pass->start_imgui();
    }
    m_imgui.enabled = imgui;
}

auto renderer::metal::context::finalize_frame(const std::function<auto() -> void> &callback) -> void
{
    renderer::render_pass *render_pass = m_swap.ptr ?: m_swap.passes[m_swap.index];

    if (m_imgui.enabled) {
        render_pass->finalize_imgui();
        m_imgui.enabled = false;
    }

    render_pass->finalize([&] {
        dispatch_semaphore_signal(m_swap.sema);
    });

    m_swap.ptr = nullptr;
}

auto renderer::metal::context::draw(const draw_buffer *buffer) -> void
{
    renderer::render_pass *render_pass = m_swap.ptr ?: m_swap.passes[m_swap.index];
    render_pass->draw(buffer);
}

// MARK: - Framebuffer

auto renderer::metal::context::create_framebuffer(const math::size &size) -> renderer::framebuffer *
{
    auto fb = new renderer::metal::framebuffer(static_cast<uint32_t>(size.width), static_cast<uint32_t>(size.height));

    return fb;
}

// MARK: - Textures

auto renderer::metal::context::create_texture(uint64_t handle, const math::size &size) -> std::shared_ptr<graphics::texture>
{
    auto ptr = const_cast<const void *>(reinterpret_cast<void *>(handle));
    return std::make_shared<metal::texture>(ptr, size);
}

auto renderer::metal::context::create_texture(const std::vector<uint32_t> &data, const math::size &size) -> std::shared_ptr<graphics::texture>
{
    auto texture = create_texture(const_cast<void *>(reinterpret_cast<const void *>(&data[0])), size);
    texture->set_data(data);
    return texture;
}

auto renderer::metal::context::create_texture(void *data, const math::size &size) -> std::shared_ptr<graphics::texture>
{
    MTLTextureDescriptor *texture_descriptor = [[[MTLTextureDescriptor alloc] init] autorelease];
    texture_descriptor.pixelFormat = MTLPixelFormatRGBA8Unorm;
    texture_descriptor.mipmapLevelCount = 1;
    texture_descriptor.width = static_cast<NSUInteger>(size.width);
    texture_descriptor.height = static_cast<NSUInteger>(size.height);

    id<MTLTexture> texture = [[m_metal.device newTextureWithDescriptor:texture_descriptor] retain];
    MTLRegion region = MTLRegionMake2D(0, 0, texture_descriptor.width, texture_descriptor.height);
    region.origin.z = 0;
    region.size.depth = 1;

    NSUInteger bytes_per_row = texture.width << 2;
    [texture replaceRegion:region mipmapLevel:0 withBytes:data bytesPerRow:bytes_per_row];

    auto out = create_texture(reinterpret_cast<uint64_t>(texture), size);
    out->set_raw_data_ptr(reinterpret_cast<const uint8_t *>(data));
    return out;
}

// MARK: - Tick Function

auto renderer::metal::context::set_tick_function(const std::function<auto()->void> &callback) -> void
{
    m_display.tick = callback;
}
