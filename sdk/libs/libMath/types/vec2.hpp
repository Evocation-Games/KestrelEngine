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

#include <cmath>
#include <numbers>
#include <libSIMD/float32.hpp>

namespace math
{
    namespace geometry {
        struct triangle;
        struct polygon;
    }

    struct vec2
    {
    public:
        vec2() = default;

        explicit vec2(simd::float32 v) : m_value(v) {}
        explicit vec2(float u) : m_value(simd::float32::constant(u)) {}
        vec2(float x, float y) : m_value(x, y, x, y) {}
        vec2(vec2&& v) noexcept = default;
        vec2(const vec2& v) = default;

        [[nodiscard]] inline auto x() const -> float { return m_value[0]; }
        [[nodiscard]] inline auto y() const -> float { return m_value[1]; }

        auto operator= (const vec2& v) -> vec2& = default;
        auto operator= (float s) -> vec2&
        {
            m_value = simd::float32::constant(s);
            return *this;
        }

        [[nodiscard]] inline auto operator== (vec2 v) const -> bool
        {
            return (m_value[0] == v.m_value[0]) && (m_value[1] == v.m_value[1]);
        }

        [[nodiscard]] inline auto operator!= (vec2 v) const -> bool
        {
            return (m_value[0] != v.m_value[0]) && (m_value[1] != v.m_value[1]);
        }

        [[nodiscard]] inline auto operator+ (float f) const -> vec2 { return vec2(m_value + f); }
        [[nodiscard]] inline auto operator- (float f) const -> vec2 { return vec2(m_value - f); }
        [[nodiscard]] inline auto operator* (float f) const -> vec2 { return vec2(m_value * f); }
        [[nodiscard]] inline auto operator/ (float f) const -> vec2 { return vec2(m_value / f); }

        [[nodiscard]] inline auto operator+ (vec2 v) const -> vec2 { return vec2(m_value + v.m_value); }
        [[nodiscard]] inline auto operator- (vec2 v) const -> vec2 { return vec2(m_value - v.m_value); }
        [[nodiscard]] inline auto operator* (vec2 v) const -> vec2 { return vec2(m_value * v.m_value); }
        [[nodiscard]] inline auto operator/ (vec2 v) const -> vec2 { return vec2(m_value / v.m_value); }

        [[nodiscard]] auto round() const -> vec2
        {
            return vec2(m_value.round());
        }

        [[nodiscard]] auto floor() const -> vec2
        {
            return vec2(m_value.floor());
        }

        [[nodiscard]] auto ceil() const -> vec2
        {
            return vec2(m_value.ceil());
        }

        [[nodiscard]] auto cross(vec2 v) const -> vec2
        {
            return vec2(m_value * v.m_value.reversed());
        }

        [[nodiscard]] auto dot(vec2 v) const -> float
        {
            auto xx = m_value * v.m_value;
            return (xx + xx.reversed())[0];
        }

        [[nodiscard]] auto lerp(vec2 v, float t) const -> vec2
        {
            return vec2(m_value + ((v.m_value - m_value) * t));
        }

        [[nodiscard]] auto magnitude() const -> float
        {
            return std::sqrt(dot(*this));
        }

        [[nodiscard]] auto distance_to(vec2 v) const -> float
        {
            auto xx = (v.m_value - m_value).pow(2);
            return std::sqrtf((xx + xx.reversed())[0]);
        }

        [[nodiscard]] auto unit() const -> vec2
        {
            return *this / magnitude();
        }

        [[nodiscard]] auto angle_to(vec2 v) const -> float
        {
            auto xx = v.m_value - m_value;
            return (::atan2f(xx[0], xx[1]) * 180.f) / std::numbers::pi_v<float>;
        }

        friend struct line;
        friend struct geometry::triangle;
        friend struct geometry::polygon;

    private:
        simd::float32 m_value;
    };
}