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
#include <libKestrel/ui/layout/aspect_ratio.hpp>
#include <libKestrel/lua/scripting.hpp>

namespace kestrel::ui::layout
{
    enum class lua_api(ScalingMode, Available_0_8) scaling_mode : std::int8_t
    {
        normal lua_case(Normal, Available_0_8) = 0,
        aspect_fill lua_case(AspectFill, Available_0_8) = 1,
        aspect_fit lua_case(AspectFit, Available_0_8) = 2,
        scale_fill lua_case(ScaleFill, Available_0_8) = 3,
        scale_fit lua_case(ScaleFit, Available_0_8) = 4
    };

    auto calculate_size(enum scaling_mode mode, const math::size& container, const math::size& size, aspect_ratio_t ratio) -> math::size;

}
