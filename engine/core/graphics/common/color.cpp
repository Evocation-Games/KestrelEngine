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

#include "core/graphics/common/color.hpp"

// MARK: - Lua

auto graphics::color::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginClass<graphics::color>("Color")
            .addConstructor<auto(*)(const double&, const double&, const double&, const double&)->void, graphics::color::lua_reference>()
            .addStaticFunction("white", &graphics::color::white)
            .addStaticFunction("rgb", &graphics::color::rgb)
            .addStaticFunction("color_value", &graphics::color::color_value)
            .addProperty("red", &graphics::color::get_red, &graphics::color::set_red)
            .addProperty("green", &graphics::color::get_green, &graphics::color::set_green)
            .addProperty("blue", &graphics::color::get_blue, &graphics::color::set_blue)
            .addProperty("alpha", &graphics::color::get_alpha, &graphics::color::set_alpha)
        .endClass();
}

// MARK: - Construction

#define COMPONENT_SCALE(_c) (static_cast<double>(_c) / 255.0)
#define COMPONENT_EXPAND(_c) (static_cast<uint8_t>((_c) * 255.0))

graphics::color::color(const double &w, const double &a)
    : red(w), green(w), blue(w), alpha(a)
{

}

graphics::color::color(const double &r, const double &g, const double &b, const double &a)
    : red(r), green(g), blue(b), alpha(a)
{

}

auto graphics::color::white(const uint8_t& w, const uint8_t& a) -> graphics::color
{
    return graphics::color(COMPONENT_SCALE(w), COMPONENT_SCALE(a))
}

auto graphics::color::rgb(const uint8_t& r, const uint8_t& g, const uint8_t& b, const uint8_t& a) -> graphics::color
{
    return graphics::color(COMPONENT_SCALE(r), COMPONENT_SCALE(g), COMPONENT_SCALE(b), COMPONENT_SCALE(a))
}

auto graphics::color::color_value(const uint32_t& value) -> graphics::color
{
    return graphics::color(
        COMPONENT_SCALE(static_cast<uint8_t>(value >> 16U)),
        COMPONENT_SCALE(static_cast<uint8_t>(value >> 8U)),
        COMPONENT_SCALE(static_cast<uint8_t>(value))
    );
}

// MARK: - Lua Accessors

auto graphics::color::set_red(const uint8_t& red) -> void
{
    this->red = COMPONENT_SCALE(red);
}

auto graphics::color::get_red() const -> uint8_t
{
    return COMPONENT_EXPAND(red);
}

auto graphics::color::set_green(const uint8_t& green) -> void
{
    this->green = COMPONENT_SCALE(green);
}

auto graphics::color::get_green() const -> uint8_t
{
    return COMPONENT_EXPAND(green);
}

auto graphics::color::set_blue(const uint8_t& blue) -> void
{
    this->blue = COMPONENT_SCALE(blue);
}

auto graphics::color::get_blue() const -> uint8_t
{
    return COMPONENT_EXPAND(blue);
}

auto graphics::color::set_alpha(const uint8_t& alpha) -> void
{
    this->alpha = COMPONENT_SCALE(alpha);
}

auto graphics::color::get_alpha() const -> uint8_t
{
    return COMPONENT_EXPAND(alpha);
}

