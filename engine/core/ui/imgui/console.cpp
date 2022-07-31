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

#include "core/ui/imgui/console.hpp"
#include "core/dev/console.hpp"
#include "core/environment.hpp"

// MARK: - Construction

ui::imgui::console::console()
{
    if (auto env = environment::active_environment().lock()) {
        m_dev_console = &env->session()->console();
    }
}

// MARK: - Operations

auto ui::imgui::console::close() -> void
{
    m_hidden = true;
}

// MARK: - Drawing

auto ui::imgui::console::draw() -> void
{
    if (m_hidden || !m_dev_console) {
        return;
    }

    ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Console", &m_open)) {
        ImGui::End();
        return;
    }

    // Reserve enough left-over height for 1 separator + 1 input text
    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("DevConsoleScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 3));

    for (const auto& v : m_dev_console->entries()) {
        auto line = v;

        if (line.substr(0, 3) == "***") {
            line = line.substr(3);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 0, 0, 1));
        }
        else if (line.substr(0, 4) == "&IN>") {
            line = line.substr(4);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8, 0.8, 0.8, 1.0));
        }

        ImGui::TextUnformatted(line.c_str());
        ImGui::PopStyleColor();
    }

    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::Separator();

    // Command-line
    bool reclaim_focus = false;
    ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
    if (ImGui::InputText("##dev.console.input", m_input_buffer, IM_ARRAYSIZE(m_input_buffer), input_text_flags, &input_handler, (void*)this))
    {
        char* s = m_input_buffer;
        if (s[0]) {
            m_dev_console->send_command(std::string(m_input_buffer));
        }
        strcpy(s, "");
        reclaim_focus = true;
    }

    // Auto-focus on window apparition
    ImGui::SetItemDefaultFocus();
    if (reclaim_focus)
        ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

    ImGui::End();
}

// MARK: - Handlers

auto ui::imgui::console::input_handler(ImGuiInputTextCallbackData *data) -> int
{
    return 0;
}
