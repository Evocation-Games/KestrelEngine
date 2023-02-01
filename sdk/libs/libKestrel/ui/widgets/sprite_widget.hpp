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
#include <libKestrel/math/rect.hpp>
#include <libKestrel/ui/entity/scene_entity.hpp>
#include <libKestrel/ui/widgets/image_widget.hpp>

namespace kestrel::ui::widgets
{
    struct lua_api(Widget.Sprite, Available_0_8) sprite_widget
    {
    public:
        has_constructable_lua_api(sprite_widget);

        luatool_type_fix(const luabridge::LuaRef&, sprite)
        lua_constructor(Available_0_8) explicit sprite_widget(const luabridge::LuaRef& sprite);

        lua_getter(entity, Available_0_8) [[nodiscard]] auto entity() const -> ui::scene_entity::lua_reference;
        lua_function(draw, Available_0_8) auto draw() -> void;

        lua_getter(frame, Available_0_8) [[nodiscard]] auto frame() const -> math::rect;
        lua_getter(image, Available_0_8) [[nodiscard]] auto image() const -> luabridge::LuaRef;
        lua_getter(frameNumber, Available_0_8) [[nodiscard]] auto frame_number() const -> std::int32_t;

        lua_setter(frame, Available_0_8) auto set_frame(const math::rect& frame) -> void;
        lua_setter(image, Available_0_8) auto set_image(const luabridge::LuaRef& image) -> void;
        lua_setter(frameNumber, Available_0_8) auto set_frame_number(std::int32_t n) -> void;

    private:
        math::rect m_frame;
        luabridge::LuaRef m_sprite_ref { nullptr };
        scene_entity::lua_reference m_entity { nullptr };
        image_content_alignment m_alignment { image_content_alignment::center };

        auto resize(bool reload = false) -> void;
    };
}