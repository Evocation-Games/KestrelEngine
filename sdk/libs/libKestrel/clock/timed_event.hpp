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

#pragma once

#include <functional>
#include <libKestrel/clock/clock.hpp>
#include <libKestrel/lua/runtime/runtime.hpp>

namespace kestrel::rtc
{
    /**
     * The `kestrel::rtc::timed_event` structure keeps tracked of real time events that either fire once after
     * a specific time period, or repeatedly fire every _n_ seconds.
     */
    struct timed_event
    {
    public:
        /**
         * The mechanism by which the event will fire.
         * - never          The event will never fire.
         * - after_date     The event will fire after the specified date is reached.
         * - after_duration The event will fire after the specified duration has elapsed.
         * - repeats        The event will fire every _n_ seconds.
         */
        enum trigger { never, after_date, after_duration, repeats };

        /**
         * Construct a new timed event that fires after a specified time. The specified callback
         * will be executed once the timed event is fired.
         * @param after     The time after which the event will be fired.
         * @param callback  The callback to execute when the event fires.
         */
        timed_event(const kestrel::rtc::clock::time& after, const luabridge::LuaRef& callback);

        /**
         * Construct a new timed event that fires after _period_ seconds. The specified callback
         * will be executed once the timed event is fired.
         * @param period    The number of seconds after which the event will be fired.
         * @param callback  The callback to execute when the event fires.
         * @param repeats   Should the event repeat?
         */
        timed_event(const double& period, const luabridge::LuaRef& callback, const bool& repeats = false);

        /**
         * Will the event ever fire again?
         * @return A boolean indicating if the event will never fire again.
         */
        [[nodiscard]] auto dead() const -> bool;

        /**
         * Is the event due to fire?
         * @return A boolean indicating if the event is due to fire.
         */
        [[nodiscard]] auto should_fire() const -> bool;

        /**
         * Fire the event now.
         */
        auto fire() -> void;

    private:
        kestrel::rtc::clock::time m_date { kestrel::rtc::clock::global().current() };
        double m_period { 0 };
        enum timed_event::trigger m_trigger { never };
        luabridge::LuaRef m_callback;
    };

}
