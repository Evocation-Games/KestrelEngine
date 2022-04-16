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

#include "scripting/state.hpp"
#include "util/hint.hpp"

namespace math
{

    struct angle;

    /**
     * Represents a 2D point.
     */
    struct point: public scripting::lua::object
    {
    public:
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

        double x { 0 };
        double y { 0 };

        point() = default;
        explicit point(double v);
        lua_api point(double x, double y);
        point(const math::point& p);

        auto operator+(const math::point& p) const -> math::point;
        auto operator-(const math::point& p) const -> math::point;
        auto operator*(const math::point& p) const -> math::point;
        auto operator/(const math::point& p) const -> math::point;

        auto operator*(double f) const -> math::point;
        auto operator/(double f) const -> math::point;

        auto operator==(const math::point& p) const -> bool;
        auto operator!=(const math::point& p) const -> bool;

        [[nodiscard]] lua_api auto add(const math::point& p) const -> math::point;
        [[nodiscard]] lua_api auto subtract(const math::point& p) const -> math::point;
        [[nodiscard]] lua_api auto multiply(double f) const -> math::point;
        [[nodiscard]] lua_api auto divide(double f) const -> math::point;

        [[nodiscard]] lua_api auto round() const -> math::point;
        [[nodiscard]] lua_api auto floor() const -> math::point;
        [[nodiscard]] lua_api auto ceil() const -> math::point;

        [[nodiscard]] lua_api auto distance_to(const math::point& p) const -> double;
        [[nodiscard]] lua_api auto magnitude() const -> double;

        [[nodiscard]] lua_api auto angle() const -> math::angle;
        [[nodiscard]] lua_api auto angle_to(const math::point& p) const -> math::angle;

        lua_api auto set_x(double x) -> void;
        [[nodiscard]] lua_api auto get_x() const -> double;

        lua_api auto set_y(double y) -> void;
        [[nodiscard]] lua_api auto get_y() const -> double;
    };

}
