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

#include "core/ui/imgui/label.hpp"

// MARK: - Fonts

static ImFont *s_normal_font = nullptr;
static ImFont *s_large_font = nullptr;

auto ui::imgui::label::configure_fonts() -> void
{
    ImGuiIO &io = ImGui::GetIO();
    ImFontConfig config;
    config.SizePixels = 13;
    config.OversampleH = config.OversampleV = 1;
    config.PixelSnapH = true;

    s_normal_font = io.Fonts->AddFontDefault(&config);

    config.SizePixels = 18;
    s_large_font = io.Fonts->AddFontDefault(&config);
}

// MARK: - Lua

auto ui::imgui::label::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("ImGui")
            .beginClass<label>("Text")
                .addConstructor<auto(*)(const std::string&)->void, lua_reference>()
                .addProperty("text", &label::text, &label::set_text)
                .addProperty("largeFont", &label::is_large_font, &label::set_large_font)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

ui::imgui::label::label(const std::string &text)
    : m_text(text)
{
}

// MARK: - Drawing

auto ui::imgui::label::draw() -> void
{
    ImGui::PushFont(m_large_font ? s_large_font : s_normal_font);
    ImGui::Text("%s", m_text.c_str());
    ImGui::PopFont();
}