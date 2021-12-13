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

event::control::text_entry::text_entry()
{
    load_default_keymap();

    m_on_enter = [] (const std::string& text) {};
    m_on_escape = [] {};
}

// MARK: - Event Processing

auto event::control::text_entry::receive(const event::key &key) -> void
{
    // TODO: Handle key repeats

    // Update the internally tracked modifier states.
    switch (key.code()) {
        case key::code::left_alt:
        case key::code::right_alt: {
            m_alt = key.is_pressed();
            return;
        }
        case key::code::left_control:
        case key::code::right_control: {
            m_control = key.is_pressed();
            return;
        }
        case key::code::left_shift:
        case key::code::right_shift: {
            m_shift = key.is_pressed();
            return;
        }
        case key::code::left_super:
        case key::code::right_super: {
            m_super = key.is_pressed();
            return;
        }
        case key::code::enter:
        case key::code::kp_enter: {
            if (key.is_released()) {
                m_on_enter(m_value);
            }
            return;
        }
        case key::code::escape: {
            if (key.is_released()) {
                m_on_escape();
            }
            return;
        }
        default: {
            break;
        }
    }

    // We don't want to deal with general key events, only pressed keys.
    if (!key.is_pressed()) {
        return;
    }

    switch (key.code()) {
        case key::code::left: {
            if (m_cursor > 0) {
                m_cursor--;
            }
            break;
        }
        case key::code::right: {
            if (m_cursor < m_value.size()) {
                m_cursor++;
            }
            break;
        }
        case key::code::backspace: {
            if (m_cursor == 0) {
                return;
            }
            m_value.erase(m_cursor - 1, 1);
            m_cursor--;
            break;
        }

        // The general handler will default to the keymap.
        default: {
            const auto& mapping = m_keymap[key.code()];
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

auto event::control::text_entry::set_string_value(const std::string &value) -> void
{
    m_value = value;
}

auto event::control::text_entry::string_value() const -> std::string
{
    return m_value;
}

auto event::control::text_entry::cursor_position() const -> int
{
    return m_cursor;
}

auto event::control::text_entry::set_cursor_position(const int& position) -> void
{
    m_cursor = position;
}

// MARK: - Keymap

auto event::control::text_entry::load_default_keymap() -> void
{
    for (int k = key::code::a; k <= key::code::z; ++k) {
        m_keymap[k] = { static_cast<char>((k - 'A') + 'a'), static_cast<char>(k) };
    }

    m_keymap[key::code::num1] = { '1', '!' };
    m_keymap[key::code::num2] = { '2', '@' };
    m_keymap[key::code::num3] = { '3', '#' };
    m_keymap[key::code::num4] = { '4', '$' };
    m_keymap[key::code::num5] = { '5', '%' };
    m_keymap[key::code::num6] = { '6', '^' };
    m_keymap[key::code::num7] = { '7', '&' };
    m_keymap[key::code::num8] = { '8', '*' };
    m_keymap[key::code::num9] = { '9', '(' };
    m_keymap[key::code::num0] = { '0', ')' };

    for (int n = key::code::num0; n <= key::code::num9; ++n) {
        m_keymap[key::code::kp_0 + (n - key::code::num0)] = m_keymap[n];
    }

    m_keymap[key::code::apostrophe] = { '\'', '"' };
    m_keymap[key::code::semi_colon] = { ';', ':' };
    m_keymap[key::code::backslash] = { '\\', '|' };
    m_keymap[key::code::grave_accent] = { '`', '~' };
    m_keymap[key::code::left_bracket] = { '[', '{' };
    m_keymap[key::code::right_bracket] = { ']', '}' };
    m_keymap[key::code::comma] = { ',', '<' };
    m_keymap[key::code::period] = { '.', '>' };
    m_keymap[key::code::slash] = { '/', '?' };
    m_keymap[key::code::equal] = { '=', '+' };
    m_keymap[key::code::minus] = { '-', '_' };
    m_keymap[key::code::tab] = { '\t', '\t' };
    m_keymap[key::code::space] = { ' ', ' ' };
}

// MARK: - Callbacks

auto event::control::text_entry::on_enter(std::function<auto(const std::string&)->void> callback) -> void
{
    m_on_enter = std::move(callback);
}

auto event::control::text_entry::on_escape(std::function<auto()->void> callback) -> void
{
    m_on_escape = std::move(callback);
}
