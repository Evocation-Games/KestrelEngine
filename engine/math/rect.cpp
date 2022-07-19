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
#include "math/rect.hpp"

// MARK: - Lua

auto math::rect::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginClass<rect>("Rect")
            .addConstructor<auto(*)(double, double, double, double)->void, luabridge::RefCountedPtr<math::rect>>()
            .addStaticFunction("macintoshRect", &rect::macintosh_rect)
            .addProperty("area", &rect::area)
            .addProperty("origin", &rect::get_origin, &rect::set_origin)
            .addProperty("size", &rect::get_size, &rect::set_size)
            .addProperty("x", &rect::get_x, &rect::set_x)
            .addProperty("y", &rect::get_y, &rect::set_y)
            .addProperty("width", &rect::get_width, &rect::set_width)
            .addProperty("height", &rect::get_height, &rect::set_height)
            .addFunction("containsPoint", &rect::contains_point)
            .addFunction("intersects", &rect::intersects)
            .addFunction("round", &rect::round)
            .addFunction("floor", &rect::floor)
            .addFunction("ceil", &rect::ceil)
        .endClass();
}

// MARK: - Construction

math::rect::rect()
    : origin(), size()
{

}

math::rect::rect(const math::point& o, const math::size& s)
    : origin(o), size(s)
{

}

math::rect::rect(double x, double y, double w, double h)
    : origin(x, y), size(w, h)
{

}

math::rect::rect(const math::rect& r)
    : origin(r.origin), size(r.size)
{

}

math::rect::rect(const graphite::quickdraw::rect<std::int16_t> &r)
{
    origin.x = r.origin.x;
    origin.y = r.origin.y;
    size.width = r.size.width;
    size.height = r.size.height;
}

auto math::rect::macintosh_rect(double top, double left, double bottom, double right) -> math::rect
{
    return { left, top, right - left, bottom - top };
}

// MARK: - Operators

auto math::rect::operator+(const math::point& p) const -> math::rect
{
    return { origin + p, size };
}

auto math::rect::operator+(const math::size& s) const -> math::rect
{
    return { origin, size + s };
}

auto math::rect::operator-(const math::point& p) const -> math::rect
{
    return { origin - p, size };
}

auto math::rect::operator-(const math::size& s) const -> math::rect
{
    return { origin, size - s };
}

auto math::rect::operator*(double f) const -> math::rect
{
    return { origin * f, size * f };
}

auto math::rect::operator/(double f) const -> math::rect
{
    return { origin / f, size / f };
}

auto math::rect::operator==(const math::rect& r) const -> bool
{
    return (origin == r.origin) && (size == r.size);
}

auto math::rect::operator!=(const math::rect& r) const -> bool
{
    return !(*this == r);
}

// MARK: - Operations

auto math::rect::round() const -> math::rect
{
    return { origin.round(), size.round() };
}

auto math::rect::floor() const -> math::rect
{
    return { origin.floor(), size.floor() };
}

auto math::rect::ceil() const -> math::rect
{
    return { origin.ceil(), size.ceil() };
}

auto math::rect::area() const -> double
{
    return size.area();
}

auto math::rect::contains_point(const math::point& p) const -> bool
{
    return (p.x >= origin.x && p.x <= origin.x + size.width)
           && (p.y >= origin.y && p.y <= origin.y + size.height);
}

auto math::rect::contains_rect(const math::rect& r) const -> bool
{
    return (r.origin.x >= origin.x && r.origin.x + r.size.width <= origin.x + size.width)
           && (r.origin.y >= origin.y && r.origin.y + r.size.height <= origin.y + size.height);
}

auto math::rect::intersects(const math::rect& r) const -> bool
{
    auto r1x1 = this->origin.x;
    auto r1x2 = this->origin.x + this->size.width;
    auto r1y1 = this->origin.y;
    auto r1y2 = this->origin.y + this->size.height;

    auto r2x1 = r.origin.x;
    auto r2x2 = r.origin.x + r.size.width;
    auto r2y1 = r.origin.y;
    auto r2y2 = r.origin.y + r.size.height;

    return ((r1x1 < r2x2) && (r1x2 > r2x1) && (r1y1 < r2y2) && (r1y2 > r2y1));
}

// MARK: - Lua Accessors

auto math::rect::set_origin(const math::point& origin) -> void
{
    this->origin = origin;
}

auto math::rect::get_origin() const -> math::point
{
    return origin;
}

auto math::rect::set_size(const math::size& size) -> void
{
    this->size = size;
}

auto math::rect::get_size() const -> math::size
{
    return size;
}

auto math::rect::set_x(double x) -> void
{
    origin.x = x;
}

auto math::rect::get_x() const -> double
{
    return origin.x;
}

auto math::rect::get_max_x() const -> double
{
    return origin.x + size.width;
}

auto math::rect::set_y(double y) -> void
{
    origin.y = y;
}

auto math::rect::get_y() const -> double
{
    return origin.y;
}

auto math::rect::get_max_y() const -> double
{
    return origin.y + size.height;
}

auto math::rect::set_width(double width) -> void
{
    size.width = width;
}

auto math::rect::get_width() const -> double
{
    return size.width;
}

auto math::rect::set_height(double height) -> void
{
    size.height = height;
}

auto math::rect::get_height() const -> double
{
    return size.height;
}

auto math::rect::inset(double amount) const -> math::rect
{
    return { origin.x + amount, origin.y + amount, size.width - (amount * 2), size.height - (amount * 2) };
}
