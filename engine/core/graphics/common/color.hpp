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

#if !defined(KESTREL_COLOR_HPP)
#define KESTREL_COLOR_HPP

#include "scripting/state.hpp"
#include "util/hint.hpp"

namespace graphics
{

    struct color: public scripting::lua::object
    {
        typedef luabridge::RefCountedPtr<graphics::color> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

        double red;
        double green;
        double blue;
        double alpha;

        explicit color(const double& w, const double& a = 1.0);
        color(const graphics::color& c);
        lua_api color(const double& r, const double& g, const double& b, const double& a = 1.0);

        static auto white(const uint8_t& w, const uint8_t& a = 255) -> graphics::color;
        static auto rgb(const uint8_t& r, const uint8_t& g, const uint8_t& b, const uint8_t& a = 255) -> graphics::color;
        static auto color_value(const uint32_t& value) -> graphics::color;

        lua_api static auto white_ref(const uint8_t& w, const uint8_t& a = 255) -> graphics::color::lua_reference ;
        lua_api static auto rgb_ref(const uint8_t& r, const uint8_t& g, const uint8_t& b, const uint8_t& a = 255) -> graphics::color::lua_reference ;
        lua_api static auto color_value_ref(const uint32_t& value) -> graphics::color::lua_reference ;

        static auto white_color() -> graphics::color;
        static auto light_grey_color() -> graphics::color;
        static auto grey_color() -> graphics::color;
        static auto dark_grey_color() -> graphics::color;
        static auto black_color() -> graphics::color;
        static auto red_color() -> graphics::color;
        static auto orange_color() -> graphics::color;
        static auto yellow_color() -> graphics::color;
        static auto lime_color() -> graphics::color;
        static auto green_color() -> graphics::color;
        static auto teal_color() -> graphics::color;
        static auto blue_color() -> graphics::color;
        static auto magenta_color() -> graphics::color;

        lua_api static auto white_color_ref() -> graphics::color::lua_reference ;
        lua_api static auto light_grey_color_ref() -> graphics::color::lua_reference ;
        lua_api static auto grey_color_ref() -> graphics::color::lua_reference ;
        lua_api static auto dark_grey_color_ref() -> graphics::color::lua_reference ;
        lua_api static auto black_color_ref() -> graphics::color::lua_reference ;
        lua_api static auto red_color_ref() -> graphics::color::lua_reference ;
        lua_api static auto orange_color_ref() -> graphics::color::lua_reference ;
        lua_api static auto yellow_color_ref() -> graphics::color::lua_reference ;
        lua_api static auto lime_color_ref() -> graphics::color::lua_reference ;
        lua_api static auto green_color_ref() -> graphics::color::lua_reference ;
        lua_api static auto teal_color_ref() -> graphics::color::lua_reference ;
        lua_api static auto blue_color_ref() -> graphics::color::lua_reference ;
        lua_api static auto magenta_color_ref() -> graphics::color::lua_reference ;

        lua_api auto set_red(const uint8_t& red) -> void;
        lua_api auto get_red() const -> uint8_t;

        lua_api auto set_green(const uint8_t& green) -> void;
        lua_api auto get_green() const -> uint8_t;

        lua_api auto set_blue(const uint8_t& blue) -> void;
        lua_api auto get_blue() const -> uint8_t;

        lua_api auto set_alpha(const uint8_t& alpha) -> void;
        lua_api auto get_alpha() const -> uint8_t;

        lua_api auto value() const -> uint32_t;
    };

}

#endif //KESTREL_COLOR_HPP
