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

#pragma once

#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>

namespace kestrel::math
{
    struct angle;

    /**
     * Represents a 2D point.
     */
    struct lua_api(Point, Available_0_8) point
    {
    public:
        has_constructable_lua_api(point);

        lua_data(x, mutable, Available_0_8) double x { 0 };
        lua_data(y, mutable, Available_0_8) double y { 0 };

        point() = default;
        explicit point(double v);
        point(const point& p);

        lua_constructor(Available_0_8) point(double x, double y);

        auto operator+(const point& p) const -> point;
        auto operator-(const point& p) const -> point;
        auto operator*(const point& p) const -> point;
        auto operator/(const point& p) const -> point;

        auto operator*(double f) const -> point;
        auto operator/(double f) const -> point;

        auto operator==(const point& p) const -> bool;
        auto operator!=(const point& p) const -> bool;

        [[nodiscard]] lua_function(add, Available_0_8) auto add(const point& p) const -> point;
        [[nodiscard]] lua_function(subtract, Available_0_8) auto subtract(const point& p) const -> point;
        [[nodiscard]] lua_function(multiply, Available_0_8) auto multiply(double f) const -> point;
        [[nodiscard]] lua_function(divide, Available_0_8) auto divide(double f) const -> point;

        [[nodiscard]] lua_function(round, Available_0_8) auto round() const -> point;
        [[nodiscard]] lua_function(floor, Available_0_8) auto floor() const -> point;
        [[nodiscard]] lua_function(ceil, Available_0_8) auto ceil() const -> point;

        [[nodiscard]] lua_function(distanceTo, Available_0_8) auto distance_to(const point& p) const -> double;
        [[nodiscard]] lua_getter(magnitude, Available_0_8) auto magnitude() const -> double;

        [[nodiscard]] lua_getter(angle, Available_0_8) auto angle() const -> angle;
        [[nodiscard]] lua_function(angleTo, Available_0_8) auto angle_to(const point& p) const -> struct angle;

        auto set_x(double x) -> void;
        [[nodiscard]] auto get_x() const -> double;

        auto set_y(double y) -> void;
        [[nodiscard]] auto get_y() const -> double;
    };

}
