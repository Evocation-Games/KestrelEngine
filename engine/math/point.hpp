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

#if !defined(KESTREL_POINT_HPP)
#define KESTREL_POINT_HPP

#include "scripting/state.hpp"
#include "util/hint.hpp"

namespace math
{

#if !defined(KESTREL_VECTOR_HPP)
    struct vector;
#endif

    /**
     * Represents a 2D point.
     */
    struct point: public scripting::lua::object
    {
    public:
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

        double x;
        double y;

        point();
        explicit point(const double& v);
        lua_api point(const double& x, const double& y);
        point(const math::point& p);

        lua_api auto operator+(const math::point& p) const -> math::point;
        lua_api auto operator-(const math::point& p) const -> math::point;
        lua_api auto operator*(const double& f) const -> math::point;
        lua_api auto operator/(const double& f) const -> math::point;
        auto operator==(const math::point& p) const -> bool;
        auto operator!=(const math::point& p) const -> bool;

        lua_api auto round() const -> math::point;
        lua_api auto floor() const -> math::point;
        lua_api auto ceil() const -> math::point;

        lua_api auto distance_to(const math::point& p) const -> double;

        lua_api auto set_x(const double& x) -> void;
        lua_api auto get_x() const -> double;

        lua_api auto set_y(const double& y) -> void;
        lua_api auto get_y() const -> double;

        lua_api auto to_vector() const -> math::vector;
    };

};

#endif //KESTREL_POINT_HPP
