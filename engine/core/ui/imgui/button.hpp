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
#include "scripting/state.hpp"
#include "util/hint.hpp"

namespace ui::imgui
{
    class button : public widget, public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<button> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        lua_api button(const std::string& label, luabridge::LuaRef action);

        [[nodiscard]] lua_api auto label() const -> std::string { return m_label; }
        lua_api auto set_label(const std::string& label) -> void { m_label = label; }

        lua_api auto set_action(luabridge::LuaRef action) -> void { m_action = action; }

        auto draw() -> void override;

    private:
        std::string m_label;
        luabridge::LuaRef m_action;
    };
}
