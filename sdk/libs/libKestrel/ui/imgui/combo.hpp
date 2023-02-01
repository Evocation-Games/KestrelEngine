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

#include <string>
#include <imgui/imgui.h>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/ui/imgui/widget.hpp>
#include <libKestrel/font/manager.hpp>
#include <libKestrel/graphics/sprites/sprite_sheet.hpp>

namespace kestrel::ui::imgui
{
    class lua_api(ImGui.Combo, Available_0_8) combo : public widget
    {
    public:
        has_constructable_lua_api(combo);

        luatool_type_fix(luabridge::LuaRef, items)
        lua_constructor(Available_0_8) explicit combo(luabridge::LuaRef items);

        lua_function(setAction, Available_0_8) auto set_action(luabridge::LuaRef callback) -> void { m_action = callback; }

        lua_getter(items, Available_0_8) [[nodiscard]] auto items() const -> luabridge::LuaRef { return m_items; }
        lua_setter(items, Available_0_8) auto set_items(luabridge::LuaRef items) -> void { m_items = items; }

        lua_getter(selectedItem, Available_0_8) [[nodiscard]] auto selected_item() const -> uint32_t { return m_selected; }
        lua_setter(selectedItem, Available_0_8) auto set_selected_item(uint32_t idx) -> void { m_selected = idx; }

        lua_getter(position, Available_0_8) [[nodiscard]] auto position() const -> math::point override { return widget::position(); }
        lua_setter(position, Available_0_8) auto set_position(const math::point &position) -> void override { widget::set_position(position); }

        lua_getter(size, Available_0_8) [[nodiscard]] auto size() const -> math::size override { return widget::size(); }
        lua_setter(size, Available_0_8) auto set_size(const math::size &size) -> void override { widget::set_size(size); }

        lua_getter(font, Available_0_8) [[nodiscard]] auto font() const -> kestrel::font::reference::lua_reference { return m_font; }
        lua_setter(font, Available_0_8) auto set_font(const kestrel::font::reference::lua_reference& font) -> void { m_font = font; }

        auto draw() -> void override;

    private:
        luabridge::LuaRef m_action { nullptr };
        luabridge::LuaRef m_items { nullptr };
        std::uint32_t m_selected { 1 };
        kestrel::font::reference::lua_reference m_font { nullptr };

        auto internal_draw() -> void;
    };
}
