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

#if !defined(KESTREL_ANGLE_HPP)
#define KESTREL_ANGLE_HPP

#include "scripting/state.hpp"
#include "util/hint.hpp"

namespace math
{

    struct vector;
    struct angular_difference;

    /**
     * Represents an angle.
     *
     * This is used by the physics engine to determine represent the angle between two vectors,
     * as required by certain calculations (trajectories, movements, etc)
     */
    struct angle: public scripting::lua::object
    {
    private:
        double m_theta { 0.0 };

    public:
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

        lua_api explicit angle(const double& theta = 0.0);
        angle(const math::angle& a);

        lua_api auto degrees() const -> double;
        lua_api auto radians() const -> double;

        lua_api auto sin(const double& magnitude = 1.0) const -> double;
        lua_api auto cos(const double& magnitude = 1.0) const -> double;

        lua_api auto fsin(const double& magnitude = 1.0) const -> double;
        lua_api auto fcos(const double& magnitude = 1.0) const -> double;

        auto operator+ (const math::angle& a) const -> math::angle;
        auto operator- (const math::angle& a) const -> math::angle;
        auto operator+ (const math::angular_difference& a) const -> math::angle;
        auto operator- (const math::angular_difference& a) const -> math::angle;

        lua_api auto add_angular_difference(const math::angular_difference& a) const -> math::angle;
        lua_api auto subtract_angular_difference(const math::angular_difference& a) const -> math::angle;

        lua_api auto vector(const double& magnitude = 1.0) const -> math::vector;

        lua_api auto opposite() const -> math::angle;
        lua_api auto rotated(const math::angular_difference& phi) const -> math::angle;

        lua_api auto normalize() -> void;

        lua_api auto is_opposing(const math::angle& a, const math::angular_difference& tolerance) const -> bool;
        lua_api auto is_equal(const math::angle& a, const math::angular_difference& tolerance) const -> bool;
    };

}

#endif //KESTREL_ANGLE_HPP
