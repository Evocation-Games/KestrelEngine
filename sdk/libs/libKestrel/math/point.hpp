// Copyright (c) 2023 Tom Hancocks
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

#include <libKestrel/math/SIMD/float4.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/math/angle.hpp>

namespace kestrel::math
{
    struct lua_api(Point, Available_0_8) point
    {
        has_constructable_lua_api(point);

        point() = default;
        explicit point(float v);
        point(const point& p);
        point(point&&) noexcept = default;
        explicit point(SIMD::float4 v);

        auto operator=(const point&) -> point& = default;

        lua_constructor(Available_0_8) point(float x, float y);

        lua_getter(x, Available_0_8) [[nodiscard]] auto x() const -> float;
        lua_setter(x, Available_0_8) auto set_x(float x) -> void;
        lua_getter(y, Available_0_8) [[nodiscard]] auto y() const -> float;
        lua_setter(y, Available_0_8) auto set_y(float y) -> void;

        auto operator+(const point& p) const -> point;
        auto operator-(const point& p) const -> point;
        auto operator*(const point& p) const -> point;
        auto operator/(const point& p) const -> point;

        auto operator*(float f) const -> point;
        auto operator/(float f) const -> point;

        auto operator==(const point& p) const -> bool;
        auto operator!=(const point& p) const -> bool;

        [[nodiscard]] lua_function(add, Available_0_8) auto add(const point& p) const -> point;
        [[nodiscard]] lua_function(subtract, Available_0_8) auto subtract(const point& p) const -> point;
        [[nodiscard]] lua_function(multiply, Available_0_8) auto multiply(float f) const -> point;
        [[nodiscard]] lua_function(divide, Available_0_8) auto divide(float f) const -> point;

        [[nodiscard]] lua_function(round, Available_0_8) auto round() const -> point;
        [[nodiscard]] lua_function(floor, Available_0_8) auto floor() const -> point;
        [[nodiscard]] lua_function(ceil, Available_0_8) auto ceil() const -> point;

        [[nodiscard]] lua_function(distanceTo, Available_0_8) auto distance_to(const point& p) const -> float;
        [[nodiscard]] lua_getter(magnitude, Available_0_8) auto magnitude() const -> float;
        [[nodiscard]] lua_function(dotProduct, Available_0_8) auto dot_product(const point& p) const -> float;

        [[nodiscard]] lua_getter(angle, Available_0_8) auto angle() const -> angle;
        [[nodiscard]] lua_function(angleTo, Available_0_8) auto angle_to(const point& p) const -> struct angle;

        lua_function(normalize, Available_0_8) auto normalize() -> void;
        [[nodiscard]] lua_getter(normalized, Available_0_8) auto normalized() const -> point;

        [[nodiscard]] lua_function(rotateAround, Available_0_9) auto rotate_around(const point& c, const struct angle& angle) const -> point;

        friend struct line;
        friend struct vec2;
        friend struct rect;
        friend struct triangle;
    private:
        SIMD::float4 m_value;
    };
}