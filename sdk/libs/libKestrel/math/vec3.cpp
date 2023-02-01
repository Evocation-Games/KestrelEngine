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

#include <libKestrel/math/vec3.hpp>
#include <libKestrel/math/SIMD.hpp>
#include <cmath>

// MARK: - Construction

kestrel::math::vec3::vec3()
    : m_value()
{}

kestrel::math::vec3::vec3(SIMD::float4 v)
    : m_value(v)
{}

kestrel::math::vec3::vec3(float u)
    : m_value(SIMD::float4::constant(u))
{}

kestrel::math::vec3::vec3(float x, float y, float z)
    : m_value(x, y, z, 0)
{}

kestrel::math::vec3::vec3(const vec2 &v, float z)
    : m_value(v.x(), v.y(), z, 0)
{}

// MARK: - Accessors

auto kestrel::math::vec3::x() const -> float
{
    return m_value[0];
}

auto kestrel::math::vec3::y() const -> float
{
    return m_value[1];
}

auto kestrel::math::vec3::z() const -> float
{
    return m_value[2];
}

// MARK: - Operators

auto kestrel::math::vec3::operator=(float s) -> vec3&
{
    m_value = SIMD::float4::constant(s);
    return *this;
}

auto kestrel::math::vec3::operator+ (float f) const -> vec3
{
    return vec3(m_value + f);
}

auto kestrel::math::vec3::operator- (float f) const -> vec3
{
    return vec3(m_value - f);
}

auto kestrel::math::vec3::operator* (float f) const -> vec3
{
    return vec3(m_value * f);
}

auto kestrel::math::vec3::operator/ (float f) const -> vec3
{
    return vec3(m_value / f);
}

auto kestrel::math::vec3::operator+ (const vec3& v) const -> vec3
{
    return vec3(m_value + v.m_value);
}
auto kestrel::math::vec3::operator- (const vec3& v) const -> vec3
{
    return vec3(m_value - v.m_value);
}
auto kestrel::math::vec3::operator* (const vec3& v) const -> vec3
{
    return vec3(m_value * v.m_value);
}
auto kestrel::math::vec3::operator/ (const vec3& v) const -> vec3
{
    return vec3(m_value / v.m_value);
}

// MARK: - Operations

auto kestrel::math::vec3::cross(const vec3 &v) const -> vec3
{
    return vec3(m_value * v.m_value.reversed());
}

auto kestrel::math::vec3::dot(const vec3 &v) const -> float
{
    auto xx = m_value * v.m_value;
    xx += xx.reversed();
    return xx[0];
}

auto kestrel::math::vec3::lerp(const vec3 &v, float t) const -> vec3
{
    return vec3(m_value + ((v.m_value - m_value) * t));
}

auto kestrel::math::vec3::magnitude() const -> float
{
    return std::sqrt(dot(*this));
}

auto kestrel::math::vec3::unit() const -> vec3
{
    return *this / magnitude();
}
