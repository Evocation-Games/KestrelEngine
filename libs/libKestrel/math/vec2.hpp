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
#include <cmath>

#include <libKestrel/math/point.hpp>
#include <libKestrel/math/size.hpp>

namespace kestrel::math
{
    struct vec2
    {
        float x { 0.f }, y { 0.f };

        vec2() = default;
        explicit vec2(float u) : x(u), y(u) {};
        vec2(float x, float y) : x(x), y(y) {};
        vec2(vec2&& v) noexcept = default;
        vec2(const vec2& v) = default;

        explicit vec2(const point& p) : x(static_cast<float>(p.x)), y(static_cast<float>(p.y)) {};
        explicit vec2(const size& s) : x(static_cast<float>(s.width)), y(static_cast<float>(s.height)) {};

        auto operator= (const vec2& v) -> vec2& = default;

        auto operator= (float s) -> vec2&
        {
            x = y = s;
            return *this;
        }

        auto operator+ (float f) const -> vec2 { return { x + f, y + f }; }
        auto operator- (float f) const -> vec2 { return { x - f, y - f }; }
        auto operator* (float f) const -> vec2 { return { x * f, y * f }; }
        auto operator/ (float f) const -> vec2 { return { x / f, y / f }; }

        auto operator+ (const vec2& v) const -> vec2 { return { x + v.x, y + v.y }; }
        auto operator- (const vec2& v) const -> vec2 { return { x - v.x, y - v.y }; }
        auto operator* (const vec2& v) const -> vec2 { return { x * v.x, y * v.y }; }
        auto operator/ (const vec2& v) const -> vec2 { return { x / v.x, y / v.y }; }

        [[nodiscard]] inline auto to_point() const -> point { return { x, y }; }
        [[nodiscard]] inline auto to_size() const -> size { return { x, y }; }

        [[nodiscard]] auto cross(const vec2& v) const -> vec2
        {
            return { x * v.y, y * v.x };
        }

        [[nodiscard]] auto dot(const vec2& v) const -> float
        {
            return (x * v.x) + (y * v.y);
        }

        [[nodiscard]] auto lerp(const vec2& v, float t) const -> vec2
        {
            return *this + (v - *this) * t;
        }

        [[nodiscard]] auto magnitude() const -> float
        {
            return std::sqrt(dot(*this));
        }

        [[nodiscard]] auto unit() const -> vec2
        {
            return *this / magnitude();
        }

        [[nodiscard]] auto angle_to(const vec2& v) const -> float
        {
            return static_cast<float>(std::atan2(v.y - y, v.x - x) * 180.0) / M_PI;
        }

    };
}