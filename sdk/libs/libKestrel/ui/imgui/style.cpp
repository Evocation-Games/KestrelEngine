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

#include <imgui/imgui.h>
#include <libKestrel/ui/imgui/style.hpp>

// MARK: - Lua

construct_custom_lua_api(kestrel::ui::imgui::style)
{
    runtime->global_namespace()
        .beginNamespace("ImGui")
            .beginNamespace("Style")
                .addProperty("framePadding", &style::frame_padding, &style::set_frame_padding)
                .addProperty("windowBorderSize", &style::window_border_size, &style::set_window_border_size)
                .addProperty("childBorderSize", &style::child_border_size, &style::set_child_border_size)
                .addProperty("frameBorderSize", &style::frame_border_size, &style::set_frame_border_size)
                .addProperty("popupBorderSize", &style::popup_border_size, &style::set_popup_border_size)
                .addProperty("tabBorderSize", &style::tab_border_size, &style::set_tab_border_size)
                .addProperty("windowRounding", &style::window_rounding, &style::set_window_rounding)
                .addProperty("frameRounding", &style::frame_rounding, &style::set_frame_rounding)
                .addProperty("windowTitleAlign", &style::window_title_align, &style::set_window_title_align)
                .addProperty("borderColor", &style::border_color, &style::set_border_color)
                .addProperty("buttonColor", &style::button_color, &style::set_button_color)
                .addProperty("buttonHoverColor", &style::button_hover_color, &style::set_button_hover_color)
                .addProperty("buttonActiveColor", &style::button_active_color, &style::set_button_active_color)
                .addProperty("windowTitleActiveColor", &style::window_title_active_color, &style::set_window_title_active_color)
                .addProperty("windowTitleInactiveColor", &style::window_title_inactive_color, &style::set_window_title_inactive_color)
                .addProperty("frameBackgroundColor", &style::frame_background_color, &style::set_frame_background_color)
                .addProperty("frameBackgroundHoverColor", &style::frame_background_hover_color, &style::set_frame_background_hover_color)
                .addProperty("frameBackgroundActiveColor", &style::frame_background_active_color, &style::set_frame_background_active_color)
                .addProperty("checkmarkColor", &style::checkmark_color, &style::set_checkmark_color)
                .addProperty("tabColor", &style::tab_color, &style::set_tab_color)
                .addProperty("tabHoverColor", &style::tab_hover_color, &style::set_tab_hover_color)
                .addProperty("tabActiveColor", &style::tab_active_color, &style::set_tab_active_color)
                .addProperty("dockingPreviewColor", &style::docking_preview_color, &style::set_docking_preview_color)
                .addProperty("textSelectedBackgroundColor", &style::text_selected_background_color, &style::set_text_selected_background_color)
            .endNamespace()
        .endNamespace();
}

// MARK: - Accessors

auto kestrel::ui::imgui::style::set_frame_padding(const math::size& size) -> void
{
    auto& style = ImGui::GetStyle();
    style.FramePadding.x = size.width();
    style.FramePadding.y = size.height();
}

auto kestrel::ui::imgui::style::frame_padding() -> math::size
{
    auto& style = ImGui::GetStyle();
    return { style.FramePadding.x, style.FramePadding.y };
}

auto kestrel::ui::imgui::style::set_window_border_size(std::int32_t size) -> void
{
    auto& style = ImGui::GetStyle();
    style.WindowBorderSize = size;
}

auto kestrel::ui::imgui::style::window_border_size() -> std::int32_t
{
    auto& style = ImGui::GetStyle();
    return static_cast<std::int32_t>(style.WindowBorderSize);
}

auto kestrel::ui::imgui::style::set_child_border_size(std::int32_t size) -> void
{
    auto& style = ImGui::GetStyle();
    style.ChildBorderSize = size;
}

