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

#include "core/event/event.hpp"
#include "core/event/key.hpp"

// MARK: - Helper Functions

#define KEY_STUB_SYMBOL(_c) event_key_##_c
#define KEY_REFERENCE(_c) &KEY_STUB_SYMBOL(_c)
#define KEY_CONSTANT_STUB(_c) static int KEY_STUB_SYMBOL(_c) = static_cast<int>(hid::key::_c);

#define EVENT_TYPE_STUB_SYMBOL(_s) event_type_##_s
#define EVENT_TYPE_REFERENCE(_s) &EVENT_TYPE_STUB_SYMBOL(_s)
#define EVENT_TYPE_CONSTANT_STUB(_s) static int EVENT_TYPE_STUB_SYMBOL(_s) = static_cast<int>(event::type::_s);

KEY_CONSTANT_STUB(a)
KEY_CONSTANT_STUB(b)
KEY_CONSTANT_STUB(c)
KEY_CONSTANT_STUB(d)
KEY_CONSTANT_STUB(e)
KEY_CONSTANT_STUB(f)
KEY_CONSTANT_STUB(g)
KEY_CONSTANT_STUB(h)
KEY_CONSTANT_STUB(i)
KEY_CONSTANT_STUB(j)
KEY_CONSTANT_STUB(k)
KEY_CONSTANT_STUB(l)
KEY_CONSTANT_STUB(m)
KEY_CONSTANT_STUB(n)
KEY_CONSTANT_STUB(o)
KEY_CONSTANT_STUB(p)
KEY_CONSTANT_STUB(q)
KEY_CONSTANT_STUB(r)
KEY_CONSTANT_STUB(s)
KEY_CONSTANT_STUB(t)
KEY_CONSTANT_STUB(u)
KEY_CONSTANT_STUB(v)
KEY_CONSTANT_STUB(w)
KEY_CONSTANT_STUB(x)
KEY_CONSTANT_STUB(y)
KEY_CONSTANT_STUB(z)
KEY_CONSTANT_STUB(kp_0)
KEY_CONSTANT_STUB(kp_1)
KEY_CONSTANT_STUB(kp_2)
KEY_CONSTANT_STUB(kp_3)
KEY_CONSTANT_STUB(kp_4)
KEY_CONSTANT_STUB(kp_5)
KEY_CONSTANT_STUB(kp_6)
KEY_CONSTANT_STUB(kp_7)
KEY_CONSTANT_STUB(kp_8)
KEY_CONSTANT_STUB(kp_9)
KEY_CONSTANT_STUB(space)
KEY_CONSTANT_STUB(comma)
KEY_CONSTANT_STUB(period)
KEY_CONSTANT_STUB(minus)
KEY_CONSTANT_STUB(equal)
KEY_CONSTANT_STUB(slash)
KEY_CONSTANT_STUB(semi_colon)
KEY_CONSTANT_STUB(apostrophe)
KEY_CONSTANT_STUB(left_bracket)
KEY_CONSTANT_STUB(right_bracket)
KEY_CONSTANT_STUB(grave_accent)
KEY_CONSTANT_STUB(escape)
KEY_CONSTANT_STUB(enter)
KEY_CONSTANT_STUB(tab)
KEY_CONSTANT_STUB(backspace)
KEY_CONSTANT_STUB(right)
KEY_CONSTANT_STUB(left)
KEY_CONSTANT_STUB(down)
KEY_CONSTANT_STUB(up)

EVENT_TYPE_CONSTANT_STUB(none)
EVENT_TYPE_CONSTANT_STUB(any_mouse_event)
EVENT_TYPE_CONSTANT_STUB(lmb_down)
EVENT_TYPE_CONSTANT_STUB(lmb_up)
EVENT_TYPE_CONSTANT_STUB(mmb_down)
EVENT_TYPE_CONSTANT_STUB(mmb_up)
EVENT_TYPE_CONSTANT_STUB(rmb_down)
EVENT_TYPE_CONSTANT_STUB(rmb_up)
EVENT_TYPE_CONSTANT_STUB(mouse_move)
EVENT_TYPE_CONSTANT_STUB(mouse_drag)
EVENT_TYPE_CONSTANT_STUB(mouse_scroll)
EVENT_TYPE_CONSTANT_STUB(any_key_event)
EVENT_TYPE_CONSTANT_STUB(key_down)
EVENT_TYPE_CONSTANT_STUB(key_up)
EVENT_TYPE_CONSTANT_STUB(key_repeat)
EVENT_TYPE_CONSTANT_STUB(key_typed)

