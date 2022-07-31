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

#include "math/point.hpp"
#include "math/size.hpp"
#include "scripting/state.hpp"
#include "util/hint.hpp"
#include <libGraphite/quickdraw/type/rect.hpp>

namespace math
{

    /**
     * Represents a frame/area in 2D space.
     */
    struct rect: public scripting::lua::object
    {
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

        math::point origin;
        math::size size;

        rect();
        rect(const math::point& o, const math::size& s);
        lua_api rect(double x, double y, double w, double h);
        rect(const math::rect& r);
        rect(const graphite::quickdraw::rect<std::int16_t>& r);

        lua_api static auto macintosh_rect(double top, double left, double bottom, double right) -> math::rect;

        lua_api auto operator+(const math::point& p) const -> math::rect;
        lua_api auto operator+(const math::size& s) const -> math::rect;
        lua_api auto operator-(const math::point& p) const -> math::rect;
        lua_api auto operator-(const math::size& s) const -> math::rect;
        lua_api auto operator*(double f) const -> math::rect;
        lua_api auto operator/(double f) const -> math::rect;
        auto operator==(const math::rect& r) const -> bool;
        auto operator!=(const math::rect& r) const -> bool;

        [[nodiscard]] lua_api auto round() const -> math::rect;
        [[nodiscard]] lua_api auto floor() const -> math::rect;
        [[nodiscard]] lua_api auto ceil() const -> math::rect;

        [[nodiscard]] lua_api auto area() const -> double;
        [[nodiscard]] lua_api auto contains_point(const math::point& p) const -> bool;
        [[nodiscard]] lua_api auto contains_rect(const math::rect& r) const -> bool;
        [[nodiscard]] lua_api auto intersects(const math::rect& r) const -> bool;

        lua_api auto set_origin(const math::point& origin) -> void;
        [[nodiscard]] lua_api auto get_origin() const -> math::point;

        lua_api auto set_size(const math::size& size) -> void;
        [[nodiscard]] lua_api auto get_size() const -> math::size;

        lua_api auto set_x(double x) -> void;
        [[nodiscard]] lua_api auto get_x() const -> double;
        [[nodiscard]] lua_api auto get_max_x() const -> double;

        lua_api auto set_y(double y) -> void;
        [[nodiscard]] lua_api auto get_y() const -> double;
        [[nodiscard]] lua_api auto get_max_y() const -> double;

        lua_api auto set_width(double width) -> void;
        [[nodiscard]] lua_api auto get_width() const -> double;

        lua_api auto set_height(double height) -> void;
        [[nodiscard]] lua_api auto get_height() const -> double;

        [[nodiscard]] lua_api auto inset(double amount) const -> math::rect;
    };

}
