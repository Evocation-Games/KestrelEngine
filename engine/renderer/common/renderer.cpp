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

#include <imgui/imgui.h>
#include "renderer/common/context.hpp"
#include "renderer/common/renderer.hpp"
#include "renderer/common/draw_buffer.hpp"
#include "renderer/opengl/context.hpp"
#include "renderer/metal/context.h"
#include "core/ui/imgui/imgui.hpp"
#include "core/clock/clock.hpp"

// MARK: - API

static struct {
    renderer::context *context { nullptr };
    enum renderer::api api { renderer::api::none };
    struct renderer::draw_buffer *drawing_buffer { nullptr };
    bool imgui { false };
    float last_frame_time { 0.f };
    float maximum_frame_time { 0.f };
    uint32_t target_framerate { 60 };
    rtc::clock::time frame_start_time { rtc::clock::global().current() };
    float time_since_last_frame { 0.f };
    bool first_frame { true };
} s_renderer_api;

auto renderer::initialize(enum renderer::api api, const std::function<auto()->void> &callback) -> void
{
    s_renderer_api.api = api;

    if (s_renderer_api.target_framerate > 0) {
        s_renderer_api.maximum_frame_time = (1.f / s_renderer_api.target_framerate);
        s_renderer_api.last_frame_time = s_renderer_api.maximum_frame_time;
    }

    switch (api) {
        case api::metal: {
#if TARGET_MACOS
            s_renderer_api.api = renderer::api::metal;
            metal::context::start_application([&, callback] (metal::context *context) {
                s_renderer_api.context = context;
                s_renderer_api.drawing_buffer = new draw_buffer(metal::constants::max_quads * 6, metal::constants::texture_slots);

                auto shader = s_renderer_api.context->shader_program("basic");
                s_renderer_api.drawing_buffer->set_shader(shader);

                callback();
            });
            break;
#endif
        }
        case api::opengl: {
            s_renderer_api.api = renderer::api::opengl;
            s_renderer_api.context = new opengl::context([] {});
            s_renderer_api.drawing_buffer = new draw_buffer(opengl::constants::max_quads * 6, opengl::constants::texture_slots);

            auto shader = s_renderer_api.context->shader_program("basic");
            s_renderer_api.drawing_buffer->set_shader(shader);

            callback();

            while (true) {
                s_renderer_api.context->tick();
            }
            break;
        }
        default: {
            // TODO: Handle this better...
            break;
        }
    }
}

// MARK: - Accessors

auto renderer::api() -> enum api
{
    return s_renderer_api.api;
}

auto renderer::api_name() -> std::string
{
    switch (s_renderer_api.api) {
        case api::opengl: {
#if TARGET_MACOS_M1
            return "OpenGL (macOS Apple Silicon)";
#elif TARGET_MACOS
            return "OpenGL (macOS Intel)";
#elif TARGET_LINUX
            return "OpenGL (Linux)";
#elif TARGET_WINDOWS
            return "OpenGL (Windows)";
#endif
        }
        case api::metal: {
#if TARGET_MACOS_M1
            return "Metal (Apple Silicon)";
#elif TARGET_MACOS
            return "Metal (Intel)";
#endif
        }
        default: {
            return "Unknown Renderer";
        }
    }
}

auto renderer::supports_metal() -> bool
{
#if TARGET_MACOS
    return metal::context::supports_metal();
#else
    return false;
#endif
}

auto renderer::scale_factor() -> float
{
    return 1.0;
}

auto renderer::set_window_title(const std::string &title) -> void
{
    s_renderer_api.context->set_viewport_title(title);
}

auto renderer::set_window_size(const math::size &size) -> void
{
    s_renderer_api.context->set_viewport_size(size);
}

auto renderer::window_title() -> std::string
{
    return s_renderer_api.context->viewport_title();
}

auto renderer::window_size() -> math::size
{
    return s_renderer_api.context->viewport_size();
}

auto renderer::frame_render_required() -> bool
{
    if (s_renderer_api.target_framerate == 0 || s_renderer_api.first_frame) {
        s_renderer_api.first_frame = false;
        return true;
    }
    return time_since_last_frame() >= s_renderer_api.maximum_frame_time;
}

auto renderer::set_target_framerate(uint32_t rate) -> void
{
    s_renderer_api.target_framerate = rate;
    s_renderer_api.maximum_frame_time = (1.f / rate);
}

auto renderer::target_framerate() -> uint32_t
{
    return s_renderer_api.target_framerate;
}

auto renderer::target_frame_time() -> float
{
    return s_renderer_api.maximum_frame_time;
}

auto renderer::approx_framerate() -> uint32_t
{
    return (1.f / s_renderer_api.last_frame_time);
}

auto renderer::resync_clock() -> void
{
    s_renderer_api.last_frame_time = s_renderer_api.maximum_frame_time;
    s_renderer_api.frame_start_time = rtc::clock::global().current();
}

