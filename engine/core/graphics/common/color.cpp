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
            .addConstructor<auto(*)(const uint8_t&, const uint8_t&, const uint8_t&, const uint8_t&)->void, graphics::color::lua_reference>()
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
            .addFunction("with_alpha", &graphics::color::with_alpha)
        .endClass();
}

// MARK: - Construction

graphics::color::color(const uint8_t &w, const uint8_t &a)
    : rgba({ .components = { .r = w, .g = w, .b = w, .a = a }})
{

}

graphics::color::color(const uint8_t &r, const uint8_t &g, const uint8_t &b, const uint8_t &a)
    : rgba({ .components = { .r = r, .g = g, .b = b, .a = a }})
{

}

graphics::color::color(const uint32_t& value)
    : rgba({ .value = value })
{

}

graphics::color::color(const graphics::color &c)
    : rgba(c.rgba)
{

}

auto graphics::color::white(const uint8_t& w, const uint8_t& a) -> graphics::color
{
    return graphics::color(w, a);
}

auto graphics::color::rgb(const uint8_t& r, const uint8_t& g, const uint8_t& b, const uint8_t& a) -> graphics::color
{
    return graphics::color(r, g, b, a);
}

auto graphics::color::color_value(const uint32_t& value) -> graphics::color
{
    return graphics::color(value);
}

auto graphics::color::white_ref(const uint8_t& w, const uint8_t& a) -> graphics::color::lua_reference
{
    return { new graphics::color(w, a) };
}

auto graphics::color::rgb_ref(const uint8_t& r, const uint8_t& g, const uint8_t& b, const uint8_t& a) -> graphics::color::lua_reference
{
    return { new graphics::color(r, g, b, a) };
}

auto graphics::color::color_value_ref(const uint32_t& value) -> graphics::color::lua_reference
{
    union rgba t = { .value = value };

    auto tmp = t.components.r;
    t.components.r = t.components.b;
    t.components.b = tmp;

    return { new graphics::color(t.value | 0xFF000000) };
}

// MARK: - Blending

auto graphics::color::blend(const graphics::color &top) const -> graphics::color
{
    return top;
//    auto c = 1.0 - top.alpha;
//    auto alp = std::min(1.0, top.alpha + (alpha * c));
//    auto r = std::min(1.0, (red * c) + top.red);
//    auto g = std::min(1.0, (green * c) + top.green);
//    auto b = std::min(1.0, (blue * c) + top.blue);
//    return { r, g, b, alp };
}

auto graphics::color::blend_in_place(const graphics::color &top) -> void
{
    rgba.value = top.rgba.value;
//    if (alpha >= 1.0) {
//        alpha = 1.0;
//        red = (top.red * top.alpha) + (red * (1.0 - top.alpha));
//        green = (top.green * top.alpha) + (green * (1.0 - top.alpha));
//        blue = (top.blue * top.alpha) + (blue * (1.0 - top.alpha));
//    }
//    else {
//        auto a = alpha;
//        alpha = top.alpha + (alpha * (1.0 - top.alpha));
//        red = ((top.red * top.alpha) + (red * a * (1.0 - top.alpha))) / alpha;
//        green = ((top.green * top.alpha) + (green * a * (1.0 - top.alpha))) / alpha;
//        blue = ((top.blue * top.alpha) + (blue * a * (1.0 - top.alpha))) / alpha;
//    }
}

auto graphics::color::with_alpha(const uint8_t& a) const -> graphics::color
{
    return graphics::color(rgba.components.r, rgba.components.g, rgba.components.b, a);
}


// MARK: - Predefined Colors

auto graphics::color::clear_color() -> graphics::color
{
    return graphics::color(0, 0, 0, 0);
}

auto graphics::color::white_color() -> graphics::color
{
    return graphics::color(255, 255, 255);
}

auto graphics::color::light_grey_color() -> graphics::color
{
    return graphics::color(230, 230, 230);
}

