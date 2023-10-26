// Copyright (c) 2023 Tom Hancocks
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

#if defined(KESTREL_PROFILER_ENABLED) || 1

#include <chrono>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

namespace foundation::profiler
{
    struct scope_timer
    {
        explicit scope_timer(const std::string& name) : m_name(name) {}
        ~scope_timer() { report(); }

    private:
        auto report() const -> void
        {
            auto duration = std::chrono::high_resolution_clock::now() - m_start;
            auto ns = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count();
            auto us = (float)std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
            auto ms = (float)std::chrono::duration_cast<std::chrono::microseconds>(duration).count() / 1000.;

            if (ns < 1000) {
                std::cout << std::setprecision(3) << std::fixed
                          << "[PROFILE] '" << m_name << "' took " << ns << "ns"
                          << std::endl;
            }
            else if (us < 1000) {
                std::cout << std::setprecision(3) << std::fixed
                          << "[PROFILE] '" << m_name << "' took " << us << "µs"
                          << std::endl;
            }
            else {
                std::cout << std::setprecision(3) << std::fixed
                          << "[PROFILE] '" << m_name << "' took " << ms << "ms"
                          << std::endl;
            }
        }

    private:
        std::string m_name;
        std::chrono::high_resolution_clock::time_point m_start { std::chrono::high_resolution_clock::now() };
    };

    static auto measure(const std::string& name, std::function<auto()->void> fn) -> void
    {
        scope_timer t(name);
        fn();
    }
}

#define __KESTREL_PROFILE_LINE(name, line)  \
    const auto profiler##line = ::foundation::profiler::scope_timer(name)
#define KESTREL_PROFILE_LINE(name, line)    __KESTREL_PROFILE_LINE(name, line)
#define KESTREL_PROFILE_SCOPE(name)         KESTREL_PROFILE_LINE(name, __LINE__)
#define KESTREL_PROFILE_FUNCTION()          KESTREL_PROFILE_SCOPE(__PRETTY_FUNCTION__)

#else
#define KESTREL_PROFILE_LINE(name, line)
#define KESTREL_PROFILE_SCOPE(name)
#define KESTREL_PROFILE_FUNCTION()
#endif