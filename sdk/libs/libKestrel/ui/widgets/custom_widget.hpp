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
#include <libKestrel/math/size.hpp>
#include <libKestrel/math/point.hpp>
#include <libKestrel/graphics/types/color.hpp>
#include <libKestrel/graphics/canvas/canvas.hpp>
#include <libKestrel/ui/entity/scene_entity.hpp>

namespace kestrel::ui::widgets
{
    struct lua_api(Widget.Custom, Available_0_8) custom_widget
    {
    public:
        has_constructable_lua_api(custom_widget);

        luatool_type_fix(const luabridge::LuaRef&, drawing_function)
        lua_constructor(Available_0_8) explicit custom_widget(const luabridge::LuaRef& drawing_function);

        lua_getter(entity, Available_0_8) [[nodiscard]] auto entity() const -> ui::scene_entity::lua_reference;
        lua_function(draw, Available_0_8) auto draw() -> void;

        lua_getter(frame, Available_0_8) [[nodiscard]] auto frame() const -> math::rect;
        lua_getter(userInfo, Available_0_8) [[nodiscard]] auto user_info() const -> luabridge::LuaRef;
        lua_getter(scrollOffset, Available_0_8) [[nodiscard]] auto scroll_offset() const -> math::point;

        lua_setter(frame, Available_0_8) auto set_frame(const math::rect& frame) -> void;
        lua_setter(userInfo, Available_0_8) auto set_user_info(const luabridge::LuaRef& info) -> void;
        lua_setter(scrollOffset, Available_0_8) auto set_scroll_offset(const math::point& offset) -> void;

        lua_function(addChildEntity, Available_0_8) auto add_child_entity(const scene_entity::lua_reference& entity) -> void;

        lua_function(drawingFunction, Available_0_8) auto set_drawing_function(const luabridge::LuaRef& block) -> void;

        lua_getter(anchorPoint, Available_0_9) [[nodiscard]] auto anchor_point() const -> layout::axis_origin;
        lua_setter(anchorPoint, Available_0_9) auto set_anchor_point(const layout::axis_origin& origin) -> void;

    private:
        bool m_dirty { true };
        luabridge::LuaRef m_drawing_function;
        luabridge::LuaRef m_user_info;
        graphics::canvas::lua_reference m_canvas;
        scene_entity::lua_reference m_entity { nullptr };
        math::rect m_frame { 0, 0, 100, 100 };
        math::point m_scroll_offset { 0 };

        auto resize() -> void;
        auto redraw() -> void;
    };
}
