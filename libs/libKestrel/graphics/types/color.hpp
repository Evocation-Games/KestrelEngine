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

namespace kestrel::graphics
{
    /**
     * Represents an RGB Color value.
     */
    struct lua_api(Color, Available_0_8) color
    {
        has_constructable_lua_api(color);

        typedef std::uint8_t    component;
        typedef std::uint32_t   value;

        union rgba
        {
            value value;
            struct {
                component r;
                component g;
                component b;
                component a;
            } components;
        } rgba = { .value = 0 };

        explicit color(component w, component a = 255);
        explicit color(value value);
        color(const graphics::color& c);

        /**
         * Construct a new color by providing the red, green, blue and alpha components.
         * @param r     The red component of the color, with a value between 0 and 255.
         * @param g     The green component of the color, with a value between 0 and 255.
         * @param b     The blue component of the color, with a value between 0 and 255.
         * @param a     The alpha component of the color, with a value between 0 and 255.
         */
        lua_constructor(Available_0_8) color(component r, component g, component b, component a = 255);

        static auto white(component w, component a = 255) -> color;
        static auto rgb(component r, component g, component b, component a = 255) -> color;
        static auto color_value(value value) -> color;

        /**
         * Construct a new color that is a shade of grey, by providing a "whiteness"/"brightness" and alpha component.
         * @param w     The brightness of the color with a value between 0 (Black) and 255 (White).
         * @param a     The alpha component of the color, with a value between 0 and 255
         * @return      The constructed color value.
         */
        lua_function(white, Available_0_8) static auto white_ref(component w, component a = 255) -> lua_reference;

        lua_function(rgb, Available_0_8 Deprecated_0_8) static auto rgb_ref(component r, component g, component b, component a = 255) -> lua_reference;
        lua_function(colorValue, Available_0_8) static auto color_value_ref(value value) -> lua_reference;

        lua_function(withAlpha, Available_0_8) [[nodiscard]] auto with_alpha(component a) const -> color;

        [[nodiscard]] auto blend(const color& c) const -> color;
        auto blend_in_place(const color& c) -> void;

        static auto clear_color() -> color;
        static auto white_color() -> color;
        static auto light_grey_color() -> color;
        static auto grey_color() -> color;
        static auto dark_grey_color() -> color;
        static auto black_color() -> color;
        static auto red_color() -> color;
        static auto orange_color() -> color;
        static auto yellow_color() -> color;
        static auto lime_color() -> color;
        static auto green_color() -> color;
        static auto teal_color() -> color;
        static auto blue_color() -> color;
        static auto magenta_color() -> color;

        lua_function(whiteColor, Available_0_8) static auto white_color_ref() -> lua_reference;
        lua_function(lightGreyColor, Available_0_8) static auto light_grey_color_ref() -> lua_reference;
        lua_function(greyColor, Available_0_8) static auto grey_color_ref() -> lua_reference;
        lua_function(darkGreyColor, Available_0_8) static auto dark_grey_color_ref() -> lua_reference;
        lua_function(blackColor, Available_0_8) static auto black_color_ref() -> lua_reference;
        lua_function(redColor, Available_0_8) static auto red_color_ref() -> lua_reference;
        lua_function(orangeColor, Available_0_8) static auto orange_color_ref() -> lua_reference;
        lua_function(yellowColor, Available_0_8) static auto yellow_color_ref() -> lua_reference;
        lua_function(limeColor, Available_0_8) static auto lime_color_ref() -> lua_reference;
        lua_function(greenColor, Available_0_8) static auto green_color_ref() -> lua_reference;
        lua_function(tealColor, Available_0_8) static auto teal_color_ref() -> lua_reference;
        lua_function(blueColor, Available_0_8) static auto blue_color_ref() -> lua_reference;
        lua_function(magentaColor, Available_0_8) static auto magenta_color_ref() -> lua_reference;

        lua_setter(red, Available_0_8) auto set_red(component red) -> void;
        lua_getter(red, Available_0_8) [[nodiscard]] auto get_red() const -> component;

        lua_setter(green, Available_0_8) auto set_green(component green) -> void;
        lua_getter(green, Available_0_8) [[nodiscard]] auto get_green() const -> component;

        lua_setter(blue, Available_0_8) auto set_blue(component blue) -> void;
        lua_getter(blue, Available_0_8) [[nodiscard]] auto get_blue() const -> component;

        lua_setter(alpha, Available_0_8) auto set_alpha(component alpha) -> void;
        lua_getter(alpha, Available_0_8) [[nodiscard]] auto get_alpha() const -> component;

        lua_getter(value, Available_0_8) [[nodiscard]] auto color_value() const -> value;
    };

}
