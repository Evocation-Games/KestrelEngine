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
#include <libKestrel/lua/scripting.hpp>

namespace kestrel::ui
{
    /**
     * Rperesents how content should be aligned vertically.
     */
    enum class lua_api(UI.VerticalAlignment, Available_0_8) vertical_alignment : std::int64_t
    {
        /**
         * Align content to the top of the frame.
         */
        top lua_case(Top, Available_0_8) = -1,

        /**
         * Align content to the middle of the frame.
         */
        middle lua_case(Middle, Available_0_8) = 0,

        /**
         * Align content to the bottom of the frame.
         */
        bottom lua_case(Bottom, Available_0_8) = 1
    };

    /**
     * Represents how content should be aligned horizontally.
     */
    enum class lua_api(UI.HorizontalAlignment, Available_0_8) horizontal_alignment : std::int64_t
    {
        left lua_case(Left, Available_0_8) = -1,
        center lua_case(Center, Available_0_8) = 0,
        right lua_case(Right, Available_0_8) = 1
    };
}