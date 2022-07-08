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

#include "scripting/state.hpp"
#include "util/hint.hpp"

static uint32_t s_animator_id = 1;

namespace renderer
{
    class animator : public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<animator> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        animator() = default;

        static auto entity_frame_animator(double duration, double delay, uint32_t frame_count, uint32_t start_frame) -> lua_reference;

        lua_api auto start() -> void;
        lua_api auto pause() -> void;
        lua_api auto reset() -> void;

        lua_api auto advance(double delta) -> void;
        lua_api auto set_time(double time) -> void;
        lua_api auto set_frame(uint32_t frame) -> void;

        lua_api auto set_custom_calculation(const luabridge::LuaRef& calculation) -> void;

        [[nodiscard]] lua_api auto time() const -> double;
        [[nodiscard]] lua_api auto frame() const -> uint32_t;
        [[nodiscard]] lua_api auto delay() const -> double;

    private:
        uint32_t m_id { s_animator_id++ };

        bool m_paused { false };
        uint32_t m_start_frame { 0 };
        uint32_t m_frame_count { 0 };

        double m_time { 0 };
        double m_total_duration { 0 };
        double m_delay { 0 };

        uint32_t m_frame { 0 };

        luabridge::LuaRef m_custom_calculation { nullptr };

        auto calculate_frame() -> void;
    };
}
