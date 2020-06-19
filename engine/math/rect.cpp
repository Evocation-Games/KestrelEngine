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

// MARK: - Construction

math::rect::rect()
    : origin(), size()
{

};

math::rect::rect(const math::point& o, const math::size& s)
    : origin(o), size(s)
{

};

math::rect::rect(const double& x, const double& y, const double& w, const double& h)
    : origin(x, y), size(w, h)
{

};

math::rect::rect(const math::rect& r)
    : origin(r.origin), size(r.size)
{

};

// MARK: - Operators

auto math::rect::operator+(const math::point& p) const -> math::rect
{
    return math::rect(origin + p, size);
}

auto math::rect::operator+(const math::size& s) const -> math::rect
{
    return math::rect(origin, size + s);
}

auto math::rect::operator-(const math::point& p) const -> math::rect
{
    return math::rect(origin - p, size);
}

auto math::rect::operator-(const math::size& s) const -> math::rect
{
    return math::rect(origin, size - s);
}

auto math::rect::operator*(const double& f) const -> math::rect
{
    return math::rect(origin * f, size * f);
}

auto math::rect::operator/(const double& f) const -> math::rect
{
    return math::rect(origin / f, size / f);
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

auto math::rect::area() const -> double
{
    return size.area();
}

auto math::rect::contains(const math::point& p) const -> bool
{
    return (p.x >= origin.x && p.x <= origin.x + size.width)
           && (p.y >= origin.y && p.y <= origin.y + size.height);
}

auto math::rect::contains(const math::rect& r) const -> bool
{
    return (r.origin.x >= origin.x && r.origin.x + r.size.width <= origin.x + size.width)
           && (r.origin.y >= origin.y && r.origin.y + r.size.height <= origin.y + size.height);
}

auto math::rect::intersects(const math::rect& r) const -> bool
{
    // TODO: Implement this in an efficient way...
    return false;
}