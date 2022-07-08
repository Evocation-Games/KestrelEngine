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

#include "math/size.hpp"
#include "core/graphics/common/color.hpp"
#include "scripting/state.hpp"
#include "util/lua_vector.hpp"

namespace ui::imgui::style
{
    auto enroll_api_in_lua_state(const std::shared_ptr<scripting::lua::state> &lua) -> void;

    lua_api auto set_frame_padding(const math::size& size) -> void;
    lua_api auto frame_padding() -> math::size;

    lua_api auto set_window_border_size(int32_t size) -> void;
    lua_api auto window_border_size() -> int32_t;

    lua_api auto set_child_border_size(int32_t size) -> void;
    lua_api auto child_border_size() -> int32_t;

    lua_api auto set_frame_border_size(int32_t size) -> void;
    lua_api auto frame_border_size() -> int32_t;

    lua_api auto set_popup_border_size(int32_t size) -> void;
    lua_api auto popup_border_size() -> int32_t;

    lua_api auto set_tab_border_size(int32_t size) -> void;
    lua_api auto tab_border_size() -> int32_t;

    lua_api auto set_window_rounding(int32_t size) -> void;
    lua_api auto window_rounding() -> int32_t;

    lua_api auto set_frame_rounding(int32_t size) -> void;
    lua_api auto frame_rounding() -> int32_t;

    lua_api auto set_window_title_align(const math::size& size) -> void;
    lua_api auto window_title_align() -> math::size;

    lua_api auto set_border_color(const graphics::color::lua_reference& color) -> void;
    lua_api auto border_color() -> graphics::color::lua_reference;

    lua_api auto set_button_color(const graphics::color::lua_reference& color) -> void;
    lua_api auto button_color() -> graphics::color::lua_reference;

    lua_api auto set_button_hover_color(const graphics::color::lua_reference& color) -> void;
    lua_api auto button_hover_color() -> graphics::color::lua_reference;

    lua_api auto set_button_active_color(const graphics::color::lua_reference& color) -> void;
    lua_api auto button_active_color() -> graphics::color::lua_reference;

    lua_api auto set_window_title_active_color(const graphics::color::lua_reference& color) -> void;
    lua_api auto window_title_active_color() -> graphics::color::lua_reference;

    lua_api auto set_window_title_inactive_color(const graphics::color::lua_reference& color) -> void;
    lua_api auto window_title_inactive_color() -> graphics::color::lua_reference;

    lua_api auto set_frame_background_color(const graphics::color::lua_reference& color) -> void;
    lua_api auto frame_background_color() -> graphics::color::lua_reference;

    lua_api auto set_frame_background_hover_color(const graphics::color::lua_reference& color) -> void;
    lua_api auto frame_background_hover_color() -> graphics::color::lua_reference;

    lua_api auto set_frame_background_active_color(const graphics::color::lua_reference& color) -> void;
    lua_api auto frame_background_active_color() -> graphics::color::lua_reference;

    lua_api auto set_checkmark_color(const graphics::color::lua_reference& color) -> void;
    lua_api auto checkmark_color() -> graphics::color::lua_reference;

    lua_api auto set_tab_color(const graphics::color::lua_reference& color) -> void;
    lua_api auto tab_color() -> graphics::color::lua_reference;

    lua_api auto set_tab_hover_color(const graphics::color::lua_reference& color) -> void;
    lua_api auto tab_hover_color() -> graphics::color::lua_reference;

    lua_api auto set_tab_active_color(const graphics::color::lua_reference& color) -> void;
    lua_api auto tab_active_color() -> graphics::color::lua_reference;

    lua_api auto set_docking_preview_color(const graphics::color::lua_reference& color) -> void;
    lua_api auto docking_preview_color() -> graphics::color::lua_reference;

    lua_api auto set_text_selected_background_color(const graphics::color::lua_reference& color) -> void;
    lua_api auto text_selected_background_color() -> graphics::color::lua_reference;
}
