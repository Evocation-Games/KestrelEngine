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

#include "renderer/common/animator.hpp"
#include <iostream>

// MARK: - Lua

auto renderer::animator::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginClass<animator>("Animator")
            .addStaticFunction("entityFrameAnimator", &entity_frame_animator)
            .addProperty("currentTime", &animator::time, &animator::set_time)
            .addProperty("currentFrame", &animator::frame, &animator::set_frame)
            .addProperty("delay", &animator::delay)
            .addFunction("useCustomFrameCalculation", &animator::set_custom_calculation)
            .addFunction("advance", &animator::advance)
            .addFunction("start", &animator::start)
            .addFunction("pause", &animator::pause)
            .addFunction("reset", &animator::reset)
        .endClass();
}

// MARK: - Construction

auto renderer::animator::entity_frame_animator(double duration, double delay, uint32_t frame_count, uint32_t start_frame) -> lua_reference
{
    lua_reference animator { new class animator() };

    animator->m_total_duration = duration;
    animator->m_start_frame = start_frame;
    animator->m_frame_count = frame_count;
    animator->m_delay = delay;

    animator->calculate_frame();

    // TODO: Add to the current GameScene.

    return animator;
}

// MARK: - Accessors

auto renderer::animator::frame() const -> uint32_t
{
    return m_frame;
}

auto renderer::animator::time() const -> double
{
    return m_time;
}

auto renderer::animator::delay() const -> double
{
    return m_delay;
}

// MARK: - Activation

auto renderer::animator::start() -> void
{
    m_paused = false;
}

auto renderer::animator::pause() -> void
{
    m_paused = true;
}

auto renderer::animator::reset() -> void
{
    pause();
    m_time = 0;
    m_frame = m_start_frame;
}

// MARK: - Computation

auto renderer::animator::calculate_frame() -> void
{
    auto time = m_time - m_delay;
    if (time < 0) {
        m_frame = m_start_frame;
        return;
    }

    if (m_custom_calculation.state() && m_custom_calculation.isFunction()) {
        m_frame = m_custom_calculation(time, m_total_duration, m_frame_count, m_frame);
        return;
    }

    auto percentage = (1.0 / m_total_duration) * time;
    auto frame = static_cast<int64_t>(m_frame_count * percentage);
    m_frame = (frame >= 0) ? m_start_frame + frame : m_start_frame;
}

auto renderer::animator::set_custom_calculation(const luabridge::LuaRef &calculation) -> void
{
    m_custom_calculation = calculation;
}

// MARK: - Timing

auto renderer::animator::advance(double delta) -> void
{
    if (!m_paused) {
        set_time(m_time + delta);
    }
}

auto renderer::animator::set_time(double time) -> void
{
    m_time = time;
    calculate_frame();
}

auto renderer::animator::set_frame(uint32_t frame) -> void
{
    m_frame = frame;
}