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

#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/lua/runtime/runtime.hpp>

namespace kestrel::math
{
    struct point;
    struct angular_difference;

    /**
     * Represents an angle.
     *
     * This is used by the physics engine to determine represent the angle between two vectors,
     * as required by certain calculations (trajectories, movements, etc)
     */
    struct lua_api(Angle, Available_0_8) angle
    {
    public:
        has_constructable_lua_api(angle);

        angle(const angle& a);
        lua_constructor(Available_0_8) explicit angle(float theta = 0.f);

        lua_getter(degrees, Available_0_8) [[nodiscard]] auto degrees() const -> float;
        lua_getter(radians, Available_0_8) [[nodiscard]] auto radians() const -> float;

        lua_function(sin, Available_0_8) [[nodiscard]] auto sin(float magnitude = 1.0) const -> float;
        lua_function(fsin, Available_0_8) [[nodiscard]] auto fsin(float magnitude = 1.0) const -> float;
        lua_function(cos, Available_0_8) [[nodiscard]] auto cos(float magnitude = 1.0) const -> float;
        lua_function(fcos, Available_0_8) [[nodiscard]] auto fcos(float magnitude = 1.0) const -> float;

        auto operator+ (const angle& a) const -> angle;
        auto operator- (const angle& a) const -> angle;
        auto operator+ (const angular_difference& a) const -> angle;
        auto operator- (const angular_difference& a) const -> angle;

        lua_function(addAngularDifference, Available_0_8) [[nodiscard]] auto add_angular_difference(const angular_difference& a) const -> angle;
        lua_function(subtractAngularDifference, Available_0_8) [[nodiscard]] auto subtract_angular_difference(const angular_difference& a) const -> angle;

        lua_function(vector, Available_0_8) [[nodiscard]] auto vector(float magnitude = 1.0) const -> point;

        lua_function(opposite, Available_0_8) [[nodiscard]] auto opposite() const -> angle;
        lua_function(rotated, Available_0_8) [[nodiscard]] auto rotated(const angular_difference& phi) const -> angle;

        lua_function(normalize, Available_0_8) auto normalize() -> void;

        lua_function(isOpposing, Available_0_8) [[nodiscard]] auto is_opposing(const math::angle& a, const math::angular_difference& tolerance) const -> bool;
        lua_function(isEqual,Available_0_8) [[nodiscard]] auto is_equal(const math::angle& a, const math::angular_difference& tolerance) const -> bool;

    private:
        float m_theta { 0.0 };
    };
}