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

#include <chrono>

namespace kestrel::rtc
{
    /**
     * The `kestrel::rtc::clock` class is responsible for managing real time events.
     */
    class clock
    {
    public:
        typedef std::chrono::steady_clock::time_point time;
        typedef std::chrono::duration<double> duration;

        clock(const clock&) = delete;
        clock &operator=(const clock &) = delete;
        clock(clock &&) = delete;
        clock &operator=(clock &&) = delete;

        static auto &global()
        {
            static clock instance;
            return instance;
        }

        auto tick() -> void;

        [[nodiscard]] auto current() const -> time;

        [[nodiscard]] auto since(time t) const -> duration;
        [[nodiscard]] auto until(time t) const -> duration;

    private:
        time m_current_time;
        uint64_t m_current_ticks { 0 };
        time m_last_time;
        uint64_t m_last_ticks { 0 };

        clock();
    };

};
