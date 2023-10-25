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
    struct rect
    {
    public:
        rect() = default;
        rect(vec2 o, vec2 s) : m_value(o.x(), o.y(), s.x(), s.y()) {};
        rect(const rect& r) = default;
        rect(rect&& r) noexcept = default;
        explicit rect(simd::float32 v) : m_value(v) {};

        auto operator= (const rect& r) -> rect& = default;

        [[nodiscard]] auto operator== (const rect& r) const -> bool { return m_value == r.m_value; }

        [[nodiscard]] inline auto origin() const -> vec2 { return vec2(m_value.lower()); }
        [[nodiscard]] inline auto size() const -> vec2 { return vec2(m_value.upper()); }

        [[nodiscard]] inline auto x() const -> float { return origin().x(); }
        [[nodiscard]] inline auto y() const -> float { return origin().y(); }
        [[nodiscard]] inline auto width() const -> float { return size().x(); }
        [[nodiscard]] inline auto height() const -> float { return size().y(); }
        [[nodiscard]] inline auto max_x() const -> float { return x() + width(); }
        [[nodiscard]] inline auto max_y() const -> float { return y() + height(); }

        [[nodiscard]] inline auto area() const -> float { return width() * height(); }

        [[nodiscard]] inline auto add_origin(vec2 o) const -> rect { return { origin() + o, size() }; }
        [[nodiscard]] inline auto add_size(vec2 s) const -> rect { return { origin(), size() + s }; }
        [[nodiscard]] inline auto subtract_origin(vec2 o) const -> rect { return { origin() - o, size() }; }
        [[nodiscard]] inline auto subtract_size(vec2 s) const -> rect { return { origin(), size() - s }; }

        [[nodiscard]] inline auto round() const -> rect { return rect(m_value.round()); }
        [[nodiscard]] inline auto floor() const -> rect { return rect(m_value.floor()); }
        [[nodiscard]] inline auto ceil() const -> rect { return rect(m_value.ceil()); }

        [[nodiscard]] inline auto inset(float d) const -> rect
        {
            return rect(m_value - simd::float32(-1, -1, 2, 2) * d);
        }

        [[nodiscard]] inline auto centered(vec2 bounds) const -> rect
        {
            return {
                (bounds - size()) / 2.f,
                size()
            };
        }

        [[nodiscard]] inline auto intersects(rect r) const -> bool
        {
            auto r1 = simd::float32::lower_upper_merge(m_value.lower(), m_value.upper() + m_value.lower().swapped());
            auto r2 = simd::float32::lower_upper_merge(m_value.lower(), m_value.upper() + m_value.lower().swapped());
            auto rr = r2 - r1;
            return (rr[0] >= 0) && (rr[1] <= 0) && (rr[2] >= 0) && (rr[3] <= 0);
        }

        [[nodiscard]] inline auto contains(rect r) const -> bool
        {
            auto p = (m_value + m_value.lower().swapped());
            return contains(vec2(p.lower() + p.lower().swapped()))
                   && contains(vec2(p.upper().swapped() + p.upper()));
        }

        [[nodiscard]] inline auto contains(vec2 v) const -> bool
        {
            auto o = origin() - v;
            auto r = rect(o, size() + o).m_value;
            return (r[0] < 0) && (r[1] < 0) && (r[2] >= 0) && (r[3] >= 0);
        }

    private:
        simd::float32 m_value;
    };
}
