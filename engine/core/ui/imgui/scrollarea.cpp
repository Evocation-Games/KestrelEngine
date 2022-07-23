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

#include "core/ui/imgui/scrollarea.hpp"
// MARK: - Lua

auto ui::imgui::scrollarea::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("ImGui")
            .beginClass<scrollarea>("ScrollArea")
                .addConstructor<auto(*)()->void, lua_reference>()
                .addFunction("addWidget", &scrollarea::add_widget)
            .endClass()
        .endNamespace();
}

// MARK: - Drawing

auto ui::imgui::scrollarea::add_widget(luabridge::LuaRef widget) -> void
{
    m_container.add_widget(widget);
}

// MARK: - Draw

auto ui::imgui::scrollarea::draw() -> void
{
    auto size = ImGui::GetContentRegionAvail();
    ImGui::BeginChild(identifier_string(), size);
    m_container.draw();
    ImGui::EndChild();
}
