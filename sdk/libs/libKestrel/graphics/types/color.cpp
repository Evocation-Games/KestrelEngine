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

#include <libKestrel/graphics/types/color.hpp>

// MARK: - Construction

kestrel::graphics::color::color(component w, component a)
{
    rgba.components.a = a;
    rgba.components.r = rgba.components.g = rgba.components.b = w;
}

kestrel::graphics::color::color(component r, component g, component b, component a)
{
    rgba.components.a = a;
    rgba.components.r = r;
    rgba.components.g = g;
    rgba.components.b = b;
}

kestrel::graphics::color::color(value value)
    : rgba({ .value = value })
{

}

kestrel::graphics::color::color(const color& c)
    : rgba(c.rgba)
{

}

auto kestrel::graphics::color::white(component w, component a) -> color
{
    return color(w, a);
}

auto kestrel::graphics::color::rgb(component r, component g, component b, component a) -> color
{
    return { r, g, b, a };
}

auto kestrel::graphics::color::color_value(value value) -> color
{
    return color(value);
}

auto kestrel::graphics::color::white_ref(component w, component a) -> lua_reference
{
    return { new color(w, a) };
}

auto kestrel::graphics::color::rgb_ref(component r, component g, component b, component a) -> lua_reference
{
    return { new color(r, g, b, a) };
}

auto kestrel::graphics::color::color_value_ref(value value) -> lua_reference
{
    union rgba t = { .value = value };

    auto tmp = t.components.r;
    t.components.r = t.components.b;
    t.components.b = tmp;

    return { new color(t.value | 0xFF000000) };
}

// MARK: - Blending

auto kestrel::graphics::color::blend(const color &top) const -> color
{
    // TODO: Assess this commented out function and determine if it is needed or not.
    return top;
//    auto c = 1.0 - top.alpha;
//    auto alp = std::min(1.0, top.alpha + (alpha * c));
//    auto r = std::min(1.0, (red * c) + top.red);
//    auto g = std::min(1.0, (green * c) + top.green);
//    auto b = std::min(1.0, (blue * c) + top.blue);
//    return { r, g, b, alp };
}

auto kestrel::graphics::color::blend_in_place(const color &top) -> void
{
    // TODO: Assess this commented out function and determine if it is needed or not.
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

auto kestrel::graphics::color::with_alpha(component a) const -> lua_reference
{
    return { new color(rgba.components.r, rgba.components.g, rgba.components.b, a) };
}


// MARK: - Predefined Colors

auto kestrel::graphics::color::clear_color() -> color
{
    return { 0, 0, 0, 0 };
}

auto kestrel::graphics::color::white_color() -> color
{
    return { 255, 255, 255 };
}

auto kestrel::graphics::color::light_grey_color() -> color
{
    return { 230, 230, 230 };
}

auto kestrel::graphics::color::grey_color() -> color
{
    return { 179, 179, 179 };
}

auto kestrel::graphics::color::dark_grey_color() -> color
{
    return { 128, 128, 128 };
}

auto kestrel::graphics::color::black_color() -> color
{
    return { 0, 0, 0 };
}

auto kestrel::graphics::color::red_color() -> color
{
    return { 255, 0, 0 };
}

auto kestrel::graphics::color::orange_color() -> color
{
    return { 255, 128, 0 };
}

auto kestrel::graphics::color::yellow_color() -> color
{
    return { 255, 255, 0 };
}

auto kestrel::graphics::color::lime_color() -> color
{
    return { 128, 255, 0 };
}

auto kestrel::graphics::color::green_color() -> color
{
    return { 0, 255, 0 };
}

auto kestrel::graphics::color::teal_color() -> color
{
    return { 0, 255, 128 };
}

auto kestrel::graphics::color::blue_color() -> color
{
    return { 0, 0, 255 };
}

auto kestrel::graphics::color::magenta_color() -> color
{
    return { 255, 0, 255 };
}

// MARK: - Predefined Color References

auto kestrel::graphics::color::white_color_ref() -> lua_reference
{
    return { new color(white_color()) };
}

auto kestrel::graphics::color::light_grey_color_ref() -> lua_reference
{
    return { new color(light_grey_color()) };
}

auto kestrel::graphics::color::grey_color_ref() -> lua_reference
{
    return { new color(grey_color()) };
}

auto kestrel::graphics::color::dark_grey_color_ref() -> lua_reference
{
    return { new color(dark_grey_color()) };
}

auto kestrel::graphics::color::black_color_ref() -> lua_reference
{
    return { new color(black_color()) };
}

auto kestrel::graphics::color::red_color_ref() -> lua_reference
{
    return { new color(red_color()) };
}

auto kestrel::graphics::color::orange_color_ref() -> lua_reference
{
    return { new color(orange_color()) };
}

auto kestrel::graphics::color::yellow_color_ref() -> lua_reference
{
    return { new color(yellow_color()) };
}

auto kestrel::graphics::color::lime_color_ref() -> lua_reference
{
    return { new color(lime_color()) };
}

auto kestrel::graphics::color::green_color_ref() -> lua_reference
{
    return { new color(green_color()) };
}

auto kestrel::graphics::color::teal_color_ref() -> lua_reference
{
    return { new color(teal_color()) };
}

auto kestrel::graphics::color::blue_color_ref() -> lua_reference
{
    return { new color(blue_color()) };
}

auto kestrel::graphics::color::magenta_color_ref() -> lua_reference
{
    return { new color(magenta_color()) };
}

// MARK: - Lua Accessors

auto kestrel::graphics::color::set_red(component red) -> void
{
    rgba.components.r = red;
}

auto kestrel::graphics::color::get_red() const -> component
{
    return rgba.components.r;
}

auto kestrel::graphics::color::set_green(component green) -> void
{
    rgba.components.g = green;
}

auto kestrel::graphics::color::get_green() const -> component
{
    return rgba.components.g;
}

auto kestrel::graphics::color::set_blue(component blue) -> void
{
    rgba.components.b = blue;
}

auto kestrel::graphics::color::get_blue() const -> component
{
    return rgba.components.b;
}

auto kestrel::graphics::color::set_alpha(component alpha) -> void
{
    rgba.components.a = alpha;
}

auto kestrel::graphics::color::get_alpha() const -> component
{
    return rgba.components.a;
}

auto kestrel::graphics::color::color_value() const -> value
{
    return rgba.value;
}
