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

#include "core/environment.hpp"
#include "core/graphics/opengl/opengl_session_window.hpp"

// MARK: - Construction

environment::environment(int argc, const char **argv)
{
    for (auto i = 1; i < argc; ++i) {
        m_options.emplace_back(std::string(argv[i]));
    }
}

// MARK: - Run Loop

auto environment::launch() -> int
{
    // Determine which graphics mode we are running in, and instantiate the correct session_window
    // subclass.
    // TODO: Add in alternate modes as they are implemented, and bind to appropriate platforms.
    m_game_window = std::make_shared<graphics::opengl::session_window>(shared_from_this());

    // Enter the main run loop, keep calling tick on the session window until such time as it is no
    // longer in existence or alive.
    while (m_game_window && m_game_window->is_running()) {
        m_game_window->tick();
    }

    return m_status;
}