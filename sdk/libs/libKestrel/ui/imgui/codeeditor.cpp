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


#include <libKestrel/ui/imgui/codeeditor.hpp>

// MARK: - Construction

kestrel::ui::imgui::code_editor::code_editor(const std::string &text)
{
    m_code_editor = TextEditor();
    m_code_editor.SetText(text);
    m_code_editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
}

// MARK: - Drawing

auto kestrel::ui::imgui::code_editor::internal_draw() -> void
{
    auto size = ImGui::GetContentRegionAvail();

    if (has_position()) {
        ImGui::SetCursorPos({ position().x(), position().y() });
    }

    if (has_size()) {
        ImGui::SetNextItemWidth(this->size().width());
    }

    m_code_editor.Render(identifier_string(), size, false);
}

auto kestrel::ui::imgui::code_editor::draw() -> void
{
    if (m_font.get()) {
        m_font->push([&] { internal_draw(); });
    }
    else {
        internal_draw();
    }
}

// MARK: - Accessors

auto kestrel::ui::imgui::code_editor::text() const -> std::string
{
    return m_code_editor.GetText();
}

auto kestrel::ui::imgui::code_editor::set_text(const std::string &text) -> void
{
    m_code_editor.SetText(text);
}

auto kestrel::ui::imgui::code_editor::font() const -> kestrel::font::reference::lua_reference
{
    return m_font;
}

auto kestrel::ui::imgui::code_editor::set_font(const kestrel::font::reference::lua_reference &font) -> void
{
    font->load_for_imgui();
    m_font = font;
}