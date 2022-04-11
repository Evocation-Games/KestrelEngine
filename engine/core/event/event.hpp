// Copyright (c) 2022 Tom Hancocks
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
#include "math/point.hpp"
#include "core/event/key.hpp"
#include "util/hint.hpp"
#include "scripting/state.hpp"

struct event : public scripting::lua::object
{
public:
    enum type : uint16_t
    {
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

        // Key Events
        any_key_event = 0xF000,
        key_down = 1 << 12,
        key_up = 1 << 13,
        key_repeat = 1 << 14,
        key_typed = 1 << 15
    };

    enum state : int { off, pressed, held, released };
    enum modifier : char { shift = 0x01, control = 0x02, alt = 0x04, super = 0x08, caps = 0x10, num = 0x20 };

public:
    typedef luabridge::RefCountedPtr<event> lua_reference;
    static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

public:
    event() = default;
    event(const event&) = default;

    static auto mouse(enum type type, const math::point& point) -> event;
    static auto key(enum type type, hid::key key, unsigned int c = '\0') -> event;

    [[nodiscard]] lua_api inline auto has(enum type type) const -> bool { return (m_type & type) != 0; }
    [[nodiscard]] lua_api inline auto type() const -> enum type { return m_type; };

    [[nodiscard]] lua_api inline auto is_mouse_event() const -> bool { return has(any_mouse_event); }
    [[nodiscard]] lua_api inline auto is_key_event() const -> bool { return has(any_key_event); }

    [[nodiscard]] lua_api inline auto key() const -> hid::key { return m_key; }
    [[nodiscard]] lua_api inline auto character() const -> unsigned int { return m_char; }

    [[nodiscard]] lua_api inline auto location() const -> math::point { return m_location; }

    [[nodiscard]] lua_api auto relocated(const math::point& point) const -> event;

public:
    // Old Key Event Style
    [[nodiscard]] lua_api auto code() const -> int;
    [[nodiscard]] auto modifiers() const -> enum modifier;
    [[nodiscard]] auto state() const -> enum state;
    [[nodiscard]] lua_api auto scancode() const -> int;

    [[nodiscard]] lua_api auto has_shift() const -> bool;
    [[nodiscard]] lua_api auto has_control() const -> bool;
    [[nodiscard]] lua_api auto has_alt() const -> bool;
    [[nodiscard]] lua_api auto has_super() const -> bool;
    [[nodiscard]] lua_api auto has_caps() const -> bool;
    [[nodiscard]] lua_api auto has_num() const -> bool;

    [[nodiscard]] lua_api auto is_pressed() const -> bool;
    [[nodiscard]] lua_api auto is_released() const -> bool;
    [[nodiscard]] lua_api auto is_held() const -> bool;
    [[nodiscard]] lua_api auto has_moved() const -> bool;

private:
    enum type m_type { type::none };
    hid::key m_key { hid::key::unknown };
    unsigned int m_char { '\0' };
    math::point m_location;
};
