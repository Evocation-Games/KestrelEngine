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

// MARK: - Event Processing

auto event::control::text_entry::receive(const event::key &key) -> void
{
    // TODO: Handle key repeats

    if (!key.is_pressed()) {
        return;
    }

    switch (key.code()) {
        case key::code::left_alt:
        case key::code::left_control:
        case key::code::left_shift:
        case key::code::left_super:
        case key::code::right_alt:
        case key::code::right_control:
        case key::code::right_shift:
        case key::code::right_super:
        case key::code::escape: {
            return;
        }
        case key::code::backspace: {
            if (m_cursor == 0) {
                return;
            }
            m_value.erase(m_cursor - 1, 1);
            m_cursor--;
            break;
        }
        case key::code::kp_1:
        case key::code::num1: {
            char c = static_cast<char>(key.has_shift() ? '!' : '1');
            char insert[2] = { c, '\0' };
            m_value.insert(m_cursor, insert);
            m_cursor++;
            break;
        }
        case key::code::kp_2:
        case key::code::num2: {
            char c = static_cast<char>(key.has_shift() ? '@' : '2');
            char insert[2] = { c, '\0' };
            m_value.insert(m_cursor, insert);
            m_cursor++;
            break;
        }
        case key::code::kp_3:
        case key::code::num3: {
            char c = static_cast<char>(key.has_shift() ? '#' : '3');
            char insert[2] = { c, '\0' };
            m_value.insert(m_cursor, insert);
            m_cursor++;
            break;
        }
        case key::code::kp_4:
        case key::code::num4: {
            char c = static_cast<char>(key.has_shift() ? '$' : '4');
            char insert[2] = { c, '\0' };
            m_value.insert(m_cursor, insert);
            m_cursor++;
            break;
        }
        case key::code::kp_5:
        case key::code::num5: {
            char c = static_cast<char>(key.has_shift() ? '%' : '5');
            char insert[2] = { c, '\0' };
            m_value.insert(m_cursor, insert);
            m_cursor++;
            break;
        }
        case key::code::kp_6:
        case key::code::num6: {
            char c = static_cast<char>(key.has_shift() ? '^' : '6');
            char insert[2] = { c, '\0' };
            m_value.insert(m_cursor, insert);
            m_cursor++;
            break;
        }
        case key::code::kp_7:
        case key::code::num7: {
            char c = static_cast<char>(key.has_shift() ? '&' : '7');
            char insert[2] = { c, '\0' };
            m_value.insert(m_cursor, insert);
            m_cursor++;
            break;
        }
        case key::code::kp_8:
        case key::code::num8: {
            char c = static_cast<char>(key.has_shift() ? '*' : '8');
            char insert[2] = { c, '\0' };
            m_value.insert(m_cursor, insert);
            m_cursor++;
            break;
        }
        case key::code::kp_9:
        case key::code::num9: {
            char c = static_cast<char>(key.has_shift() ? '!' : '9');
            char insert[2] = { c, '\0' };
            m_value.insert(m_cursor, insert);
            m_cursor++;
            break;
        }
        case key::code::kp_0:
        case key::code::num0: {
            char c = static_cast<char>(key.has_shift() ? ')' : '0');
            char insert[2] = { c, '\0' };
            m_value.insert(m_cursor, insert);
            m_cursor++;
            break;
        }
        default: {
            char c = static_cast<char>(key.code());
            if (c >= 'A' && c <= 'Z') {
                c = static_cast<char>(key.has_shift() ? : (c - 'A') + 'a');
            }
            char insert[2] = { c, '\0' };
            m_value.insert(m_cursor, insert);
            m_cursor++;
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