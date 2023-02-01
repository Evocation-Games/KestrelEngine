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

#include <libKestrel/math/vec2.hpp>

namespace kestrel::math
{
    struct triangle
    {
        vec2 a, b, c;

        triangle() = default;
        triangle(const vec2& a, const vec2& b, const vec2& c);
        triangle(triangle&& tri) noexcept = default;
        triangle(const triangle& tri) = default;

        auto operator=(const triangle& tri) -> triangle& = default;

        auto operator+(const math::point& p) const -> triangle;
        auto operator+(const math::vec2& v) const -> triangle;
        auto operator-(const math::point& p) const -> triangle;
        auto operator-(const math::vec2& v) const -> triangle;
    };
}