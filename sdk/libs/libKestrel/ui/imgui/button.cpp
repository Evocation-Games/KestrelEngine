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

#include <libKestrel/ui/imgui/button.hpp>
#include <libKestrel/font/manager.hpp>

// MARK: - Construction

kestrel::ui::imgui::button::button(const std::string& label)
    : m_label(label)
{
}

kestrel::ui::imgui::button::button(const std::string& label, luabridge::LuaRef action)
    : m_label(label), m_action(action)
{
}

// MARK: - Drawing

auto kestrel::ui::imgui::button::internal_draw() -> void
{
    if (has_position()) {
        ImGui::SetCursorPos({ position().x(), position().y() });
    }

    if (has_size()) {
        ImGui::SetNextItemWidth(this->size().width());
    }

    if (ImGui::Button(m_label.c_str())) {
        if (m_action.index() == 2 && std::get<luabridge::LuaRef>(m_action).isFunction()) {
            std::get<luabridge::LuaRef>(m_action)();
        }
        else if (m_action.index() == 1) {
            std::get<ui::action>(m_action).execute();
        }
    }
}

auto kestrel::ui::imgui::button::draw() -> void
{
    if (m_font.get()) {
        m_font->push([&] { internal_draw(); });
    }
    else {
        internal_draw();
    }
}
