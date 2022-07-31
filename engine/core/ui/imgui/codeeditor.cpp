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


#include "core/ui/imgui/codeeditor.hpp"

// MARK: - Lua

auto ui::imgui::code_editor::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("ImGui")
            .beginClass<code_editor>("CodeEditor")
                .addConstructor<auto(*)(const std::string&)->void, lua_reference>()
                .addProperty("position", &code_editor::position, &code_editor::set_position)
                .addProperty("size", &code_editor::size, &code_editor::set_size)
                .addProperty("text", &code_editor::text, &code_editor::set_text)
                .addProperty("font", &code_editor::font, &code_editor::set_font)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

ui::imgui::code_editor::code_editor(const std::string &text)
{
    m_code_editor = TextEditor();
    m_code_editor.SetText(text);
    m_code_editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
}

// MARK: - Destruction

ui::imgui::code_editor::~code_editor()
{
}

// MARK: - Drawing

auto ui::imgui::code_editor::internal_draw() -> void
{
    auto size = ImGui::GetContentRegionAvail();

    if (has_position()) {
        ImGui::SetCursorPos({ static_cast<float>(position().x), static_cast<float>(position().y) });
    }

    if (has_size()) {
        ImGui::SetNextItemWidth(static_cast<float>(this->size().width));
    }

    m_code_editor.Render(identifier_string(), size, false);
}

auto ui::imgui::code_editor::draw() -> void
{
    if (m_font.get()) {
        m_font->push([&] { internal_draw(); });
    }
    else {
        internal_draw();
    }
}

// MARK: - Accessors

auto ui::imgui::code_editor::text() const -> std::string
{
    return m_code_editor.GetText();
}

auto ui::imgui::code_editor::set_text(const std::string &text) -> void
{
    m_code_editor.SetText(text);
}

auto ui::imgui::code_editor::font() const -> ui::font::reference::lua_reference
{
    return m_font;
}

auto ui::imgui::code_editor::set_font(const ui::font::reference::lua_reference &font) -> void
{
    font->load_for_imgui();
    m_font = font;
}