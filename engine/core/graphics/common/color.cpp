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
            .addStaticFunction("whiteColor", &graphics::color::white_ref)
            .addStaticFunction("rgb", &graphics::color::rgb_ref)
            .addStaticFunction("colorValue", &graphics::color::color_value_ref)
            .addStaticFunction("white", &graphics::color::white_color_ref)
            .addStaticFunction("lightGrey", &graphics::color::light_grey_color_ref)
            .addStaticFunction("grey", &graphics::color::grey_color_ref)
            .addStaticFunction("darkGrey", &graphics::color::dark_grey_color_ref)
            .addStaticFunction("black", &graphics::color::black_color_ref)
            .addStaticFunction("red", &graphics::color::red_color_ref)
            .addStaticFunction("orange", &graphics::color::orange_color_ref)
            .addStaticFunction("yellow", &graphics::color::yellow_color_ref)
            .addStaticFunction("lime", &graphics::color::lime_color_ref)
            .addStaticFunction("green", &graphics::color::green_color_ref)
            .addStaticFunction("teal", &graphics::color::teal_color_ref)
            .addStaticFunction("blue", &graphics::color::blue_color_ref)
            .addStaticFunction("magenta", &graphics::color::magenta_color_ref)
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

graphics::color::color(const graphics::color &c)
    : red(c.red), green(c.green), blue(c.blue), alpha(c.alpha)
{

}

auto graphics::color::white(const uint8_t& w, const uint8_t& a) -> graphics::color
{
    return graphics::color(COMPONENT_SCALE(w), COMPONENT_SCALE(a));
}

auto graphics::color::rgb(const uint8_t& r, const uint8_t& g, const uint8_t& b, const uint8_t& a) -> graphics::color
{
    return graphics::color(COMPONENT_SCALE(r), COMPONENT_SCALE(g), COMPONENT_SCALE(b), COMPONENT_SCALE(a));
}

auto graphics::color::color_value(const uint32_t& value) -> graphics::color
{
    return graphics::color(
        COMPONENT_SCALE(static_cast<uint8_t>(value >> 16U)),
        COMPONENT_SCALE(static_cast<uint8_t>(value >> 8U)),
        COMPONENT_SCALE(static_cast<uint8_t>(value))
    );
}

auto graphics::color::white_ref(const uint8_t& w, const uint8_t& a) -> graphics::color::lua_reference
{
    return {new graphics::color(COMPONENT_SCALE(w), COMPONENT_SCALE(a))};
}

auto graphics::color::rgb_ref(const uint8_t& r, const uint8_t& g, const uint8_t& b, const uint8_t& a) -> graphics::color::lua_reference
{
    return {new graphics::color(COMPONENT_SCALE(r), COMPONENT_SCALE(g), COMPONENT_SCALE(b), COMPONENT_SCALE(a))};
}

auto graphics::color::color_value_ref(const uint32_t& value) -> graphics::color::lua_reference
{
    return {new graphics::color(
            COMPONENT_SCALE(static_cast<uint8_t>(value >> 16U)),
            COMPONENT_SCALE(static_cast<uint8_t>(value >> 8U)),
            COMPONENT_SCALE(static_cast<uint8_t>(value))
    )};
}

// MARK: - Blending

auto graphics::color::blend(const graphics::color &top) const -> graphics::color
{
    auto c = 1.0 - top.alpha;
    auto alp = std::min(1.0, top.alpha + (alpha * c));
    auto r = std::min(1.0, (red * c) + top.red);
    auto g = std::min(1.0, (green * c) + top.green);
    auto b = std::min(1.0, (blue * c) + top.blue);
    return { r, g, b, alp };
}

auto graphics::color::blend_in_place(const graphics::color &top) -> void
{
    auto c = 1.0 - top.alpha;
    auto alp = std::min(1.0, top.alpha + (alpha * c));
    red = std::min(1.0, (red * c) + top.red);
    green = std::min(1.0, (green * c) + top.green);
    blue = std::min(1.0, (blue * c) + top.blue);
    alpha = alp;
}

// MARK: - Predefined Colors

