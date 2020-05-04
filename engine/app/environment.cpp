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

#include "app/environment.hpp"
#include "foundation/scene/stack.hpp"
#include "libDiamane/platform/application.hpp"

// MARK: - Construction

kestrel::app::environment::environment()
{
}

// MARK: -

auto kestrel::app::environment::set_game_coordinates() const -> void
{
    m_game_window->set_coordinate_space(diamane::ui::window::coordinate_space::game);
}

auto kestrel::app::environment::set_interface_coordinates() const -> void
{
    m_game_window->set_coordinate_space(diamane::ui::window::coordinate_space::ui);
}

// MARK: - Properties

auto kestrel::app::environment::size() const -> kestrel::math::size
{
    return m_game_window->size();
}

// MARK: - Life Cycle

auto kestrel::app::environment::start(int argc, const char **argv) -> int
{
    diamane::platform::application::shared()->run(argc, argv, [&] {
        // We need to get a basic menubar in place (primarily for macOS)
        diamane::platform::application::shared()->set_menubar(diamane::ui::menubar::create());

        // Setup a new game window
        m_game_window = diamane::ui::window::create("Kestrel", diamane::size(1280, 800));
        m_game_window->set_background_color(diamane::gl::color::blackColor());

        // Configure the window.
        m_game_window->center();
        m_game_window->show();
        m_game_window->on_draw([&] {
            // Reset to game coordinates
            set_game_coordinates();

            // Update the clock
            clock::global().tick();

            // Check for the current scene and render it if it exists.
            auto current_scene = scene_stack::global().current();
            current_scene->render();
        });

        // Run the initial script from the game. This is Lua Script #0.
        lua::script(0).execute();
    });
    return 0;
}
