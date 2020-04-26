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
#include "foundation/math/angular_difference.hpp"
#include "foundation/math/angle.hpp"

// MARK: - Constructors

kestrel::math::angular_difference::angular_difference(double phi)
        : m_phi(phi)
{

}

auto kestrel::math::angular_difference::between(const math::angle& a, const math::angle& b) -> math::angular_difference
{
    auto phi = b - a + math::angle(180);
    auto phi_value = std::fmod(phi.degrees(), 360.0) + 180.0;
    return phi_value > 180.0 ? 360 - phi_value : phi_value;
}

// MARK: - Operators

auto kestrel::math::angular_difference::operator== (math::angular_difference& a) const -> bool
{
    return m_phi == a.m_phi;
}

auto kestrel::math::angular_difference::operator!= (math::angular_difference& a) const -> bool
{
    return m_phi != a.m_phi;
}

auto kestrel::math::angular_difference::operator<= (math::angular_difference& a) const -> bool
{
    return m_phi <= a.m_phi;
}

auto kestrel::math::angular_difference::operator< (math::angular_difference& a) const -> bool
{
    return m_phi < a.m_phi;
}

auto kestrel::math::angular_difference::operator> (math::angular_difference& a) const -> bool
{
    return m_phi > a.m_phi;
}

auto kestrel::math::angular_difference::operator>= (math::angular_difference& a) const -> bool
{
    return m_phi >= a.m_phi;
}

// MARK: - Operations

auto kestrel::math::angular_difference::calculate_for(const kestrel::math::angle& a) const -> math::angle
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

auto kestrel::math::angular_difference::is_opposing(const math::angular_difference& tolerance) const -> bool
{
    return std::abs(m_phi) >= (180.0 - tolerance.phi());
}

auto kestrel::math::angular_difference::is_equal(const math::angular_difference& tolerance) const -> bool
{
    return std::abs(m_phi) <= tolerance.phi();
}

// MARK: - Accessors

auto kestrel::math::angular_difference::phi() const -> double
{
    return m_phi;
}
