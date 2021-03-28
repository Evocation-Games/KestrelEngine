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

#include "core/clock/clock.hpp"

// MARK: - Construction

rtc::clock::clock()
    : m_current_time(std::chrono::steady_clock::now()), m_last_time(m_current_time)
{

}

// MARK: - Accessors

auto rtc::clock::current() const -> time
{
    return m_current_time;
}

// MARK: - Calculations

auto rtc::clock::since(time t) const -> rtc::clock::duration
{
    return m_current_time - t;
}

auto rtc::clock::until(rtc::clock::time t) const -> rtc::clock::duration
{
    return t - m_current_time;
}

// MARK: - Actions

auto rtc::clock::tick() -> void
{
    m_current_ticks++;
    m_current_time = std::chrono::steady_clock::now();

    auto time_diff = since(m_last_time).count();
    if (time_diff >= 1.0) {
        auto tick_diff = m_current_ticks - m_last_ticks;
        m_last_ticks = m_current_ticks;
        m_last_time = m_current_time;
    }
}
