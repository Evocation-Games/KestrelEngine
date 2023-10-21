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
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/util/availability.hpp>
#include <libUI/event/event.hpp>
#include <libUI/hid/key.hpp>

namespace kestrel
{
    /**
     * Represents a user generated interaction event, from either the keyboard or mouse. Events _can_
     * represent multiple events, but generally do not.
     */
    struct lua_api(Event, Available_0_8) event
    {
    public:
        luatool_no_implementation
        has_constructable_lua_api(event);

        event() = default;
        event(const event&) = default;

        static auto mouse(enum ::ui::event::type type, const math::point& point) -> event;
        static auto key(enum ::ui::event::type type, enum ::ui::hid::key key, unsigned int c = '\0') -> event;

        [[nodiscard]] inline auto has(enum ::ui::event::type type) const -> bool { return m_event.has(type); }

        /**
         * Check if the receiver represents the specified event type.
         * @param type  An `Event.Type` value to be tested.
         * @return      A boolean indicating whether the receiver represents the specified type.
         */
        lua_function(hasType, Available_0_8) [[nodiscard]] inline auto has_type(std::uint32_t type) const -> bool
        {
            return has(static_cast<enum ::ui::event::type>(type));
        }

        /**
         * Returns a value combining all of the types represented by the receiver.
         */
        lua_getter(type, Available_0_8) [[nodiscard]] inline auto type() const -> enum ::ui::event::type { return m_event.type(); };

        /**
         * Does the receiver contain any kind of mouse event?
         */
        lua_getter(isMouseEvent, Available_0_8) [[nodiscard]] inline auto is_mouse_event() const -> bool { return m_event.is_mouse_event(); }

        /**
         * Does the receiver contain any kind of keyboard event?
         */
        lua_getter(isKeyEvent, Available_0_8) [[nodiscard]] inline auto is_key_event() const -> bool { return m_event.is_key_event(); }

        /**
         * If the receiver is a keyboard event, then this will return an `Event.Key` value representing the key that
         * the user interacted with.
         */
        lua_getter(key, Available_0_8) [[nodiscard]] inline auto key() const -> enum ::ui::hid::key { return m_event.key(); }

        /**
         * If the receiver is a keyboard event, relating to printable character, then this will return the character.
         */
        lua_getter(character, Available_0_8) [[nodiscard]] inline auto character() const -> unsigned int { return m_event.character(); }

        [[nodiscard]] inline auto is(enum ::ui::hid::key key) const -> bool { return m_event.is(key); }
        [[nodiscard]] inline auto is(unsigned int character) const -> bool { return m_event.is(character); }

        /**
         * If the receiver is a mouse event, then this will return the mouse location in relation to the top left of the
         * game window viewport.
         */
        lua_getter(location, Available_0_8) [[nodiscard]] inline auto location() const -> math::point
        {
            return math::point(m_event.location());
        }

        /**
         * Returns a copy of the receiver with the location changed to the new location specified.
         * @param point     A new `Point` value for a new location for the receiver copy.
         * @return          A copy of the receiver with a new location.
         */
        lua_function(relocated, Available_0_8) [[nodiscard]] auto relocated(const math::point& point) const -> event;

    private:
        struct ::ui::event m_event;
    };

}

