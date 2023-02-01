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
#include <libKestrel/math/rect.hpp>
#include <libKestrel/math/point.hpp>

// MARK: - Construction

kestrel::math::rect::rect()
    : m_value()
{}

kestrel::math::rect::rect(const struct point& o, const struct size& s)
    : m_value(o.x(), o.y(), s.width(), s.height())
{}

kestrel::math::rect::rect(float x, float y, float w, float h)
    : m_value(x, y, w, h)
{}

kestrel::math::rect::rect(SIMD::float4 v)
    : m_value(v)
{}

kestrel::math::rect::rect(const graphite::quickdraw::rect<std::int16_t> &r)
    : m_value(
        static_cast<float>(r.origin.x), static_cast<float>(r.origin.y),
        static_cast<float>(r.size.width), static_cast<float>(r.size.height)
    )
{}

auto kestrel::math::rect::macintosh_rect(float top, float left, float bottom, float right) -> rect
{
    return { left, top, right - left, bottom - top };
}

// MARK: - Operators

auto kestrel::math::rect::operator+(const struct point& p) const -> rect
{
    return rect(m_value + p.m_value.lower());
}

auto kestrel::math::rect::operator+(const struct size& s) const -> rect
{
    return rect(m_value + s.m_value.upper());
}

auto kestrel::math::rect::operator-(const struct point& p) const -> rect
{
    return rect(m_value - p.m_value.lower());
}

auto kestrel::math::rect::operator-(const struct size& s) const -> rect
{
    return rect(m_value - s.m_value.upper());
}

auto kestrel::math::rect::operator*(float f) const -> rect
{
    return rect(m_value * f);
}

auto kestrel::math::rect::operator/(float f) const -> rect
{
    return rect(m_value / f);
}

auto kestrel::math::rect::operator==(const rect& r) const -> bool
{
    return m_value == r.m_value;
}

auto kestrel::math::rect::operator!=(const rect& r) const -> bool
{
    return m_value != r.m_value;
}

auto kestrel::math::rect::add_point(const struct point& p) const -> rect
{
    return operator+(p);
}

auto kestrel::math::rect::add_size(const struct size& s) const -> rect
{
    return operator+(s);
}

auto kestrel::math::rect::subtract_point(const struct point& p) const -> rect
{
    return operator-(p);
}

auto kestrel::math::rect::subtract_size(const struct size& s) const -> rect
{
    return operator-(s);
}

// MARK: - Operations

auto kestrel::math::rect::round() const -> rect
{
    return rect(m_value.round());
}

auto kestrel::math::rect::floor() const -> rect
{
    return rect(m_value.floor());
}

auto kestrel::math::rect::ceil() const -> rect
{
    return rect(m_value.ceil());
}

auto kestrel::math::rect::area() const -> float
{
    return math::size(m_value.upper()).area();
}

auto kestrel::math::rect::contains_point(const point& p) const -> bool
{
    auto r = (m_value + m_value.lower().swapped()) - p.m_value;
    return (r[0] < 0) && (r[1] < 0) && (r[2] >= 0) && (r[3] >= 0);
}

auto kestrel::math::rect::contains_rect(const rect& r) const -> bool
{
    auto p = (m_value + m_value.lower().swapped());
    return contains_point(point(p.lower() + p.lower().swapped()))
        && contains_point(point(p.upper().swapped() + p.upper()));
}

auto kestrel::math::rect::intersects(const rect& r) const -> bool
{
    auto r1 = m_value + m_value.lower().swapped();
    auto r2 = r.m_value + r.m_value.lower().swapped();
    auto rr = r1 - r2;
    return (rr[0] >= 0) && (rr[1] < 0) && (rr[2] >= 0) && (rr[3] < 0);
}

// MARK: - Lua Accessors

auto kestrel::math::rect::set_origin(const struct point& origin) -> void
{
    m_value = m_value.upper() + origin.m_value.lower();
}

auto kestrel::math::rect::origin() const -> struct point
{
    return point(m_value.lower() + m_value.lower().swapped());
}

auto kestrel::math::rect::set_size(const struct size& size) -> void
{
    m_value = m_value.lower() + size.m_value.upper();
}

auto kestrel::math::rect::size() const -> struct size
{
    return math::size(m_value.upper() + m_value.upper().swapped());
}

auto kestrel::math::rect::set_x(float x) -> void
{
    m_value.set(0, x);
}

auto kestrel::math::rect::x() const -> float
{
    return m_value[0];
}

auto kestrel::math::rect::max_x() const -> float
{
    return (m_value + m_value.upper().swapped())[0];
}

auto kestrel::math::rect::set_y(float y) -> void
{
    m_value.set(1, y);
}

auto kestrel::math::rect::y() const -> float
{
    return m_value[1];
}

auto kestrel::math::rect::max_y() const -> float
{
    return (m_value + m_value.upper().swapped())[1];
}

auto kestrel::math::rect::set_width(float width) -> void
{
    m_value.set(2, width);
}

auto kestrel::math::rect::width() const -> float
{
    return m_value[2];
}

auto kestrel::math::rect::set_height(float height) -> void
{
    m_value.set(3, height);
}

auto kestrel::math::rect::height() const -> float
{
    return m_value[3];
}

auto kestrel::math::rect::inset(float amount) const -> rect
{
    auto delta = SIMD::float4(1, 1, -2, -2) * amount;
    return rect(m_value * delta);
}
