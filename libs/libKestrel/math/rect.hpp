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

#include <libKestrel/math/point.hpp>
#include <libKestrel/math/size.hpp>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libGraphite/quickdraw/type/rect.hpp>

namespace kestrel::math
{
    /**
     * Represents a frame/area in 2D space.
     */
    struct lua_api(Rect, Available_0_8) rect
    {
    public:
        has_constructable_lua_api(rect);

        struct point origin;
        struct size size;

        rect();
        rect(const struct point& o, const struct size& s);
        rect(const math::rect& r);
        explicit rect(const graphite::quickdraw::rect<std::int16_t>& r);

        lua_constructor(Available_0_8) rect(double x, double y, double w, double h);

        lua_function(macintosh, Available_0_8) static auto macintosh_rect(double top, double left, double bottom, double right) -> rect;

        auto operator+(const struct point& p) const -> rect;
        auto operator+(const struct size& s) const -> rect;
        auto operator-(const struct point& p) const -> rect;
        auto operator-(const struct size& s) const -> rect;
        lua_function(multiply, Available_0_8) auto operator*(double f) const -> rect;
        lua_function(divide, Available_0_8) auto operator/(double f) const -> rect;
        auto operator==(const rect& r) const -> bool;
        auto operator!=(const rect& r) const -> bool;

        lua_function(addPoint, Available_0_8) auto add_point(const struct point& p) const -> rect;
        lua_function(addSize, Available_0_8) auto add_size(const struct size& s) const -> rect;
        lua_function(subtractPoint, Available_0_8) auto subtract_point(const struct point& p) const -> rect;
        lua_function(subtractSize, Available_0_8) auto subtract_size(const struct size& s) const -> rect;

        lua_function(round, Available_0_8) [[nodiscard]] auto round() const -> rect;
        lua_function(floor, Available_0_8) [[nodiscard]] auto floor() const -> rect;
        lua_function(ceil, Available_0_8) [[nodiscard]] auto ceil() const -> rect;

        lua_getter(area, Available_0_8) [[nodiscard]] auto area() const -> double;
        lua_function(containsPoint, Available_0_8) [[nodiscard]] auto contains_point(const struct point& p) const -> bool;
        lua_function(containsRect, Available_0_8) [[nodiscard]] auto contains_rect(const rect& r) const -> bool;
        lua_function(intersects, Available_0_8) [[nodiscard]] auto intersects(const rect& r) const -> bool;

        lua_setter(origin, Available_0_8) auto set_origin(const struct point& origin) -> void;
        lua_getter(origin, Available_0_8) [[nodiscard]] auto get_origin() const -> struct point;

        lua_setter(size, Available_0_8) auto set_size(const struct size& size) -> void;
        lua_getter(size, Available_0_8) [[nodiscard]] auto get_size() const -> struct size;

        lua_setter(x, Available_0_8) auto set_x(double x) -> void;
        lua_getter(x, Available_0_8) [[nodiscard]] auto get_x() const -> double;
        lua_getter(maxX, Available_0_8) [[nodiscard]] auto get_max_x() const -> double;

        lua_setter(y, Available_0_8) auto set_y(double y) -> void;
        lua_getter(y, Available_0_8) [[nodiscard]] auto get_y() const -> double;
        lua_getter(maxY, Available_0_8) [[nodiscard]] auto get_max_y() const -> double;

        lua_setter(width, Available_0_8) auto set_width(double width) -> void;
        lua_getter(width, Available_0_8) [[nodiscard]] auto get_width() const -> double;

        lua_setter(height, Available_0_8) auto set_height(double height) -> void;
        lua_getter(height, Available_0_8) [[nodiscard]] auto get_height() const -> double;

        lua_function(inset, Available_0_8) [[nodiscard]] auto inset(double amount) const -> rect;
    };

}
