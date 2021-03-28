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

#if !defined(KESTREL_VECTOR_HPP)
#define KESTREL_VECTOR_HPP

#include "scripting/state.hpp"
#include "util/hint.hpp"

namespace math
{

    struct angle;

#if !defined(KESTREL_POINT_HPP)
    struct point;
#endif

    /**
     * Represents a 3D point.
     *
     * Unlike the `math::point` structure, this structure is intended for use in the physics engine
     * and for specifying locations of game entities.
     */
    struct vector: public scripting::lua::object
    {
    public:
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

        double x;
        double y;
        double z;

        vector();
        vector(const double& v);
        lua_api vector(const double& x, const double& y); // z = 1.0
        lua_api vector(const double& x, const double& y, const double& z);
        vector(const math::vector& v);

        lua_api auto operator+(const math::vector& v) const -> math::vector;
        lua_api auto operator-(const math::vector& v) const -> math::vector;
        lua_api auto operator*(const double& f) const -> math::vector;
        lua_api auto operator/(const double& f) const -> math::vector;
        auto operator==(const math::vector& v) const -> bool;
        auto operator!=(const math::vector& v) const -> bool;

        lua_api auto point() const -> math::point;

        lua_api auto angle() const -> math::angle;
        lua_api auto angle_to(const math::vector& v) const -> math::angle;

        lua_api auto distance_to(const math::vector& v) const -> double;
        lua_api auto magnitude() const -> double;

        lua_api auto set_x(const double& x) -> void;
        lua_api auto get_x() const -> double;

        lua_api auto set_y(const double& y) -> void;
        lua_api auto get_y() const -> double;

        lua_api auto set_z(const double& z) -> void;
        lua_api auto get_z() const -> double;
    };

};


#endif //KESTREL_VECTOR_HPP
