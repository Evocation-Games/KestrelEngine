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

#pragma once

#include <array>
#include <string>
#include <memory>
#include <functional>
#include <type_traits>
#include <libKestrel/math/rect.hpp>
#include <libKestrel/math/vec4.hpp>
#include <libKestrel/graphics/types/color.hpp>
#include <libKestrel/graphics/texture/texture.hpp>
#include <libKestrel/graphics/renderer/common/api.hpp>
#include <libKestrel/graphics/renderer/common/blending.hpp>
#include <libKestrel/graphics/renderer/common/camera.hpp>
#include <libKestrel/graphics/renderer/common/render_pass.hpp>
#include <libKestrel/graphics/renderer/common/shader/program.hpp>

namespace kestrel::renderer
{

    class context;

    auto initialize(enum api api, const std::function<auto()->void>& callback) -> void;
    auto deinitialize() -> void;

    auto current_context() -> renderer::context *;

    auto api() -> enum api;
    auto api_name() -> std::string;

    auto supports_metal() -> bool;

    auto scale_factor() -> float;

    auto toggle_hitbox_debug() -> void;
    [[nodiscard]] auto hitbox_debug() -> bool;

    auto native_screen_size() -> math::size;
    auto set_fullscreen(bool f) -> void;
    auto set_window_title(const std::string& title) -> void;
    auto set_window_size(const math::size& size) -> void;
    auto window_title() -> std::string;
    auto window_size() -> math::size;

    auto set_tick_function(const std::function<auto()->void>& callback) -> void;

    auto enable_imgui() -> void;
    auto disable_imgui() -> void;

    auto start_frame(struct camera& camera, bool imgui = false) -> void;
    auto end_frame() -> void;
    auto flush_frame() -> void;

    auto last_frame_time() -> float;
    auto time_since_last_frame() -> float;
    auto frame_render_required() -> bool;
    auto set_target_framerate(std::uint32_t rate) -> void;
    auto target_framerate() -> std::uint32_t;
    auto target_frame_time() -> float;
    auto approx_framerate() -> std::uint32_t;
    auto resync_clock() -> void;

    auto create_texture(const math::size& size, const data::block& data) -> std::shared_ptr<graphics::texture>;

    auto draw_quad(const std::shared_ptr<graphics::texture>& texture,
                   const math::rect& frame,
                   const math::rect& tex_coords,
                   enum blending mode,
                   float alpha = 1.f,
                   float scale = 1.f,
                   const std::shared_ptr<shader::program>& shader = nullptr,
                   const std::array<math::vec4, 8>& shader_info = {}) -> void;

    auto draw_line(const math::point& p,
                   const math::point& q,
                   enum blending mode,
                   const graphics::color& color,
                   float weight = 1.f,
                   const std::shared_ptr<shader::program>& shader = nullptr,
                   const std::array<math::vec4, 8>& shader_info = {}) -> void;

}
