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

#if !defined(KESTREL_TIMED_EVENT_HPP)
#define KESTREL_TIMED_EVENT_HPP

#include <functional>
#include "core/clock/clock.hpp"
#include "scripting/state.hpp"

namespace rtc
{

    struct timed_event
    {
    public:
        enum trigger { never, after_date, after_duration, repeats };

    private:
        rtc::clock::time m_date { rtc::clock::global().current() };
        double m_period { 0 };
        enum timed_event::trigger m_trigger { never };
        luabridge::LuaRef m_callback;

    public:
        timed_event(const rtc::clock::time& after, const luabridge::LuaRef& callback);
        timed_event(const double& period, const luabridge::LuaRef& callback, const bool& repeats = false);

        auto dead() const -> bool;
        auto should_fire() const -> bool;

        auto fire() -> void;
    };

}

#endif //KESTREL_TIMED_EVENT_HPP
