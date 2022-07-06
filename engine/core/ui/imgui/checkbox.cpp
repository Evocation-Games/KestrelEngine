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

#include "core/ui/imgui/checkbox.hpp"

// MARK: - Lua

auto ui::imgui::checkbox::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("ImGui")
            .beginClass<checkbox>("Checkbox")
                .addConstructor<auto(*)(const std::string&, bool)->void, lua_reference>()
                .addProperty("position", &checkbox::position, &checkbox::set_position)
                .addProperty("size", &checkbox::size, &checkbox::set_size)
                .addProperty("label", &checkbox::label, &checkbox::set_label)
                .addProperty("value", &checkbox::value, &checkbox::set_value)
                .addFunction("onValueChanged", &checkbox::on_value_changed)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

ui::imgui::checkbox::checkbox(const std::string& label, bool value)
    : widget(), m_label(label), m_value(value)
{
}

// MARK: - Accessors

auto ui::imgui::checkbox::on_value_changed(luabridge::LuaRef callback) -> void
{
    m_value_changed = callback;
}

// MARK: - Drawing

auto ui::imgui::checkbox::draw() -> void
{
    if (has_position()) {
        ImGui::SetCursorPos({ static_cast<float>(position().x), static_cast<float>(position().y) });
    }

    if (has_size()) {
        ImGui::SetNextItemWidth(static_cast<float>(this->size().width));
    }

    auto previous = m_value;
    ImGui::Checkbox(identified_label().c_str(), &m_value);

    if (m_value != previous && m_value_changed.state() && m_value_changed.isFunction()) {
        m_value_changed();
    }
}