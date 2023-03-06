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

#include <libKestrel/math/point.hpp>
#include <cmath>
#include <numbers>

// MARK: - Construction

kestrel::math::point::point(float v)
    : m_value(simd::float32::constant(v))
{}

kestrel::math::point::point(const point &p)
    : m_value(p.m_value)
{}

kestrel::math::point::point(float x, float y)
    : m_value(x, y, x, y)
{}

kestrel::math::point::point(simd::float32 v)
    : m_value(v)
{}

// MARK: - Accessors

auto kestrel::math::point::x() const -> float
{
    return m_value[0];
}

auto kestrel::math::point::set_x(float x) -> void
{
    m_value.set(0, x);
    m_value.set(2, x);
}

auto kestrel::math::point::y() const -> float
{
    return m_value[1];
}

auto kestrel::math::point::set_y(float y) -> void
{
    m_value.set(1, y);
    m_value.set(3, y);
}

// MARK: - Operators

auto kestrel::math::point::operator+(const point& p) const -> point
{
    return point(m_value + p.m_value);
}

auto kestrel::math::point::operator-(const point& p) const -> point
{
    return point(m_value - p.m_value);
}

auto kestrel::math::point::operator*(const point& p) const -> point
{
    return point(m_value * p.m_value);
}

auto kestrel::math::point::operator/(const point& p) const -> point
{
    return point(m_value / p.m_value);
}

auto kestrel::math::point::operator*(float f) const -> point
{
    return point(m_value * f);
}

auto kestrel::math::point::operator/(float f) const -> point
{
    return point(m_value / f);
}

auto kestrel::math::point::operator==(const point& p) const -> bool
{
    return (m_value[0] == p.m_value[0]) && (m_value[1] == p.m_value[1]);
}

auto kestrel::math::point::operator!=(const point& p) const -> bool
{
    return (m_value[0] != p.m_value[0]) || (m_value[1] != p.m_value[1]);
}

// MARK: - Operations

auto kestrel::math::point::add(const point &p) const -> point
{
    return point(m_value + p.m_value);
}

auto kestrel::math::point::subtract(const point &p) const -> point
{
    return point(m_value - p.m_value);
}

auto kestrel::math::point::multiply(float f) const -> point
{
    return point(m_value * f);
}

auto kestrel::math::point::divide(float f) const -> point
{
    return point(m_value / f);
}

auto kestrel::math::point::round() const -> point
{
    return point(m_value.round());
}

auto kestrel::math::point::floor() const -> point
{
    return point(m_value.floor());
}

auto kestrel::math::point::ceil() const -> point
{
    return point(m_value.ceil());
}

auto kestrel::math::point::distance_to(const point& p) const -> float
{
    auto xx = (p.m_value - m_value).pow(2);
    auto rr = xx.reversed();
    xx += rr;
    return std::sqrt(xx[0]);
}

auto kestrel::math::point::magnitude() const -> float
{
    auto xx = m_value.pow(2).abs();
    return std::sqrt(xx[0] + xx[1]);
}

auto kestrel::math::point::dot_product(const point &p) const -> float
{
    auto xx = m_value * p.m_value;
    auto rr = xx.reversed();
    xx += rr;
    return xx[0];
}

auto kestrel::math::point::angle() const -> struct angle
{
    auto r = std::atan2(m_value[1], m_value[0]);
    return math::angle(static_cast<float>((r * 180.0) / std::numbers::pi_v<float>));
}

auto kestrel::math::point::angle_to(const point &p) const -> struct angle
{
    auto v = p - *this;
    auto r = std::atan2(m_value[1], m_value[0]);
    return math::angle(static_cast<float>((r * 180.0) / std::numbers::pi_v<float>));
}

auto kestrel::math::point::normalize() -> void
{
    m_value /= magnitude();
}

auto kestrel::math::point::normalized() const -> point
{
    return point(m_value / magnitude());
}

auto kestrel::math::point::rotate_around(const point& c, const struct angle& angle) const -> point
{
    auto fsin = angle.fsin();
    auto fcos = angle.fcos();
    math::point q = *this - c;

    auto n = (m_value - c.m_value) * simd::float32(fcos, fsin, -fsin, fcos);
    n += n.reversed();
    n += c.m_value;

    return point(n);
}
