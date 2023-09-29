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

#include <libKestrel/ui/imgui/tabbar.hpp>

// MARK: - Construction

kestrel::ui::imgui::tabbar::tabbar(luabridge::LuaRef tabs)
    : widget(), m_tabs(tabs), m_action(nullptr)
{
}

kestrel::ui::imgui::tabbar_item::tabbar_item(const std::string &title)
    : m_title(title)
{
}

// MARK: - Drawing

auto kestrel::ui::imgui::tabbar::draw() -> void
{
    if (has_position()) {
        ImGui::SetCursorPos({ position().x(), position().y() });
    }

    if (has_size()) {
        ImGui::SetNextItemWidth(this->size().width());
    }

    if (ImGui::BeginTabBar(identifier_string(), ImGuiTabBarFlags_FittingPolicyScroll | ImGuiTabBarFlags_TabListPopupButton | ImGuiTabBarFlags_NoCloseWithMiddleMouseButton)) {
        auto tab_count = m_tabs.length();
        for (auto i = 1; i <= tab_count; ++i) {
            const char *name = "<undefined>";
            if (lua::ref_isa<tabbar_item>(m_tabs[i])) {
                name = m_tabs[i].cast<tabbar_item::lua_reference>()->title_ref().c_str();
            }

            if (ImGui::BeginTabItem(name)) {
                if (m_selected != i && m_action.state() && m_action.isFunction()) {
                    m_action();
                }
                m_selected = i;

                m_tabs[i].cast<tabbar_item::lua_reference>()->draw();
                ImGui::EndTabItem();
            }
        }

        ImGui::EndTabBar();
    }
}

auto kestrel::ui::imgui::tabbar_item::draw() -> void
{
    m_contents.draw();
}

// MARK: - Operations

auto kestrel::ui::imgui::tabbar_item::add_widget(luabridge::LuaRef widget) -> void
{
    m_contents.add_widget(widget);
}