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

#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>

static std::uint32_t s_animator_id = 1;

namespace kestrel::renderer
{
    class lua_api(Animator, Available_0_8) animator
    {
    public:
        has_constructable_lua_api(animator);

        animator() = default;

        static auto entity_frame_animator(double duration, double delay, std::uint32_t frame_count, std::uint32_t start_frame) -> lua_reference;

        lua_function(start, Available_0_8) auto start() -> void;
        lua_function(pause, Available_0_8) auto pause() -> void;
        lua_function(reset, Available_0_8) auto reset() -> void;

        lua_function(advance, Available_0_8) auto advance(double delta) -> void;
        lua_setter(currentTime, Available_0_8) auto set_time(double time) -> void;
        lua_setter(currentFrame, Available_0_8) auto set_frame(std::uint32_t frame) -> void;

        lua_function(useCustomFrameCalculation, Available_0_8) auto set_custom_calculation(const luabridge::LuaRef& calculation) -> void;

        lua_getter(currentTime, Available_0_8) [[nodiscard]] auto time() const -> double;
        lua_getter(currentFrame, Available_0_8) [[nodiscard]] auto frame() const -> std::uint32_t;
        lua_getter(delay, Available_0_8) [[nodiscard]] auto delay() const -> double;

    private:
        std::uint32_t m_id { s_animator_id++ };

        bool m_paused { false };
        std::uint32_t m_start_frame { 0 };
        std::uint32_t m_frame_count { 0 };

        double m_time { 0 };
        double m_total_duration { 0 };
        double m_delay { 0 };

        std::uint32_t m_frame { 0 };

        luabridge::LuaRef m_custom_calculation { nullptr };

        auto calculate_frame() -> void;
    };
}
