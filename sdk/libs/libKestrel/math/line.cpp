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

#include <libKestrel/math/line.hpp>
#include <libSIMD/float32.hpp>
#include <cmath>

// MARK: - Construction

kestrel::math::line::line(const point &p, const point &q)
    : p(p), q(q)
{}

// MARK: - Operators

auto kestrel::math::line::operator*(float f) const -> line
{
    auto x = simd::float32::lower_upper_merge(p.m_value.lower(), q.m_value.upper()) * f;
    return { point(x.lower().swapped()), point(x.upper().swapped()) };
}

auto kestrel::math::line::operator/(float f) const -> line
{
    auto x = simd::float32::lower_upper_merge(p.m_value.lower(), q.m_value.upper()) / f;
    return { point(x.lower().swapped()), point(x.upper().swapped()) };
}

// MARK: - Operations

auto kestrel::math::line::intersects(const line &l) const -> bool
{
    const auto determinant = [](float a, float b, float c, float d) -> float {
        auto r = simd::float32(a, b) * simd::float32(d, c);
        return r[0] - r[1];
    };

    auto det_l1 = determinant(p.x(), p.y(), q.x(), q.y());
    auto det_l2 = determinant(l.p.x(), l.p.y(), l.q.x(), l.q.y());

    auto mx = simd::float32(p.x(), l.p.x(), p.y(), l.p.y());
    auto nx = simd::float32(q.x(), l.q.x(), q.y(), l.q.y());
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