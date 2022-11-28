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
#include <libKestrel/math/point.hpp>
#include <libKestrel/math/angle.hpp>

// MARK: - Construction

kestrel::math::point::point(double v)
    : x(v), y(v)
{
}

kestrel::math::point::point(double x, double y)
    : x(x), y(y)
{
}

kestrel::math::point::point(const point& p)
    : x(p.x), y(p.y)
{
}

// MARK: - Operators

auto kestrel::math::point::operator+(const point& p) const -> point
{
    return { x + p.x, y + p.y };
}

auto kestrel::math::point::operator-(const point& p) const -> point
{
    return { x - p.x, y - p.y };
}

auto kestrel::math::point::operator*(const point& p) const -> point
{
    return { x * p.x, y * p.y };
}

auto kestrel::math::point::operator/(const point& p) const -> point
{
    return { x / p.x, y / p.y };
}

auto kestrel::math::point::operator*(double f) const -> point
{
    return { x * f, y * f };
}

auto kestrel::math::point::operator/(double f) const -> point
{
    return { x / f, y / f };
}

auto kestrel::math::point::operator==(const point& p) const -> bool
{
    return (x == p.x) && (y == p.y);
}

auto kestrel::math::point::operator!=(const point& p) const -> bool
{
    return !(*this == p);
}

// MARK: - Operations


auto kestrel::math::point::add(const point& p) const -> point
{
    return operator+(p);
}

auto kestrel::math::point::subtract(const point& p) const -> point
{
    return operator-(p);
}

auto kestrel::math::point::multiply(double f) const -> point
{
    return operator*(f);
}

auto kestrel::math::point::divide(double f) const -> point
{
    return operator/(f);
}

auto kestrel::math::point::round() const -> point
{
    return { std::round(x), std::round(y) };
}

auto kestrel::math::point::floor() const -> point
{
    return { std::floor(x), std::floor(y) };
}

auto kestrel::math::point::ceil() const -> point
{
    return { std::ceil(x), std::ceil(y) };
}

auto kestrel::math::point::distance_to(const point& p) const -> double
{
    auto dx = p.x - x;
    auto dy = p.y - y;
    return std::sqrt((dx * dx) + (dy * dy));
}

auto kestrel::math::point::angle() const -> struct angle
{
    return math::angle((std::atan2(y, x) * 180.0) / M_PI);
}

auto kestrel::math::point::angle_to(const point& v) const -> struct angle
{
    return math::angle((std::atan2(v.y - y, v.x - x) * 180.0) / M_PI);
}

auto kestrel::math::point::magnitude() const -> double
{
    return std::sqrt(std::fabs(std::pow(x, 2)) + std::fabs(std::pow(y, 2)));
}

auto kestrel::math::point::dot_product(const point& p) const -> double
{
    return this->x * p.x + this->y * p.y;
}

auto kestrel::math::point::normalize() -> void
{
    auto magnitude = this->magnitude();
    x /= magnitude;
    y /= magnitude;
}

auto kestrel::math::point::normalized() const -> point
{
    auto p = *this;
    p.normalize();
    return p;
}

// MARK: - Lua Accessors

auto kestrel::math::point::set_x(double v) -> void
{
    this->x = v;
}

auto kestrel::math::point::get_x() const -> double
{
    return x;
}

auto kestrel::math::point::set_y(double v) -> void
{
    this->y = v;
}

auto kestrel::math::point::get_y() const -> double
{
    return y;
}