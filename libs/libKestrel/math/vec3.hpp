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
#include <libKestrel/math/vec2.hpp>

namespace kestrel::math
{
    struct vec3
    {
        float x { 0.f }, y { 0.f }, z { 0.f };

        vec3() = default;
        explicit vec3(float u) : x(u), y(u), z(u) {};
        vec3(float x, float y, float z) : x(x), y(y), z(z) {};
        vec3(vec3&& v) noexcept = default;
        vec3(const vec3& v) = default;
        vec3(const vec2& v, float z = 0.f) : x(v.x), y(v.y), z(z) {};

        auto operator= (const vec3& v) -> vec3&
        {
            x = v.x; y = v.y; z = v.z;
            return *this;
        }

        auto operator= (float s) -> vec3&
        {
            x = y = z = s;
            return *this;
        }

        auto operator+ (float f) const -> vec3 { return { x + f, y + f, z + f }; }
        auto operator- (float f) const -> vec3 { return { x - f, y - f, z - f }; }
        auto operator* (float f) const -> vec3 { return { x * f, y * f, z * f }; }
        auto operator/ (float f) const -> vec3 { return { x / f, y / f, z / f }; }

        auto operator+ (const vec3& v) const -> vec3 { return { x + v.x, y + v.y, z + v.z }; }
        auto operator- (const vec3& v) const -> vec3 { return { x - v.x, y - v.y, z - v.z }; }

        [[nodiscard]] auto cross(const vec3& v) const -> vec3
        {
            return { (y * v.z) - (z * v.y),
                     (z * v.x) - (x * v.z),
                     (x * v.y) - (y * v.x) };
        }

        [[nodiscard]] auto dot(const vec3& v) const -> float
        {
            return (x * v.x) + (y * v.y) + (z * v.z);
        }

        [[nodiscard]] auto lerp(const vec3& v, float t) const -> vec3
        {
            return *this + (v - *this) * t;
        }

        [[nodiscard]] auto magnitude() const -> float
        {
            return std::sqrt(dot(*this));
        }

    };
}