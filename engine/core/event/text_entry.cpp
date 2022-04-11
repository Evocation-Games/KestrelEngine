// Copyright (c) 2021 Tom Hancocks
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

#include "core/event/text_entry.hpp"

// MARK: - Constructor

text_entry_event::text_entry_event()
{
    load_default_keymap();

    m_on_enter = [] (const std::string& text) {};
    m_on_escape = [] {};
}

// MARK: - Event Processing

auto text_entry_event::receive(const event& e) -> void
{
    // TODO: Handle key repeats

    // Update the internally tracked modifier states.
    switch (e.key()) {
        case hid::left_alt:
        case hid::right_alt: {
            m_alt = e.has(event::key_down);
            return;
        }
        case hid::left_control:
        case hid::right_control: {
            m_control = e.has(event::key_down);
            return;
        }
        case hid::left_shift:
        case hid::right_shift: {
            m_shift = e.has(event::key_down);
            return;
        }
        case hid::left_super:
        case hid::right_super: {
            m_super = e.has(event::key_down);
            return;
        }
        case hid::enter:
        case hid::kp_enter: {
            if (e.has(event::key_up)) {
                m_on_enter(m_value);
            }
            return;
        }
        case hid::escape: {
            if (e.has(event::key_up)) {
                m_on_escape();
            }
            return;
        }
        default: {
            break;
        }
    }

    // We don't want to deal with general key events, only pressed keys.
    if (!e.has(event::key_down)) {
        return;
    }

    switch (e.key()) {
        case hid::left: {
            if (m_cursor > 0) {
                m_cursor--;
            }
            break;
        }
        case  hid::right: {
            if (m_cursor < m_value.size()) {
                m_cursor++;
            }
            break;
        }
        case hid::backspace: {
            if (m_cursor == 0) {
                return;
            }
            m_value.erase(m_cursor - 1, 1);
            m_cursor--;
            break;
        }

        // The general handler will default to the keymap.
        default: {
            const auto& mapping = m_keymap[e.character()];
            char c[2] = { m_shift ? mapping.shifted : mapping.base, '\0' };
            if (c[0] != '\0') {
                m_value.insert(m_cursor, c);
                m_cursor++;
            }
            break;
        }
    }
}

// MARK: - Accessors

auto text_entry_event::set_string_value(const std::string &value) -> void
{
    m_value = value;
}

auto text_entry_event::string_value() const -> std::string
{
    return m_value;
}

auto text_entry_event::cursor_position() const -> int
{
    return m_cursor;
}

auto text_entry_event::set_cursor_position(const int& position) -> void
{
    m_cursor = position;
}

// MARK: - Keymap

auto text_entry_event::load_default_keymap() -> void
{
    for (int k = hid::a; k <= hid::z; ++k) {
        m_keymap[k] = { static_cast<char>((k - 'A') + 'a'), static_cast<char>(k) };
    }

    m_keymap[hid::num1] = { '1', '!' };
    m_keymap[hid::num2] = { '2', '@' };
    m_keymap[hid::num3] = { '3', '#' };
    m_keymap[hid::num4] = { '4', '$' };
    m_keymap[hid::num5] = { '5', '%' };
    m_keymap[hid::num6] = { '6', '^' };
    m_keymap[hid::num7] = { '7', '&' };
    m_keymap[hid::num8] = { '8', '*' };
    m_keymap[hid::num9] = { '9', '(' };
    m_keymap[hid::num0] = { '0', ')' };

    for (int n = hid::num0; n <= hid::num9; ++n) {
        m_keymap[hid::kp_0 + (n - hid::num0)] = m_keymap[n];
    }

    m_keymap[hid::apostrophe] = { '\'', '"' };
    m_keymap[hid::semi_colon] = { ';', ':' };
    m_keymap[hid::backslash] = { '\\', '|' };
    m_keymap[hid::grave_accent] = { '`', '~' };
    m_keymap[hid::left_bracket] = { '[', '{' };
    m_keymap[hid::right_bracket] = { ']', '}' };
    m_keymap[hid::comma] = { ',', '<' };
    m_keymap[hid::period] = { '.', '>' };
    m_keymap[hid::slash] = { '/', '?' };
    m_keymap[hid::equal] = { '=', '+' };
    m_keymap[hid::minus] = { '-', '_' };
    m_keymap[hid::tab] = { '\t', '\t' };
    m_keymap[hid::space] = { ' ', ' ' };
}

// MARK: - Callbacks

auto text_entry_event::on_enter(std::function<auto(const std::string&)->void> callback) -> void
{
    m_on_enter = std::move(callback);
}

auto text_entry_event::on_escape(std::function<auto()->void> callback) -> void
{
    m_on_escape = std::move(callback);
}
