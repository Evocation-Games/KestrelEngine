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

#include <libUI/text/editor.hpp>

// MARK: - Construction

ui::text::editor::editor(const ui::key_map& map)
    : m_map(map)
{
    m_on_changed = [] (const std::string&) {};
    m_on_enter = [] (const std::string&) {};
    m_on_escape = [] (const std::string&) {};
}

// MARK: - Events

auto ui::text::editor::send_event(const ui::event &event) -> void
{
    if (!event.is_key_event()) {
        return;
    }

    switch (event.key()) {
        case ui::hid::key::enter:
        case ui::hid::key::kp_enter: {
            if (event.has(ui::event::type::key_up)) {
                m_on_enter(m_state.value);
            }
            return;
        }
        case ui::hid::key::escape: {
            if (event.has(ui::event::type::key_up)) {
                m_on_escape(m_state.value);
            }
            return;
        }
        default: break;
    }

    if (!event.has(ui::event::type::key_down)) {
        return;
    }

    switch (event.key()) {
        case ui::hid::key::left: {
            cursor_left();
            break;
        }
        case ui::hid::key::right: {
            cursor_right();
            break;
        }
        case ui::hid::key::backspace: {
            backspace();
            break;
        }
        default: {
            insert(m_map.string_value(event.key(), event.has(ui::event::type::has_shift_modifier)));
            break;
        }
    }
}

auto ui::text::editor::cursor_left() -> void
{
    if (m_state.cursor > 0) {
        m_state.cursor--;
    }
}

auto ui::text::editor::cursor_right() -> void
{
    if (m_state.cursor < m_state.value.size()) {
        m_state.cursor++;
    }
}

auto ui::text::editor::backspace() -> void
{
    if (m_state.cursor == 0) {
        return;
    }
    cursor_left();
    m_state.value.erase(m_state.cursor, 1);
    m_on_changed(m_state.value);
}

auto ui::text::editor::insert(const std::string &c) -> void
{
    if (c.empty()) {
        return;
    }
    m_state.value.insert(m_state.cursor, c);
    m_state.cursor += static_cast<cursor>(c.size());
    m_on_changed(m_state.value);
}

// MARK: - Accessors

auto ui::text::editor::string_value() const -> std::string
{
    return m_state.value;
}

auto ui::text::editor::set_string_value(const std::string &value) -> void
{
    m_state.value = value;
    validate_cursor_position();
}

auto ui::text::editor::cursor_position() const -> cursor
{
    return m_state.cursor;
}

auto ui::text::editor::set_cursor_position(ui::text::editor::cursor position) -> void
{
    m_state.cursor = position;
    validate_cursor_position();
}

// MARK: - Cursor

auto ui::text::editor::validate_cursor_position() -> void
{
    if (m_state.cursor >= m_state.value.size()) {
        m_state.cursor = static_cast<cursor>(m_state.value.size());
    }
    else if (m_state.cursor < 0) {
        m_state.cursor = 0;
    }
}

// MARK: - Callbacks

auto ui::text::editor::on_changed(const ui::text::editor::callback_function &callback) -> void
{
    m_on_changed = callback;
}

auto ui::text::editor::on_enter(const ui::text::editor::callback_function &callback) -> void
{
    m_on_enter = callback;
}

auto ui::text::editor::on_escape(const ui::text::editor::callback_function &callback) -> void
{
    m_on_escape = callback;
}