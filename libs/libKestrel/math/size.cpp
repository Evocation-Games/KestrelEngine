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
#include <libKestrel/math/size.hpp>

// MARK: - Construction

kestrel::math::size::size(double v)
    : width(v), height(v)
{
}

kestrel::math::size::size(double w, double h)
    : width(w), height(h)
{
}

kestrel::math::size::size(const size& s)
    : width(s.width), height(s.height)
{
}

// MARK: - Operators

auto kestrel::math::size::operator+(const size& s) const -> size
{
    return { width + s.width, height + s.height };
}

auto kestrel::math::size::operator-(const size& s) const -> size
{
    return { width - s.width, height - s.height };
}

auto kestrel::math::size::operator*(const size& s) const -> size
{
    return { width * s.width, height * s.height };
}

auto kestrel::math::size::operator/(const size& s) const -> size
{
    return { width / s.width, height / s.height };
}

auto kestrel::math::size::operator*(double f) const -> size
{
    return { width * f, height * f };
}

auto kestrel::math::size::operator/(double f) const -> size
{
    return { width / f, height / f };
}

auto kestrel::math::size::operator==(const size& s) const -> bool
{
    return (width == s.width) && (height == s.height);
}

auto kestrel::math::size::operator!=(const size& s) const -> bool
{
    return !(*this == s);
}

// MARK: - Operations

auto kestrel::math::size::add(const size& s) const -> size
{
    return operator+(s);
}

auto kestrel::math::size::subtract(const size& s) const -> size
{
    return operator-(s);
}

auto kestrel::math::size::multiply(double f) const -> size
{
    return operator*(f);
}

auto kestrel::math::size::divide(double f) const -> size
{
    return operator/(f);
}

auto kestrel::math::size::round() const -> size
{
    return { std::round(width), std::round(height) };
}

auto kestrel::math::size::floor() const -> size
{
    return { std::floor(width), std::floor(height) };
}

auto kestrel::math::size::ceil() const -> size
{
    return { std::ceil(width), std::ceil(height) };
}

auto kestrel::math::size::area() const -> double
{
    return width * height;
}

auto kestrel::math::size::aspect_ratio() const -> double
{
    return width / height;
}

auto kestrel::math::size::is_portrait() const -> bool
{
    return width <= height;
}

auto kestrel::math::size::is_landscape() const -> bool
{
    return width >= height;
}

// MARK: - Lua Accessors

auto kestrel::math::size::set_width(double width) -> void
{
    this->width = width;
}

auto kestrel::math::size::get_width() const -> double
{
    return width;
}

auto kestrel::math::size::set_height(double height) -> void
{
    this->height = height;
}

auto kestrel::math::size::get_height() const -> double
{
    return height;
}
