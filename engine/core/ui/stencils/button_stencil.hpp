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
#include "scripting/state.hpp"
#include "util/hint.hpp"
#include "core/asset/static_image.hpp"
#include "core/graphics/common/color.hpp"
#include "core/graphics/common/canvas.hpp"
#include "math/size.hpp"

namespace ui::stencils
{
    struct button_stencil: public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<button_stencil> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

        enum class drawing_style: int { simple, stretch };

        struct info: public scripting::lua::object
        {
            typedef luabridge::RefCountedPtr<button_stencil::info> lua_reference;

            std::string text;
            std::string font;
            int16_t font_size;
            math::rect bounds;
            graphics::color::lua_reference text_color { new graphics::color(255, 255) };
            asset::static_image::lua_reference left { nullptr };
            asset::static_image::lua_reference center { nullptr };
            asset::static_image::lua_reference right { nullptr };
            asset::static_image::lua_reference mask_left { nullptr };
            asset::static_image::lua_reference mask_center { nullptr };
            asset::static_image::lua_reference mask_right { nullptr };
            asset::static_image::lua_reference icon { nullptr };
        };

    public:
        explicit button_stencil(drawing_style style);

        lua_api static auto simple(const luabridge::LuaRef& image) -> lua_reference;
        lua_api static auto stretch(const luabridge::LuaRef& left, const luabridge::LuaRef& center, const luabridge::LuaRef& right) -> lua_reference;

        lua_api auto apply_simple_mask(const luabridge::LuaRef& image) -> void;
        lua_api auto apply_stretch_mask(const luabridge::LuaRef& left, const luabridge::LuaRef& center, const luabridge::LuaRef& right) -> void;

        lua_api auto draw(const std::shared_ptr<graphics::canvas>& canvas, const stencils::button_stencil::info::lua_reference& info) -> void;

    private:
        drawing_style m_style;
        asset::static_image::lua_reference m_left { nullptr };
        asset::static_image::lua_reference m_center { nullptr };
        asset::static_image::lua_reference m_right { nullptr };
        asset::static_image::lua_reference m_mask_left { nullptr };
        asset::static_image::lua_reference m_mask_center { nullptr };
        asset::static_image::lua_reference m_mask_right { nullptr };
        luabridge::LuaRef m_drawing_function { nullptr };

    };
}
