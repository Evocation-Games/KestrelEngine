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

#include "foundation/clock/timed_event.hpp"

// MARK: - Constructor

kestrel::timed_event::timed_event(clock::time after)
        : m_period(clock::global().until(after).count()), m_trigger(after_date)
{

}

kestrel::timed_event::timed_event(double period, bool repeats)
        : m_period(period), m_trigger(repeats ? trigger::repeats : trigger::after_duration)
{

}

// MARK: - Accessors

auto kestrel::timed_event::dead() const -> bool
{
    return m_trigger == never;
}


// MARK: - Callbacks

auto kestrel::timed_event::set_callback(luabridge::LuaRef callback) -> void
{
    m_lua_callback = callback;
}

auto kestrel::timed_event::set_callback(function callback) -> void
{
    m_callback = callback;
}

// MARK: - Firing

auto kestrel::timed_event::should_fire() const -> bool
{
    switch (m_trigger) {
        case after_date: {
            return (clock::global().current() >= m_date);
        }
        case repeats:
        case after_duration: {
            return (clock::global().since(m_date).count() >= m_period);
        }
        default: {
            return false;
        }
    }
}

auto kestrel::timed_event::fire() -> void
{
    if (should_fire()) {
        if (m_callback.has_value()) {
            m_callback.value()();
        }
        m_lua_callback();
        m_trigger = never;
    }
}