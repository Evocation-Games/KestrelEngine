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

#include <libKestrel/math/vec4.hpp>
#include <libKestrel/math/SIMD.hpp>
#include <cmath>

// MARK: - Construction

kestrel::math::vec4::vec4()
    : m_value()
{}

kestrel::math::vec4::vec4(simd::float32 v)
    : m_value(v)
{}

kestrel::math::vec4::vec4(float u)
    : m_value(simd::float32::constant(u))
{}

kestrel::math::vec4::vec4(float x, float y, float z, float w)
    : m_value(x, y, z, w)
{}

kestrel::math::vec4::vec4(const vec2 &v, float z, float w)
    : m_value(v.x(), v.y(), z, w)
{}

kestrel::math::vec4::vec4(const vec3 &v, float w)
    : m_value(v.x(), v.y(), v.z(), w)
{}

// MARK: - Accessors

auto kestrel::math::vec4::x() const -> float
{
    return m_value[0];
}

auto kestrel::math::vec4::y() const -> float
{
    return m_value[1];
}

auto kestrel::math::vec4::z() const -> float
{
    return m_value[2];
}

auto kestrel::math::vec4::w() const -> float
{
    return m_value[3];
}

// MARK: - Operators

auto kestrel::math::vec4::operator=(float s) -> vec4&
{
    m_value = simd::float32::constant(s);
    return *this;
}

auto kestrel::math::vec4::operator+ (float f) const -> vec4
{
    return vec4(m_value + f);
}

auto kestrel::math::vec4::operator- (float f) const -> vec4
{
    return vec4(m_value - f);
}

auto kestrel::math::vec4::operator* (float f) const -> vec4
{
    return vec4(m_value * f);
}

auto kestrel::math::vec4::operator/ (float f) const -> vec4
{
    return vec4(m_value / f);
}

auto kestrel::math::vec4::operator+ (const vec4& v) const -> vec4
{
    return vec4(m_value + v.m_value);
}
auto kestrel::math::vec4::operator- (const vec4& v) const -> vec4
{
    return vec4(m_value - v.m_value);
}
auto kestrel::math::vec4::operator* (const vec4& v) const -> vec4
{
    return vec4(m_value * v.m_value);
}
auto kestrel::math::vec4::operator/ (const vec4& v) const -> vec4
{
    return vec4(m_value / v.m_value);
}

// MARK: - Operations

auto kestrel::math::vec4::cross(const vec4 &v) const -> vec4
{
    return vec4(
        vec3(v.x(), v.y(), v.z())
            .cross(vec3(v.x(), v.y(), v.z()))
    );
}

auto kestrel::math::vec4::dot(const vec4 &v) const -> float
{
    auto xx = m_value * v.m_value;
    xx += xx.reversed();
    return xx[0];
}

auto kestrel::math::vec4::lerp(const vec4 &v, float t) const -> vec4
{
    return vec4(m_value + ((v.m_value - m_value) * t));
}

auto kestrel::math::vec4::magnitude() const -> float
{
    return std::sqrt(dot(*this));
}

auto kestrel::math::vec4::unit() const -> vec4
{
    return *this / magnitude();
}
