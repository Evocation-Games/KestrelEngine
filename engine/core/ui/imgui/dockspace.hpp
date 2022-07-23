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

#include <vector>
#include <imgui/imgui.h>
#include "core/ui/imgui/window.hpp"
#include "core/ui/imgui/diagnostics.hpp"
#include "core/ui/imgui/console.hpp"
#include "core/event/event.hpp"
#include "scripting/state.hpp"
#include "util/hint.hpp"

namespace ui::imgui
{
    class dockspace : public scripting::lua::object
    {
    public:
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        dockspace();

        auto draw() -> void;

        auto add_window(const window::lua_reference& window) -> void;
        auto receive_event(const event& e) -> void;

        auto erase() -> void;

        lua_api static auto start_dockspace() -> void;
        lua_api static auto end_dockspace() -> void;

        lua_api static auto start_console() -> void;
        lua_api static auto stop_console() -> void;

    private:
        bool m_open { true };
        ImGuiDockNodeFlags m_flags { ImGuiDockNodeFlags_None };
        ImGuiWindowFlags m_window_flags {
            ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus
        };
        std::vector<window::lua_reference> m_windows;
        diagnostics::lua_reference m_diagnostics;

    private:
        auto map_key(hid::key k) -> ImGuiKey;
        auto internal_draw() -> void;

    };
}


