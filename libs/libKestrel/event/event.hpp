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
#include <libKestrel/math/point.hpp>
#include <libKestrel/event/key.hpp>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/util/availability.hpp>

namespace kestrel
{
    enum lua_api(EventType, Available_0_8) event_type : std::uint32_t
    {
        none lua_case(None, Available_0_8) = 0,

        // Mouse Events
        any_mouse_event lua_case(AnyMouseEvent, Available_0_8) = 0xFFF,
        lmb_down lua_case(LeftMouseDown, Available_0_8) = 1 << 0,
        lmb_up lua_case(LeftMouseUp, Available_0_8) = 1 << 1,
        mmb_down lua_case(MiddleMouseDown, Available_0_8) = 1 << 2,
        mmb_up lua_case(MiddleMouseUp, Available_0_8) = 1 << 3,
        rmb_down lua_case(RightMouseDown, Available_0_8) = 1 << 4,
        rmb_up lua_case(RightMouseUp, Available_0_8) = 1 << 5,
        mouse_move lua_case(MouseMoved, Available_0_8) = 1 << 6,
        mouse_drag lua_case(MouseDragged, Available_0_8) = 1 << 7,
        mouse_scroll lua_case(MouseScrolled, Available_0_8) = 1 << 8,
        any_mouse_down lua_case(AnyMouseDown, Available_0_8) = lmb_down | mmb_down | rmb_down,
        any_mouse_up lua_case(AnyMouseUp, Available_0_8) = lmb_up | mmb_up | rmb_up,

        // Key Events
        any_key_event lua_case(AnyKeyEvent, Available_0_8) = 0xF000,
        key_down lua_case(KeyDown, Available_0_8) = 1 << 12,
        key_up lua_case(KeyUp, Available_0_8) = 1 << 13,
        key_repeat lua_case(KeyRepeat, Available_0_8) = 1 << 14,
        key_typed lua_case(KeyTyped, Available_0_8) = 1 << 15,

        // Key Modifiers
        has_caps_lock_modifier lua_case(HasCapsLockModifier, Available_0_8) = 1 << 16,
        has_shift_modifier lua_case(HasShiftModifier, Available_0_8) = 1 << 17,
        has_control_modifier lua_case(HasControlModifier, Available_0_8) = 1 << 18,
        has_alt_modifier lua_case(HasAltModifier, Available_0_8) = 1 << 19,
        has_super_modifier lua_case(HasSuperModifier, Available_0_8) = 1 << 20,
    };

    struct lua_api(Event, Available_0_8) event
    {
    public:
        luatool_no_implementation
        has_constructable_lua_api(event);

        event() = default;
        event(const event&) = default;

        static auto mouse(enum event_type type, const math::point& point) -> event;
        static auto key(enum event_type type, enum hid::key key, unsigned int c = '\0') -> event;

        lua_function(hasType, Available_0_8) [[nodiscard]] inline auto has(enum event_type type) const -> bool { return (m_type & type) != 0; }
        lua_getter(type, Available_0_8) [[nodiscard]] inline auto type() const -> enum event_type { return m_type; };

        lua_getter(isMouseEvent, Available_0_8) [[nodiscard]] inline auto is_mouse_event() const -> bool { return has(event_type::any_mouse_event); }
        lua_getter(isKeyEvent, Available_0_8) [[nodiscard]] inline auto is_key_event() const -> bool { return has(event_type::any_key_event); }

        lua_getter(key, Available_0_8) [[nodiscard]] inline auto key() const -> hid::key { return m_key; }
        lua_getter(character, Available_0_8) [[nodiscard]] inline auto character() const -> unsigned int { return m_char; }

        [[nodiscard]] inline auto is(hid::key key) const -> bool { return m_key == key; }
        [[nodiscard]] inline auto is(unsigned int character) const -> bool { return m_char == character; }

        lua_getter(location, Available_0_8) [[nodiscard]] inline auto location() const -> math::point { return m_location; }

        lua_function(relocated, Available_0_8) [[nodiscard]] auto relocated(const math::point& point) const -> event;

    private:
        enum event_type m_type { event_type::none };
        hid::key m_key { hid::key::unknown };
        unsigned int m_char { '\0' };
        math::point m_location;
    };

}

