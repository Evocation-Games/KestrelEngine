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

#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>

namespace kestrel::math
{
    struct angle;

    struct lua_api(AngularDifference, Available_0_8) angular_difference
    {
    public:
        has_constructable_lua_api(angular_difference);

        angular_difference(const angular_difference& d);
        lua_constructor(Available_0_8) explicit angular_difference(float phi = 0.0);

        lua_function(between, Available_0_8) static auto between(const angle& a, const angle& b) -> angular_difference;

        auto operator== (angular_difference& a) const -> bool;
        auto operator!= (angular_difference& a) const -> bool;
        auto operator<= (angular_difference& a) const -> bool;
        auto operator< (angular_difference& a) const -> bool;
        auto operator> (angular_difference& a) const -> bool;
        auto operator>= (angular_difference& a) const -> bool;

        lua_function(calculateAngleFrom, Available_0_8) [[nodiscard]] auto calculate_for(const angle& a) const -> angle;

        lua_getter(isClockwise, Available_0_8) [[nodiscard]] auto is_clockwise() const -> bool;
        lua_getter(isAntiClockwise, Available_0_8) [[nodiscard]] auto is_anti_clockwise() const -> bool;

        lua_function(isOpposing, Available_0_8) [[nodiscard]] auto is_opposing(const angular_difference& tolerance = angular_difference(0.0)) const -> bool;
        lua_function(isEqual, Available_0_8) [[nodiscard]] auto is_equal(const angular_difference& tolerance = angular_difference(0.0)) const -> bool;

        lua_getter(value, Available_0_8) [[nodiscard]] auto phi() const -> float;

    private:
        float m_phi { 0.0 };
    };

}