auto graphics::color::grey_color() -> graphics::color
{
    return graphics::color(179, 179, 179);
}

auto graphics::color::dark_grey_color() -> graphics::color
{
    return graphics::color(128, 128, 128);
}

auto graphics::color::black_color() -> graphics::color
{
    return graphics::color(0, 0, 0);
}

auto graphics::color::red_color() -> graphics::color
{
    return graphics::color(255, 0, 0);
}

auto graphics::color::orange_color() -> graphics::color
{
    return graphics::color(255, 128, 0);
}

auto graphics::color::yellow_color() -> graphics::color
{
    return graphics::color(255, 255, 0);
}

auto graphics::color::lime_color() -> graphics::color
{
    return graphics::color(128, 255, 0);
}

auto graphics::color::green_color() -> graphics::color
{
    return graphics::color(0, 255, 0);
}

auto graphics::color::teal_color() -> graphics::color
{
    return graphics::color(0, 255, 128);
}

auto graphics::color::blue_color() -> graphics::color
{
    return graphics::color(0, 0, 255);
}

auto graphics::color::magenta_color() -> graphics::color
{
    return graphics::color(255, 0, 255);
}

// MARK: - Predefined Color References

auto graphics::color::white_color_ref() -> graphics::color::lua_reference
{
    return { new graphics::color(white_color()) };
}

auto graphics::color::light_grey_color_ref() -> graphics::color::lua_reference
{
    return { new graphics::color(light_grey_color()) };
}

auto graphics::color::grey_color_ref() -> graphics::color::lua_reference
{
    return { new graphics::color(grey_color()) };
}

auto graphics::color::dark_grey_color_ref() -> graphics::color::lua_reference
{
    return { new graphics::color(dark_grey_color()) };
}

auto graphics::color::black_color_ref() -> graphics::color::lua_reference
{
    return { new graphics::color(black_color()) };
}

auto graphics::color::red_color_ref() -> graphics::color::lua_reference
{
    return { new graphics::color(red_color()) };
}

auto graphics::color::orange_color_ref() -> graphics::color::lua_reference
{
    return { new graphics::color(orange_color()) };
}

auto graphics::color::yellow_color_ref() -> graphics::color::lua_reference
{
    return { new graphics::color(yellow_color()) };
}

auto graphics::color::lime_color_ref() -> graphics::color::lua_reference
{
    return { new graphics::color(lime_color()) };
}

auto graphics::color::green_color_ref() -> graphics::color::lua_reference
{
    return { new graphics::color(green_color()) };
}

auto graphics::color::teal_color_ref() -> graphics::color::lua_reference
{
    return { new graphics::color(teal_color()) };
}

auto graphics::color::blue_color_ref() -> graphics::color::lua_reference
{
    return { new graphics::color(blue_color()) };
}

auto graphics::color::magenta_color_ref() -> graphics::color::lua_reference
{
    return { new graphics::color(magenta_color()) };
}


// MARK: - Lua Accessors

auto graphics::color::set_red(const uint8_t& red) -> void
{
    rgba.components.r = red;
}

auto graphics::color::get_red() const -> uint8_t
{
    return rgba.components.r;
}

auto graphics::color::set_green(const uint8_t& green) -> void
{
    rgba.components.g = green;
}

auto graphics::color::get_green() const -> uint8_t
{
    return rgba.components.g;
}

auto graphics::color::set_blue(const uint8_t& blue) -> void
{
    rgba.components.b = blue;
}

auto graphics::color::get_blue() const -> uint8_t
{
    return rgba.components.b;
}

auto graphics::color::set_alpha(const uint8_t& alpha) -> void
{
    rgba.components.a = alpha;
}

auto graphics::color::get_alpha() const -> uint8_t
{
    return rgba.components.a;
}

auto graphics::color::value() const -> uint32_t
{
    return rgba.value;
}