auto kestrel::ui::imgui::style::child_border_size() -> std::int32_t
{
    auto& style = ImGui::GetStyle();
    return static_cast<std::int32_t>(style.ChildBorderSize);
}

auto kestrel::ui::imgui::style::set_frame_border_size(std::int32_t size) -> void
{
    auto& style = ImGui::GetStyle();
    style.FrameBorderSize = size;
}

auto kestrel::ui::imgui::style::frame_border_size() -> std::int32_t
{
    auto& style = ImGui::GetStyle();
    return static_cast<std::int32_t>(style.FrameBorderSize);
}

auto kestrel::ui::imgui::style::set_popup_border_size(std::int32_t size) -> void
{
    auto& style = ImGui::GetStyle();
    style.PopupBorderSize = size;
}

auto kestrel::ui::imgui::style::popup_border_size() -> std::int32_t
{
    auto& style = ImGui::GetStyle();
    return static_cast<std::int32_t>(style.PopupBorderSize);
}

auto kestrel::ui::imgui::style::set_tab_border_size(std::int32_t size) -> void
{
    auto& style = ImGui::GetStyle();
    style.TabBorderSize = size;
}

auto kestrel::ui::imgui::style::tab_border_size() -> std::int32_t
{
    auto& style = ImGui::GetStyle();
    return static_cast<std::int32_t>(style.TabBorderSize);
}

auto kestrel::ui::imgui::style::set_window_rounding(std::int32_t size) -> void
{
    auto& style = ImGui::GetStyle();
    style.WindowRounding = size;
}

auto kestrel::ui::imgui::style::window_rounding() -> std::int32_t
{
    auto& style = ImGui::GetStyle();
    return static_cast<std::int32_t>(style.WindowRounding);
}

auto kestrel::ui::imgui::style::set_frame_rounding(std::int32_t size) -> void
{
    auto& style = ImGui::GetStyle();
    style.FrameRounding = static_cast<std::int32_t>(size);
}

auto kestrel::ui::imgui::style::frame_rounding() -> std::int32_t
{
    auto& style = ImGui::GetStyle();
    return static_cast<std::int32_t>(style.FrameRounding);
}

auto kestrel::ui::imgui::style::set_window_title_align(const math::size& size) -> void
{
    auto& style = ImGui::GetStyle();
    style.WindowTitleAlign.x = size.width();
    style.WindowTitleAlign.y = size.height();
}

auto kestrel::ui::imgui::style::window_title_align() -> math::size
{
    auto& style = ImGui::GetStyle();
    return { style.WindowTitleAlign.x, style.WindowTitleAlign.y };
}

auto kestrel::ui::imgui::style::set_border_color(const graphics::color::lua_reference& color) -> void
{
    auto& style = ImGui::GetStyle();
    style.Colors[5] = {
        color->get_red() / 255.f,
        color->get_green() / 255.f,
        color->get_blue() / 255.f,
        color->get_alpha() / 255.f
    };
}

auto kestrel::ui::imgui::style::border_color() -> graphics::color::lua_reference
{
    auto& style = ImGui::GetStyle();
    auto& c = style.Colors[5];
    return { new graphics::color(c.x, c.y, c.z, c.w) };
}

auto kestrel::ui::imgui::style::set_button_color(const graphics::color::lua_reference& color) -> void
{
    auto& style = ImGui::GetStyle();
    style.Colors[21] = {
        color->get_red() / 255.f,
        color->get_green() / 255.f,
        color->get_blue() / 255.f,
        color->get_alpha() / 255.f
    };
}

auto kestrel::ui::imgui::style::button_color() -> graphics::color::lua_reference
{
    auto& style = ImGui::GetStyle();
    auto& c = style.Colors[21];
    return { new graphics::color(c.x, c.y, c.z, c.w) };
}

auto kestrel::ui::imgui::style::set_button_hover_color(const graphics::color::lua_reference& color) -> void
{
    auto& style = ImGui::GetStyle();
    style.Colors[22] = {
        color->get_red() / 255.f,
        color->get_green() / 255.f,
        color->get_blue() / 255.f,
        color->get_alpha() / 255.f
    };
}

