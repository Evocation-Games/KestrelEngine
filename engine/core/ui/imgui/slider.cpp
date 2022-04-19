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

#include "core/ui/imgui/slider.hpp"

// MARK: - Lua

auto ui::imgui::slider::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("ImGui")
            .beginClass<slider>("Slider")
                .addConstructor<auto(*)(int32_t, int32_t, int32_t)->void, lua_reference>()
                .addProperty("minimumValue", &slider::minimum, &slider::set_minimum)
                .addProperty("maximumValue", &slider::maximum, &slider::set_maximum)
                .addProperty("value", &slider::value, &slider::set_value)
                .addFunction("valueChanged", &slider::set_value_changed)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

ui::imgui::slider::slider(int32_t value, int32_t min, int32_t max)
    : widget(), m_value(value), m_min(min), m_max(max), m_changed(nullptr)
{
}

// MARK: - Drawing

auto ui::imgui::slider::draw() -> void
{
    if (ImGui::SliderInt(identifier_string(), &m_value, m_min, m_max, "%d", ImGuiSliderFlags_None)) {
        if (!m_changed.isNil() && m_changed.isFunction()) {
            m_changed();
        }
    }

}

// MARK: - Accessors

auto ui::imgui::slider::set_value_changed(luabridge::LuaRef callback) -> void
{
    m_changed = callback;
}