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
#include <ImGuiColorTextEdit/TextEditor.h>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/ui/imgui/widget.hpp>
#include <libKestrel/font/font.hpp>

namespace kestrel::ui::imgui
{
    class lua_api(ImGui.CodeEditor, Available_0_8) code_editor : public widget
    {
    public:
        has_constructable_lua_api(code_editor);

        lua_constructor(Available_0_8) explicit code_editor(const std::string& text);

        auto draw() -> void override;

        lua_getter(position, Available_0_8) [[nodiscard]] auto position() const -> math::point override { return widget::position(); }
        lua_setter(position, Available_0_8) auto set_position(const math::point &position) -> void override { widget::set_position(position); }

        lua_getter(size, Available_0_8) [[nodiscard]] auto size() const -> math::size override { return widget::size(); }
        lua_setter(size, Available_0_8) auto set_size(const math::size &size) -> void override { widget::set_size(size); }

        lua_getter(text, Available_0_8) [[nodiscard]] auto text() const -> std::string;
        lua_setter(text, Available_0_8) auto set_text(const std::string& text) -> void;

        lua_getter(font, Available_0_8) [[nodiscard]] auto font() const -> kestrel::font::reference::lua_reference;
        lua_setter(font, Available_0_8) auto set_font(const kestrel::font::reference::lua_reference& font) -> void;

    private:
        TextEditor m_code_editor;
        kestrel::font::reference::lua_reference m_font { nullptr };

        auto internal_draw() -> void;
    };
}
