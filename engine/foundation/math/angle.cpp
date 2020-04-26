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
#include "foundation/math/angle.hpp"
#include "foundation/math/angular_difference.hpp"
#include "foundation/math/vector.hpp"

// MARK: - Constructors

kestrel::math::angle::angle(const double& theta)
        : m_theta(theta)
{
    normalise();
}

kestrel::math::angle::angle(const math::angle& a)
        : m_theta(a.m_theta)
{
    normalise();
}

// MARK: - Conversions

auto kestrel::math::angle::degrees() const -> double
{
    return m_theta;
}

auto kestrel::math::angle::radians() const -> double
{
    return (m_theta * M_PI) / 180.0;
}

// MARK: - Trig

auto kestrel::math::angle::sin(const double magnitude) const -> double
{
    return std::sin(radians()) * magnitude;
}
auto kestrel::math::angle::cos(const double magnitude) const -> double
{
    return std::cos(radians()) * magnitude;
}

// MARK: - Operators

auto kestrel::math::angle::operator+ (const math::angle& a) const -> math::angle
{
    return m_theta + a.m_theta;
}

auto kestrel::math::angle::operator- (const math::angle& a) const -> math::angle
{
    return m_theta - a.m_theta;
}

auto kestrel::math::angle::operator+ (const math::angular_difference& a) const -> math::angle
{
    return m_theta + a.phi();
}

auto kestrel::math::angle::operator- (const math::angular_difference& a) const -> math::angle
{
    return m_theta - a.phi();
}

// MARK: - Operations

auto kestrel::math::angle::vector(const double magnitude) const -> math::vector
{
    return math::vector(cos(magnitude), sin(magnitude));
}

auto kestrel::math::angle::opposite() const -> math::angle
{
    return rotated(180);
}

auto kestrel::math::angle::rotated(const math::angular_difference& phi) const -> math::angle
{
    return phi.calculate_for(*this);
}

auto kestrel::math::angle::normalise() -> void
{
    m_theta = std::fmod(m_theta, 360.0);
    if (m_theta < 0) {
        m_theta += 360;
    }
}

// MARK: - Conditions / Comparisons

auto kestrel::math::angle::is_opposing(const math::angle& a, const math::angular_difference& tolerance) const -> bool
{
    auto phi = kestrel::math::angular_difference::between(*this, a);
    return phi.is_opposing(tolerance);
}

auto kestrel::math::angle::is_equal(const math::angle& a, const math::angular_difference& tolerance) const -> bool
{
    auto phi = kestrel::math::angular_difference::between(*this, a);
    return phi.is_equal(tolerance);
}