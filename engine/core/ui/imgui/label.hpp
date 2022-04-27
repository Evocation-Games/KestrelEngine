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
#include "core/ui/imgui/widget.hpp"
#include "scripting/state.hpp"
#include "util/hint.hpp"

namespace ui::imgui
{
    class label : public widget, public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<label> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        static auto configure_fonts() -> void;

        lua_api explicit label(const std::string& text);

        [[nodiscard]] lua_api auto text() const -> std::string { return m_text; }
        lua_api auto set_text(const std::string& text) -> void { m_text = text; }

        [[nodiscard]] lua_api auto is_large_font() const -> bool { return m_font_size > 13; }
        lua_api auto set_large_font(bool f) -> void { m_font_size = f ? 18 : 13; }

        [[nodiscard]] lua_api auto wrapping() const -> bool { return m_wraps; }
        lua_api auto set_wrapping(bool f) -> void { m_wraps = f; }

        [[nodiscard]] lua_api auto text_size() const -> uint32_t { return m_font_size; }
        lua_api auto set_text_size(uint32_t size) -> void { m_font_size = size; }

        [[nodiscard]] lua_api auto position() const -> math::point override { return widget::position(); }
        lua_api auto set_position(const math::point &position) -> void override { widget::set_position(position); }

        [[nodiscard]] lua_api auto size() const -> math::size override { return widget::size(); }
        lua_api auto set_size(const math::size &size) -> void override { widget::set_size(size); }

        auto draw() -> void override;

    private:
        uint32_t m_font_size { 13 };
        bool m_wraps { false };
        std::string m_text;

    };
}
