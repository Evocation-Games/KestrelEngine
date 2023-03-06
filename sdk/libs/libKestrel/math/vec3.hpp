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
#include <libKestrel/math/vec2.hpp>

namespace kestrel::math
{
    struct vec3
    {
        vec3();
        explicit vec3(simd::float32 v);
        explicit vec3(float u);
        vec3(float x, float y, float z);
        vec3(vec3&& v) noexcept = default;
        vec3(const vec3& v) = default;
        explicit vec3(const vec2& v, float z = 0.f);

        [[nodiscard]] auto x() const -> float;
        [[nodiscard]] auto y() const -> float;
        [[nodiscard]] auto z() const -> float;

        auto operator= (const vec3& v) -> vec3& = default;
        auto operator= (float s) -> vec3&;

        auto operator+ (float f) const -> vec3;
        auto operator- (float f) const -> vec3;
        auto operator* (float f) const -> vec3;
        auto operator/ (float f) const -> vec3;

        auto operator+ (const vec3& v) const -> vec3;
        auto operator- (const vec3& v) const -> vec3;
        auto operator* (const vec3& v) const -> vec3;
        auto operator/ (const vec3& v) const -> vec3;

        [[nodiscard]] auto cross(const vec3& v) const -> vec3;
        [[nodiscard]] auto dot(const vec3& v) const -> float;
        [[nodiscard]] auto lerp(const vec3& v, float t) const -> vec3;
        [[nodiscard]] auto magnitude() const -> float;
        [[nodiscard]] auto unit() const -> vec3;

    private:
        simd::float32 m_value;
    };
}