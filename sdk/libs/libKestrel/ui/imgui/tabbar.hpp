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
#include <libKestrel/ui/imgui/widget.hpp>
#include <libKestrel/ui/imgui/container.hpp>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>

namespace kestrel::ui::imgui
{
    class lua_api(ImGui.TabItem, Available_0_8) tabbar_item : public widget
    {
    public:
        has_constructable_lua_api(tabbar_item);

        lua_constructor(Available_0_8) explicit tabbar_item(const std::string& title);

        [[nodiscard]] auto title_ref() const -> const std::string& { return m_title; }
        lua_getter(title, Available_0_8) [[nodiscard]] auto title() const -> std::string { return m_title; }
        lua_setter(title, Available_0_8) auto set_title(const std::string& title) -> void { m_title = title; }

        lua_function(addWidget, Available_0_8) auto add_widget(luabridge::LuaRef widget) -> void;

        auto draw() -> void override;

    private:
        std::string m_title;
        widget_container m_contents;
    };

    class lua_api(ImGui.TabBar, Available_0_8) tabbar : public widget
    {
    public:
        has_constructable_lua_api(tabbar);

        luatool_type_fix(luabridge::LuaRef, tabs)
        lua_constructor(Available_0_8) explicit tabbar(luabridge::LuaRef tabs);

        lua_function(setAction, Available_0_8) auto set_action(luabridge::LuaRef callback) -> void { m_action = callback; }

        lua_getter(tabs, Available_0_8) [[nodiscard]] auto tabs() const -> luabridge::LuaRef { return m_tabs; }
        lua_setter(tabs, Available_0_8) auto set_tabs(luabridge::LuaRef tabs) -> void { m_tabs = tabs; }

        lua_getter(selectedTab, Available_0_8) [[nodiscard]] auto selected_tab() const -> std::uint32_t { return m_selected; }
        lua_setter(selectedTab, Available_0_8) auto set_selected_tab(std::uint32_t idx) -> void { m_selected = idx; }

        lua_getter(position, Available_0_8) [[nodiscard]] auto position() const -> math::point override { return widget::position(); }
        lua_setter(position, Available_0_8) auto set_position(const math::point &position) -> void override { widget::set_position(position); }

        lua_getter(size, Available_0_8) [[nodiscard]] auto size() const -> math::size override { return widget::size(); }
        lua_setter(size, Available_0_8) auto set_size(const math::size &size) -> void override { widget::set_size(size); }

        auto draw() -> void override;

    private:
        luabridge::LuaRef m_action;
        luabridge::LuaRef m_tabs;
        std::uint32_t m_selected { 1 };

    };
}
