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
#include <libKestrel/math/vec3.hpp>

namespace kestrel::math
{
    struct vec4
    {
        vec4();
        explicit vec4(simd::float32 v);
        explicit vec4(float u);
        vec4(float x, float y, float z, float w);
        vec4(vec4&& v) noexcept = default;
        vec4(const vec4& v) = default;
        explicit vec4(const vec2& v, float z = 0.f, float w = 0.f);
        explicit vec4(const vec3& v, float w = 0.f);

        [[nodiscard]] auto x() const -> float;
        [[nodiscard]] auto y() const -> float;
        [[nodiscard]] auto z() const -> float;
        [[nodiscard]] auto w() const -> float;

        auto operator= (const vec4& v) -> vec4& = default;
        auto operator= (float s) -> vec4&;

        auto operator+ (float f) const -> vec4;
        auto operator- (float f) const -> vec4;
        auto operator* (float f) const -> vec4;
        auto operator/ (float f) const -> vec4;

        auto operator+ (const vec4& v) const -> vec4;
        auto operator- (const vec4& v) const -> vec4;
        auto operator* (const vec4& v) const -> vec4;
        auto operator/ (const vec4& v) const -> vec4;

        [[nodiscard]] auto cross(const vec4& v) const -> vec4;
        [[nodiscard]] auto dot(const vec4& v) const -> float;
        [[nodiscard]] auto lerp(const vec4& v, float t) const -> vec4;
        [[nodiscard]] auto magnitude() const -> float;
        [[nodiscard]] auto unit() const -> vec4;

    private:
        simd::float32 m_value;
    };
}