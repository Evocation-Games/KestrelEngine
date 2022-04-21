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
#include "math/size.hpp"
#include "math/point.hpp"
#include "core/ui/imgui/widget.hpp"
#include "core/ui/imgui/container.hpp"
#include "scripting/state.hpp"

namespace ui::imgui
{
    class window : public widget, public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<window> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        lua_api window(const std::string& title, const math::size& size);

        lua_api auto show() -> void;
        lua_api auto hide() -> void;
        lua_api auto close() -> void;
        lua_api auto center() -> void;

        [[nodiscard]] inline auto is_closed() const -> bool { return m_closed; }

        [[nodiscard]] lua_api auto title() const -> std::string;
        [[nodiscard]] auto identified_title() const -> std::string { return m_title + identifier_string(); }
        lua_api auto set_title(const std::string& title) -> void;

        [[nodiscard]] lua_api auto size() const -> math::size;
        lua_api auto set_size(const math::size& size) -> void;

        auto draw() -> void override;

        lua_api auto add_widget(luabridge::LuaRef widget) -> void;

    private:
        std::string m_title;
        math::size m_size;
        math::point m_position;
        bool m_dirty_size { true };
        bool m_dirty_position { false };
        bool m_shown { true };
        bool m_closed { false };
        widget_container m_contents;
    };
}
