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
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/ui/imgui/widget.hpp>
#include <libKestrel/font/manager.hpp>

namespace kestrel::ui::imgui
{
    class lua_api(ImGui.Text, Available_0_8) label : public widget
    {
    public:
        has_constructable_lua_api(label);
        
        lua_constructor(Available_0_8) explicit label(const std::string& text);

        lua_getter(text, Available_0_8) [[nodiscard]] auto text() const -> std::string { return m_text; }
        lua_setter(text, Available_0_8) auto set_text(const std::string& text) -> void { m_text = text; }

        lua_getter(largeFont, Available_0_8) [[nodiscard]] auto is_large_font() const -> bool { return m_font_size > 15; }
        lua_setter(largeFont, Available_0_8) auto set_large_font(bool f) -> void { m_font_size = f ? 20 : 15; }

        lua_getter(wrapping, Available_0_8) [[nodiscard]] auto wrapping() const -> bool { return m_wraps; }
        lua_setter(wrapping, Available_0_8) auto set_wrapping(bool f) -> void { m_wraps = f; }

        lua_getter(textSize, Available_0_8) [[nodiscard]] auto text_size() const -> std::uint32_t { return m_font_size; }
        lua_setter(textSize, Available_0_8) auto set_text_size(std::uint32_t size) -> void { m_font_size = size; }

        lua_getter(position, Available_0_8) [[nodiscard]] auto position() const -> math::point override { return widget::position(); }
        lua_setter(position, Available_0_8) auto set_position(const math::point &position) -> void override { widget::set_position(position); }

        lua_getter(size, Available_0_8) [[nodiscard]] auto size() const -> math::size override { return widget::size(); }
        lua_setter(size, Available_0_8) auto set_size(const math::size &size) -> void override { widget::set_size(size); }

        lua_getter(font, Available_0_8) [[nodiscard]] auto font() const -> font::reference::lua_reference { return m_font; }
        lua_setter(font, Available_0_8) auto set_font(const font::reference::lua_reference& font) -> void { m_font = font; }

        auto draw() -> void override;

    private:
        std::uint32_t m_font_size { 15 };
        bool m_wraps { false };
        std::string m_text;
        font::reference::lua_reference m_font { nullptr };

        auto internal_draw() -> void;
    };
}