auto kestrel::ui::imgui::style::button_hover_color() -> graphics::color::lua_reference
{
    auto& style = ImGui::GetStyle();
    auto& c = style.Colors[22];
    return { new graphics::color(c.x, c.y, c.z, c.w) };
}

auto kestrel::ui::imgui::style::set_button_active_color(const graphics::color::lua_reference& color) -> void
{
    auto& style = ImGui::GetStyle();
    style.Colors[23] = {
        color->get_red() / 255.f,
        color->get_green() / 255.f,
        color->get_blue() / 255.f,
        color->get_alpha() / 255.f
    };
}

auto kestrel::ui::imgui::style::button_active_color() -> graphics::color::lua_reference
{
    auto& style = ImGui::GetStyle();
    auto& c = style.Colors[23];
    return { new graphics::color(c.x, c.y, c.z, c.w) };
}

auto kestrel::ui::imgui::style::set_window_title_active_color(const graphics::color::lua_reference& color) -> void
{
    auto& style = ImGui::GetStyle();
    style.Colors[11] = {
        color->get_red() / 255.f,
        color->get_green() / 255.f,
        color->get_blue() / 255.f,
        color->get_alpha() / 255.f
    };
}

auto kestrel::ui::imgui::style::window_title_active_color() -> graphics::color::lua_reference
{
    auto& style = ImGui::GetStyle();
    auto& c = style.Colors[24];
    return { new graphics::color(c.x, c.y, c.z, c.w) };
}

auto kestrel::ui::imgui::style::set_window_title_inactive_color(const graphics::color::lua_reference& color) -> void
{
    auto& style = ImGui::GetStyle();
    style.Colors[10] = {
        color->get_red() / 255.f,
        color->get_green() / 255.f,
        color->get_blue() / 255.f,
        color->get_alpha() / 255.f
    };
}

auto kestrel::ui::imgui::style::window_title_inactive_color() -> graphics::color::lua_reference
{
    auto& style = ImGui::GetStyle();
    auto& c = style.Colors[25];
    return { new graphics::color(c.x, c.y, c.z, c.w) };
}

auto kestrel::ui::imgui::style::set_frame_background_color(const graphics::color::lua_reference& color) -> void
{
    auto& style = ImGui::GetStyle();
    style.Colors[7] = {
        color->get_red() / 255.f,
        color->get_green() / 255.f,
        color->get_blue() / 255.f,
        color->get_alpha() / 255.f
    };
}

auto kestrel::ui::imgui::style::frame_background_color() -> graphics::color::lua_reference
{
    auto& style = ImGui::GetStyle();
    auto& c = style.Colors[7];
    return { new graphics::color(c.x, c.y, c.z, c.w) };
}

auto kestrel::ui::imgui::style::set_frame_background_hover_color(const graphics::color::lua_reference& color) -> void
{
    auto& style = ImGui::GetStyle();
    style.Colors[8] = {
        color->get_red() / 255.f,
        color->get_green() / 255.f,
        color->get_blue() / 255.f,
        color->get_alpha() / 255.f
    };
}

auto kestrel::ui::imgui::style::frame_background_hover_color() -> graphics::color::lua_reference
{
    auto& style = ImGui::GetStyle();
    auto& c = style.Colors[8];
    return { new graphics::color(c.x, c.y, c.z, c.w) };
}

auto kestrel::ui::imgui::style::set_frame_background_active_color(const graphics::color::lua_reference& color) -> void
{
    auto& style = ImGui::GetStyle();
    style.Colors[9] = {
        color->get_red() / 255.f,
        color->get_green() / 255.f,
        color->get_blue() / 255.f,
        color->get_alpha() / 255.f
    };
}

