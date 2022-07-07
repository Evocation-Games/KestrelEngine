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

#include "core/task/async_queue.hpp"
#include "renderer/common/renderer.hpp"

// MARK: - Lua

auto task::async_queue::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginClass<async_queue>("AsyncTaskQueue")
            .addProperty("paused", &async_queue::paused, &async_queue::set_paused)
            .addProperty("hasTasksAvailable", &async_queue::has_tasks_available)
            .addProperty("tasksRemaining", &async_queue::tasks_remaining)
            .addFunction("enqueueTask", &async_queue::enqueue_task)
        .endClass();
}

// MARK: - Task Management

auto task::async_queue::enqueue_task(const std::string& name, const luabridge::LuaRef& task) -> void
{
    struct task t {
        .name = name,
        .block = task
    };
    m_tasks.emplace_back(std::move(t));
}

// MARK: - Accessors

auto task::async_queue::paused() const -> bool
{
    return m_paused;
}

auto task::async_queue::set_paused(bool paused) -> void
{
    m_paused = paused;
}

auto task::async_queue::has_tasks_available() const -> bool
{
    return !m_tasks.empty();
}

auto task::async_queue::tasks_remaining() const -> std::size_t
{
    return m_tasks.size();
}

// MARK: - Execution

inline auto time_available_for_task() -> bool
{
    return renderer::target_frame_time() > renderer::time_since_last_frame();
}

auto task::async_queue::execute_tasks() -> void
{
    while (time_available_for_task() && has_tasks_available()) {
        execute_next_task();
    }
}

auto task::async_queue::execute_next_task() -> void
{
    auto task = m_tasks.front();
    m_tasks.erase(m_tasks.begin());

    std::cout << "Executing Task: " << task.name << std::endl;

    if (task.block.state() && task.block.isFunction()) {
        task.block();
    }
}