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

#include "core/event/mouse.hpp"

// MARK: - Lua

auto event::mouse::enroll_object_apu_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginClass<event::mouse>("MouseEvent")
            .addProperty("point", &mouse::get_point)
            .addProperty("action", &mouse::get_action)
            .addProperty("button", &mouse::get_button)
            .addProperty("moved", &mouse::get_moved)
            .addProperty("isPressed", &mouse::get_pressed)
            .addProperty("isReleased", &mouse::get_released)
        .endClass();
}

// MARK: - Construction

event::mouse::mouse(const event::mouse::action &action, const event::mouse::button &button)
    : m_action(action), m_button(button), m_point(0)
{

}

event::mouse::mouse(const math::point &point, const event::mouse::action &action, const event::mouse::button &button)
    : m_action(action), m_button(button), m_point(point)
{

}

event::mouse::mouse(const int &x, const int &y, const event::mouse::action &action, const event::mouse::button &button)
    : m_action(action), m_button(button), m_point(x, y)
{

}

// MARK: - Lua Accessors

auto event::mouse::get_point() const -> math::point
{
    return m_point;
}

auto event::mouse::get_action() const -> int
{
    return m_action;
}

auto event::mouse::get_button() const -> int
{
    return m_button;
}

auto event::mouse::get_moved() const -> bool
{
    return m_action == event::mouse::moved;
}

auto event::mouse::get_released() const -> bool
{
    return m_action == event::mouse::released;
}

auto event::mouse::get_pressed() const -> bool
{
    return m_action == event::mouse::pressed;
}
