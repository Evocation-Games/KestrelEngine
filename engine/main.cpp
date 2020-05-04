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

#include "scripting/lua/lua.hpp"
#include "game/scenario_loader.hpp"
#include "app/environment.hpp"

using namespace kestrel;

auto main(int argc, const char **argv) -> int
{
    // Do initial setup and configuration for Kestrel.
    lua::state::global().prepare();

    if (argc > 1) {
        // We're potentially injecting a scenario to play.
        // First argument is the GameCore and the second is the Scenario Data Files.
        game::load_scenario(argv[2]);
        game::load_game_core(argv[1]);
    }
    else {
        // Built-in scenario
        game::load_scenario();
        game::load_game_core();
    }

    // Build an environment and launch it.
    return app::environment::global().start(argc, argv);
}