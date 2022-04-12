// Copyright (c) 2022 Tom Hancocks
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

#include <memory>
#include <chrono>
#include <string>
#include <vector>
#include "math/size.hpp"
#include "core/ui/scene.hpp"
#include "core/dev/console.hpp"
#include "scripting/script.hpp"
#include "core/ui/game_scene.hpp"

using session_clock = std::chrono::steady_clock;
using ms = std::chrono::milliseconds;
using seconds = std::chrono::duration<float>;
using time_point = std::chrono::time_point<session_clock, seconds>;

namespace ui
{

    class session : public std::enable_shared_from_this<session>
    {
    public:
        session();

        [[nodiscard]] inline auto is_running() const -> bool { return m_alive; }

        [[nodiscard]] auto current_scene() const -> game_scene::lua_reference;
        auto new_scene(const std::string& name, const scripting::lua::script& script) -> std::shared_ptr<scene>;
        auto present_scene(const game_scene::lua_reference& scene) -> void;
        auto pop_scene() -> void;

        auto set_title(const std::string& title) -> void;
        auto set_size(const math::size& size) -> void;
        [[nodiscard]] auto size() const -> math::size;

        auto tick() -> void;
        auto receive_event(const event& e) -> void;

        auto console() -> dev::console& { return m_console; }

    private:
        bool m_alive { true };
        double m_time { 0.0 };
        time_point m_current_time;
        time_point m_cache_purge_time;
        std::vector<game_scene::lua_reference> m_scenes;
        dev::console m_console;
    };

}
