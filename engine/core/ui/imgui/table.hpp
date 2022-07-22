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
#include "core/ui/imgui/widget.hpp"
#include "core/ui/imgui/container.hpp"
#include "scripting/state.hpp"
#include "util/hint.hpp"

namespace ui::imgui
{
    class table : public widget, public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<table> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

        class cell : public widget, public scripting::lua::object
        {
        public:
            typedef luabridge::RefCountedPtr<table::cell> lua_reference;
            static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

        public:
            lua_api cell();

            lua_api static auto text(const std::string& text) -> lua_reference;

            lua_api auto add_widget(luabridge::LuaRef widget) -> void;

            lua_api auto on_selected(luabridge::LuaRef callback) -> void;
            auto selected() -> void;

            lua_api auto on_double_click(luabridge::LuaRef callback) -> void;
            auto double_click() -> void;

            lua_api auto add_child(luabridge::LuaRef child) -> void;
            [[nodiscard]] lua_api auto has_children() const -> bool;

            auto draw() -> void override;
            auto draw(const std::string& selected_id) -> std::string;

        private:
            luabridge::LuaRef m_on_selected { nullptr };
            luabridge::LuaRef m_on_double_click { nullptr };
            widget_container m_contents;
            std::vector<lua_reference> m_children;

        };

    public:
        lua_api explicit table(float width);

        [[nodiscard]] lua_api auto row_count() const -> std::size_t { return m_rows.size(); }
        [[nodiscard]] lua_api auto row(uint32_t idx) -> luabridge::LuaRef { return m_rows[idx]; }

        [[nodiscard]] lua_api auto header() const -> std::string { return m_header; }
        lua_api auto set_header(const std::string& text) -> void { m_header = text; }

        lua_api auto add_row(luabridge::LuaRef row) -> void;

        [[nodiscard]] auto position() const -> math::point override { return widget::position(); }
        auto set_position(const math::point &position) -> void override { widget::set_position(position); }

        [[nodiscard]] auto size() const -> math::size override { return widget::size(); }
        auto set_size(const math::size &size) -> void override { widget::set_size(size); }

        [[nodiscard]] auto selected_index() const -> std::uint32_t;
        auto set_selected_index(std::uint32_t idx) -> void;

        auto draw() -> void override;

    private:
        float m_width { 1.f };
        uint32_t m_selected_row { 1 };
        std::string m_selected_identifier;
        std::vector<luabridge::LuaRef> m_rows;
        std::string m_header;

    };
}