// MARK: - Lua

auto event::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("Key")
            .addVariable("A", KEY_REFERENCE(a), false)
            .addVariable("B", KEY_REFERENCE(b), false)
            .addVariable("C", KEY_REFERENCE(c), false)
            .addVariable("D", KEY_REFERENCE(d), false)
            .addVariable("E", KEY_REFERENCE(e), false)
            .addVariable("F", KEY_REFERENCE(f), false)
            .addVariable("G", KEY_REFERENCE(g), false)
            .addVariable("H", KEY_REFERENCE(h), false)
            .addVariable("I", KEY_REFERENCE(i), false)
            .addVariable("J", KEY_REFERENCE(j), false)
            .addVariable("K", KEY_REFERENCE(k), false)
            .addVariable("L", KEY_REFERENCE(l), false)
            .addVariable("M", KEY_REFERENCE(m), false)
            .addVariable("N", KEY_REFERENCE(n), false)
            .addVariable("O", KEY_REFERENCE(o), false)
            .addVariable("P", KEY_REFERENCE(p), false)
            .addVariable("Q", KEY_REFERENCE(q), false)
            .addVariable("R", KEY_REFERENCE(r), false)
            .addVariable("S", KEY_REFERENCE(s), false)
            .addVariable("T", KEY_REFERENCE(t), false)
            .addVariable("U", KEY_REFERENCE(u), false)
            .addVariable("V", KEY_REFERENCE(v), false)
            .addVariable("W", KEY_REFERENCE(w), false)
            .addVariable("X", KEY_REFERENCE(x), false)
            .addVariable("Y", KEY_REFERENCE(y), false)
            .addVariable("Z", KEY_REFERENCE(z), false)
            .addVariable("N0", KEY_REFERENCE(kp_0), false)
            .addVariable("N1", KEY_REFERENCE(kp_1), false)
            .addVariable("N2", KEY_REFERENCE(kp_2), false)
            .addVariable("N3", KEY_REFERENCE(kp_3), false)
            .addVariable("N4", KEY_REFERENCE(kp_4), false)
            .addVariable("N5", KEY_REFERENCE(kp_5), false)
            .addVariable("N6", KEY_REFERENCE(kp_6), false)
            .addVariable("N7", KEY_REFERENCE(kp_7), false)
            .addVariable("N8", KEY_REFERENCE(kp_8), false)
            .addVariable("N9", KEY_REFERENCE(kp_9), false)
            .addVariable("SPACE", KEY_REFERENCE(space), false)
            .addVariable("COMMA", KEY_REFERENCE(comma), false)
            .addVariable("PERIOD", KEY_REFERENCE(period), false)
            .addVariable("MINUS", KEY_REFERENCE(minus), false)
            .addVariable("EQUAL", KEY_REFERENCE(equal), false)
            .addVariable("SLASH", KEY_REFERENCE(slash), false)
            .addVariable("SEMICOLON", KEY_REFERENCE(semi_colon), false)
            .addVariable("APOSTROPHE", KEY_REFERENCE(apostrophe), false)
            .addVariable("LBRACKET", KEY_REFERENCE(left_bracket), false)
            .addVariable("RBRACKET", KEY_REFERENCE(right_bracket), false)
            .addVariable("GRAVE", KEY_REFERENCE(grave_accent), false)
            .addVariable("ESCAPE", KEY_REFERENCE(escape), false)
            .addVariable("ENTER", KEY_REFERENCE(enter), false)
            .addVariable("TAB", KEY_REFERENCE(tab), false)
            .addVariable("BACKSPACE", KEY_REFERENCE(backspace), false)
            .addVariable("RIGHT", KEY_REFERENCE(right), false)
            .addVariable("LEFT", KEY_REFERENCE(left), false)
            .addVariable("DOWN", KEY_REFERENCE(down), false)
            .addVariable("UP", KEY_REFERENCE(up), false)
        .endNamespace()

        .beginNamespace("EventType")
            .addVariable("NONE", EVENT_TYPE_REFERENCE(none), false)
            .addVariable("ANY_MOUSE_EVENT", EVENT_TYPE_REFERENCE(any_mouse_event), false)
            .addVariable("LEFT_MOUSE_BUTTON_DOWN", EVENT_TYPE_REFERENCE(lmb_down), false)
            .addVariable("LEFT_MOUSE_BUTTON_UP", EVENT_TYPE_REFERENCE(lmb_up), false)
            .addVariable("MIDDLE_MOUSE_BUTTON_DOWN", EVENT_TYPE_REFERENCE(mmb_down), false)
            .addVariable("MIDDLE_MOUSE_BUTTON_UP", EVENT_TYPE_REFERENCE(mmb_up), false)
            .addVariable("RIGHT_MOUSE_BUTTON_DOWN", EVENT_TYPE_REFERENCE(rmb_down), false)
            .addVariable("RIGHT_MOUSE_BUTTON_UP", EVENT_TYPE_REFERENCE(rmb_up), false)
            .addVariable("MOUSE_MOVE", EVENT_TYPE_REFERENCE(mouse_move), false)
            .addVariable("MOUSE_DRAG", EVENT_TYPE_REFERENCE(mouse_drag), false)
            .addVariable("MOUSE_SCROLL", EVENT_TYPE_REFERENCE(mouse_scroll), false)
            .addVariable("ANY_KEY_EVENT", EVENT_TYPE_REFERENCE(any_key_event), false)
            .addVariable("KEY_DOWN", EVENT_TYPE_REFERENCE(key_down), false)
            .addVariable("KEY_UP", EVENT_TYPE_REFERENCE(key_up), false)
            .addVariable("KEY_REPEAT", EVENT_TYPE_REFERENCE(key_repeat), false)
            .addVariable("KEY_TYPED", EVENT_TYPE_REFERENCE(key_typed), false)
        .endNamespace()

        .beginClass<event>("Event")
            .addProperty("isMouseEvent", &event::is_mouse_event)
            .addProperty("isKeyEvent", &event::is_key_event)
            .addProperty("key", &event::key)
            .addProperty("character", &event::character)
            .addProperty("location", &event::location)
            .addFunction("has", &event::has)
        .endClass();
}

