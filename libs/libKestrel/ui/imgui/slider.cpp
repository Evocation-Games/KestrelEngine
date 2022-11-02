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

#include <libKestrel/ui/imgui/slider.hpp>

// MARK: - Construction

kestrel::ui::imgui::slider::slider(std::int32_t value, std::int32_t min, std::int32_t max)
    : widget(), m_value(value), m_min(min), m_max(max), m_changed(nullptr)
{
}

// MARK: - Drawing

auto kestrel::ui::imgui::slider::draw() -> void
{
    if (has_position()) {
        ImGui::SetCursorPos({ static_cast<float>(position().x), static_cast<float>(position().y) });
    }

    if (has_size()) {
        ImGui::SetNextItemWidth(static_cast<float>(this->size().width));
    }

    if (ImGui::SliderInt(identifier_string(), &m_value, m_min, m_max, "%d", ImGuiSliderFlags_None)) {
        if (!m_changed.isNil() && m_changed.isFunction()) {
            m_changed();
        }
    }

}

// MARK: - Accessors

auto kestrel::ui::imgui::slider::set_value_changed(luabridge::LuaRef callback) -> void
{
    m_changed = callback;
}