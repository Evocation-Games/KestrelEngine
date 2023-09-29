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

#include <libKestrel/graphics/renderer/common/vertex.hpp>
#include <libKestrel/graphics/renderer/metal/context.h>
#include <libKestrel/graphics/renderer/metal/macos.h>
#include <libKestrel/graphics/renderer/metal/default_shader.hpp>
#include <libKestrel/graphics/renderer/metal/framebuffer.h>
#include <libKestrel/graphics/renderer/metal/swap_chain.h>
#include <libKestrel/graphics/renderer/metal/texture.h>
#include <libKestrel/graphics/renderer/metal/shader.h>
#include <imgui/backends/imgui_impl_metal.h>

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

auto kestrel::renderer::metal::context::start_application(const std::function<auto(metal::context *)->void> &callback) -> void
{
    platform::macos::start_application([&, callback] (KestrelApplication *app) {
        auto context = new metal::context();

        context->m_window = [app createWindowWithTitle:@"Cosmic Frontier: Override" withSize: {
            static_cast<float>(context->m_metal.viewport_width),
            static_cast<float>(context->m_metal.viewport_height)
        }];

        context->configure_device();
        context->create_shader_library("basic", metal::s_default_vertex_function, metal::s_default_fragment_function);

        callback(context);
    });
}

// MARK: - Support

auto kestrel::renderer::metal::context::supports_metal() -> bool
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

kestrel::renderer::metal::context::context(const std::function<auto() -> void> &callback)
{
}


// MARK: - Configuration

auto kestrel::renderer::metal::context::configure_device() -> void
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
        tick();
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

    auto viewDisplayID = (CGDirectDisplayID)[m_window.screen.deviceDescription[@"NSScreenNumber"] unsignedIntegerValue];
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
    auto source = (__bridge dispatch_source_t)displayLinkContext;
    dispatch_source_merge_data(source, 1);
    return kCVReturnSuccess;
}

auto kestrel::renderer::metal::context::tick() -> void
{
    m_metal.viewport_width = static_cast<uint32_t>(m_window.contentView.bounds.size.width);
    m_metal.viewport_height = static_cast<uint32_t>(m_window.contentView.bounds.size.height);

    m_display.tick();
}

// MARK: - Window Accessors

auto kestrel::renderer::metal::context::set_viewport_size(const math::size &viewport_size) -> void
{
    auto titlebar_height = [m_window frame].size.height - [[m_window contentView] frame].size.height;
    [[m_window contentView] setFrame:NSMakeRect(0.0, 0.0, viewport_size.width(), viewport_size.height())];
    [m_window setFrame:NSMakeRect(0, 0, viewport_size.width(), viewport_size.height() + titlebar_height) display:YES animate:NO];
    [m_window center];

    m_metal.viewport_width = static_cast<uint32_t>(viewport_size.width());
    m_metal.viewport_height = static_cast<uint32_t>(viewport_size.height());

    [m_output_layer setDrawableSize:CGSizeMake(m_metal.viewport_width, m_metal.viewport_height)];
}

auto kestrel::renderer::metal::context::viewport_size() const -> math::size
{
    return { static_cast<float>(m_metal.viewport_width), static_cast<float>(m_metal.viewport_height) };
}

auto kestrel::renderer::metal::context::scaled_viewport_size() const -> math::size
{
    return viewport_size();
}

auto kestrel::renderer::metal::context::set_viewport_title(const std::string &title) -> void
{
    [m_window setTitle:[NSString stringWithUTF8String:title.c_str()]];
}

auto kestrel::renderer::metal::context::viewport_title() const -> std::string
{
    std::string title { [[m_window title] UTF8String] };
    return title;
}


// MARK: - Shader Management

auto kestrel::renderer::metal::context::create_shader_library(const std::string& name, const std::string &source) -> void
{
    dispatch_group_t group = dispatch_group_create();
    dispatch_group_enter(group);

    NSString *sourceString = [NSString stringWithUTF8String:source.c_str()];
    [m_metal.device newLibraryWithSource:sourceString options:nullptr completionHandler:^(id<MTLLibrary> lib, NSError *err) {
        if (lib && !err) {
            m_metal.libraries.emplace(std::pair(name, lib));
        }
        else {
            // TODO: Handle errors gracefully
            std::cerr << [[err description] UTF8String] << std::endl;
        }
        dispatch_group_leave(group);
    }];

    dispatch_group_wait(group, DISPATCH_TIME_FOREVER);
}

