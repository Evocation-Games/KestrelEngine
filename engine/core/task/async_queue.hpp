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

#include <thread>
#include "scripting/state.hpp"
#include "util/hint.hpp"

namespace task
{

    class async_queue
    {
    public:
        typedef luabridge::RefCountedPtr<async_queue> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        async_queue() = default;

        lua_api auto enqueue_task(const std::string& name, const luabridge::LuaRef& task) -> void;

        [[nodiscard]] lua_api auto has_tasks_available() const -> bool;
        [[nodiscard]] lua_api auto tasks_remaining() const -> std::size_t;

        auto execute_tasks() -> void;
        auto execute_next_task() -> void;

        [[nodiscard]] lua_api auto paused() const -> bool;
        lua_api auto set_paused(bool paused) -> void;

    private:
        struct task {
            std::string name;
            luabridge::LuaRef block;
        };

        bool m_paused { false };
        std::vector<task> m_tasks;

    };

}

