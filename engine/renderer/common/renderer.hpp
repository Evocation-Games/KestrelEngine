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

#include <string>
#include <memory>
#include <functional>
#include <type_traits>
#include "math/rect.hpp"
#include "core/graphics/common/color.hpp"
#include "core/graphics/common/texture.hpp"
#include "renderer/common/api.hpp"
#include "renderer/common/blending.hpp"
#include "renderer/common/camera.hpp"
#include "renderer/common/render_pass.hpp"

namespace renderer
{

    auto initialize(enum api api, const std::function<auto()->void>& callback) -> void;
    auto deinitialize() -> void;

    auto api() -> enum api;
    auto api_name() -> std::string;

    auto supports_metal() -> bool;

    auto scale_factor() -> float;

    auto set_window_title(const std::string& title) -> void;
    auto set_window_size(const math::size& size) -> void;
    auto window_title() -> std::string;
    auto window_size() -> math::size;

    auto set_tick_function(const std::function<auto()->void>& callback) -> void;

    auto enable_imgui() -> void;
    auto disable_imgui() -> void;

    auto start_frame(struct camera& camera) -> void;
    auto end_frame() -> void;
    auto flush_frame() -> void;

    auto create_texture(uint64_t handle, const math::size& size) -> std::shared_ptr<graphics::texture>;
    auto create_texture(const math::size& size, const std::vector<uint32_t>& data) -> std::shared_ptr<graphics::texture>;
    auto create_texture(const math::size& size, const uint8_t *data) -> std::shared_ptr<graphics::texture>;

    auto draw_quad(const std::shared_ptr<graphics::texture>& texture,
                   const math::rect& frame,
                   const math::rect& tex_coords,
                   enum blending mode,
                   float alpha = 1.0,
                   float scale = 1.0) -> void;

}
