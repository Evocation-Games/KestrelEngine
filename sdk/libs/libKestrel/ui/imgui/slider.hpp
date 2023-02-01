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
#include <libKestrel/ui/imgui/widget.hpp>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>

namespace kestrel::ui::imgui
{
    class lua_api(ImGui.Slider, Available_0_8) slider : public widget
    {
    public:
        has_constructable_lua_api(slider);

        lua_constructor(Available_0_8) slider(std::int32_t value, std::int32_t min, std::int32_t max);

        lua_getter(minimumValue, Available_0_8) [[nodiscard]] auto minimum() const -> std::int32_t { return m_min; }
        lua_getter(maximumValue, Available_0_8) [[nodiscard]] auto maximum() const -> std::int32_t { return m_max; }
        lua_getter(value, Available_0_8) [[nodiscard]] auto value() const -> std::int32_t { return m_value; }

        lua_setter(minimumValue, Available_0_8) auto set_minimum(std::int32_t min) -> void { m_min = min; }
        lua_setter(maximumValue, Available_0_8) auto set_maximum(std::int32_t max) -> void { m_max = max; }
        lua_setter(value, Available_0_8) auto set_value(std::int32_t value) -> void { m_value = value; }

        lua_function(valueChanged, Available_0_8) auto set_value_changed(luabridge::LuaRef callback) -> void;

        lua_getter(position, Available_0_8) auto position() const -> math::point override { return widget::position(); }
        lua_setter(position, Available_0_8) auto set_position(const math::point &position) -> void override { widget::set_position(position); }

        lua_getter(size, Available_0_8) auto size() const -> math::size override { return widget::size(); }
        lua_setter(size, Available_0_8) auto set_size(const math::size &size) -> void override { widget::set_size(size); }

        auto draw() -> void override;

    private:
        std::int32_t m_min { 0 };
        std::int32_t m_max { 100 };
        std::int32_t m_value { 0 };
        luabridge::LuaRef m_changed;
    };
}
