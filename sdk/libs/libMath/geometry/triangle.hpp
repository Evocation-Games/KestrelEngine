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

#include <libMath/types/vec2.hpp>

namespace math::geometry
{
    struct triangle
    {
    public:
        triangle() = default;
        triangle(vec2 a, vec2 b, vec2 c);
        triangle(triangle&& t) noexcept = default;
        triangle(const triangle& t) = default;

        auto operator= (const triangle& t) -> triangle& = default;

        [[nodiscard]] inline auto operator+ (vec2 v) const -> triangle
        {
            auto dv = (m_a.m_value.lower() + m_b.m_value.lower().swapped()) + v.m_value;
            auto du = (m_c.m_value.lower()) + v.m_value;
            return {
                vec2(dv.lower() + dv.lower().swapped()),
                vec2(dv.upper() + dv.upper().swapped()),
                vec2(du.lower() + du.lower().swapped())
            };
        }

        [[nodiscard]] inline auto operator- (vec2 v) const -> triangle
        {
            auto dv = (m_a.m_value.lower() + m_b.m_value.lower().swapped()) - v.m_value;
            auto du = (m_c.m_value.lower()) - v.m_value;
            return {
                vec2(dv.lower() + dv.lower().swapped()),
                vec2(dv.upper() + dv.upper().swapped()),
                vec2(du.lower() + du.lower().swapped())
            };
        }

        [[nodiscard]] inline auto intersects(triangle t) const -> bool
        {
            constexpr float eps = 0.f;
            if (check_winding(m_a, m_b, m_c, false) || check_winding(t.m_a, t.m_b, t.m_c, false)) {
                return false;
            }

            if (boundary_collision(m_a, m_b, t.m_a, eps) && boundary_collision(m_a, m_b, t.m_b, eps) && boundary_collision(m_a, m_b, t.m_c, eps)) {
                return false;
            }
            if (boundary_collision(m_b, m_c, t.m_a, eps) && boundary_collision(m_b, m_c, t.m_b, eps) && boundary_collision(m_b, m_c, t.m_c, eps)) {
                return false;
            }
            if (boundary_collision(m_c, m_a, t.m_a, eps) && boundary_collision(m_c, m_a, t.m_b, eps) && boundary_collision(m_c, m_a, t.m_c, eps)) {
                return false;
            }

            if (boundary_collision(t.m_a, t.m_b, m_a, eps) && boundary_collision(t.m_a, t.m_b, m_b, eps) && boundary_collision(t.m_a, t.m_b, m_c, eps)) {
                return false;
            }
            if (boundary_collision(t.m_b, t.m_c, m_a, eps) && boundary_collision(t.m_b, t.m_c, m_b, eps) && boundary_collision(t.m_b, t.m_c, m_c, eps)) {
                return false;
            }
            if (boundary_collision(t.m_c, t.m_a, m_a, eps) && boundary_collision(t.m_c, t.m_a, m_b, eps) && boundary_collision(t.m_c, t.m_a, m_c, eps)) {
                return false;
            }

            return true;
        }

    private:
        [[nodiscard]] static inline auto det(vec2 a, vec2 b, vec2 c) -> float
        {
            auto r = (simd::float32(b.y(), c.y(), a.y()) - simd::float32(c.y(), a.y(), b.y()))
                     * simd::float32(a.x(), b.x(), c.x());
            return r[0] + r[1] + r[2];
        }

        [[nodiscard]] static inline auto check_winding(vec2 a, vec2 b, vec2 c, bool allow_reversed) -> bool
        {
            auto det = triangle::det(a, b, c);
            if (det >= 0.0 || allow_reversed) {
                return true;
            }
            return false;
        }

        [[nodiscard]] static inline auto boundary_collision(vec2 a, vec2 b, vec2 c, float eps) -> bool
        {
            return triangle::det(a, b, c) < eps;
        }

    private:
        vec2 m_a { 0.f };
        vec2 m_b { 0.f };
        vec2 m_c { 0.f };
    };
}
