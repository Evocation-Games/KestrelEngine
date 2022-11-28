// Copyright (c) 2022 Tom Hancocks
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

#include <memory>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/graphics/image/static_image.hpp>
#include <libKestrel/graphics/canvas/canvas.hpp>
#include <libKestrel/graphics/types/color.hpp>
#include <libKestrel/math/size.hpp>
#include <libKestrel/font/font.hpp>

namespace kestrel::ui::stencils
{

    enum button_drawing_style: std::int32_t { simple, stretch };

    struct lua_api(Widget.Stencil.ButtonRenderInfo, Available_0_8) button_render_info
    {
        has_constructable_lua_api(button_render_info);

        lua_data(text, const, Available_0_8) std::string text;
        lua_data(font, const, Available_0_8) font::reference::lua_reference font { nullptr };
        lua_data(bounds, const, Available_0_8) math::rect bounds;
        lua_data(textColor, const, Available_0_8) graphics::color::lua_reference text_color { new graphics::color(255, 255) };
        lua_data(leftCapImage, const, Available_0_8) image::static_image::lua_reference left { nullptr };
        lua_data(centerImage, const, Available_0_8) image::static_image::lua_reference center { nullptr };
        lua_data(rightCapImage, const, Available_0_8) image::static_image::lua_reference right { nullptr };
        lua_data(leftCapMask, const, Available_0_8) image::static_image::lua_reference mask_left { nullptr };
        lua_data(centerMask, const, Available_0_8) image::static_image::lua_reference mask_center { nullptr };
        lua_data(rightCapMask, const, Available_0_8) image::static_image::lua_reference mask_right { nullptr };
        lua_data(icon, const, Available_0_8) image::static_image::lua_reference icon { nullptr };
    };

    struct lua_api(Widget.Stencil.Button, Available_0_8) button_stencil
    {
    public:
        has_constructable_lua_api(button_stencil);

        explicit button_stencil(button_drawing_style style);

        lua_function(simple, Available_0_8) static auto simple(const luabridge::LuaRef& image) -> lua_reference;
        lua_function(stretch, Available_0_8) static auto stretch(const luabridge::LuaRef& left, const luabridge::LuaRef& center, const luabridge::LuaRef& right) -> lua_reference;

        lua_function(applySimpleMask, Available_0_8) auto apply_simple_mask(const luabridge::LuaRef& image) -> void;
        lua_function(applyStretchMask, Available_0_8) auto apply_stretch_mask(const luabridge::LuaRef& left, const luabridge::LuaRef& center, const luabridge::LuaRef& right) -> void;

        auto draw(const std::shared_ptr<graphics::canvas>& canvas, const stencils::button_render_info::lua_reference& info) -> void;

    private:
        button_drawing_style m_style;
        image::static_image::lua_reference m_left { nullptr };
        image::static_image::lua_reference m_center { nullptr };
        image::static_image::lua_reference m_right { nullptr };
        image::static_image::lua_reference m_mask_left { nullptr };
        image::static_image::lua_reference m_mask_center { nullptr };
        image::static_image::lua_reference m_mask_right { nullptr };
        luabridge::LuaRef m_drawing_function { nullptr };

    };
}
