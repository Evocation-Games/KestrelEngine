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

#include <cstdint>
#include <libUI/types/point.hpp>
#include <libUI/hid/key.hpp>

namespace ui
{
    struct event
    {
    public:
        enum type : std::uint32_t {
            none = 0,

            // Mouse Events
            any_mouse_event = 0xFFF,
            lmb_down = 1 << 0,
            lmb_up = 1 << 1,
            mmb_down = 1 << 2,
            mmb_up = 1 << 3,
            rmb_down = 1 << 4,
            rmb_up = 1 << 5,
            mouse_move = 1 << 6,
            mouse_drag = 1 << 7,
            mouse_scroll = 1 << 8,
            any_mouse_down = lmb_down | mmb_down | rmb_down,
            any_mouse_up = lmb_up | mmb_up | rmb_up,

            // Key Events
            any_key_event = 0xF000,
            key_down = 1 << 12,
            key_up = 1 << 13,
            key_repeat = 1 << 14,
            key_typed = 1 << 15,

            // Key Modifiers
            has_caps_lock_modifier = 1 << 16,
            has_shift_modifier = 1 << 17,
            has_control_modifier = 1 << 18,
            has_alt_modifier = 1 << 19,
            has_super_modifier = 1 << 20,
        };

        event() = default;
        event(const event&) = default;

        static auto mouse(enum type type, const point& point) -> event;
        static auto key(enum type type, enum hid::key key, unsigned int c = '\0') -> event;

        [[nodiscard]] auto has(enum type type) const -> bool
        {
            return (m_type & type) != 0;
        }

        [[nodiscard]] inline auto type() const -> enum type
        {
            return m_type;
        }

        [[nodiscard]] inline auto is_mouse_event() const -> bool
        {
            return has(type::any_mouse_event);
        }

        [[nodiscard]] inline auto is_key_event() const -> bool
        {
            return has(type::any_key_event);
        }

        [[nodiscard]] inline auto key() const -> enum hid::key
        {
            return m_key;
        }

        [[nodiscard]] inline auto character() const -> unsigned int
        {
            return m_char;
        }

        [[nodiscard]] inline auto location() const -> point
        {
            return m_location;
        }

        [[nodiscard]] inline auto is(enum hid::key key) const -> bool
        {
            return (m_key == key);
        }

        [[nodiscard]] inline auto is(unsigned char c) const -> bool
        {
            return (m_char == c);
        }

        [[nodiscard]] auto relocated(const point& point) const -> event;

    private:
        enum type m_type { type::none };
        enum hid::key m_key { hid::key::unknown };
        unsigned int m_char { '\0' };
        point m_location { 0, 0 };
    };
}