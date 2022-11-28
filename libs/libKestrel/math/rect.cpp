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
#include <libKestrel/math/rect.hpp>

// MARK: - Construction

kestrel::math::rect::rect()
    : origin(), size()
{
}

kestrel::math::rect::rect(const struct point& o, const struct size& s)
    : origin(o), size(s)
{
}

kestrel::math::rect::rect(double x, double y, double w, double h)
    : origin(x, y), size(w, h)
{
}

kestrel::math::rect::rect(const rect& r)
    : origin(r.origin), size(r.size)
{

}

kestrel::math::rect::rect(const graphite::quickdraw::rect<std::int16_t> &r)
{
    origin.x = r.origin.x;
    origin.y = r.origin.y;
    size.width = r.size.width;
    size.height = r.size.height;
}

auto kestrel::math::rect::macintosh_rect(double top, double left, double bottom, double right) -> rect
{
    return { left, top, right - left, bottom - top };
}

// MARK: - Operators

auto kestrel::math::rect::operator+(const struct point& p) const -> rect
{
    return { origin + p, size };
}

auto kestrel::math::rect::operator+(const struct size& s) const -> rect
{
    return { origin, size + s };
}

auto kestrel::math::rect::operator-(const struct point& p) const -> rect
{
    return { origin - p, size };
}

auto kestrel::math::rect::operator-(const struct size& s) const -> rect
{
    return { origin, size - s };
}

auto kestrel::math::rect::operator*(double f) const -> rect
{
    return { origin * f, size * f };
}

auto kestrel::math::rect::operator/(double f) const -> rect
{
    return { origin / f, size / f };
}

auto kestrel::math::rect::operator==(const rect& r) const -> bool
{
    return (origin == r.origin) && (size == r.size);
}

auto kestrel::math::rect::operator!=(const rect& r) const -> bool
{
    return !(*this == r);
}

auto kestrel::math::rect::add_point(const struct point& p) const -> rect
{
    return { origin + p, size };
}

auto kestrel::math::rect::add_size(const struct size& s) const -> rect
{
    return { origin, size + s };
}

auto kestrel::math::rect::subtract_point(const struct point& p) const -> rect
{
    return { origin - p, size };
}

auto kestrel::math::rect::subtract_size(const struct size& s) const -> rect
{
    return { origin, size - s };
}

// MARK: - Operations

auto kestrel::math::rect::round() const -> rect
{
    return { origin.round(), size.round() };
}

auto kestrel::math::rect::floor() const -> rect
{
    return { origin.floor(), size.floor() };
}

auto kestrel::math::rect::ceil() const -> rect
{
    return { origin.ceil(), size.ceil() };
}

auto kestrel::math::rect::area() const -> double
{
    return size.area();
}

auto kestrel::math::rect::contains_point(const point& p) const -> bool
{
    return (p.x >= origin.x && p.x <= origin.x + size.width)
           && (p.y >= origin.y && p.y <= origin.y + size.height);
}

auto kestrel::math::rect::contains_rect(const rect& r) const -> bool
{
    return (r.origin.x >= origin.x && r.origin.x + r.size.width <= origin.x + size.width)
           && (r.origin.y >= origin.y && r.origin.y + r.size.height <= origin.y + size.height);
}

auto kestrel::math::rect::intersects(const rect& r) const -> bool
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

auto kestrel::math::rect::set_origin(const struct point& origin) -> void
{
    this->origin = origin;
}

auto kestrel::math::rect::get_origin() const -> struct point
{
    return origin;
}

auto kestrel::math::rect::set_size(const struct size& size) -> void
{
    this->size = size;
}

auto kestrel::math::rect::get_size() const -> struct size
{
    return size;
}

auto kestrel::math::rect::set_x(double x) -> void
{
    origin.x = x;
}

auto kestrel::math::rect::get_x() const -> double
{
    return origin.x;
}

auto kestrel::math::rect::get_max_x() const -> double
{
    return origin.x + size.width;
}

auto kestrel::math::rect::set_y(double y) -> void
{
    origin.y = y;
}

auto kestrel::math::rect::get_y() const -> double
{
    return origin.y;
}

auto kestrel::math::rect::get_max_y() const -> double
{
    return origin.y + size.height;
}

auto kestrel::math::rect::set_width(double width) -> void
{
    size.width = width;
}

auto kestrel::math::rect::get_width() const -> double
{
    return size.width;
}

auto kestrel::math::rect::set_height(double height) -> void
{
    size.height = height;
}

auto kestrel::math::rect::get_height() const -> double
{
    return size.height;
}

auto kestrel::math::rect::inset(double amount) const -> rect
{
    return { origin.x + amount, origin.y + amount, size.width - (amount * 2), size.height - (amount * 2) };
}
