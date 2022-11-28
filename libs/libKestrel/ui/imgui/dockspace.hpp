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
#include <libKestrel/ui/imgui/window.hpp>
#include <libKestrel/ui/imgui/console.hpp>
#include <libKestrel/event/event.hpp>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>

namespace kestrel::ui::imgui
{
    class lua_api(ImGui.Dockspace, Available_0_8) dockspace
    {
    public:
        luatool_no_implementation
        has_constructable_lua_api(dockspace);

        dockspace() = default;

        auto draw() -> void;

        auto add_window(const window::lua_reference& window) -> void;
        auto remove_window(const window *window) -> void;
        auto receive_event(const event& e) -> void;

        auto erase() -> void;

        lua_function(startDockspace, Available_0_8) static auto start_dockspace() -> void;
        lua_function(endDockspace, Available_0_8) static auto end_dockspace() -> void;

        lua_function(startConsole, Available_0_8) static auto start_console() -> void;
        lua_function(endConsole, Available_0_8) static auto stop_console() -> void;

    private:
        bool m_open { true };
        ImGuiDockNodeFlags m_flags { ImGuiDockNodeFlags_None };
        ImGuiWindowFlags m_window_flags {
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus
        };
        std::vector<window::lua_reference> m_windows;

    private:
        auto map_key(hid::key k) -> ImGuiKey;
        auto internal_draw() -> void;

    };
}


