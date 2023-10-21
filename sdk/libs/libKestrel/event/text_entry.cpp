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

#include <libKestrel/event/text_entry.hpp>

// MARK: - Constructor

kestrel::text_entry_event::text_entry_event()
{
    load_default_keymap();

    m_on_enter = [] (const std::string& text) {};
    m_on_escape = [] {};
}

// MARK: - Event Processing

auto kestrel::text_entry_event::receive(const event& e) -> void
{
    if (e.is_key_event()) {
        switch (e.key()) {
            case ui::hid::enter:
            case ui::hid::kp_enter: {
                if (e.has(ui::event::type::key_up)) {
                    m_on_enter(m_value);
                }
                return;
            }
            case ui::hid::escape: {
                if (e.has(ui::event::type::key_up)) {
                    m_on_escape();
                }
                return;
            }
            default: {
                break;
            }
        }

        // We don't want to deal with general key events, only pressed keys.
        if (!e.has(ui::event::type::key_down)) {
            return;
        }

        switch (e.key()) {
            case ui::hid::left: {
                if (m_cursor > 0) {
                    m_cursor--;
                }
                break;
            }
            case  ui::hid::right: {
                if (m_cursor < m_value.size()) {
                    m_cursor++;
                }
                break;
            }
            case ui::hid::backspace: {
                if (m_cursor == 0) {
                    return;
                }
                m_value.erase(m_cursor - 1, 1);
                m_cursor--;
                break;
            }

            // The general handler will default to the keymap.
            default: {
                const auto& mapping = m_keymap[static_cast<int>(e.key())];
                char c[2] = { e.has(ui::event::type::has_shift_modifier) ? mapping.shifted : mapping.base, '\0' };
                if (c[0] != '\0') {
                    m_value.insert(m_cursor, c);
                    m_cursor++;
                }
                break;
            }
        }
    }
}

// MARK: - Accessors

auto kestrel::text_entry_event::set_string_value(const std::string &value) -> void
{
    m_value = value;
}

auto kestrel::text_entry_event::string_value() const -> std::string
{
    return m_value;
}

auto kestrel::text_entry_event::cursor_position() const -> int
{
    return m_cursor;
}

auto kestrel::text_entry_event::set_cursor_position(const int& position) -> void
{
    m_cursor = position;
}

// MARK: - Keymap

auto kestrel::text_entry_event::load_default_keymap() -> void
{
    for (int k = ui::hid::a; k <= ui::hid::z; ++k) {
        m_keymap[k] = { static_cast<char>((k - 'A') + 'a'), static_cast<char>(k) };
        m_keymap[m_keymap[k].base] = m_keymap[k];
    }

    m_keymap[ui::hid::num1] = { '1', '!' };
    m_keymap[ui::hid::num2] = { '2', '@' };
    m_keymap[ui::hid::num3] = { '3', '#' };
    m_keymap[ui::hid::num4] = { '4', '$' };
    m_keymap[ui::hid::num5] = { '5', '%' };
    m_keymap[ui::hid::num6] = { '6', '^' };
    m_keymap[ui::hid::num7] = { '7', '&' };
    m_keymap[ui::hid::num8] = { '8', '*' };
    m_keymap[ui::hid::num9] = { '9', '(' };
    m_keymap[ui::hid::num0] = { '0', ')' };

    for (int n = ui::hid::num0; n <= ui::hid::num9; ++n) {
        m_keymap[ui::hid::kp_0 + (n - ui::hid::num0)] = m_keymap[n];
    }

    m_keymap[ui::hid::apostrophe] = { '\'', '"' };
    m_keymap[ui::hid::semi_colon] = { ';', ':' };
    m_keymap[ui::hid::backslash] = { '\\', '|' };
    m_keymap[ui::hid::grave_accent] = { '`', '~' };
    m_keymap[ui::hid::left_bracket] = { '[', '{' };
    m_keymap[ui::hid::right_bracket] = { ']', '}' };
    m_keymap[ui::hid::comma] = { ',', '<' };
    m_keymap[ui::hid::period] = { '.', '>' };
    m_keymap[ui::hid::slash] = { '/', '?' };
    m_keymap[ui::hid::equal] = { '=', '+' };
    m_keymap[ui::hid::minus] = { '-', '_' };
    m_keymap[ui::hid::tab] = { '\t', '\t' };
    m_keymap[ui::hid::space] = { ' ', ' ' };
    m_keymap[ui::hid::apostrophe] = { '\'', '"' };
    m_keymap[ui::hid::left_bracket] = { '[', '{' };
    m_keymap[ui::hid::right_bracket] = { ']', '}' };
}

// MARK: - Callbacks

auto kestrel::text_entry_event::on_enter(std::function<auto(const std::string&)->void> callback) -> void
{
    m_on_enter = std::move(callback);
}

auto kestrel::text_entry_event::on_escape(std::function<auto()->void> callback) -> void
{
    m_on_escape = std::move(callback);
}
