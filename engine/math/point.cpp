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
#include "math/point.hpp"
#include "math/vector.hpp"

// MARK: - Lua

auto math::point::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginClass<math::point>("Point")
            .addConstructor<auto(*)(const double&, const double&)->void, luabridge::RefCountedPtr<math::point>>()
            .addProperty("x", &math::point::get_x, &math::point::set_x)
            .addProperty("y", &math::point::get_y, &math::point::set_y)
            .addFunction("distanceTo", &math::point::distance_to)
            .addFunction("subtract", &math::point::operator-)
            .addFunction("add", &math::point::operator+)
            .addFunction("multiply", &math::point::operator*)
            .addFunction("divide", &math::point::operator/)
            .addFunction("toVector", &math::point::to_vector)
        .endClass();
}

// MARK: - Construction

math::point::point()
    : x(0), y(0)
{

};

math::point::point(const double& v)
    : x(v), y(v)
{

};

math::point::point(const double& x, const double& y)
    : x(x), y(y)
{

};

math::point::point(const math::point& p)
    : x(p.x), y(p.y)
{

};

// MARK: - Operators

auto math::point::operator+(const math::point& p) const -> math::point
{
    return math::point(x + p.x, y + p.y);
}

auto math::point::operator-(const math::point& p) const -> math::point
{
    return math::point(x - p.x, y - p.y);
}

auto math::point::operator*(const double& f) const -> math::point
{
    return math::point(x * f, y * f);
}

auto math::point::operator/(const double& f) const -> math::point
{
    return math::point(x / f, y / f);
}

auto math::point::operator==(const math::point& p) const -> bool
{
    return (x == p.x) && (y == p.y);
}

auto math::point::operator!=(const math::point& p) const -> bool
{
    return !(*this == p);
}

// MARK: - Operations

auto math::point::distance_to(const math::point& p) const -> double
{
    auto dx = p.x - x;
    auto dy = p.y - y;
    return std::sqrt((dx * dx) + (dy * dy));
}

// MARK: - Lua Accessors

auto math::point::set_x(const double& x) -> void
{
    this->x = x;
}

auto math::point::get_x() const -> double
{
    return x;
}

auto math::point::set_y(const double& y) -> void
{
    this->y = y;
}

auto math::point::get_y() const -> double
{
    return y;
}

auto math::point::to_vector() const -> math::vector
{
    return math::vector(x, y);
}
