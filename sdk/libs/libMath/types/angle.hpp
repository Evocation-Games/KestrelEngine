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
#include <libMath/trig/fast_trig.hpp>
#include <libMath/types/vec2.hpp>

namespace math
{
    struct angle
    {
    public:
        static constexpr float pi = std::numbers::pi_v<float>;
        static constexpr float max_r = pi * 2.f;

        angle() = default;
        explicit angle(float theta) : m_theta(convert_degrees_to_radians(theta)) { normalize(); }
        angle(angle&& a) noexcept = default;
        angle(const angle& a) = default;

        [[nodiscard]] inline auto degrees() const -> float { return convert_radians_to_degrees(m_theta); }
        [[nodiscard]] inline auto radians() const -> float { return m_theta; }

        [[nodiscard]] inline auto sin(float magnitude = 1.f) const -> float
        {
            return fast::sin(degrees()) * magnitude;
        }

        [[nodiscard]] inline auto cos(float magnitude = 1.f) const -> float
        {
            return fast::cos(degrees()) * magnitude;
        }

        [[nodiscard]] inline auto operator+ (angle a) const -> angle { return angle(m_theta + a.m_theta); }
        [[nodiscard]] inline auto operator- (angle a) const -> angle { return angle(m_theta - a.m_theta); }
        [[nodiscard]] inline auto operator+ (float d) const -> angle { return angle(m_theta + convert_degrees_to_radians(d)); }
        [[nodiscard]] inline auto operator- (float d) const -> angle { return angle(m_theta - convert_degrees_to_radians(d)); }

        [[nodiscard]] auto vector(float magnitude) const -> vec2
        {
            return (vec2(cos(magnitude), sin(magnitude)) * 1000.f).round() / 1000.0;
        }

        [[nodiscard]] auto opposite() const -> angle { return rotated_r(pi); }
        [[nodiscard]] auto rotated_r(float r) const -> angle { return *this + r; }
        [[nodiscard]] auto rotated(float d) const -> angle { return rotated_r(convert_degrees_to_radians(d)); }

        auto normalize() -> void
        {
            m_theta = ::fmodf(m_theta, max_r);
            if (m_theta < 0) {
                m_theta += max_r;
            }
        }

        [[nodiscard]] auto distance_r(angle a) const -> float
        {
            auto phi = ::fmodf((a - *this + pi).m_theta, max_r) + pi;
            return phi > pi ? max_r - phi : phi;
        }

        [[nodiscard]] auto is_opposing(angle a, float tolerance = 5.f) const -> bool
        {
            return std::fabsf(distance_r(a)) >= (pi - convert_degrees_to_radians(tolerance));
        }

        [[nodiscard]] auto is_equal(angle a, float tolerance = 5.f) const -> bool
        {
            return std::fabsf(distance_r(a)) <= convert_degrees_to_radians(tolerance);
        }

    private:
        static inline auto convert_degrees_to_radians(float d) -> float
        {
            return (d * pi) / 180.f;
        }

        static inline auto convert_radians_to_degrees(float r) -> float
        {
            return (r * 180.f) / pi;
        }

    private:
        float m_theta { 0.f };
    };
}