auto kestrel::renderer::metal::context::create_shader_library(const std::string &name,
                                                              const std::string &vertex_function,
                                                              const std::string &fragment_function) -> std::shared_ptr<renderer::shader::program>
{
    // Build the single source string.
    std::string library_source(metal::s_shader_code_template);

    std::string vertex_function_token = "@@VERTEX_FUNCTION@@";
    auto vertex_function_it = library_source.find(vertex_function_token);
    if (vertex_function_it != std::string::npos) {
        library_source.replace(vertex_function_it, vertex_function_token.length(), vertex_function);
    }

    std::string fragment_function_token = "@@FRAGMENT_FUNCTION@@";
    auto fragment_function_it = library_source.find(fragment_function_token);
    if (fragment_function_it != std::string::npos) {
        library_source.replace(fragment_function_it, fragment_function_token.length(), fragment_function);
    }

    create_shader_library(name, library_source);
    return add_shader_program(name, metal::s_vertex_function_name, metal::s_fragment_function_name);
}

auto kestrel::renderer::metal::context::add_shader_program(const std::string &name, const std::string &vertex_function_name, const std::string &fragment_function_name) -> std::shared_ptr<renderer::shader::program>
{
    auto it = m_metal.libraries.find(name);
    if (it == m_metal.libraries.end()) {
        return nullptr;
    }
    auto library = it->second;

    id<MTLFunction> vertex_function = [library newFunctionWithName:[NSString stringWithUTF8String:vertex_function_name.c_str()]];
    id<MTLFunction> fragment_function = [library newFunctionWithName:[NSString stringWithUTF8String:fragment_function_name.c_str()]];

    if (!vertex_function) {
        // TODO: Handle missing vertex function
    }

    if (!fragment_function) {
        // TODO: Handle missing fragment function
    }

    // Compile the functions into a pipeline that can be used.
    util::uid id { name };
    auto program = std::make_shared<metal::shader::program>(this, name, vertex_function, fragment_function, m_output_layer.pixelFormat);
    m_metal.shader_programs.insert(std::pair(id, program));

    return shader_program(name);
}

auto kestrel::renderer::metal::context::shader_program(const std::string &name) -> std::shared_ptr<renderer::shader::program>
{
    util::uid id { name };
    auto it = m_metal.shader_programs.find(id);
    if (it == m_metal.shader_programs.end()) {
        // TODO: Handle this error
        abort();
    }
    return it->second;
}

// MARK: - ImGui

auto kestrel::renderer::metal::context::enable_imgui() -> void
{
    m_imgui.enabled = ImGui_ImplMetal_Init(m_metal.device);
}

auto kestrel::renderer::metal::context::disable_imgui() -> void
{
    m_imgui.enabled = false;
    ImGui_ImplMetal_Shutdown();
}

// MARK: - Frames

auto kestrel::renderer::metal::context::start_frame(const render_pass *pass, bool imgui) -> void
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

auto kestrel::renderer::metal::context::finalize_frame(const std::function<auto() -> void> &callback) -> void
{
    renderer::render_pass *render_pass = m_swap.ptr ?: m_swap.passes[m_swap.index];

    if (m_imgui.enabled) {
        render_pass->finalize_imgui();
        m_imgui.enabled = false;
    }

    render_pass->finalize([&, callback] {
        dispatch_semaphore_signal(m_swap.sema);
        callback();
    });

    m_swap.ptr = nullptr;
}

auto kestrel::renderer::metal::context::draw(const draw_buffer *buffer) -> void
{
    renderer::render_pass *render_pass = m_swap.ptr ?: m_swap.passes[m_swap.index];
    render_pass->draw(buffer);
}

// MARK: - Framebuffer

auto kestrel::renderer::metal::context::create_framebuffer(const math::size &size) -> renderer::framebuffer *
{
    auto fb = new renderer::metal::framebuffer(static_cast<uint32_t>(size.width()), static_cast<uint32_t>(size.height()));
    return fb;
}

// MARK: - Textures

auto kestrel::renderer::metal::context::create_texture(const data::block& data, const math::size &size) -> std::shared_ptr<graphics::texture>
{
    auto tex = std::make_shared<metal::texture>(size, data);
    return std::static_pointer_cast<graphics::texture>(tex);
}

// MARK: - Tick Function

auto kestrel::renderer::metal::context::set_tick_function(const std::function<auto()->void> &callback) -> void
{
    m_display.tick = callback;
}

// MARK: - Screen

auto kestrel::renderer::metal::context::set_fullscreen(bool f) -> void
{
    [m_window toggleFullScreen:m_window];
}

auto kestrel::renderer::metal::context::native_screen_scale() const -> float
{
    // TODO: Metal is handling the scaling automatically, so simply report a scale factor or 1.0
    return 1.0f;
}

auto kestrel::renderer::metal::context::native_screen_size() const -> math::size
{
    NSScreen *screen = [m_window screen];
    NSRect frame = [screen frame];
    return { static_cast<float>(frame.size.width), static_cast<float>(frame.size.height) };
}
