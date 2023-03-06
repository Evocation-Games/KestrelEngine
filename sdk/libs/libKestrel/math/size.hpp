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

#include <libSIMD/float32.hpp>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>

namespace kestrel::math
{

    /**
     * Represents a 2D size.
     */
    struct lua_api(Size, Available_0_8) size
    {
    public:
        has_constructable_lua_api(size);

        size() = default;
        explicit size(float v);
        size(size&& v) noexcept = default;
        size(const size& v) = default;
        explicit size(simd::float32 v);

        auto operator=(const size&) -> size& = default;

        lua_constructor(Available_0_8) size(float w, float h);

        auto operator+(const size& s) const -> size;
        auto operator-(const size& s) const -> size;
        auto operator*(const size& s) const -> size;
        auto operator/(const size& s) const -> size;

        auto operator+(float f) const -> size;
        auto operator-(float f) const -> size;
        auto operator*(float f) const -> size;
        auto operator/(float f) const -> size;

        auto operator==(const size& s) const -> bool;
        auto operator!=(const size& s) const -> bool;

        lua_function(add, Available_0_8) [[nodiscard]] auto add(const size& s) const -> size;
        lua_function(subtract, Available_0_8) [[nodiscard]] auto subtract(const size& s) const -> size;
        lua_function(multiply, Available_0_8) [[nodiscard]] auto multiply(float f) const -> size;
        lua_function(divide, Available_0_8) [[nodiscard]] auto divide(float f) const -> size;

        lua_function(round, Available_0_8) [[nodiscard]] auto round() const -> size;
        lua_function(floor, Available_0_8) [[nodiscard]] auto floor() const -> size;
        lua_function(ceil, Available_0_8) [[nodiscard]] auto ceil() const -> size;

        lua_getter(area, Available_0_8) [[nodiscard]] auto area() const -> float;
        lua_getter(aspectRatio, Available_0_8) [[nodiscard]] auto aspect_ratio() const -> float;
        lua_getter(isPortrait, Available_0_8) [[nodiscard]] auto is_portrait() const -> bool;
        lua_getter(isLandscape, Available_0_8) [[nodiscard]] auto is_landscape() const -> bool;

        lua_setter(width, Available_0_8) auto set_width(float width) -> void;
        lua_getter(width, Available_0_8) [[nodiscard]] auto width() const -> float;

        lua_setter(height, Available_0_8) auto set_height(float height) -> void;
        lua_getter(height, Available_0_8) [[nodiscard]] auto height() const -> float;

        friend struct triangulated_polygon;
        friend struct vec2;
        friend struct rect;

    private:
        simd::float32 m_value;
    };

};
