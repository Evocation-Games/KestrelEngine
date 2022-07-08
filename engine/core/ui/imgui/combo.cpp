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

#include "core/ui/imgui/combo.hpp"

// MARK: - Lua

auto ui::imgui::combo::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("ImGui")
            .beginClass<combo>("Combo")
                .addConstructor<auto(*)(luabridge::LuaRef)->void, lua_reference>()
                .addProperty("position", &combo::position, &combo::set_position)
                .addProperty("size", &combo::size, &combo::set_size)
                .addProperty("items", &combo::items, &combo::set_items)
                .addProperty("selectedItem", &combo::selected_item, &combo::set_selected_item)
                .addFunction("setAction", &combo::set_action)
            .endClass()
        .endNamespace();
}

// MARK: - Constructor

ui::imgui::combo::combo(luabridge::LuaRef items)
    : widget(), m_items(items), m_action(nullptr)
{

}

// MARK: - Drawing

auto ui::imgui::combo::internal_draw() -> void
{
    if (!m_items.isTable()) {
        return;
    }

    auto count = m_items.length();
    std::string preview;

    if (m_items[m_selected].isString()) {
        preview = m_items[m_selected].tostring();
    }

    if (has_position()) {
        ImGui::SetCursorPos({ static_cast<float>(position().x), static_cast<float>(position().y) });
    }

    if (has_size()) {
        ImGui::SetNextItemWidth(static_cast<float>(this->size().width));
    }

    if (ImGui::BeginCombo(identifier_string(), preview.c_str(), ImGuiComboFlags_None)) {
        for (auto i = 1; i <= count; ++i) {
            bool is_selected = (m_selected == i);

            if (m_items[i].isString()) {
                if (ImGui::Selectable(m_items[i].tostring().c_str(), is_selected)) {
                    auto previous_selected = m_selected;
                    m_selected = i;
                    if (previous_selected != i) {
                        if (m_action.state() && m_action.isFunction()) {
                            m_action();
                        }
                    }
                }
            }
            else {
                ImGui::TextDisabled("<undefined>");
            }

            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }

        ImGui::EndCombo();
    }
}

auto ui::imgui::combo::draw() -> void
{
    if (m_font.get()) {
        m_font->push([&] { internal_draw(); });
    }
    else {
        internal_draw();
    }
}