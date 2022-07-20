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

#include "core/ui/imgui/label.hpp"
#include "core/ui/font/manager.hpp"

// MARK: - Lua

auto ui::imgui::label::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("ImGui")
            .beginClass<label>("Text")
                .addConstructor<auto(*)(const std::string&)->void, lua_reference>()
                .addProperty("position", &label::position, &label::set_position)
                .addProperty("size", &label::size, &label::set_size)
                .addProperty("text", &label::text, &label::set_text)
                .addProperty("font", &label::font, &label::set_font)
                .addProperty("wrapping", &label::wrapping, &label::set_wrapping)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

ui::imgui::label::label(const std::string &text)
    : m_text(text)
{
}

// MARK: - Drawing

auto ui::imgui::label::internal_draw() -> void
{
    if (has_position()) {
        ImGui::SetCursorPos({ static_cast<float>(position().x), static_cast<float>(position().y) });
    }

    if (has_size()) {
        ImGui::SetNextItemWidth(static_cast<float>(this->size().width));
    }

    if (m_wraps) {
        ImGui::TextWrapped("%s", m_text.c_str());
    }
    else {
        ImGui::Text("%s", m_text.c_str());
    }
}

auto ui::imgui::label::draw() -> void
{
    if (m_font.get()) {
        m_font->push([&] { internal_draw(); });
    }
    else {
        internal_draw();
    }
}