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

#if !defined(TEXT_ENTRY_HPP)
#define TEXT_ENTRY_HPP

#include <array>
#include "core/event/key.hpp"

namespace event::control
{

    class text_entry
    {
    public:
        struct key_mapping
        {
            char base;
            char shifted;
            key_mapping(char base = 0, char shifted = 0) : base(base), shifted(shifted) {}
        };

        text_entry();

        auto load_default_keymap() -> void;

        auto receive(const event::key& key) -> void;

        [[nodiscard]] auto string_value() const -> std::string;
        auto set_string_value(const std::string& value) -> void;

        [[nodiscard]] auto cursor_position() const -> int;
        auto set_cursor_position(const int& position) -> void;

    private:
        std::array<key_mapping, 336> m_keymap;
        std::string m_value;
        bool m_alt { false };
        bool m_shift { false };
        bool m_control { false };
        bool m_super { false };
        int m_cursor { 0 };
    };

}

#endif //TEXT_ENTRY_HPP
