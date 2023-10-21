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

#include <libSIMD/float32.hpp>
#include <libKestrel/math/point.hpp>
#include <libKestrel/math/size.hpp>

namespace kestrel::math
{
    struct vec2
    {
        vec2();
        explicit vec2(simd::float32 v);
        explicit vec2(float u);
        vec2(float x, float y);
        vec2(vec2&& v) noexcept = default;
        vec2(const vec2& v) = default;
        explicit vec2(const point& p);
        explicit vec2(const size& s);

        [[nodiscard]] auto x() const -> float;
        [[nodiscard]] auto y() const -> float;

        [[nodiscard]] auto to_point() const -> point;
        [[nodiscard]] auto to_size() const -> size;

        auto operator= (const vec2& v) -> vec2& = default;
        auto operator= (float s) -> vec2&;

        auto operator+ (float f) const -> vec2;
        auto operator- (float f) const -> vec2;
        auto operator* (float f) const -> vec2;
        auto operator/ (float f) const -> vec2;

        auto operator+ (const vec2& v) const -> vec2;
        auto operator- (const vec2& v) const -> vec2;
        auto operator* (const vec2& v) const -> vec2;
        auto operator/ (const vec2& v) const -> vec2;

        [[nodiscard]] auto cross(const vec2& v) const -> vec2;
        [[nodiscard]] auto dot(const vec2& v) const -> float;
        [[nodiscard]] auto lerp(const vec2& v, float t) const -> vec2;
        [[nodiscard]] auto magnitude() const -> float;
        [[nodiscard]] auto unit() const -> vec2;
        [[nodiscard]] auto angle_to(const vec2& v) const -> float;

        [[nodiscard]] auto round() const -> vec2;

        friend struct triangle;

    private:
        simd::float32 m_value;
    };
}