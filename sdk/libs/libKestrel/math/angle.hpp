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
     *
     * @example
     * local position = Angle(45):vector(100)
     *
     * @warning This is a test warning
     */
    struct lua_api(Angle, Available_0_8) angle
    {
    public:
        has_constructable_lua_api(angle);

        angle(const angle& a);

        /**
         * Construct a new `Angle`
         * @param theta The angle to be represented in degrees.
         */
        lua_constructor(Available_0_8) explicit angle(float theta = 0.f);

        /**
         * Returns the value of the angle in degrees.
         */
        lua_getter(degrees, Available_0_8) [[nodiscard]] auto degrees() const -> float;

        /**
         * Returns the value of the angle in radians.
         */
        lua_getter(radians, Available_0_8) [[nodiscard]] auto radians() const -> float;

        /**
         * Compute the sine of the angle to the specified magnitude.
         * @param magnitude The value that the sine will be multiplied by.
         * @warning         This method is more accurate than `Angle:fsin()`, but it is slower. Make sure you use the
         *                  most appropriate version for your use-case.
         */
        lua_function(sin, Available_0_8) [[nodiscard]] auto sin(float magnitude = 1.0) const -> float;

        /**
         * Compute the sine of the angle to the specified magnitude. The sin is determined using a look up table.
         * This is generally faster than using `Angle:sin()` but is less accurate.
         * @param magnitude The value that the sine will be multiplied by.
         * @warning         This method is less accurate than `Angle:sin()`, but it is faster. Make sure you use the
         *                  most appropriate version for your use-case.
         */
        lua_function(fsin, Available_0_8) [[nodiscard]] auto fsin(float magnitude = 1.0) const -> float;

        /**
         * Compute the cosine of the angle to the specified magnitude.
         * @param magnitude The value that the cosine will be multiplied by.
         * @warning         This method is more accurate than `Angle:fcos()`, but it is slower. Make sure you use the
         *                  most appropriate version for your use-case.
         */
        lua_function(cos, Available_0_8) [[nodiscard]] auto cos(float magnitude = 1.0) const -> float;

        /**
         * Compute the cosine of the angle to the specified magnitude. The sin is determined using a look up table.
         * This is generally faster than using `Angle:cos()` but is less accurate.
         * @param magnitude The value that the cosine will be multiplied by.
         * @warning         This method is less accurate than `Angle:cos()`, but it is faster. Make sure you use the
         *                  most appropriate version for your use-case.
         */
        lua_function(fcos, Available_0_8) [[nodiscard]] auto fcos(float magnitude = 1.0) const -> float;

        auto operator+ (const angle& a) const -> angle;
        auto operator- (const angle& a) const -> angle;
        auto operator+ (const angular_difference& a) const -> angle;
        auto operator- (const angular_difference& a) const -> angle;

        /**
         * Compute a new `Angle` by adding an `AngularDifference` to it.
         * @param a The `AngularDifference to add to the angle.
         */
        lua_function(addAngularDifference, Available_0_8) [[nodiscard]] auto add_angular_difference(const angular_difference& a) const -> angle;

        /**
         * Compute a new `Angle` by subtracting an `AngularDifference` to it.
         * @param a The `AngularDifference to subtract from the angle.
         */
        lua_function(subtractAngularDifference, Available_0_8) [[nodiscard]] auto subtract_angular_difference(const angular_difference& a) const -> angle;

        /**
         * Compute a point that is the specified distance from a zero point, at the given angle.
         * @param magnitude The distance from the zero point.
         */
        lua_function(vector, Available_0_8) [[nodiscard]] auto vector(float magnitude = 1.0) const -> point;

        /**
         * Compute the opposing angle to the receiver. This essentially is just adding 180º to the receiver and
         * then normalizing the result.
         */
        lua_function(opposite, Available_0_8) [[nodiscard]] auto opposite() const -> angle;

        /**
         * Compute an angle that is rotated from the receiver. The result will then be normalized.
         * @param phi The `AngularDifference` to add the receiver.
         */
        lua_function(rotated, Available_0_8) [[nodiscard]] auto rotated(const angular_difference& phi) const -> angle;

        /**
         * Normalize the receiver.
         */
        lua_function(normalize, Available_0_8) auto normalize() -> void;

        /**
         * Determine if the receiver is opposing the specified angle, within a specified tolerance.
         * @param a          The `Angle` to test the receiver against.
         * @param tolerance  The `AngularDifference` specifying the tolerance.
         * @return           A true value if the receiver is opposing the specified angle.
         */
        lua_function(isOpposing, Available_0_8) [[nodiscard]] auto is_opposing(const math::angle& a, const math::angular_difference& tolerance) const -> bool;

        /**
         * Determine if the receiver is equal to the specified angle, within a specified tolerance.
         * @param a          The `Angle` to test the receiver against.
         * @param tolerance  The `AngularDifference` specifying the tolerance.
         * @return           A true value if the receiver is equal to the specified angle.
         */
        lua_function(isEqual, Available_0_8) [[nodiscard]] auto is_equal(const math::angle& a, const math::angular_difference& tolerance) const -> bool;

    private:
        float m_theta { 0.0 };
    };
}