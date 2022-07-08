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
#include "math/vec2.hpp"
#include "math/vec3.hpp"

namespace math
{
    struct vec4
    {
        float x { 0.f }, y { 0.f }, z { 0.f }, w { 0.f };

        vec4() = default;
        explicit vec4(float u) : x(u), y(u), z(u), w(u) {};
        vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {};
        vec4(vec4&& v) noexcept = default;
        vec4(const vec4& v) = default;
        explicit vec4(const vec2& v, float z = 0.f, float w = 0.f) : x(v.x), y(v.y), z(z), w(w) {};
        explicit vec4(const vec3& v, float w = 0.f) : x(v.x), y(v.y), z(v.z), w(w) {};

        auto operator= (const vec4& v) -> vec4&
        {
            x = v.x; y = v.y; z = v.z; w = v.w;
            return *this;
        }

        auto operator= (float s) -> vec4&
        {
            x = y = z = w = s;
            return *this;
        }

        auto operator+ (float f) const -> vec4 { return { x + f, y + f, z + f, w + f }; }
        auto operator- (float f) const -> vec4 { return { x - f, y - f, z - f, w - f }; }
        auto operator* (float f) const -> vec4 { return { x * f, y * f, z * f, w * f }; }
        auto operator/ (float f) const -> vec4 { return { x / f, y / f, z / f, w / f }; }

        auto operator+ (const vec4& v) const -> vec4 { return { x + v.x, y + v.y, z + v.z, w + v.w }; }
        auto operator- (const vec4& v) const -> vec4 { return { x - v.x, y - v.y, z - v.z, w - v.w }; }

        [[nodiscard]] auto cross(const vec4& v) const -> vec4
        {
            return { (y * v.z) - (z * v.y),
                     (z * v.x) - (x * v.z),
                     (x * v.y) - (y * v.x),
                     0 };
        }

        [[nodiscard]] auto dot(const vec4& v) const -> float
        {
            return (x * v.x) + (y * v.y) + (z * v.z) + (w * v.w);
        }

        [[nodiscard]] auto lerp(const vec4& v, float t) const -> vec4
        {
            return *this + (v - *this) * t;
        }

        [[nodiscard]] auto magnitude() const -> float
        {
            return std::sqrt(dot(*this));
        }

    };
}