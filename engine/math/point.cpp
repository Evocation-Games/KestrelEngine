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
#include "math/angle.hpp"

// MARK: - Lua

auto math::point::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginClass<math::point>("Point")
            .addConstructor<auto(*)(const double&, const double&)->void, luabridge::RefCountedPtr<math::point>>()
            .addProperty("x", &math::point::get_x, &math::point::set_x)
            .addProperty("y", &math::point::get_y, &math::point::set_y)
            .addProperty("angle", &math::point::angle)
            .addProperty("magnitude", &math::point::magnitude)
            .addFunction("distanceTo", &math::point::distance_to)
            .addFunction("subtract", &math::point::subtract)
            .addFunction("add", &math::point::add)
            .addFunction("multiply", &math::point::multiply)
            .addFunction("divide", &math::point::divide)
            .addFunction("round", &math::point::round)
            .addFunction("floor", &math::point::floor)
            .addFunction("ceil", &math::point::ceil)
            .addFunction("angleTo", &math::point::angle_to)
        .endClass();
}

// MARK: - Construction

math::point::point(double v)
    : x(v), y(v)
{
}

math::point::point(double x, double y)
    : x(x), y(y)
{
}

math::point::point(const math::point& p)
    : x(p.x), y(p.y)
{
}

// MARK: - Operators

auto math::point::operator+(const math::point& p) const -> math::point
{
    return { x + p.x, y + p.y };
}

auto math::point::operator-(const math::point& p) const -> math::point
{
    return { x - p.x, y - p.y };
}

auto math::point::operator*(const math::point& p) const -> math::point
{
    return { x * p.x, y * p.y };
}

auto math::point::operator/(const math::point& p) const -> math::point
{
    return { x / p.x, y / p.y };
}

auto math::point::operator*(double f) const -> math::point
{
    return { x * f, y * f };
}

auto math::point::operator/(double f) const -> math::point
{
    return { x / f, y / f };
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


auto math::point::add(const math::point& p) const -> math::point
{
    return operator+(p);
}

auto math::point::subtract(const math::point& p) const -> math::point
{
    return operator-(p);
}

auto math::point::multiply(double f) const -> math::point
{
    return operator*(f);
}

auto math::point::divide(double f) const -> math::point
{
    return operator/(f);
}

auto math::point::round() const -> math::point
{
    return { std::round(x), std::round(y) };
}

auto math::point::floor() const -> math::point
{
    return { std::floor(x), std::floor(y) };
}

auto math::point::ceil() const -> math::point
{
    return { std::ceil(x), std::ceil(y) };
}

auto math::point::distance_to(const math::point& p) const -> double
{
    auto dx = p.x - x;
    auto dy = p.y - y;
    return std::sqrt((dx * dx) + (dy * dy));
}

auto math::point::angle() const -> math::angle
{
    return math::angle((std::atan2(y, x) * 180.0) / M_PI);
}

auto math::point::angle_to(const math::point& v) const -> math::angle
{
    return math::angle((std::atan2(v.y - y, v.x - x) * 180.0) / M_PI);
}

auto math::point::magnitude() const -> double
{
    return std::sqrt(std::fabs(std::pow(x, 2)) + std::fabs(std::pow(y, 2)));
}
// MARK: - Lua Accessors

auto math::point::set_x(double v) -> void
{
    this->x = v;
}

auto math::point::get_x() const -> double
{
    return x;
}

auto math::point::set_y(double v) -> void
{
    this->y = v;
}

auto math::point::get_y() const -> double
{
    return y;
}