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
#include <cmath>
#include <numbers>
#include <libMath/types/vec2.hpp>
#include <libMath/types/vec3.hpp>

namespace math
{
    struct vec4
    {
    public:
        vec4() = default;

        explicit vec4(simd::float32 v) : m_value(v) {}
        explicit vec4(float u) : m_value(simd::float32::constant(u)) {}
        vec4(float x, float y, float z, float w) : m_value(x, y, z, w) {}
        vec4(vec4&& v) noexcept = default;
        vec4(const vec4& v) = default;
        explicit vec4(const vec2& v, float z = 0.f, float w = 0.f) : m_value(v.x(), v.y(), z, w) {}
        explicit vec4(const vec3& v, float w = 0.f) : m_value(v.x(), v.y(), v.z(), w) {}

        [[nodiscard]] inline auto x() const -> float { return m_value[0]; }
        [[nodiscard]] inline auto y() const -> float { return m_value[1]; }
        [[nodiscard]] inline auto z() const -> float { return m_value[2]; }
        [[nodiscard]] inline auto w() const -> float { return m_value[3]; }

        auto operator= (const vec4& v) -> vec4& = default;
        auto operator= (float s) -> vec4&
        {
            m_value = simd::float32::constant(s);
            return *this;
        }

        auto operator+ (float f) const -> vec4 { return vec4(m_value + f); }
        auto operator- (float f) const -> vec4 { return vec4(m_value - f); }
        auto operator* (float f) const -> vec4 { return vec4(m_value * f); }
        auto operator/ (float f) const -> vec4 { return vec4(m_value / f); }

        auto operator+ (const vec4& v) const -> vec4 { return vec4(m_value + v.m_value); }
        auto operator- (const vec4& v) const -> vec4 { return vec4(m_value - v.m_value); }
        auto operator* (const vec4& v) const -> vec4 { return vec4(m_value * v.m_value); }
        auto operator/ (const vec4& v) const -> vec4 { return vec4(m_value / v.m_value); }

        [[nodiscard]] auto cross(const vec4& v) const -> vec4
        {
            return vec4(m_value * v.m_value.reversed());
        }

        [[nodiscard]] auto dot(const vec4& v) const -> float
        {
            auto xx = m_value * v.m_value;
            return (xx + xx.reversed())[0];
        }

        [[nodiscard]] auto lerp(const vec4& v, float t) const -> vec4
        {
            return vec4(m_value + ((v.m_value - m_value) * t));
        }

        [[nodiscard]] auto magnitude() const -> float
        {
            return std::sqrt(dot(*this));
        }

        [[nodiscard]] auto unit() const -> vec4
        {
            return *this / magnitude();
        }

        [[nodiscard]] auto angle_to(const vec4& v) const -> float
        {
            auto xx = v.m_value - m_value;
            return (::atan2f(xx[0], xx[1]) * 180.f) / std::numbers::pi_v<float>;
        }

        [[nodiscard]] auto round() const -> vec4
        {
            return vec4(m_value.round());
        }

    private:
        simd::float32 m_value;
    };
}