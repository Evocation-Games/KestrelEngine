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

#include "core/environment.hpp"
#include "core/ui/imgui/window.hpp"
#include "core/ui/imgui/container.hpp"

// MARK: - Lua

auto ui::imgui::window::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("ImGui")
            .beginClass<window>("Window")
//                .addConstructor<auto(*)(const std::string&, const math::size&)->void, lua_reference>()
                .addStaticFunction("create", &window::create)
                .addProperty("title", &window::title, &window::set_title)
                .addProperty("size", &window::size, &window::set_size)
                .addFunction("show", &window::show)
                .addFunction("hide", &window::hide)
                .addFunction("close", &window::close)
                .addFunction("addWidget", &window::add_widget)
                .addFunction("center", &window::center)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

ui::imgui::window::window(const std::string& title, const math::size& size)
    : m_title(title), m_size(size), m_shown(false)
{
}

auto ui::imgui::window::create(const std::string &title, const math::size &size) -> lua_reference
{
    lua_reference ref { new window(title, size) };

    if (auto env = environment::active_environment().lock()) {
        env->imgui_dockspace().add_window(ref);
    }

    return ref;
}

// MARK: -

auto ui::imgui::window::show() -> void
{
    m_shown = true;
}

auto ui::imgui::window::hide() -> void
{
    m_shown = false;
}

auto ui::imgui::window::close() -> void
{
    m_closed = true;
}

auto ui::imgui::window::center() -> void
{
    if (auto env = environment::active_environment().lock()) {
        auto session_size = env->session()->size();
        m_position = math::point((session_size.width - m_size.width) / 2.f, (session_size.height - m_size.height) / 2.f);
        m_dirty_position = true;
    }
}

// MARK: - Accessors

auto ui::imgui::window::title() const -> std::string
{
    return m_title;
}

auto ui::imgui::window::set_title(const std::string &title) -> void
{
    m_title = title;
}

auto ui::imgui::window::size() const -> math::size
{
    return m_size;
}

auto ui::imgui::window::set_size(const math::size &size) -> void
{
    m_size = size;
    m_dirty_size = true;
}

// MARK: - Drawing

auto ui::imgui::window::draw() -> void
{
    if (m_closed) {
        return;
    }

    if (m_dirty_size) {
        ImGui::SetNextWindowSize(ImVec2(m_size.width, m_size.height));
        m_dirty_size = false;
    }

    if (m_dirty_position) {
        ImGui::SetNextWindowPos(ImVec2(m_position.x, m_position.y));
        m_dirty_position = false;
    }

    if (!ImGui::Begin(identified_title().c_str(), &m_shown)) {
        ImGui::End();

        return;
    }

    m_contents.draw();

    ImGui::End();
}

// MARK: - Widgets

auto ui::imgui::window::add_widget(luabridge::LuaRef widget) -> void
{
    m_contents.add_widget(widget);
}