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

#include <libUI/event/event.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/util/availability.hpp>

namespace kestrel
{
    enum lua_api(Event.Type, Available_0_8) event_type : std::uint32_t
    {
        none lua_case(None, Available_0_8) = ::ui::event::type::none,

        // Mouse Events
        any_mouse_event lua_case(AnyMouseEvent, Available_0_8) = ::ui::event::type::any_mouse_event,
        lmb_down lua_case(LeftMouseDown, Available_0_8) = ::ui::event::type::lmb_down,
        lmb_up lua_case(LeftMouseUp, Available_0_8) = ::ui::event::type::lmb_up,
        mmb_down lua_case(MiddleMouseDown, Available_0_8) = ::ui::event::type::mmb_down,
        mmb_up lua_case(MiddleMouseUp, Available_0_8) = ::ui::event::type::mmb_up,
        rmb_down lua_case(RightMouseDown, Available_0_8) = ::ui::event::type::rmb_down,
        rmb_up lua_case(RightMouseUp, Available_0_8) = ::ui::event::type::rmb_up,
        mouse_move lua_case(MouseMoved, Available_0_8) = ::ui::event::type::mouse_move,
        mouse_drag lua_case(MouseDragged, Available_0_8) = ::ui::event::type::mouse_drag,
        mouse_scroll lua_case(MouseScrolled, Available_0_8) = ::ui::event::type::mouse_scroll,
        any_mouse_down lua_case(AnyMouseDown, Available_0_8) = ::ui::event::type::any_mouse_down,
        any_mouse_up lua_case(AnyMouseUp, Available_0_8) = ::ui::event::type::any_mouse_up,

        // Key Events
        any_key_event lua_case(AnyKeyEvent, Available_0_8) = ::ui::event::type::any_key_event,
        key_down lua_case(KeyDown, Available_0_8) = ::ui::event::type::key_down,
        key_up lua_case(KeyUp, Available_0_8) = ::ui::event::type::key_up,
        key_repeat lua_case(KeyRepeat, Available_0_8) = ::ui::event::type::key_repeat,
        key_typed lua_case(KeyTyped, Available_0_8) = ::ui::event::type::key_typed,

        // Key Modifiers
        has_caps_lock_modifier lua_case(HasCapsLockModifier, Available_0_8) = ::ui::event::type::has_caps_lock_modifier,
        has_shift_modifier lua_case(HasShiftModifier, Available_0_8) = ::ui::event::type::has_shift_modifier,
        has_control_modifier lua_case(HasControlModifier, Available_0_8) = ::ui::event::type::has_control_modifier,
        has_alt_modifier lua_case(HasAltModifier, Available_0_8) = ::ui::event::type::has_alt_modifier,
        has_super_modifier lua_case(HasSuperModifier, Available_0_8) = ::ui::event::type::has_super_modifier,
    };
}