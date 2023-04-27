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

#include <cmath>
#include <libKestrel/ui/imgui/dockspace.hpp>
#include <libKestrel/ui/imgui/console.hpp>
#include <libKestrel/font/manager.hpp>

static bool s_dockspace_enabled = false;
static kestrel::ui::imgui::console s_console;

// MARK: - Drawing

auto kestrel::ui::imgui::dockspace::internal_draw() -> void
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

        for (const auto& window : m_windows) {
            if (!window.get()) {
                continue;
            }
            window->draw();
        }

        ImGui::End();
    }
    else {
        for (const auto& window : m_windows) {
            window->draw();
        }
    }

    s_console.draw();
}

auto kestrel::ui::imgui::dockspace::draw() -> void
{
    auto default_font = kestrel::font::manager::shared_manager().default_font();
    if (default_font.get()) {
        default_font->push([&] { internal_draw(); });
    }
    else {
        internal_draw();
    }
}

// MARK: - Operations

auto kestrel::ui::imgui::dockspace::start_dockspace() -> void
{
    s_dockspace_enabled = true;
}

auto kestrel::ui::imgui::dockspace::end_dockspace() -> void
{
    s_dockspace_enabled = false;
}

auto kestrel::ui::imgui::dockspace::start_console() -> void
{
    s_console = ui::imgui::console();
}

auto kestrel::ui::imgui::dockspace::stop_console() -> void
{
    s_console.close();
}

auto kestrel::ui::imgui::dockspace::add_window(const window::lua_reference& window) -> void
{
    m_windows.emplace_back(window);
}

auto kestrel::ui::imgui::dockspace::remove_window(const window *window) -> void
{
    for (auto it = m_windows.begin(); it != m_windows.end(); ++it) {
        if (it->get() == window) {
            m_windows.erase(it);
            return;
        }
    }
}

auto kestrel::ui::imgui::dockspace::erase() -> void
{
    for (int i = static_cast<int>(m_windows.size()) - 1; i >= 0; --i) {
        auto& window = m_windows[i];
        window.reset();
    }
    m_windows.clear();
}

// MARK: - Events

auto kestrel::ui::imgui::dockspace::receive_event(const event &e) -> void
{
    ImGuiIO& io = ImGui::GetIO();

    if (e.is_mouse_event()) {
        io.MousePosPrev = io.MousePos;
        io.MousePos = ImVec2(e.location().x(), e.location().y());

        if (e.has(::ui::event::type::lmb_down)) {
            io.MouseDown[0] = true;
            io.MouseClickedPos[0] = io.MousePos;
        }
        else if (e.has(::ui::event::type::lmb_up)) {
            io.MouseDown[0] = false;
        }

        if (e.has(::ui::event::type::mouse_drag)) {
            io.MouseDragThreshold = 2;
            io.MouseDragMaxDistanceAbs[0] = {io.MousePos.x - io.MousePosPrev.x, io.MousePos.y - io.MousePosPrev.y};
            io.MouseDragMaxDistanceSqr[0] = std::sqrtf(std::powf(io.MouseDragMaxDistanceAbs[0].x, 2.f) + std::powf(io.MouseDragMaxDistanceAbs[0].y, 2.f));
        }

        if (e.has(::ui::event::type::mmb_down)) {
            io.MouseDown[1] = true;
            io.MouseClickedPos[1] = io.MousePos;
        }
        else if (e.has(::ui::event::type::mmb_up)) {
            io.MouseDown[1] = false;
        }

        if (e.has(::ui::event::type::rmb_down)) {
            io.MouseDown[2] = true;
            io.MouseClickedPos[2] = io.MousePos;
        }
        else if (e.has(::ui::event::type::rmb_up)) {
            io.MouseDown[2] = false;
        }
    }

    if (e.is_key_event()) {
        auto k = map_key(e.key());
        if (e.has(::ui::event::type::key_down) || e.has(::ui::event::type::key_repeat)) {
            io.KeysDown[k] = true;
            io.AddKeyEvent(k, true);
        }
        else if (e.has(::ui::event::type::key_up)) {
            io.KeysDown[map_key(e.key())] = false;
            io.AddKeyEvent(k, false);
        }

        if (e.has(::ui::event::type::key_typed)) {
            io.AddInputCharacter(e.character());
        }
    }
}


// MARK: - Key Mappings

