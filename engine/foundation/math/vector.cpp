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

#include <complex>
#include "foundation/math/vector.hpp"
#include "foundation/math/angle.hpp"

// MARK: - Constructors

kestrel::math::vector::vector()
{

}

kestrel::math::vector::vector(double x, double y, double z)
        : m_x(x), m_y(y), m_z(z)
{

}

kestrel::math::vector::vector(const math::vector& v)
        : m_x(v.m_x), m_y(v.m_y), m_z(v.m_z)
{

}

auto kestrel::math::vector::zero() -> math::vector
{
    return math::vector(0, 0, 0);
}

// MARK: - Accessors

auto kestrel::math::vector::x() const -> double
{
    return m_x;
}

auto kestrel::math::vector::y() const -> double
{
    return m_y;
}

auto kestrel::math::vector::z() const -> double
{
    return m_z;
}

auto kestrel::math::vector::set_x(double x) -> void
{
    m_x = x;
}

auto kestrel::math::vector::set_y(double y) -> void
{
    m_y = y;
}

auto kestrel::math::vector::set_z(double z) -> void
{
    m_z = z;
}

// MARK: - Operators

auto kestrel::math::vector::operator+ (const math::vector& v) const -> math::vector
{
    return math::vector(m_x + v.m_x, m_y + v.m_y, m_z + v.m_z);
}

auto kestrel::math::vector::operator- (const math::vector& v) const -> math::vector
{
    return math::vector(m_x - v.m_x, m_y - v.m_y, m_z - v.m_z);
}

auto kestrel::math::vector::operator* (double s) const -> math::vector
{
    return math::vector(m_x * s, m_y * s, m_z * s);
}

auto kestrel::math::vector::operator/ (double s) const -> vector
{
    return math::vector(m_x / s, m_y / s, m_z / s);
}

// MARK: - Calculations

auto kestrel::math::vector::angle() const -> math::angle
{
    return (std::atan2(m_y, m_x) * 180.0) / M_PI;
}

auto kestrel::math::vector::angle(const math::vector& v) const -> math::angle
{
    auto x = v.m_x - m_x;
    auto y = v.m_y - m_y;
    return (std::atan2(y, x) * 180.0) / M_PI;
}

auto kestrel::math::vector::magnitude() const -> double
{
    return std::sqrt(std::fabs(std::pow(m_x, 2.0)) + std::fabs(std::pow(m_y, 2.0)));
}
