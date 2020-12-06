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
#define KEY_CONSTANT_STUB(_c) static auto KEY_STUB_SYMBOL(_c)() -> int { return static_cast<int>(event::key::code::_c); }

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

// MARK: - Lua

auto event::key::enroll_object_apu_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{

    luabridge::getGlobalNamespace(lua->internal_state())
        .beginNamespace("Key")
            .addProperty("A", KEY_REFERENCE(a), false)
            .addProperty("B", KEY_REFERENCE(b), false)
            .addProperty("C", KEY_REFERENCE(c), false)
            .addProperty("D", KEY_REFERENCE(d), false)
            .addProperty("E", KEY_REFERENCE(e), false)
            .addProperty("F", KEY_REFERENCE(f), false)
            .addProperty("G", KEY_REFERENCE(g), false)
            .addProperty("H", KEY_REFERENCE(h), false)
            .addProperty("I", KEY_REFERENCE(i), false)
            .addProperty("J", KEY_REFERENCE(j), false)
            .addProperty("K", KEY_REFERENCE(k), false)
            .addProperty("L", KEY_REFERENCE(l), false)
            .addProperty("M", KEY_REFERENCE(m), false)
            .addProperty("N", KEY_REFERENCE(n), false)
            .addProperty("O", KEY_REFERENCE(o), false)
            .addProperty("P", KEY_REFERENCE(p), false)
            .addProperty("Q", KEY_REFERENCE(q), false)
            .addProperty("R", KEY_REFERENCE(r), false)
            .addProperty("S", KEY_REFERENCE(s), false)
            .addProperty("T", KEY_REFERENCE(t), false)
            .addProperty("U", KEY_REFERENCE(u), false)
            .addProperty("V", KEY_REFERENCE(v), false)
            .addProperty("W", KEY_REFERENCE(w), false)
            .addProperty("X", KEY_REFERENCE(x), false)
            .addProperty("Y", KEY_REFERENCE(y), false)
            .addProperty("Z", KEY_REFERENCE(z), false)
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
    : m_keycode(code), m_scancode(scancode), m_state(state)
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