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

#pragma once

#include <array>
#include <functional>
#include "core/event/event.hpp"

class text_entry_event
{
public:
    struct key_mapping
    {
        char base { 0 };
        char shifted { 0 };
        key_mapping() = default;
        key_mapping(char base, char shifted) : base(base), shifted(shifted) {}
    };

    text_entry_event();

    auto load_default_keymap() -> void;

    auto receive(const event& e) -> void;

    [[nodiscard]] auto string_value() const -> std::string;
    auto set_string_value(const std::string& value) -> void;

    [[nodiscard]] auto cursor_position() const -> int;
    auto set_cursor_position(const int& position) -> void;

    auto on_enter(std::function<auto(const std::string&)->void> callback) -> void;
    auto on_escape(std::function<auto()->void> callback) -> void;

private:
    std::function<auto(const std::string&)->void> m_on_enter;
    std::function<auto()->void> m_on_escape;
    std::array<key_mapping, 336> m_keymap;
    std::string m_value;
    bool m_alt { false };
    bool m_shift { false };
    bool m_control { false };
    bool m_super { false };
    int m_cursor { 0 };
};
