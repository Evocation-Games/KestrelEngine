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

#include <libKestrel/math/vec2.hpp>
#include <libKestrel/math/point.hpp>
#include <libKestrel/math/SIMD.hpp>
#include <cmath>
#include <numbers>

// MARK: - Construction

kestrel::math::vec2::vec2()
    : m_value()
{}

kestrel::math::vec2::vec2(SIMD::float4 v)
    : m_value(v)
{}

kestrel::math::vec2::vec2(float u)
    : m_value(SIMD::float4::constant(u))
{}

kestrel::math::vec2::vec2(float x, float y)
    : m_value(x, y, x, y)
{}

kestrel::math::vec2::vec2(const point &p)
    : m_value(p.m_value)
{}

kestrel::math::vec2::vec2(const size &s)
    : m_value(s.m_value)
{}

// MARK: - Accessors

auto kestrel::math::vec2::x() const -> float
{
    return m_value[0];
}

auto kestrel::math::vec2::y() const -> float
{
    return m_value[1];
}

auto kestrel::math::vec2::to_point() const -> point
{
    return point(m_value);
}

auto kestrel::math::vec2::to_size() const -> size
{
    return size(m_value);
}

// MARK: - Operators

auto kestrel::math::vec2::operator=(float s) -> vec2&
{
    m_value = SIMD::float4::constant(s);
    return *this;
}

auto kestrel::math::vec2::operator+ (float f) const -> vec2
{
    return vec2(m_value + f);
}

auto kestrel::math::vec2::operator- (float f) const -> vec2
{
    return vec2(m_value + f);
}

auto kestrel::math::vec2::operator* (float f) const -> vec2
{
    return vec2(m_value * f);
}

auto kestrel::math::vec2::operator/ (float f) const -> vec2
{
    return vec2(m_value / f);
}

auto kestrel::math::vec2::operator+ (const vec2& v) const -> vec2
{
    return vec2(m_value + v.m_value);
}
auto kestrel::math::vec2::operator- (const vec2& v) const -> vec2
{
    return vec2(m_value - v.m_value);
}
auto kestrel::math::vec2::operator* (const vec2& v) const -> vec2
{
    return vec2(m_value * v.m_value);
}
auto kestrel::math::vec2::operator/ (const vec2& v) const -> vec2
{
    return vec2(m_value / v.m_value);
}

// MARK: - Operations

auto kestrel::math::vec2::cross(const vec2 &v) const -> vec2
{
    return vec2(m_value * v.m_value.reversed());
}

auto kestrel::math::vec2::dot(const vec2 &v) const -> float
{
    auto xx = m_value * v.m_value;
    xx += xx.reversed();
    return xx[0];
}

auto kestrel::math::vec2::lerp(const vec2 &v, float t) const -> vec2
{
    return vec2(m_value + ((v.m_value - m_value) * t));
}

auto kestrel::math::vec2::magnitude() const -> float
{
    return std::sqrt(dot(*this));
}

auto kestrel::math::vec2::unit() const -> vec2
{
    return *this / magnitude();
}

auto kestrel::math::vec2::angle_to(const vec2 &v) const -> float
{
    auto xx = v.m_value - m_value;
    return (std::atan2f(xx[0], xx[1]) * 180.0f) / std::numbers::pi_v<float>;
}