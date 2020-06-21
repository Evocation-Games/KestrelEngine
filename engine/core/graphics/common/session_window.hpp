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

#if !defined(KESTREL_SESSION_WINDOW_HPP)
#define KESTREL_SESSION_WINDOW_HPP

#include <memory>
#include <chrono>
#include <string>
#include "core/environment.hpp"

namespace graphics
{
    using session_clock = std::chrono::steady_clock;
    using ms = std::chrono::milliseconds;
    using seconds = std::chrono::duration<float>;
    using time_point = std::chrono::time_point<session_clock, seconds>;

    /**
     * The session window represents the main game/content window of the application
     * session. This class is intended to be subclassed for an appropriate graphics
     * driver such as OpenGL, Metal, etc...
     */
    class session_window
    {
    protected:
        bool m_alive { false };
        double m_time { 0.0 };
        double m_delta { 0.01 };
        time_point m_current_time;
        double m_accumulator { 0.0 };
        std::weak_ptr<environment> m_environment;

    public:
        session_window(std::shared_ptr<environment> env);

        auto is_running() const -> bool;

        auto tick() -> void;
        virtual auto update() -> void;
        virtual auto render() -> void;

    };
}

#endif //KESTREL_SESSION_WINDOW_HPP
