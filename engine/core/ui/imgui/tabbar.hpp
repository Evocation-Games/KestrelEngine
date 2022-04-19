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
#include "core/ui/imgui/widget.hpp"
#include "core/ui/imgui/container.hpp"
#include "scripting/state.hpp"
#include "util/hint.hpp"

namespace ui::imgui
{
    class tabbar : public widget, public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<tabbar> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

        class item : public widget, public scripting::lua::object
        {
        public:
            typedef luabridge::RefCountedPtr<tabbar::item> lua_reference;
            static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

        public:
            lua_api explicit item(const std::string& title);

            [[nodiscard]] lua_api auto title() const -> std::string { return m_title; }
            lua_api auto set_title(const std::string& title) -> void { m_title = title; }

            auto add_widget(luabridge::LuaRef widget) -> void;

            auto draw() -> void override;

        private:
            std::string m_title;
            widget_container m_contents;
        };

    public:
        lua_api explicit tabbar(luabridge::LuaRef tabs);

        [[nodiscard]] lua_api auto tabs() const -> luabridge::LuaRef { return m_tabs; }
        lua_api auto set_tabs(luabridge::LuaRef tabs) -> void { m_tabs = tabs; }

        [[nodiscard]] lua_api auto selected_tab() const -> uint32_t { return m_selected; }
        lua_api auto set_selected_tab(uint32_t idx) -> void { m_selected = idx; }

        lua_api auto set_action(luabridge::LuaRef callback) -> void { m_action = callback; }

        auto draw() -> void override;

    private:
        luabridge::LuaRef m_action;
        luabridge::LuaRef m_tabs;
        uint32_t m_selected { 1 };

    };
}
