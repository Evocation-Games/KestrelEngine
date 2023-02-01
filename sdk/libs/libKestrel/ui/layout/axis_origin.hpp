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

#include <cstdint>
#include <libKestrel/math/size.hpp>
#include <libKestrel/math/point.hpp>
#include <libKestrel/lua/scripting.hpp>

namespace kestrel::ui::layout
{
    enum class lua_api(AxisOrigin, Available_0_8) axis_origin : std::int8_t
    {
        top_left lua_case(TopLeft, Available_0_8) = 0,
        center_left lua_case(CenterLeft, Available_0_8) = 1,
        bottom_left lua_case(BottomLeft, Available_0_8) = 2,
        top_center lua_case(TopCenter, Available_0_8) = 3,
        center lua_case(Center, Available_0_8) = 4,
        bottom_center lua_case(BottomCenter, Available_0_8) = 5,
        top_right lua_case(TopRight, Available_0_8) = 6,
        center_right lua_case(CenterRight, Available_0_8) = 7,
        bottom_right lua_case(BottomRight, Available_0_8) = 8,
    };

    auto origin_for_axis(const math::size& size, enum axis_origin axis) -> math::point;
    auto position_for_axis(const math::size& container, enum axis_origin axis, const math::size& size) -> math::point;
}