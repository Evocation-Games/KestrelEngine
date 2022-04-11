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

    /**
     * Represents a 2D size.
     */
    struct size: public scripting::lua::object
    {
    public:
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

        double width;
        double height;

        size();
        explicit size(const double& v);
        lua_api size(const double& w, const double& h);
        size(const math::size& s);

        auto operator+(const math::size& s) const -> math::size;
        auto operator-(const math::size& s) const -> math::size;
        auto operator*(const math::size& s) const -> math::size;
        auto operator/(const math::size& s) const -> math::size;
        auto operator*(double f) const -> math::size;
        auto operator/(double f) const -> math::size;
        auto operator==(const math::size& s) const -> bool;
        auto operator!=(const math::size& s) const -> bool;

        lua_api auto add(const math::size& s) const -> math::size;
        lua_api auto subtract(const math::size& s) const -> math::size;
        lua_api auto multiply(double f) const -> math::size;
        lua_api auto divide(double f) const -> math::size;

        lua_api auto round() const -> math::size;
        lua_api auto floor() const -> math::size;
        lua_api auto ceil() const -> math::size;

        lua_api auto area() const -> double;

        lua_api auto set_width(const double& width) -> void;
        lua_api auto get_width() const -> double;

        lua_api auto set_height(const double& height) -> void;
        lua_api auto get_height() const -> double;
    };

};
