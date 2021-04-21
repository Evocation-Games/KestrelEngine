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
#include "math/size.hpp"

// MARK: - Lua

auto math::size::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginClass<math::size>("Size")
            .addConstructor<auto(*)(const double&, const double&)->void, luabridge::RefCountedPtr<math::size>>()
            .addProperty("width", &math::size::get_width, &math::size::set_width)
            .addProperty("height", &math::size::get_height, &math::size::set_height)
            .addProperty("area", &math::size::area)
            .addFunction("add", &math::size::operator+)
            .addFunction("subtract", &math::size::operator-)
            .addFunction("multiply", &math::size::operator*)
            .addFunction("divide", &math::size::operator/)
        .endClass();
}

// MARK: - Construction

math::size::size()
    : width(0), height(0)
{

};

math::size::size(const double& v)
    : width(v), height(v)
{

};

math::size::size(const double& w, const double& h)
    : width(w), height(h)
{

};

math::size::size(const math::size& s)
    : width(s.width), height(s.height)
{

};

// MARK: - Operators

auto math::size::operator+(const math::size& s) const -> math::size
{
    return math::size(width + s.width, height + s.height);
}

auto math::size::operator-(const math::size& s) const -> math::size
{
    return math::size(width - s.width, height - s.height);
}

auto math::size::operator*(const double& f) const -> math::size
{
    return math::size(width * f, height * f);
}

auto math::size::operator/(const double& f) const -> math::size
{
    return math::size(width / f, height / f);
}

auto math::size::operator==(const math::size& s) const -> bool
{
    return (width == s.width) && (height == s.height);
}

auto math::size::operator!=(const math::size& s) const -> bool
{
    return !(*this == s);
}

// MARK: - Operations

auto math::size::round() const -> math::size
{
    return { std::round(width), std::round(height) };
}

auto math::size::floor() const -> math::size
{
    return { std::floor(width), std::floor(height) };
}

auto math::size::ceil() const -> math::size
{
    return { std::ceil(width), std::ceil(height) };
}

auto math::size::area() const -> double
{
    return width * height;
}

// MARK: - Lua Accessors

auto math::size::set_width(const double& width) -> void
{
    this->width = width;
}

auto math::size::get_width() const -> double
{
    return width;
}

auto math::size::set_height(const double& height) -> void
{
    this->height = height;
}

auto math::size::get_height() const -> double
{
    return height;
}
