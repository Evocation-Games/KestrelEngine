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
#include <vector>
#include <imgui/imgui.h>
#include <libKestrel/ui/imgui/widget.hpp>
#include <libKestrel/ui/imgui/container.hpp>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>

namespace kestrel::ui::imgui
{
    class lua_api(ImGui.TableCell, Available_0_8) table_cell : public widget
    {
    public:
        has_constructable_lua_api(table_cell);

        lua_constructor(Available_0_8) table_cell();

        lua_function(text, Available_0_8) static auto text(const std::string& text) -> lua_reference;

        lua_function(addWidget, Available_0_8) auto add_widget(luabridge::LuaRef widget) -> void;

        lua_function(onSelected, Available_0_8) auto on_selected(luabridge::LuaRef callback) -> void;
        auto selected() -> void;

        lua_function(onDoubleClick, Available_0_8) auto on_double_click(luabridge::LuaRef callback) -> void;
        auto double_click() -> void;

        lua_function(addChild, Available_0_8) auto add_child(luabridge::LuaRef child) -> void;
        lua_getter(hasChildren, Available_0_8) [[nodiscard]] auto has_children() const -> bool;

        auto draw() -> void override;
        auto draw(const std::string& selected_id) -> std::string;

    private:
        luabridge::LuaRef m_on_selected { nullptr };
        luabridge::LuaRef m_on_double_click { nullptr };
        widget_container m_contents;
        std::vector<lua_reference> m_children;

    };

    class lua_api(ImGui.Table, Available_0_8) table : public widget
    {
    public:
        has_constructable_lua_api(table);

        lua_constructor(Available_0_8) explicit table(float width);

        lua_function(row, Available_0_8) [[nodiscard]] auto row(std::uint32_t idx) -> luabridge::LuaRef { return m_rows[idx]; }
        lua_function(addRow, Available_0_8) auto add_row(luabridge::LuaRef row) -> void;
        lua_getter(rowCount, Available_0_8) [[nodiscard]] auto row_count() const -> std::size_t { return m_rows.size(); }

        lua_getter(header, Available_0_8) [[nodiscard]] auto header() const -> std::string { return m_header; }
        lua_setter(header, Available_0_8) auto set_header(const std::string& text) -> void { m_header = text; }

        lua_getter(position, Available_0_8) [[nodiscard]] auto position() const -> math::point override { return widget::position(); }
        lua_setter(position, Available_0_8) auto set_position(const math::point &position) -> void override { widget::set_position(position); }

        lua_getter(size, Available_0_8) [[nodiscard]] auto size() const -> math::size override { return widget::size(); }
        lua_setter(size, Available_0_8) auto set_size(const math::size &size) -> void override { widget::set_size(size); }

        lua_getter(selectedIndex, Available_0_8) [[nodiscard]] auto selected_index() const -> std::uint32_t;
        lua_setter(selectedIndex, Available_0_8) auto set_selected_index(std::uint32_t idx) -> void;

        auto draw() -> void override;

    private:
        float m_width { 1.f };
        std::uint32_t m_selected_row { 1 };
        std::string m_selected_identifier;
        std::vector<luabridge::LuaRef> m_rows;
        std::string m_header;

    };
}
