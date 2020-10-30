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
#include "math/angular_difference.hpp"
#include "math/angle.hpp"

// MARK: - Lua

auto math::angular_difference::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginClass<math::angular_difference>("AngularDifference")
            .addConstructor<auto(*)(const double&)->void, luabridge::RefCountedPtr<math::angular_difference>>()
            .addProperty("value", &math::angular_difference::phi)
            .addProperty("isClockwise", &math::angular_difference::is_clockwise)
            .addProperty("isAntiClockwise", &math::angular_difference::is_anti_clockwise)
            .addStaticFunction("of", &math::angular_difference::between)
            .addFunction("calculateAngleFrom", &math::angular_difference::calculate_for)
            .addFunction("isOpposing", &math::angular_difference::is_opposing)
            .addFunction("isEqual", &math::angular_difference::is_equal)
        .endClass();
}

// MARK: - Constructors

math::angular_difference::angular_difference(const double& phi)
    : m_phi(phi)
{

}

math::angular_difference::angular_difference(const math::angular_difference& d)
    : m_phi(d.m_phi)
{

}

auto math::angular_difference::between(const math::angle& a, const math::angle& b) -> math::angular_difference
{
    auto phi = b - a + math::angle(180);
    auto phi_value = std::fmod(phi.degrees(), 360.0) + 180.0;
    return math::angular_difference(phi_value > 180.0 ? 360 - phi_value : phi_value);
}

// MARK: - Operators

auto math::angular_difference::operator== (math::angular_difference& a) const -> bool
{
    return m_phi == a.m_phi;
}

auto math::angular_difference::operator!= (math::angular_difference& a) const -> bool
{
    return m_phi != a.m_phi;
}

auto math::angular_difference::operator<= (math::angular_difference& a) const -> bool
{
    return m_phi <= a.m_phi;
}

auto math::angular_difference::operator< (math::angular_difference& a) const -> bool
{
    return m_phi < a.m_phi;
}

auto math::angular_difference::operator> (math::angular_difference& a) const -> bool
{
    return m_phi > a.m_phi;
}

auto math::angular_difference::operator>= (math::angular_difference& a) const -> bool
{
    return m_phi >= a.m_phi;
}

// MARK: - Operations

auto math::angular_difference::calculate_for(const math::angle& a) const -> math::angle
{
    return a + *this;
}

auto math::angular_difference::is_clockwise() const -> bool
{
    return m_phi > 0;
}

auto math::angular_difference::is_anti_clockwise() const -> bool
{
    return m_phi < 0;
}

auto math::angular_difference::is_opposing(const math::angular_difference& tolerance) const -> bool
{
    return std::abs(m_phi) >= (180.0 - tolerance.phi());
}

auto math::angular_difference::is_equal(const math::angular_difference& tolerance) const -> bool
{
    return std::abs(m_phi) <= tolerance.phi();
}

// MARK: - Accessors

auto math::angular_difference::phi() const -> double
{
    return m_phi;
}
