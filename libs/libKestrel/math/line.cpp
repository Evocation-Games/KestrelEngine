// Copyright (c) 2020 Tom Hancocks
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

#include <cmath>
#include <libKestrel/math/line.hpp>

// MARK: - Construction

kestrel::math::line::line(const point &p, const point &q)
    : p(p), q(q)
{
}

// MARK: - Accessors

auto kestrel::math::line::get_p() const -> math::point
{
    return p;
}

auto kestrel::math::line::set_p(const point& p) -> void
{
    this->p = p;
}

auto kestrel::math::line::get_q() const -> point
{
    return q;
}

auto kestrel::math::line::set_q(const point& q) -> void
{
    this->q = q;
}

// MARK: - Operators

auto kestrel::math::line::operator*(double f) const -> line
{
    return { p * f, q * f };
}

auto kestrel::math::line::operator/(double f) const -> line
{
    return { p / f, q / f };
}


// MARK: - Functions

auto kestrel::math::line::intersects(const line& l) const -> bool
{
    const auto determinant = [](const double& a, const double& b, const double& c, const double& d) -> double {
        return (a * d) - (b * c);
    };

    auto det_l1 = determinant(p.x, p.y, q.x, q.y);
    auto det_l2 = determinant(l.p.x, l.p.y, l.q.x, l.q.y);
    auto x1mx2 = p.x - q.x;
    auto x3mx4 = l.p.x - l.q.x;
    auto y1my2 = p.y - q.y;
    auto y3my4 = l.p.y - l.q.y;

    auto xnom = determinant(det_l1, x1mx2, det_l2, x3mx4);
    auto ynom = determinant(det_l1, y1my2, det_l2, y3my4);
    auto denom = determinant(x1mx2, y1my2, x3mx4, y3my4);

    if (denom == 0.0) {
        return false;
    }

    auto x = xnom / denom;
    auto y = ynom / denom;
    if (!std::isfinite(x) || !std::isfinite(y)) {
        return false;
    }

    // TODO: Pass out the coordinate to the caller.
    return true;
}
