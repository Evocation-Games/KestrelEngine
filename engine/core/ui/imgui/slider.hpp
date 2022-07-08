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

#include <string>
#include <imgui/imgui.h>
#include "core/ui/imgui/widget.hpp"
#include "scripting/state.hpp"
#include "util/hint.hpp"

namespace ui::imgui
{
    class slider : public widget, public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<slider> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        lua_api slider(int32_t value, int32_t min, int32_t max);

        [[nodiscard]] lua_api auto minimum() const -> int32_t { return m_min; }
        [[nodiscard]] lua_api auto maximum() const -> int32_t { return m_max; }
        [[nodiscard]] lua_api auto value() const -> int32_t { return m_value; }

        lua_api auto set_minimum(int32_t min) -> void { m_min = min; }
        lua_api auto set_maximum(int32_t max) -> void { m_max = max; }
        lua_api auto set_value(int32_t value) -> void { m_value = value; }

        lua_api auto set_value_changed(luabridge::LuaRef callback) -> void;

        auto position() const -> math::point override { return widget::position(); }
        auto set_position(const math::point &position) -> void override { widget::set_position(position); }

        auto size() const -> math::size override { return widget::size(); }
        auto set_size(const math::size &size) -> void override { widget::set_size(size); }

        auto draw() -> void override;

    private:
        int32_t m_min { 0 };
        int32_t m_max { 100 };
        int32_t m_value { 0 };
        luabridge::LuaRef m_changed;
    };
}
