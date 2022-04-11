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

#include "core/clock/timed_event.hpp"

// MARK: - Construction

rtc::timed_event::timed_event(const clock::time &after, const luabridge::LuaRef &callback)
    : m_period(rtc::clock::global().until(after).count()), m_callback(callback)
{
}

rtc::timed_event::timed_event(const double &period, const luabridge::LuaRef &callback, const bool &repeats)
    : m_period(period), m_trigger(repeats ? trigger::repeats : trigger::after_duration), m_callback(callback)
{
}

// MARK: - Accessors

auto rtc::timed_event::dead() const -> bool
{
    return m_trigger == never;
}

// MARK: - Firing

auto rtc::timed_event::should_fire() const -> bool
{
    switch (m_trigger) {
        case after_date: {
            return (rtc::clock::global().current() >= m_date);
        }
        case repeats:
        case after_duration: {
            return (rtc::clock::global().since(m_date).count() >= m_period);
        }
        default: {
            return false;
        }
    }
}

auto rtc::timed_event::fire() -> void
{
    if (should_fire()) {
        m_callback();
        if (m_trigger != repeats) {
            m_trigger = never;
        }
        else {
            m_date = rtc::clock::global().current();
        }
    }
}