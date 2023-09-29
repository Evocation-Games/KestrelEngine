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

#include <libKestrel/graphics/sprites/sprite_sheet.hpp>
#include <libKestrel/math/polygon.hpp>
#include <libKestrel/math/triangulated_polygon.hpp>
#include <libKestrel/physics/hitbox.hpp>

namespace kestrel::physics::hitbox_constructor
{
    static constexpr std::uint32_t accuracy = 15;
    static constexpr std::uint8_t alpha_threshold = 5;

    auto polygon(std::shared_ptr<graphics::sprite_sheet> sheet, const math::rect& sprite_frame, std::uint32_t accuracy = hitbox_constructor::accuracy) -> math::polygon;
    auto triangulated_polygon(std::shared_ptr<graphics::sprite_sheet> sheet, const math::rect& sprite_frame, std::uint32_t accuracy = hitbox_constructor::accuracy) -> math::triangulated_polygon;

    auto hitbox(std::shared_ptr<graphics::sprite_sheet> sheet, const math::rect& sprite_frame, std::uint32_t accuracy = hitbox_constructor::accuracy) -> hitbox;
}
