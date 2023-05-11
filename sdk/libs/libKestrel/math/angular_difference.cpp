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

#include <libKestrel/math/angular_difference.hpp>
#include <libKestrel/math/angle.hpp>
#include <cmath>
#include <numbers>

// MARK: - Constructors

kestrel::math::angular_difference::angular_difference(float phi)
    : m_phi(phi)
{
}

kestrel::math::angular_difference::angular_difference(const angular_difference& d)
    : m_phi(d.m_phi)
{
}

auto kestrel::math::angular_difference::between(const angle& a, const angle& b) -> angular_difference
{
    auto phi = b - a + math::angle(180);
    auto phi_value = std::fmodf(phi.degrees(), 360.0f) + 180.0f;
    return math::angular_difference(phi_value > 180.0 ? 360 - phi_value : phi_value);
}

// MARK: - Operators

auto kestrel::math::angular_difference::operator== (angular_difference& a) const -> bool
{
    return m_phi == a.m_phi;
}

auto kestrel::math::angular_difference::operator!= (angular_difference& a) const -> bool
{
    return m_phi != a.m_phi;
}

auto kestrel::math::angular_difference::operator<= (angular_difference& a) const -> bool
{
    return m_phi <= a.m_phi;
}

auto kestrel::math::angular_difference::operator< (angular_difference& a) const -> bool
{
    return m_phi < a.m_phi;
}

auto kestrel::math::angular_difference::operator> (angular_difference& a) const -> bool
{
    return m_phi > a.m_phi;
}

auto kestrel::math::angular_difference::operator>= (angular_difference& a) const -> bool
{
    return m_phi >= a.m_phi;
}

// MARK: - Operations

auto kestrel::math::angular_difference::calculate_for(const angle& a) const -> angle
{
    return a + *this;
}

auto kestrel::math::angular_difference::is_clockwise() const -> bool
{
    return m_phi > 0;
}

auto kestrel::math::angular_difference::is_anti_clockwise() const -> bool
{
    return m_phi < 0;
}

auto kestrel::math::angular_difference::is_opposing(const angular_difference& tolerance) const -> bool
{
    return std::fabs(m_phi) >= (180.0f - tolerance.phi());
}

auto kestrel::math::angular_difference::is_equal(const angular_difference& tolerance) const -> bool
{
    const auto phi = std::fabs(m_phi);
    const auto tol = std::fabs(tolerance.phi());
    return phi <= tol;
}

// MARK: - Accessors

auto kestrel::math::angular_difference::phi() const -> float
{
    return m_phi;
}
