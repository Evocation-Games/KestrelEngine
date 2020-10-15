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
#include "math/angle.hpp"
#include "math/angular_difference.hpp"
#include "math/vector.hpp"

// MARK: - Lua

auto math::angle::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginClass<math::angle>("Angle")
            .addConstructor<auto(*)(const double&)->void, math::angle::lua_reference>()
            .addProperty("degrees", &math::angle::degrees)
            .addProperty("radians", &math::angle::radians)
            .addFunction("sin", &math::angle::sin)
            .addFunction("cos", &math::angle::cos)
            .addFunction("vector", &math::angle::vector)
            .addFunction("opposite", &math::angle::opposite)
            .addFunction("rotated", &math::angle::rotated)
            .addFunction("normalize", &math::angle::normalize)
            .addFunction("isOpposing", &math::angle::is_opposing)
            .addFunction("isEqual", &math::angle::is_equal)
            .addFunction("addAngularDifference", &math::angle::add_angular_difference)
            .addFunction("subtractAngularDifference", &math::angle::subtract_angular_difference)
        .endClass();
}

// MARK: - Constructors

math::angle::angle(const double& theta)
    : m_theta(theta)
{
    normalize();
}

math::angle::angle(const math::angle& a)
    : m_theta(a.m_theta)
{
    normalize();
}

// MARK: - Conversions

auto math::angle::degrees() const -> double
{
    return m_theta;
}

auto math::angle::radians() const -> double
{
    return (m_theta * M_PI) / 180.0;
}

// MARK: - Trig

auto math::angle::sin(const double& magnitude) const -> double
{
    return std::sin(radians()) * magnitude;
}

auto math::angle::cos(const double& magnitude) const -> double
{
    return std::cos(radians()) * magnitude;
}

// MARK: - Operators

auto math::angle::operator+ (const math::angle& a) const -> math::angle
{
    return math::angle(m_theta + a.m_theta);
}

auto math::angle::operator- (const math::angle& a) const -> math::angle
{
    return math::angle(m_theta - a.m_theta);
}

auto math::angle::operator+ (const math::angular_difference& a) const -> math::angle
{
    return math::angle(m_theta + a.phi());
}

auto math::angle::operator- (const math::angular_difference& a) const -> math::angle
{
    return math::angle(m_theta - a.phi());
}

auto math::angle::add_angular_difference(const math::angular_difference &a) const -> math::angle
{
    return *this + a;
}

auto math::angle::subtract_angular_difference(const math::angular_difference &a) const -> math::angle
{
    return *this - a;
}


// MARK: - Operations

auto math::angle::vector(const double& magnitude) const -> math::vector
{
    return math::vector(cos(magnitude), sin(magnitude));
}

auto math::angle::opposite() const -> math::angle
{
    return rotated(math::angular_difference(180));
}

auto math::angle::rotated(const math::angular_difference& phi) const -> math::angle
{
    return phi.calculate_for(*this);
}

auto math::angle::normalize() -> void
{
    m_theta = std::fmod(m_theta, 360.0);
    if (m_theta < 0) {
        m_theta += 360;
    }
}

// MARK: - Conditions / Comparisons

auto math::angle::is_opposing(const math::angle& a, const math::angular_difference& tolerance) const -> bool
{
    auto phi = math::angular_difference::between(*this, a);
    return phi.is_opposing(tolerance);
}

auto math::angle::is_equal(const math::angle& a, const math::angular_difference& tolerance) const -> bool
{
    auto phi = math::angular_difference::between(*this, a);
    return phi.is_equal(tolerance);
}
