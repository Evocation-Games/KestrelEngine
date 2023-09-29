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

#include <libUI/event/keymap.hpp>

// MARK: - Access

auto ui::key_map::key(enum ui::hid::key k) const -> record
{
    return m_keys.at(k);
}

auto ui::key_map::string_value(enum ui::hid::key k, bool shift) const -> std::string
{
    // TODO: Perform this operation in a better way...
    const auto& key = this->key(k);
    const auto value = shift ? key.shifted : key.base;

    if (value == '\0') {
        return "";
    }
    else {
        return { static_cast<char>(value), 1 };
    }
}

// MARK: - Default Key Map

auto ui::key_map::default_map() -> key_map
{
    key_map map;

    for (auto k = ui::hid::a; k <= ui::hid::z; ++k) {
        map.m_keys[k] = { static_cast<character_value>((k - 'A') + 'a'), static_cast<character_value>(k) };
        map.m_keys[static_cast<enum hid::key>(map.m_keys[k].base)] = map.m_keys[k];
    }

    map.m_keys[ui::hid::num1] = { '1', '!' };
    map.m_keys[ui::hid::num2] = { '2', '@' };
    map.m_keys[ui::hid::num3] = { '3', '#' };
    map.m_keys[ui::hid::num4] = { '4', '$' };
    map.m_keys[ui::hid::num5] = { '5', '%' };
    map.m_keys[ui::hid::num6] = { '6', '^' };
    map.m_keys[ui::hid::num7] = { '7', '&' };
    map.m_keys[ui::hid::num8] = { '8', '*' };
    map.m_keys[ui::hid::num9] = { '9', '(' };
    map.m_keys[ui::hid::num0] = { '0', ')' };

    for (auto n = ui::hid::num0; n <= ui::hid::num9; ++n) {
        auto kp = static_cast<enum ui::hid::key>(ui::hid::key::kp_0 + (n - ui::hid::key::num0));
        map.m_keys[kp] = { map.m_keys[n].base, map.m_keys[n].base };
    }

    map.m_keys[ui::hid::apostrophe] = { '\'', '"' };
    map.m_keys[ui::hid::semi_colon] = { ';', ':' };
    map.m_keys[ui::hid::backslash] = { '\\', '|' };
    map.m_keys[ui::hid::grave_accent] = { '`', '~' };
    map.m_keys[ui::hid::left_bracket] = { '[', '{' };
    map.m_keys[ui::hid::right_bracket] = { ']', '}' };
    map.m_keys[ui::hid::comma] = { ',', '<' };
    map.m_keys[ui::hid::period] = { '.', '>' };
    map.m_keys[ui::hid::slash] = { '/', '?' };
    map.m_keys[ui::hid::equal] = { '=', '+' };
    map.m_keys[ui::hid::minus] = { '-', '_' };
    map.m_keys[ui::hid::tab] = { '\t', '\t' };
    map.m_keys[ui::hid::space] = { ' ', ' ' };
    map.m_keys[ui::hid::apostrophe] = { '\'', '"' };
    map.m_keys[ui::hid::left_bracket] = { '[', '{' };
    map.m_keys[ui::hid::right_bracket] = { ']', '}' };

    return map;
}
