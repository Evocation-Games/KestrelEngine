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

#include "core/ui/imgui/tabbar.hpp"

// MARK: - Lua

auto ui::imgui::tabbar::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("ImGui")
            .beginClass<tabbar>("TabBar")
                .addConstructor<auto(*)(luabridge::LuaRef)->void, lua_reference>()
                .addProperty("tabs", &tabbar::tabs, &tabbar::set_tabs)
                .addProperty("selectedTabs", &tabbar::selected_tab, &tabbar::set_selected_tab)
                .addFunction("setAction", &tabbar::set_action)
            .endClass()
        .endNamespace();

    item::enroll_object_api_in_state(lua);
}

auto ui::imgui::tabbar::item::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("ImGui")
            .beginClass<tabbar::item>("TabItem")
                .addConstructor<auto(*)(const std::string&)->void, lua_reference>()
                .addFunction("addWidget", &tabbar::item::add_widget)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

ui::imgui::tabbar::tabbar(luabridge::LuaRef tabs)
    : widget(), m_tabs(tabs), m_action(nullptr)
{

}

ui::imgui::tabbar::item::item(const std::string &title)
    : m_title(title)
{

}

// MARK: - Drawing

auto ui::imgui::tabbar::draw() -> void
{
    if (ImGui::BeginTabBar(identifier_string(), ImGuiTabBarFlags_FittingPolicyScroll | ImGuiTabBarFlags_TabListPopupButton)) {
        auto tab_count = m_tabs.length();
        for (auto i = 1; i <= tab_count; ++i) {
            const char *name = "<undefined>";
            bool open = (m_selected = i);

            if (scripting::lua::ref_isa<tabbar::item>(m_tabs[i])) {
                name = m_tabs[i].cast<tabbar::item::lua_reference>()->title().c_str();
            }

            if (ImGui::BeginTabItem(name, &open, ImGuiTabItemFlags_None)) {
                m_tabs[i].cast<tabbar::item::lua_reference>()->draw();
                ImGui::EndTabItem();
            }
        }

        ImGui::EndTabBar();
    }
}

auto ui::imgui::tabbar::item::draw() -> void
{
    m_contents.draw();
}

// MARK: - Operations

auto ui::imgui::tabbar::item::add_widget(luabridge::LuaRef widget) -> void
{
    m_contents.add_widget(widget);
}