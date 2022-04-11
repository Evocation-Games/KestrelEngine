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

#include "renderer/common/context.hpp"
#include "renderer/common/renderer.hpp"
#include "renderer/common/draw_buffer.hpp"
#include "renderer/opengl/context.hpp"
#include "renderer/metal/context.h"

// MARK: - API

static struct {
    renderer::context *context { nullptr };
    enum renderer::api api { renderer::api::none };
    struct renderer::draw_buffer *drawing_buffer { nullptr };
} s_renderer_api;

auto renderer::initialize(enum renderer::api api, const std::function<auto()->void> &callback) -> void
{
    s_renderer_api.api = api;

    switch (api) {
        case api::metal: {
#if TARGET_MACOS
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
            s_renderer_api.context = new opengl::context([] {});
            s_renderer_api.drawing_buffer = new draw_buffer(opengl::constants::max_quads * 6, opengl::constants::texture_slots);

            auto shader = s_renderer_api.context->shader_program("basic");
            s_renderer_api.drawing_buffer->set_shader(shader);

            callback();
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

}

auto renderer::set_window_size(const math::size &size) -> void
{

}

auto renderer::window_title() -> std::string
{
    return "";
}

auto renderer::window_size() -> math::size
{
    return {};
}

// MARK: - Draw Calls

auto renderer::start_frame(struct camera &camera) -> void
{
    s_renderer_api.drawing_buffer->set_camera(camera);
    s_renderer_api.context->start_frame();
}

auto renderer::end_frame() -> void
{
    flush_frame();
    s_renderer_api.context->finalize_frame([] {});
}

auto renderer::flush_frame() -> void
{
    if (!s_renderer_api.drawing_buffer->is_empty()) {
        s_renderer_api.context->draw(s_renderer_api.drawing_buffer);
        s_renderer_api.drawing_buffer->clear();
    }
}

auto renderer::draw_quad(const std::shared_ptr<graphics::texture> &texture, const math::rect &frame,
                         const math::rect &tex_coords, enum blending mode, float alpha, float scale) -> void
{
    auto buffer = s_renderer_api.drawing_buffer;

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

auto renderer::create_texture(const math::size& size, const std::vector<uint32_t>& data) -> std::shared_ptr<graphics::texture>
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