// MARK: - Construction

auto event::mouse(enum type type, const math::point &point) -> event
{
    event e;
    e.m_type = type;
    e.m_location = point;
    return e;
}

auto event::key(enum type type, hid::key key, unsigned int c) -> event
{
    event e;
    e.m_type = type;
    e.m_key = key;
    e.m_char = c;
    return e;
}

auto event::relocated(const math::point &point) const -> event
{
    return mouse(type(), point);
}

// MARK: - Deprecated Key Accessor Functions

auto event::code() const -> int
{
    return static_cast<int>(m_key);
}

auto event::scancode() const -> int
{
    return 0;
}

// MARK: - Deprecated Mouse Accessor Functions

auto event::has_shift() const -> bool
{
    return key() == hid::left_shift || key() == hid::right_shift;
}

auto event::has_alt() const -> bool
{
    return key() == hid::left_alt || key() == hid::right_alt;
}

auto event::has_caps() const -> bool
{
    return key() == hid::caps_lock;
}

auto event::has_control() const -> bool
{
    return key() == hid::left_control || key() == hid::right_control;
}

auto event::has_num() const -> bool
{
    return key() == hid::num_lock;
}

auto event::has_super() const -> bool
{
    return key() == hid::left_super || key() == hid::right_super;
}

auto event::is_pressed() const -> bool
{
    return has(event::lmb_down) | has(event::mmb_down) | has(event::rmb_down) | has(event::key_down);
}

auto event::is_released() const -> bool
{
    return has(event::lmb_up) | has(event::mmb_up) | has(event::rmb_up) | has(event::key_up);
}

auto event::is_held() const -> bool
{
    return false;
}

auto event::was_dragged() const -> bool
{
    return has(event::mouse_drag);
}

auto event::has_moved() const -> bool
{
    return (event::mouse_move);
}