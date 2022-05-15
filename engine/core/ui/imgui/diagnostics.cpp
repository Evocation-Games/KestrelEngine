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

#include "core/ui/imgui/diagnostics.hpp"
#include "core/ui/imgui/same_line.hpp"
#include "core/environment.hpp"
#include "renderer/common/renderer.hpp"

// MARK: - Lua

auto ui::imgui::diagnostics::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("Debug")
            .beginNamespace("ImGui")
                .beginClass<diagnostics>("Diagnostics")
                    .addConstructor<auto(*)()->void, lua_reference>()
                .endClass()
            .endNamespace()
        .endNamespace();
}

// MARK: - Construction

ui::imgui::diagnostics::diagnostics()
    : widget()
{
}

auto ui::imgui::diagnostics::construct_contents() -> void
{
    if (m_constructed) {
        return;
    }

    auto env = environment::active_environment().lock();
    if (!env) {
        return;
    }

    auto state = env->lua_runtime()->internal_state();

    // Construct the diagnostic window and elements within.
    label::lua_reference driver_label { new label("Renderer Driver:") };
    m_window.add_widget({ state, driver_label });
    m_window.add_widget({ state, same_line::lua_reference(new same_line()) });
    m_window.add_widget({state, m_driver});

    switch (renderer::api()) {
        case renderer::api::metal: {
            m_driver->set_text("Metal");
            break;
        }
        case renderer::api::opengl: {
            m_driver->set_text("OpenGL");
            break;
        }
        default: {
            m_driver->set_text("Unknown");
            break;
        }
    }

    label::lua_reference last_frame_duration_label { new label("Last Frame Duration:") };
    m_window.add_widget({ state, last_frame_duration_label });
    m_window.add_widget({ state, same_line::lua_reference(new same_line()) });
    m_window.add_widget({state, m_frame_duration});

    label::lua_reference last_update_duration_label { new label("Last Update Duration:") };
    m_window.add_widget({ state, last_update_duration_label });
    m_window.add_widget({ state, same_line::lua_reference(new same_line()) });
    m_window.add_widget({state, m_update_duration});

    label::lua_reference approx_fps_label { new label("Approximate FPS:") };
    m_window.add_widget({ state, approx_fps_label });
    m_window.add_widget({ state, same_line::lua_reference(new same_line()) });
    m_window.add_widget({state, m_approx_fps});

    label::lua_reference target_fps_label { new label("Target FPS:") };
    m_window.add_widget({ state, target_fps_label });
    m_window.add_widget({ state, same_line::lua_reference(new same_line()) });
    m_window.add_widget({state, m_target_fps});

    m_constructed = true;
    m_throttle = renderer::target_framerate() > 0 ? renderer::target_framerate() : 32;
}

// MARK: - Drawing

auto ui::imgui::diagnostics::draw() -> void
{
    construct_contents();

    m_frame_duration->set_text(std::to_string(renderer::last_frame_time()));

    if (m_throttle-- == 0) {
        m_approx_fps->set_text(std::to_string(renderer::approx_framerate()));
    }
    m_target_fps->set_text(std::to_string(renderer::target_framerate()));

    auto env = environment::active_environment().lock();
    if (env) {
        m_update_duration->set_text(std::to_string(env->session()->last_update_period()));
    }
    else {
        m_update_duration->set_text("--");
    }

    m_window.draw();
}
