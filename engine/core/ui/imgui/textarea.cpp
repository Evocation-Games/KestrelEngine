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


#include "core/ui/imgui/textarea.hpp"

// MARK: - Lua

auto ui::imgui::textarea::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("ImGui")
        .beginClass<textarea>("TextArea")
            .addConstructor<auto(*)(std::size_t, const std::string&)->void, lua_reference>()
                .addProperty("position", &textarea::position, &textarea::set_position)
                .addProperty("size", &textarea::size, &textarea::set_size)
                .addProperty("text", &textarea::text, &textarea::set_text)
                .addProperty("font", &textarea::font, &textarea::set_font)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

ui::imgui::textarea::textarea(std::size_t buffer_size, const std::string &text)
    : m_buffer_size(std::min(std::size_t(64), buffer_size))
{
    m_buffer = new char[buffer_size + 1];
    memset(m_buffer, 0, buffer_size + 1);
    memcpy(m_buffer, text.c_str(), std::min(text.size(), buffer_size));
}

// MARK: - Destruction

ui::imgui::textarea::~textarea()
{
    delete m_buffer;
}

// MARK: - Drawing

auto ui::imgui::textarea::internal_draw() -> void
{
    auto size = ImGui::GetContentRegionAvail();

    if (has_position()) {
        ImGui::SetCursorPos({ static_cast<float>(position().x), static_cast<float>(position().y) });
    }

    if (has_size()) {
        ImGui::SetNextItemWidth(static_cast<float>(this->size().width));
    }

    ImGui::InputTextMultiline(identifier_string(), m_buffer, m_buffer_size, size);
}

auto ui::imgui::textarea::draw() -> void
{
    if (m_font.get()) {
        m_font->push([&] { internal_draw(); });
    }
    else {
        internal_draw();
    }
}

// MARK: - Accessors

auto ui::imgui::textarea::text() const -> std::string
{
    return { m_buffer, m_buffer_size };
}

auto ui::imgui::textarea::set_text(const std::string &text) -> void
{
    delete m_buffer;

    m_buffer_size = std::max(text.size(), m_buffer_size);
    m_buffer = new char[m_buffer_size + 1];
    memset(m_buffer, 0, m_buffer_size + 1);
    memcpy(m_buffer, text.c_str(), std::min(text.size(), m_buffer_size));
}

auto ui::imgui::textarea::font() const -> ui::font::reference::lua_reference
{
    return m_font;
}

auto ui::imgui::textarea::set_font(const ui::font::reference::lua_reference &font) -> void
{
    font->load_for_imgui();
    m_font = font;
}