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

#include "core/ui/imgui/dockspace.hpp"
#include <cmath>

static bool s_dockspace_enabled = false;

// MARK: - Lua

auto ui::imgui::dockspace::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("ImGui")
            .addFunction("startDockspace", &dockspace::start_dockspace)
            .addFunction("endDockspace", &dockspace::end_dockspace)
        .endNamespace();
}

// MARK: - Drawing

auto ui::imgui::dockspace::draw() -> void
{
    if (s_dockspace_enabled) {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("KestrelDockspace", &m_open, m_window_flags);
        ImGui::PopStyleVar(3);

        ImGuiID dockspace_id = ImGui::GetID("KestrelDockspace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), m_flags);

        for (auto window : m_windows) {
            window->draw();
        }

//        static bool demo = true;
//        if (demo) {
//            ImGui::ShowDemoWindow(&demo);
//        }

        ImGui::End();
    }
    else {
        for (auto window : m_windows) {
            window->draw();
        }
    }
}

// MARK: - Operations

auto ui::imgui::dockspace::start_dockspace() -> void
{
    s_dockspace_enabled = true;
}

auto ui::imgui::dockspace::end_dockspace() -> void
{
    s_dockspace_enabled = false;
}

auto ui::imgui::dockspace::add_window(const window::lua_reference& window) -> void
{
    m_windows.emplace_back(window);
}

auto ui::imgui::dockspace::erase() -> void
{
    for (int i = static_cast<int>(m_windows.size()) - 1; i >= 0; --i) {
        auto& window = m_windows[i];
        window.reset();
    }
    m_windows.empty();
}

// MARK: - Events

auto ui::imgui::dockspace::receive_event(const event &e) -> void
{
    ImGuiIO& io = ImGui::GetIO();

    if (e.is_mouse_event()) {
        io.MousePosPrev = io.MousePos;
        io.MousePos = ImVec2(static_cast<float>(e.location().x), static_cast<float>(e.location().y));

        if (e.has(event::lmb_down)) {
            io.MouseDown[0] = true;
            io.MouseClickedPos[0] = io.MousePos;
        }
        else if (e.has(event::lmb_up)) {
            io.MouseDown[0] = false;
        }

        if (e.has(event::mouse_drag)) {
            io.MouseDragThreshold = 2;
            io.MouseDragMaxDistanceAbs[0] = {io.MousePos.x - io.MousePosPrev.x, io.MousePos.y - io.MousePosPrev.y};
            io.MouseDragMaxDistanceSqr[0] = std::sqrt(std::pow(io.MouseDragMaxDistanceAbs[0].x, 2) + std::pow(io.MouseDragMaxDistanceAbs[0].y, 2));
        }

        if (e.has(event::mmb_down)) {
            io.MouseDown[1] = true;
            io.MouseClickedPos[1] = io.MousePos;
        }
        else if (e.has(event::mmb_up)) {
            io.MouseDown[1] = false;
        }

        if (e.has(event::rmb_down)) {
            io.MouseDown[2] = true;
            io.MouseClickedPos[2] = io.MousePos;
        }
        else if (e.has(event::rmb_up)) {
            io.MouseDown[2] = false;
        }
    }

    if (e.is_key_event()) {
        auto k = map_key(e.key());
        if (e.has(event::key_down) || e.has(event::key_repeat)) {
            io.KeysDown[k] = true;
            io.AddKeyEvent(k, true);
        }
        else if (e.has(event::key_up)) {
            io.KeysDown[map_key(e.key())] = false;
            io.AddKeyEvent(k, false);
        }

        if (e.has(event::key_typed)) {
            io.AddInputCharacter(e.character());
        }
    }
}


// MARK: - Key Mappings

auto ui::imgui::dockspace::map_key(hid::key k) -> ImGuiKey
{
    switch (k) {
        case hid::unknown:          return ImGuiKey_0;
        case hid::enter:            return ImGuiKey_Enter;
        case hid::tab:              return ImGuiKey_Tab;
        case hid::backspace:        return ImGuiKey_Backspace;
        case hid::escape:           return ImGuiKey_Escape;
        case hid::left:             return ImGuiKey_LeftArrow;
        case hid::right:            return ImGuiKey_RightArrow;
        case hid::down:             return ImGuiKey_DownArrow;
        case hid::up:               return ImGuiKey_UpArrow;
        case hid::a:                return ImGuiKey_A;
        case hid::s:                return ImGuiKey_S;
        case hid::d:                return ImGuiKey_D;
        case hid::f:                return ImGuiKey_F;
        case hid::h:                return ImGuiKey_H;
        case hid::g:                return ImGuiKey_G;
        case hid::z:                return ImGuiKey_Z;
        case hid::x:                return ImGuiKey_X;
        case hid::c:                return ImGuiKey_C;
        case hid::v:                return ImGuiKey_V;
        case hid::b:                return ImGuiKey_B;
        case hid::q:                return ImGuiKey_Q;
        case hid::w:                return ImGuiKey_W;
        case hid::e:                return ImGuiKey_E;
        case hid::r:                return ImGuiKey_R;
        case hid::y:                return ImGuiKey_Y;
        case hid::t:                return ImGuiKey_T;
        case hid::o:                return ImGuiKey_O;
        case hid::u:                return ImGuiKey_U;
        case hid::i:                return ImGuiKey_I;
        case hid::p:                return ImGuiKey_P;
        case hid::l:                return ImGuiKey_L;
        case hid::j:                return ImGuiKey_J;
        case hid::k:                return ImGuiKey_K;
        case hid::n:                return ImGuiKey_N;
        case hid::m:                return ImGuiKey_M;
        case hid::kp_0:             return ImGuiKey_Keypad0;
        case hid::kp_1:             return ImGuiKey_Keypad1;
        case hid::kp_2:             return ImGuiKey_Keypad2;
        case hid::kp_3:             return ImGuiKey_Keypad3;
        case hid::kp_4:             return ImGuiKey_Keypad4;
        case hid::kp_5:             return ImGuiKey_Keypad5;
        case hid::kp_6:             return ImGuiKey_Keypad6;
        case hid::kp_7:             return ImGuiKey_Keypad7;
        case hid::kp_8:             return ImGuiKey_Keypad8;
        case hid::kp_9:             return ImGuiKey_Keypad9;
        case hid::space:            return ImGuiKey_Space;
        case hid::period:           return ImGuiKey_Period;
        case hid::comma:            return ImGuiKey_Comma;
        case hid::semi_colon:       return ImGuiKey_Semicolon;
        case hid::apostrophe:       return ImGuiKey_Apostrophe;
        case hid::left_bracket:     return ImGuiKey_LeftBracket;
        case hid::right_bracket:    return ImGuiKey_RightBracket;
        case hid::grave_accent:     return ImGuiKey_GraveAccent;
        case hid::backslash:        return ImGuiKey_Backslash;
        case hid::slash:            return ImGuiKey_Slash;
        case hid::minus:            return ImGuiKey_Minus;
        case hid::equal:            return ImGuiKey_Equal;
        case hid::f1:               return ImGuiKey_F1;
        case hid::f2:               return ImGuiKey_F2;
        case hid::f3:               return ImGuiKey_F3;
        case hid::f4:               return ImGuiKey_F4;
        case hid::f5:               return ImGuiKey_F5;
        case hid::f6:               return ImGuiKey_F6;
        case hid::f7:               return ImGuiKey_F7;
        case hid::f8:               return ImGuiKey_F8;
        case hid::f9:               return ImGuiKey_F9;
        case hid::f10:              return ImGuiKey_F10;
        case hid::f11:              return ImGuiKey_F11;
        case hid::f12:              return ImGuiKey_F12;
        case hid::num0:             return ImGuiKey_0;
        case hid::num1:             return ImGuiKey_1;
        case hid::num2:             return ImGuiKey_2;
        case hid::num3:             return ImGuiKey_3;
        case hid::num4:             return ImGuiKey_4;
        case hid::num5:             return ImGuiKey_5;
        case hid::num6:             return ImGuiKey_6;
        case hid::num7:             return ImGuiKey_7;
        case hid::num8:             return ImGuiKey_8;
        case hid::num9:             return ImGuiKey_9;
        case hid::world_1:          return ImGuiKey_None;
        case hid::world_2:          return ImGuiKey_None;
        case hid::insert:           return ImGuiKey_Insert;
        case hid::del:              return ImGuiKey_Delete;
        case hid::page_up:          return ImGuiKey_PageUp;
        case hid::page_down:        return ImGuiKey_PageDown;
        case hid::home:             return ImGuiKey_Home;
        case hid::end:              return ImGuiKey_End;
        case hid::caps_lock:        return ImGuiKey_CapsLock;
        case hid::scroll_lock:      return ImGuiKey_ScrollLock;
        case hid::num_lock:         return ImGuiKey_NumLock;
        case hid::print_screen:     return ImGuiKey_PrintScreen;
        case hid::pause:            return ImGuiKey_Pause;
        case hid::f13:              return ImGuiKey_None;
        case hid::f14:              return ImGuiKey_None;
        case hid::f15:              return ImGuiKey_None;
        case hid::f16:              return ImGuiKey_None;
        case hid::f17:              return ImGuiKey_None;
        case hid::f18:              return ImGuiKey_None;
        case hid::f19:              return ImGuiKey_None;
        case hid::f20:              return ImGuiKey_None;
        case hid::f21:              return ImGuiKey_None;
        case hid::f22:              return ImGuiKey_None;
        case hid::f23:              return ImGuiKey_None;
        case hid::f24:              return ImGuiKey_None;
        case hid::f25:              return ImGuiKey_None;
        case hid::kp_decimal:       return ImGuiKey_KeypadDecimal;
        case hid::kp_divide:        return ImGuiKey_KeypadDivide;
        case hid::kp_multiply:      return ImGuiKey_KeypadMultiply;
        case hid::kp_subtract:      return ImGuiKey_KeypadSubtract;
        case hid::kp_add:           return ImGuiKey_KeypadAdd;
        case hid::kp_enter:         return ImGuiKey_KeypadEnter;
        case hid::kp_equal:         return ImGuiKey_KeypadEqual;
        case hid::left_shift:       return ImGuiKey_LeftShift;
        case hid::left_control:     return ImGuiKey_LeftCtrl;
        case hid::left_alt:         return ImGuiKey_LeftAlt;
        case hid::left_super:       return ImGuiKey_LeftSuper;
        case hid::right_shift:      return ImGuiKey_RightShift;
        case hid::right_control:    return ImGuiKey_RightCtrl;
        case hid::right_alt:        return ImGuiKey_RightAlt;
        case hid::right_super:      return ImGuiKey_RightSuper;
        case hid::menu:             return ImGuiKey_Menu;
    }
}