auto kestrel::ui::imgui::style::frame_background_active_color() -> graphics::color::lua_reference
{
    auto& style = ImGui::GetStyle();
    auto& c = style.Colors[9];
    return { new graphics::color(c.x, c.y, c.z, c.w) };
}

auto kestrel::ui::imgui::style::set_checkmark_color(const graphics::color::lua_reference& color) -> void
{
    auto& style = ImGui::GetStyle();
    style.Colors[18] = {
        color->get_red() / 255.f,
        color->get_green() / 255.f,
        color->get_blue() / 255.f,
        color->get_alpha() / 255.f
    };
}

auto kestrel::ui::imgui::style::checkmark_color() -> graphics::color::lua_reference
{
    auto& style = ImGui::GetStyle();
    auto& c = style.Colors[18];
    return { new graphics::color(c.x, c.y, c.z, c.w) };
}

auto kestrel::ui::imgui::style::set_tab_color(const graphics::color::lua_reference& color) -> void
{
    auto& style = ImGui::GetStyle();
    style.Colors[33] = {
        color->get_red() / 255.f,
        color->get_green() / 255.f,
        color->get_blue() / 255.f,
        color->get_alpha() / 255.f
    };
}

auto kestrel::ui::imgui::style::tab_color() -> graphics::color::lua_reference
{
    auto& style = ImGui::GetStyle();
    auto& c = style.Colors[33];
    return { new graphics::color(c.x, c.y, c.z, c.w) };
}

auto kestrel::ui::imgui::style::set_tab_hover_color(const graphics::color::lua_reference& color) -> void
{
    auto& style = ImGui::GetStyle();
    style.Colors[34] = {
        color->get_red() / 255.f,
        color->get_green() / 255.f,
        color->get_blue() / 255.f,
        color->get_alpha() / 255.f
    };
}

auto kestrel::ui::imgui::style::tab_hover_color() -> graphics::color::lua_reference
{
    auto& style = ImGui::GetStyle();
    auto& c = style.Colors[34];
    return { new graphics::color(c.x, c.y, c.z, c.w) };
}

auto kestrel::ui::imgui::style::set_tab_active_color(const graphics::color::lua_reference& color) -> void
{
    auto& style = ImGui::GetStyle();
    style.Colors[35] = {
        color->get_red() / 255.f,
        color->get_green() / 255.f,
        color->get_blue() / 255.f,
        color->get_alpha() / 255.f
    };
}

auto kestrel::ui::imgui::style::tab_active_color() -> graphics::color::lua_reference
{
    auto& style = ImGui::GetStyle();
    auto& c = style.Colors[35];
    return { new graphics::color(c.x, c.y, c.z, c.w) };
}

auto kestrel::ui::imgui::style::set_docking_preview_color(const graphics::color::lua_reference& color) -> void
{
    auto& style = ImGui::GetStyle();
    style.Colors[38] = {
        color->get_red() / 255.f,
        color->get_green() / 255.f,
        color->get_blue() / 255.f,
        color->get_alpha() / 255.f
    };
}

auto kestrel::ui::imgui::style::docking_preview_color() -> graphics::color::lua_reference
{
    auto& style = ImGui::GetStyle();
    auto& c = style.Colors[38];
    return { new graphics::color(c.x, c.y, c.z, c.w) };
}

auto kestrel::ui::imgui::style::set_text_selected_background_color(const graphics::color::lua_reference& color) -> void
{
    auto& style = ImGui::GetStyle();
    style.Colors[49] = {
        color->get_red() / 255.f,
        color->get_green() / 255.f,
        color->get_blue() / 255.f,
        color->get_alpha() / 255.f
    };
}

auto kestrel::ui::imgui::style::text_selected_background_color() -> graphics::color::lua_reference
{
    auto& style = ImGui::GetStyle();
    auto& c = style.Colors[49];
    return { new graphics::color(c.x, c.y, c.z, c.w) };
}

