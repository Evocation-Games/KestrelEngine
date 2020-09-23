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

#if !defined(KESTREL_RECT_HPP)
#define KESTREL_RECT_HPP

#include "math/point.hpp"
#include "math/size.hpp"
#include "scripting/state.hpp"
#include "util/hint.hpp"

namespace math
{

    /**
     * Represents a frame/area in 2D space.
     */
    struct rect: public scripting::lua::object
    {
        typedef luabridge::RefCountedPtr<math::rect> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

        math::point origin;
        math::size size;

        rect();
        rect(const math::point& o, const math::size& s);
        lua_api rect(const double& x, const double& y, const double& w, const double& h);
        rect(const math::rect& r);

        lua_api auto operator+(const math::point& p) const -> math::rect;
        lua_api auto operator+(const math::size& s) const -> math::rect;
        lua_api auto operator-(const math::point& p) const -> math::rect;
        lua_api auto operator-(const math::size& s) const -> math::rect;
        lua_api auto operator*(const double& f) const -> math::rect;
        lua_api auto operator/(const double& f) const -> math::rect;
        auto operator==(const math::rect& r) const -> bool;
        auto operator!=(const math::rect& r) const -> bool;

        lua_api auto area() const -> double;
        lua_api auto contains_point(const math::point& p) const -> bool;
        lua_api auto contains_rect(const math::rect& r) const -> bool;
        lua_api auto intersects(const math::rect& r) const -> bool;

        lua_api auto set_origin(const math::point& origin) -> void;
        lua_api auto get_origin() const -> math::point;

        lua_api auto set_size(const math::size& size) -> void;
        lua_api auto get_size() const -> math::size;

        lua_api auto set_x(const double& x) -> void;
        lua_api auto get_x() const -> double;

        lua_api auto set_y(const double& y) -> void;
        lua_api auto get_y() const -> double;

        lua_api auto set_width(const double& width) -> void;
        lua_api auto get_width() const -> double;

        lua_api auto set_height(const double& height) -> void;
        lua_api auto get_height() const -> double;
    };

}

#endif //KESTREL_RECT_HPP