// MARK: - Draw Calls

auto renderer::start_frame(struct camera &camera, bool imgui) -> void
{
    s_renderer_api.last_frame_time = rtc::clock::global().since(s_renderer_api.frame_start_time).count();
    s_renderer_api.frame_start_time = rtc::clock::global().current();

    s_renderer_api.drawing_buffer->set_camera(camera);
    s_renderer_api.drawing_buffer->set_shader(s_renderer_api.context->shader_program("basic"));
    s_renderer_api.drawing_buffer->set_blend(blending::normal);
    s_renderer_api.context->start_frame(nullptr, imgui);
}

auto renderer::end_frame() -> void
{
    flush_frame();
    s_renderer_api.context->finalize_frame([] {
        auto duration = rtc::clock::global().since(s_renderer_api.frame_start_time);
        s_renderer_api.last_frame_time = duration.count();
    });
}

auto renderer::flush_frame() -> void
{
    if (!s_renderer_api.drawing_buffer->is_empty()) {
        s_renderer_api.context->draw(s_renderer_api.drawing_buffer);
        s_renderer_api.drawing_buffer->clear();
        s_renderer_api.drawing_buffer->set_shader(s_renderer_api.context->shader_program("basic"));
    }
}

auto renderer::draw_quad(const std::shared_ptr<graphics::texture> &texture, const math::rect &frame,
                         const math::rect &tex_coords, enum blending mode, float alpha, float scale) -> void
{
    auto buffer = s_renderer_api.drawing_buffer;

    if (buffer->blend() != mode && !buffer->is_empty()) {
        flush_frame();
    }
    buffer->set_blend(mode);

#if TARGET_MACOS
    if (api() == api::metal) {
        switch (mode) {
            case blending::normal:
                buffer->set_shader(s_renderer_api.context->shader_program("basic"));
                break;
            case blending::light:
                buffer->set_shader(s_renderer_api.context->shader_program("light"));
                break;
        }
    }
#endif

    if (!buffer->can_accept_texture(texture)) {
        flush_frame();
    }

    auto texture_slot = buffer->push_texture(texture);

    auto p = (math::vec2(frame.origin) + buffer->camera().translation()) * buffer->camera().scale();
    auto s = (math::vec2(frame.size)) * buffer->camera().scale();

    auto uv_x = tex_coords.origin.x;
    auto uv_y = tex_coords.origin.y;
    auto uv_w = tex_coords.size.width;
    auto uv_h = tex_coords.size.height;

    buffer->push_vertex({ p.x, p.y + s.y }, { uv_x, uv_y +uv_h }, alpha, texture_slot);
    buffer->push_vertex({ p.x + s.x, p.y + s.y }, { uv_x +uv_w, uv_y +uv_h }, alpha, texture_slot);
    buffer->push_vertex({ p.x + s.x, p.y}, { uv_x +uv_w, uv_y }, alpha, texture_slot);
    buffer->push_vertex({ p.x, p.y + s.y }, { uv_x, uv_y +uv_h }, alpha, texture_slot);
    buffer->push_vertex({ p.x, p.y }, { uv_x, uv_y }, alpha, texture_slot);
    buffer->push_vertex({ p.x + s.x, p.y }, { uv_x +uv_w, uv_y }, alpha, texture_slot);

    if (buffer->is_full()) {
        flush_frame();
    }
}

// MARK: - Textures

auto renderer::create_texture(uint64_t handle, const math::size &size) -> std::shared_ptr<graphics::texture>
{
    return s_renderer_api.context->create_texture(handle, size);
}

auto renderer::create_texture(const math::size& size, const graphite::data::block& data) -> std::shared_ptr<graphics::texture>
{
    return s_renderer_api.context->create_texture(data, size);
}

auto renderer::create_texture(const math::size& size, const uint8_t *data) -> std::shared_ptr<graphics::texture>
{
    return s_renderer_api.context->create_texture(reinterpret_cast<void *>(const_cast<uint8_t *>(data)), size);
}

// MARK: - Tick Function

auto renderer::set_tick_function(const std::function<auto() -> void> &callback) -> void
{
    s_renderer_api.context->set_tick_function(callback);
}

// MARK: - ImGui

auto renderer::enable_imgui() -> void
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    ui::font::manager::shared_manager().load_all_fonts_for_imgui();

    s_renderer_api.imgui = true;
    s_renderer_api.context->enable_imgui();
}

auto renderer::disable_imgui() -> void
{
    s_renderer_api.context->disable_imgui();
    s_renderer_api.imgui = false;

    ui::font::manager::shared_manager().unload_all_imgui_fonts();
}

// MARK: - Frame Timings

auto renderer::last_frame_time() -> float
{
    return s_renderer_api.last_frame_time;
}

auto renderer::time_since_last_frame() -> float
{
    return rtc::clock::global().since(s_renderer_api.frame_start_time).count();
}