auto kestrel::ui::imgui::dockspace::map_key(::ui::hid::key k) -> ImGuiKey
{
    switch (k) {
        case ::ui::hid::unknown:          return ImGuiKey_0;
        case ::ui::hid::enter:            return ImGuiKey_Enter;
        case ::ui::hid::tab:              return ImGuiKey_Tab;
        case ::ui::hid::backspace:        return ImGuiKey_Backspace;
        case ::ui::hid::escape:           return ImGuiKey_Escape;
        case ::ui::hid::left:             return ImGuiKey_LeftArrow;
        case ::ui::hid::right:            return ImGuiKey_RightArrow;
        case ::ui::hid::down:             return ImGuiKey_DownArrow;
        case ::ui::hid::up:               return ImGuiKey_UpArrow;
        case ::ui::hid::a:                return ImGuiKey_A;
        case ::ui::hid::s:                return ImGuiKey_S;
        case ::ui::hid::d:                return ImGuiKey_D;
        case ::ui::hid::f:                return ImGuiKey_F;
        case ::ui::hid::h:                return ImGuiKey_H;
        case ::ui::hid::g:                return ImGuiKey_G;
        case ::ui::hid::z:                return ImGuiKey_Z;
        case ::ui::hid::x:                return ImGuiKey_X;
        case ::ui::hid::c:                return ImGuiKey_C;
        case ::ui::hid::v:                return ImGuiKey_V;
        case ::ui::hid::b:                return ImGuiKey_B;
        case ::ui::hid::q:                return ImGuiKey_Q;
        case ::ui::hid::w:                return ImGuiKey_W;
        case ::ui::hid::e:                return ImGuiKey_E;
        case ::ui::hid::r:                return ImGuiKey_R;
        case ::ui::hid::y:                return ImGuiKey_Y;
        case ::ui::hid::t:                return ImGuiKey_T;
        case ::ui::hid::o:                return ImGuiKey_O;
        case ::ui::hid::u:                return ImGuiKey_U;
        case ::ui::hid::i:                return ImGuiKey_I;
        case ::ui::hid::p:                return ImGuiKey_P;
        case ::ui::hid::l:                return ImGuiKey_L;
        case ::ui::hid::j:                return ImGuiKey_J;
        case ::ui::hid::k:                return ImGuiKey_K;
        case ::ui::hid::n:                return ImGuiKey_N;
        case ::ui::hid::m:                return ImGuiKey_M;
        case ::ui::hid::kp_0:             return ImGuiKey_Keypad0;
        case ::ui::hid::kp_1:             return ImGuiKey_Keypad1;
        case ::ui::hid::kp_2:             return ImGuiKey_Keypad2;
        case ::ui::hid::kp_3:             return ImGuiKey_Keypad3;
        case ::ui::hid::kp_4:             return ImGuiKey_Keypad4;
        case ::ui::hid::kp_5:             return ImGuiKey_Keypad5;
        case ::ui::hid::kp_6:             return ImGuiKey_Keypad6;
        case ::ui::hid::kp_7:             return ImGuiKey_Keypad7;
        case ::ui::hid::kp_8:             return ImGuiKey_Keypad8;
        case ::ui::hid::kp_9:             return ImGuiKey_Keypad9;
        case ::ui::hid::space:            return ImGuiKey_Space;
        case ::ui::hid::period:           return ImGuiKey_Period;
        case ::ui::hid::comma:            return ImGuiKey_Comma;
        case ::ui::hid::semi_colon:       return ImGuiKey_Semicolon;
        case ::ui::hid::apostrophe:       return ImGuiKey_Apostrophe;
        case ::ui::hid::left_bracket:     return ImGuiKey_LeftBracket;
        case ::ui::hid::right_bracket:    return ImGuiKey_RightBracket;
        case ::ui::hid::grave_accent:     return ImGuiKey_GraveAccent;
        case ::ui::hid::backslash:        return ImGuiKey_Backslash;
        case ::ui::hid::slash:            return ImGuiKey_Slash;
        case ::ui::hid::minus:            return ImGuiKey_Minus;
        case ::ui::hid::equal:            return ImGuiKey_Equal;
        case ::ui::hid::f1:               return ImGuiKey_F1;
        case ::ui::hid::f2:               return ImGuiKey_F2;
        case ::ui::hid::f3:               return ImGuiKey_F3;
        case ::ui::hid::f4:               return ImGuiKey_F4;
        case ::ui::hid::f5:               return ImGuiKey_F5;
        case ::ui::hid::f6:               return ImGuiKey_F6;
        case ::ui::hid::f7:               return ImGuiKey_F7;
        case ::ui::hid::f8:               return ImGuiKey_F8;
        case ::ui::hid::f9:               return ImGuiKey_F9;
        case ::ui::hid::f10:              return ImGuiKey_F10;
        case ::ui::hid::f11:              return ImGuiKey_F11;
        case ::ui::hid::f12:              return ImGuiKey_F12;
        case ::ui::hid::num0:             return ImGuiKey_0;
        case ::ui::hid::num1:             return ImGuiKey_1;
        case ::ui::hid::num2:             return ImGuiKey_2;
        case ::ui::hid::num3:             return ImGuiKey_3;
        case ::ui::hid::num4:             return ImGuiKey_4;
        case ::ui::hid::num5:             return ImGuiKey_5;
        case ::ui::hid::num6:             return ImGuiKey_6;
        case ::ui::hid::num7:             return ImGuiKey_7;
        case ::ui::hid::num8:             return ImGuiKey_8;
        case ::ui::hid::num9:             return ImGuiKey_9;
        case ::ui::hid::world_1:          return ImGuiKey_None;
        case ::ui::hid::world_2:          return ImGuiKey_None;
        case ::ui::hid::insert:           return ImGuiKey_Insert;
        case ::ui::hid::del:              return ImGuiKey_Delete;
        case ::ui::hid::page_up:          return ImGuiKey_PageUp;
        case ::ui::hid::page_down:        return ImGuiKey_PageDown;
        case ::ui::hid::home:             return ImGuiKey_Home;
        case ::ui::hid::end:              return ImGuiKey_End;
        case ::ui::hid::caps_lock:        return ImGuiKey_CapsLock;
        case ::ui::hid::scroll_lock:      return ImGuiKey_ScrollLock;
        case ::ui::hid::num_lock:         return ImGuiKey_NumLock;
        case ::ui::hid::print_screen:     return ImGuiKey_PrintScreen;
        case ::ui::hid::pause:            return ImGuiKey_Pause;
        case ::ui::hid::f13:              return ImGuiKey_None;
        case ::ui::hid::f14:              return ImGuiKey_None;
        case ::ui::hid::f15:              return ImGuiKey_None;
        case ::ui::hid::f16:              return ImGuiKey_None;
        case ::ui::hid::f17:              return ImGuiKey_None;
        case ::ui::hid::f18:              return ImGuiKey_None;
        case ::ui::hid::f19:              return ImGuiKey_None;
        case ::ui::hid::f20:              return ImGuiKey_None;
        case ::ui::hid::f21:              return ImGuiKey_None;
        case ::ui::hid::f22:              return ImGuiKey_None;
        case ::ui::hid::f23:              return ImGuiKey_None;
        case ::ui::hid::f24:              return ImGuiKey_None;
        case ::ui::hid::f25:              return ImGuiKey_None;
        case ::ui::hid::kp_decimal:       return ImGuiKey_KeypadDecimal;
        case ::ui::hid::kp_divide:        return ImGuiKey_KeypadDivide;
        case ::ui::hid::kp_multiply:      return ImGuiKey_KeypadMultiply;
        case ::ui::hid::kp_subtract:      return ImGuiKey_KeypadSubtract;
        case ::ui::hid::kp_add:           return ImGuiKey_KeypadAdd;
        case ::ui::hid::kp_enter:         return ImGuiKey_KeypadEnter;
        case ::ui::hid::kp_equal:         return ImGuiKey_KeypadEqual;
        case ::ui::hid::left_shift:       return ImGuiKey_LeftShift;
        case ::ui::hid::left_control:     return ImGuiKey_LeftCtrl;
        case ::ui::hid::left_alt:         return ImGuiKey_LeftAlt;
        case ::ui::hid::left_super:       return ImGuiKey_LeftSuper;
        case ::ui::hid::right_shift:      return ImGuiKey_RightShift;
        case ::ui::hid::right_control:    return ImGuiKey_RightCtrl;
        case ::ui::hid::right_alt:        return ImGuiKey_RightAlt;
        case ::ui::hid::right_super:      return ImGuiKey_RightSuper;
        case ::ui::hid::menu:             return ImGuiKey_Menu;
    }
}
