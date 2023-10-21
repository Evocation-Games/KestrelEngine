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
#include <libSIMD/float32.hpp>
#include <libMath/types/vec2.hpp>

namespace math
{
    struct line
    {
    public:
        line() = default;
        line(vec2 p, vec2 q) : m_p(p), m_q(q) {};
        line(line&& l) noexcept = default;
        line(const line& l) = default;

        [[nodiscard]] inline auto p() const { return m_p; }
        [[nodiscard]] inline auto q() const { return m_q; }

        [[nodiscard]] inline auto operator* (float f) const -> line
        {
            auto x = simd::float32::lower_upper_merge(m_p.m_value.lower(), m_q.m_value.upper()) * f;
            return { vec2(x.lower().swapped()), vec2(x.upper().swapped()) };
        }

        [[nodiscard]] inline auto operator/ (float f) const -> line
        {
            auto x = simd::float32::lower_upper_merge(m_p.m_value.lower(), m_q.m_value.upper()) / f;
            return { vec2(x.lower().swapped()), vec2(x.upper().swapped()) };
        }

        [[nodiscard]] inline auto intersects(const line& l) const -> bool
        {
            const auto determinant = [](float a, float b, float c, float d) -> float {
                auto r = simd::float32(a, b) * simd::float32(d, c);
                return r[0] - r[1];
            };

            auto det_l1 = determinant(m_p.x(), m_p.y(), m_q.x(), m_q.y());
            auto det_l2 = determinant(l.m_p.x(), l.m_p.y(), l.m_q.x(), l.m_q.y());

            auto mx = simd::float32(m_p.x(), l.m_p.x(), m_p.y(), l.m_p.y());
            auto nx = simd::float32(m_q.x(), l.m_q.x(), m_q.y(), l.m_q.y());
            auto rx = mx - nx;
            auto xnom = determinant(det_l1, rx[0], det_l2, rx[1]);
            auto ynom = determinant(det_l1, rx[2], det_l2, rx[3]);
            auto denom = determinant(rx[0], rx[2], rx[1], rx[3]);

            if (denom == 0.f) {
                return false;
            }

            auto coords = simd::float32(xnom, ynom) / denom;
            if (!std::isfinite(coords[0]) || !std::isfinite(coords[1])) {
                return false;
            }

            return true;
        }

    private:
        vec2 m_p { 0.f };
        vec2 m_q { 1.f };
    };
}