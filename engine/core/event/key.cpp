// Copyright (c) 2020 Tom Hancocks
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

#include "core/event/key.hpp"

// MARK: - Helper Functions

#define KEY_STUB_SYMBOL(_c) event_key_##_c
#define KEY_REFERENCE(_c) &KEY_STUB_SYMBOL(_c)
#define KEY_CONSTANT_STUB(_c) static int KEY_STUB_SYMBOL(_c) = static_cast<int>(event::key::code::_c);

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
KEY_CONSTANT_STUB(num0)
KEY_CONSTANT_STUB(num1)
KEY_CONSTANT_STUB(num2)
KEY_CONSTANT_STUB(num3)
KEY_CONSTANT_STUB(num4)
KEY_CONSTANT_STUB(num5)
KEY_CONSTANT_STUB(num6)
KEY_CONSTANT_STUB(num7)
KEY_CONSTANT_STUB(num8)
KEY_CONSTANT_STUB(num9)
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

// MARK: - Lua

auto event::key::enroll_object_apu_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{

    luabridge::getGlobalNamespace(lua->internal_state())
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
            .addVariable("N0", KEY_REFERENCE(num0), false)
            .addVariable("N1", KEY_REFERENCE(num1), false)
            .addVariable("N2", KEY_REFERENCE(num2), false)
            .addVariable("N3", KEY_REFERENCE(num3), false)
            .addVariable("N4", KEY_REFERENCE(num4), false)
            .addVariable("N5", KEY_REFERENCE(num5), false)
            .addVariable("N6", KEY_REFERENCE(num6), false)
            .addVariable("N7", KEY_REFERENCE(num7), false)
            .addVariable("N8", KEY_REFERENCE(num8), false)
            .addVariable("N9", KEY_REFERENCE(num9), false)
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

        .beginClass<event::key>("KeyEvent")
            .addProperty("keycode", &key::code)
            .addProperty("scancode", &key::scancode)
            .addProperty("shift", &key::has_shift)
            .addProperty("alt", &key::has_alt)
            .addProperty("control", &key::has_control)
            .addProperty("super", &key::has_super)
            .addProperty("caps", &key::has_caps)
            .addProperty("num", &key::has_num)
            .addProperty("pressed", &key::is_pressed)
            .addProperty("held", &key::is_held)
            .addProperty("released", &key::is_released)
        .endClass();
}

// MARK: - Construction

event::key::key(const enum key::code& code, const int& scancode, const enum key::state& state)
    : m_keycode(code), m_scancode(scancode), m_state(state), m_modifiers()
{

}

event::key::key(const key &key)
    : m_keycode(key.m_keycode), m_scancode(key.m_scancode), m_state(key.m_state), m_modifiers(key.m_modifiers)
{

}

// MARK: - Accessors

auto event::key::code() const -> int
{
    return m_keycode;
};

auto event::key::modifiers() const -> enum key::modifier
{
    return m_modifiers;
};

auto event::key::state() const -> enum key::state
{
    return m_state;
};

auto event::key::scancode() const -> int
{
    return m_scancode;
}

// MARK: - Lua Accessors

auto event::key::has_shift() const -> bool
{
    return m_modifiers & key::modifier::shift;
}

auto event::key::has_control() const -> bool
{
    return m_modifiers & key::modifier::control;
}

auto event::key::has_alt() const -> bool
{
    return m_modifiers & key::modifier::alt;
}

auto event::key::has_super() const -> bool
{
    return m_modifiers & key::modifier::super;
}

auto event::key::has_caps() const -> bool
{
    return m_modifiers & key::modifier::caps;
}

auto event::key::has_num() const -> bool
{
    return m_modifiers & key::modifier::num;
}


auto event::key::is_pressed() const -> bool
{
    return m_state == key::state::pressed;
}

auto event::key::is_held() const -> bool
{
    return m_state == key::state::held;
}

auto event::key::is_released() const -> bool
{
    return m_state == key::state::released;
}