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
#include "math/vector.hpp"
#include "math/angle.hpp"

// MARK: - Lua

auto math::vector::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginClass<math::vector>("Vec3")
            .addConstructor<auto(*)(const double&, const double&, const double&)->void, math::vector::lua_reference>()
            .addProperty("x", &math::vector::get_x, &math::vector::set_x)
            .addProperty("y", &math::vector::get_y, &math::vector::set_y)
            .addProperty("z", &math::vector::get_z, &math::vector::set_z)
            .addProperty("magnitude", &math::vector::magnitude)
            .addProperty("angle", &math::vector::angle)
            .addFunction("angleTo", &math::vector::angle_to)
            .addFunction("distanceTo", &math::vector::distance_to)
            .addFunction("add", &math::vector::operator+)
            .addFunction("subtract", &math::vector::operator-)
            .addFunction("multiply", &math::vector::operator*)
            .addFunction("divide", &math::vector::operator/)
        .endClass();
}

// MARK: - Construction

math::vector::vector()
    : x(0), y(0), z(0)
{

};

math::vector::vector(const double& v)
    : x(v), y(v), z(v)
{

};

math::vector::vector(const double& x, const double& y, const double& z)
    : x(x), y(y), z(z)
{

};

math::vector::vector(const math::vector& v)
    : x(v.x), y(v.y), z(v.z)
{

};

// MARK: - Operators

auto math::vector::operator+(const math::vector& v) const -> math::vector
{
    return math::vector(x + v.x, y + v.y, z + v.z);
}

auto math::vector::operator-(const math::vector& v) const -> math::vector
{
    return math::vector(x - v.x, y - v.y, z - v.z);
}

auto math::vector::operator*(const double& f) const -> math::vector
{
    return math::vector(x * f, y * f, z * f);
}

auto math::vector::operator/(const double& f) const -> math::vector
{
    return math::vector(x / f, y / f, z / f);
}

auto math::vector::operator==(const math::vector& v) const -> bool
{
    return (x == v.x) && (y == v.y);
}

auto math::vector::operator!=(const math::vector& v) const -> bool
{
    return !(*this == v);
}

// MARK: - Operations

auto math::vector::angle() const -> math::angle
{
    return math::angle((std::atan2(x, y) * 180.0) / M_PI);
}

auto math::vector::angle_to(const math::vector& v) const -> math::angle
{
    return math::angle((std::atan2(v.y - y, v.x - x) * 180.0) / M_PI);
}

auto math::vector::distance_to(const math::vector& v) const -> double
{
    auto dx = v.x - x;
    auto dy = v.y - y;
    return std::sqrt((dx * dx) + (dy * dy));
}

auto math::vector::magnitude() const -> double
{
    return std::sqrt(std::fabs(std::pow(x, 2)) + std::fabs(std::pow(y, 2)));
}

// MARK: - Lua Accessors

auto math::vector::set_x(const double& x) -> void
{
    this->x = x;
}

auto math::vector::get_x() const -> double
{
    return x;
}

auto math::vector::set_y(const double& y) -> void
{
    this->y = y;
}

auto math::vector::get_y() const -> double
{
    return y;
}

auto math::vector::set_z(const double& z) -> void
{
    this->z = z;
}

auto math::vector::get_z() const -> double
{
    return z;
}