auto graphics::color::white_color() -> graphics::color
{
    return graphics::color(1.0, 1.0, 1.0);
}

auto graphics::color::light_grey_color() -> graphics::color
{
    return graphics::color(0.9, 0.9, 0.9);
}

auto graphics::color::grey_color() -> graphics::color
{
    return graphics::color(0.7, 0.7, 0.7);
}

auto graphics::color::dark_grey_color() -> graphics::color
{
    return graphics::color(0.5, 0.5, 0.5);
}

auto graphics::color::black_color() -> graphics::color
{
    return graphics::color(0.0, 0.0, 0.0);
}

auto graphics::color::red_color() -> graphics::color
{
    return graphics::color(1.0, 0.0, 0.0);
}

auto graphics::color::orange_color() -> graphics::color
{
    return graphics::color(1.0, 0.5, 0.0);
}

auto graphics::color::yellow_color() -> graphics::color
{
    return graphics::color(1.0, 1.0, 0.0);
}

auto graphics::color::lime_color() -> graphics::color
{
    return graphics::color(0.5, 1.0, 0.0);
}

auto graphics::color::green_color() -> graphics::color
{
    return graphics::color(0.0, 1.0, 0.0);
}

auto graphics::color::teal_color() -> graphics::color
{
    return graphics::color(0.0, 1.0, 0.5);
}

auto graphics::color::blue_color() -> graphics::color
{
    return graphics::color(0.0, 0.0, 1.0);
}

auto graphics::color::magenta_color() -> graphics::color
{
    return graphics::color(1.0, 0.0, 1.0);
}

// MARK: - Predefined Color References

auto graphics::color::white_color_ref() -> graphics::color::lua_reference
{
    return graphics::color::lua_reference(new graphics::color(white_color()));
}

auto graphics::color::light_grey_color_ref() -> graphics::color::lua_reference
{
    return graphics::color::lua_reference(new graphics::color(light_grey_color()));
}

auto graphics::color::grey_color_ref() -> graphics::color::lua_reference
{
    return graphics::color::lua_reference(new graphics::color(grey_color()));
}

auto graphics::color::dark_grey_color_ref() -> graphics::color::lua_reference
{
    return graphics::color::lua_reference(new graphics::color(dark_grey_color()));
}

auto graphics::color::black_color_ref() -> graphics::color::lua_reference
{
    return graphics::color::lua_reference(new graphics::color(black_color()));
}

auto graphics::color::red_color_ref() -> graphics::color::lua_reference
{
    return graphics::color::lua_reference(new graphics::color(red_color()));
}

auto graphics::color::orange_color_ref() -> graphics::color::lua_reference
{
    return graphics::color::lua_reference(new graphics::color(orange_color()));
}

auto graphics::color::yellow_color_ref() -> graphics::color::lua_reference
{
    return graphics::color::lua_reference(new graphics::color(yellow_color()));
}

auto graphics::color::lime_color_ref() -> graphics::color::lua_reference
{
    return graphics::color::lua_reference(new graphics::color(lime_color()));
}

auto graphics::color::green_color_ref() -> graphics::color::lua_reference
{
    return graphics::color::lua_reference(new graphics::color(green_color()));
}

auto graphics::color::teal_color_ref() -> graphics::color::lua_reference
{
    return graphics::color::lua_reference(new graphics::color(teal_color()));
}

auto graphics::color::blue_color_ref() -> graphics::color::lua_reference
{
    return graphics::color::lua_reference(new graphics::color(blue_color()));
}

auto graphics::color::magenta_color_ref() -> graphics::color::lua_reference
{
    return graphics::color::lua_reference(new graphics::color(magenta_color()));
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

auto graphics::color::value() const -> uint32_t
{
    return static_cast<uint32_t>(COMPONENT_EXPAND(alpha) << 24U)
         | static_cast<uint32_t>(COMPONENT_EXPAND(red))
         | static_cast<uint32_t>(COMPONENT_EXPAND(green) << 8U)
         | static_cast<uint32_t>(COMPONENT_EXPAND(blue) << 16U);
}

