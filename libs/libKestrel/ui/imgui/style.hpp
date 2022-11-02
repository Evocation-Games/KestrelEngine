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

#include <libKestrel/math/size.hpp>
#include <libKestrel/graphics/types/color.hpp>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/lua/support/vector.hpp>

namespace kestrel::ui::imgui
{
    namespace lua_api(ImGui.Style, Available_0_8) style
    {
        has_lua_api;

        lua_setter(framePadding, Available_0_8) auto set_frame_padding(const math::size& size) -> void;
        lua_getter(framePadding, Available_0_8) auto frame_padding() -> math::size;

        lua_setter(windowBorderSize, Available_0_8) auto set_window_border_size(std::int32_t size) -> void;
        lua_getter(windowBorderSize, Available_0_8) auto window_border_size() -> std::int32_t;

        lua_setter(childBorderSize, Available_0_8) auto set_child_border_size(std::int32_t size) -> void;
        lua_getter(childBorderSize, Available_0_8) auto child_border_size() -> std::int32_t;

        lua_setter(frameBorderSize, Available_0_8) auto set_frame_border_size(std::int32_t size) -> void;
        lua_getter(frameBorderSize, Available_0_8) auto frame_border_size() -> std::int32_t;

        lua_setter(popupBorderSize, Available_0_8) auto set_popup_border_size(std::int32_t size) -> void;
        lua_getter(popupBorderSize, Available_0_8) auto popup_border_size() -> std::int32_t;

        lua_setter(tabBorderSize, Available_0_8) auto set_tab_border_size(std::int32_t size) -> void;
        lua_getter(tabBorderSize, Available_0_8) auto tab_border_size() -> std::int32_t;

        lua_setter(windowRounding, Available_0_8) auto set_window_rounding(std::int32_t size) -> void;
        lua_getter(windowRounding, Available_0_8) auto window_rounding() -> std::int32_t;

        lua_setter(frameRounding, Available_0_8) auto set_frame_rounding(std::int32_t size) -> void;
        lua_getter(frameRounding, Available_0_8) auto frame_rounding() -> std::int32_t;

        lua_setter(windowTitleAlign, Available_0_8) auto set_window_title_align(const math::size& size) -> void;
        lua_getter(windowTitleAlign, Available_0_8) auto window_title_align() -> math::size;

        lua_setter(borderColor, Available_0_8) auto set_border_color(const graphics::color::lua_reference& color) -> void;
        lua_getter(borderColor, Available_0_8) auto border_color() -> graphics::color::lua_reference;

        lua_setter(buttonColor, Available_0_8) auto set_button_color(const graphics::color::lua_reference& color) -> void;
        lua_getter(buttonColor, Available_0_8) auto button_color() -> graphics::color::lua_reference;

        lua_setter(buttonHoverColor, Available_0_8) auto set_button_hover_color(const graphics::color::lua_reference& color) -> void;
        lua_getter(buttonHoverColor, Available_0_8) auto button_hover_color() -> graphics::color::lua_reference;

        lua_setter(buttonActiveColor, Available_0_8) auto set_button_active_color(const graphics::color::lua_reference& color) -> void;
        lua_getter(buttonActiveColor, Available_0_8) auto button_active_color() -> graphics::color::lua_reference;

        lua_setter(windowTitleActiveColor, Available_0_8) auto set_window_title_active_color(const graphics::color::lua_reference& color) -> void;
        lua_getter(windowTitleActiveColor, Available_0_8) auto window_title_active_color() -> graphics::color::lua_reference;

        lua_setter(windowTitleInactiveColor, Available_0_8) auto set_window_title_inactive_color(const graphics::color::lua_reference& color) -> void;
        lua_getter(windowTitleInactiveColor, Available_0_8) auto window_title_inactive_color() -> graphics::color::lua_reference;

        lua_setter(frameBackgroundColor, Available_0_8) auto set_frame_background_color(const graphics::color::lua_reference& color) -> void;
        lua_getter(frameBackgroundColor, Available_0_8) auto frame_background_color() -> graphics::color::lua_reference;

        lua_setter(frameBackgroundHoverColor, Available_0_8) auto set_frame_background_hover_color(const graphics::color::lua_reference& color) -> void;
        lua_getter(frameBackgroundHoverColor, Available_0_8) auto frame_background_hover_color() -> graphics::color::lua_reference;

        lua_setter(frameBackgroundActiveColor, Available_0_8) auto set_frame_background_active_color(const graphics::color::lua_reference& color) -> void;
        lua_getter(frameBackgroundActiveColor, Available_0_8) auto frame_background_active_color() -> graphics::color::lua_reference;

        lua_setter(checkmarkColor, Available_0_8) auto set_checkmark_color(const graphics::color::lua_reference& color) -> void;
        lua_getter(checkmarkColor, Available_0_8) auto checkmark_color() -> graphics::color::lua_reference;

        lua_setter(tabColor, Available_0_8) auto set_tab_color(const graphics::color::lua_reference& color) -> void;
        lua_getter(tabColor, Available_0_8) auto tab_color() -> graphics::color::lua_reference;

        lua_setter(tabHoverColor, Available_0_8) auto set_tab_hover_color(const graphics::color::lua_reference& color) -> void;
        lua_getter(tabHoverColor, Available_0_8) auto tab_hover_color() -> graphics::color::lua_reference;

        lua_setter(tabActiveColor, Available_0_8) auto set_tab_active_color(const graphics::color::lua_reference& color) -> void;
        lua_getter(tabActiveColor, Available_0_8) auto tab_active_color() -> graphics::color::lua_reference;

        lua_setter(dockingPreviewColor, Available_0_8) auto set_docking_preview_color(const graphics::color::lua_reference& color) -> void;
        lua_getter(dockingPreviewColor, Available_0_8) auto docking_preview_color() -> graphics::color::lua_reference;

        lua_setter(textSelectedBackgroundColor, Available_0_8) auto set_text_selected_background_color(const graphics::color::lua_reference& color) -> void;
        lua_getter(textSelectedBackgroundColor, Available_0_8) auto text_selected_background_color() -> graphics::color::lua_reference;
    }
}
