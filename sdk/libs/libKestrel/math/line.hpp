// Copyright (c) 2023 Tom Hancocks
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

#include <libKestrel/math/point.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/lua/runtime/runtime.hpp>

namespace kestrel::math
{
    struct lua_api(Line, Available_0_8) line
    {
        has_constructable_lua_api(line);

        lua_data(p, mutable, Available_0_8) point p;
        lua_data(q, mutable, Available_0_8) point q;

        lua_constructor(Available_0_8) line(const point& p, const point& q);

        lua_function(multiply, Available_0_8) auto operator*(float f) const -> line;
        lua_function(divide, Available_0_8) auto operator/(float f) const -> line;

        lua_function(intersects, Available_0_8) [[nodiscard]] auto intersects(const line& l) const -> bool;

    };
}