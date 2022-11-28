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
#include <libKestrel/math/size.hpp>
#include <libKestrel/math/point.hpp>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/ui/imgui/widget.hpp>
#include <libKestrel/ui/imgui/window.hpp>
#include <libKestrel/ui/imgui/container.hpp>

namespace kestrel::ui::imgui
{
    class lua_api(ImGui.Window, Available_0_8) window : public widget
    {
    public:
        has_constructable_lua_api(window);

        window(const std::string& title, const math::size& size);
        ~window() = default;

        lua_function(create, Available_0_8) static auto create(const std::string& title, const math::size& size) -> lua_reference;

        lua_function(show, Available_0_8) auto show() -> void;
        lua_function(hide, Available_0_8) auto hide() -> void;
        lua_function(close, Available_0_8) auto close() -> void;
        lua_function(center, Available_0_8) auto center() -> void;
        lua_function(destroy, Available_0_8) auto destroy() -> void;

        lua_getter(isClosed, Available_0_8) [[nodiscard]] inline auto is_closed() const -> bool { return m_closed; }
        lua_getter(isHidden, Available_0_8) [[nodiscard]] inline auto is_hidden() const -> bool { return !m_shown; }
        lua_getter(hasClosedButton, Available_0_8) [[nodiscard]] inline auto has_close_button() const -> bool { return m_has_close_button; }
        lua_setter(hasClosedButton, Available_0_8) auto set_has_close_button(bool f) -> void { m_has_close_button = f; }

        lua_getter(resizable, Available_0_8) [[nodiscard]] inline auto is_resizable() const -> bool { return m_resizable; }
        lua_setter(resizable, Available_0_8) auto set_resizable(bool f) -> void { m_resizable = f; }

        lua_getter(title, Available_0_8) [[nodiscard]] auto title() const -> std::string;
        [[nodiscard]] auto identified_title() const -> std::string { return m_title + identifier_string(); }
        lua_setter(title, Available_0_8) auto set_title(const std::string& title) -> void;

        lua_getter(size, Available_0_8) [[nodiscard]] auto size() const -> math::size override;
        lua_setter(size, Available_0_8) auto set_size(const math::size& size) -> void override;

        auto draw() -> void override;

        lua_function(addWidget, Available_0_8) auto add_widget(luabridge::LuaRef widget) -> void;

    private:
        std::string m_title;
        math::size m_size;
        math::point m_position;
        bool m_dirty_size { true };
        bool m_dirty_position { false };
        bool m_shown { true };
        bool m_closed { false };
        bool m_has_close_button { true };
        bool m_resizable { true };
        widget_container m_contents;
    };
}
