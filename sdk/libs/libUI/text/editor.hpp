// Copyright (c) 2023 Tom Hancocks
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
#include <functional>
#include <libUI/event/event.hpp>
#include <libUI/event/keymap.hpp>

namespace ui::text
{
    struct editor
    {
    public:
        typedef std::function<auto(const std::string&)->void> callback_function;
        typedef std::int32_t cursor;

        explicit editor(const key_map& map = key_map::default_map());

        auto send_event(const event& event) -> void;

        [[nodiscard]] auto string_value() const -> std::string;
        auto set_string_value(const std::string& value) -> void;

        [[nodiscard]] auto cursor_position() const -> cursor;
        auto set_cursor_position(cursor position) -> void;

        auto on_enter(const callback_function& callback) -> void;
        auto on_escape(const callback_function& callback) -> void;
        auto on_changed(const callback_function& callback) -> void;

        auto validate_cursor_position() -> void;
        auto cursor_left() -> void;
        auto cursor_right() -> void;
        auto backspace() -> void;
        auto insert(const std::string& c) -> void;

    private:
        callback_function m_on_enter;
        callback_function m_on_escape;
        callback_function m_on_changed;
        key_map m_map;
        struct {
            std::string value;
            bool alt { false };
            bool shift { false };
            bool control { false };
            bool super { false };
            cursor cursor { 0 };
        } m_state;
    };
}