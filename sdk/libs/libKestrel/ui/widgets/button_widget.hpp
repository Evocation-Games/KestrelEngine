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

#include <memory>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/graphics/canvas/canvas.hpp>
#include <libKestrel/graphics/types/color.hpp>
#include <libKestrel/math/point.hpp>
#include <libKestrel/math/size.hpp>
#include <libKestrel/math/rect.hpp>
#include <libKestrel/ui/stencils/button_stencil.hpp>
#include <libKestrel/event/responder/responder_chain.hpp>
#include <libKestrel/ui/entity/scene_entity.hpp>
#include <libKestrel/font/manager.hpp>
#include <libKestrel/font/font.hpp>
#include <libKestrel/ui/types/action/action.hpp>
#include <libUI/controls/button.hpp>

namespace kestrel::ui::widgets
{
    struct lua_api(Widget.Button, Available_0_8) button_widget: public responder_chain::mouse_responder
    {
    public:
        has_constructable_lua_api(button_widget);

        lua_function(setGlobalNormalStencil, Available_0_8) static auto set_global_normal_stencil(const stencils::button_stencil::lua_reference& stencil) -> void;
        lua_function(setGlobalPressedStencil, Available_0_8) static auto set_global_pressed_stencil(const stencils::button_stencil::lua_reference& stencil) -> void;
        lua_function(setGlobalDisabledStencil, Available_0_8) static auto set_global_disabled_stencil(const stencils::button_stencil::lua_reference& stencil) -> void;

        lua_constructor(Available_0_8) explicit button_widget(const std::string& label);

        lua_function(draw, Available_0_8) auto draw() -> void;
        lua_function(entity, Available_0_8) [[nodiscard]] auto entity() const -> scene_entity::lua_reference;

        lua_getter(normalStencil, Available_0_8) [[nodiscard]] auto normal_stencil() const -> stencils::button_stencil::lua_reference;
        lua_getter(pressedStencil, Available_0_8) [[nodiscard]] auto pressed_stencil() const -> stencils::button_stencil::lua_reference;
        lua_getter(disabledStencil, Available_0_8) [[nodiscard]] auto disabled_stencil() const -> stencils::button_stencil::lua_reference;
        lua_getter(title, Available_0_8) [[nodiscard]] auto label() const -> std::string;
        lua_getter(font, Available_0_8) [[nodiscard]] auto font() const -> font::reference::lua_reference;
        lua_getter(isPressed, Available_0_8) [[nodiscard]] auto is_pressed() const -> bool;
        lua_getter(isDisabled, Available_0_8) [[nodiscard]] auto is_disabled() const -> bool;
        lua_getter(labelColor, Available_0_8) [[nodiscard]] auto label_color() const -> graphics::color::lua_reference;
        lua_getter(pressedColor, Available_0_8) [[nodiscard]] auto label_pressed_color() const -> graphics::color::lua_reference;
        lua_getter(disabledColor, Available_0_8) [[nodiscard]] auto label_disabled_color() const -> graphics::color::lua_reference;
        lua_getter(icon, Available_0_8) [[nodiscard]] auto icon() const -> image::static_image::lua_reference;
        lua_getter(action, Available_0_8) [[nodiscard]] auto action_body() const -> luabridge::LuaRef;
        lua_function(setMouseEnter, Available_0_8) lua_getter(mouseEnter, Available_0_8) [[nodiscard]] auto mouse_enter_body() const -> luabridge::LuaRef;
        lua_function(setMouseExit, Available_0_8)  lua_getter(mouseExit, Available_0_8) [[nodiscard]] auto mouse_exit_body() const -> luabridge::LuaRef;
        lua_getter(userInfo, Available_0_8) [[nodiscard]] auto user_info() const -> luabridge::LuaRef;
        lua_getter(frame, Available_0_8) [[nodiscard]] auto frame() const -> math::rect;
        lua_getter(disabled, Available_0_8) [[nodiscard]] auto disabled() const -> bool;
        lua_getter(continuous, Available_0_8) [[nodiscard]] auto continuous() const -> bool;
        lua_getter(hidden, Available_0_9) [[nodiscard]] auto hidden() const -> bool;

        lua_setter(normalStencil, Available_0_8) auto set_normal_stencil(const stencils::button_stencil::lua_reference& stencil) -> void;
        lua_setter(pressedStencil, Available_0_8) auto set_pressed_stencil(const stencils::button_stencil::lua_reference& stencil) -> void;
        lua_setter(disabledStencil, Available_0_8) auto set_disabled_stencil(const stencils::button_stencil::lua_reference& stencil) -> void;
        lua_setter(title, Available_0_8) auto set_label(const std::string& label) -> void;
        lua_setter(font, Available_0_8) auto set_font(const font::reference::lua_reference& font) -> void;
        lua_setter(labelColor, Available_0_8) auto set_label_color(const graphics::color::lua_reference& color) -> void;
        lua_setter(pressedColor, Available_0_8) auto set_label_pressed_color(const graphics::color::lua_reference& color) -> void;
        lua_setter(disabledColor, Available_0_8) auto set_label_disabled_color(const graphics::color::lua_reference& color) -> void;
        lua_setter(icon, Available_0_8) auto set_icon(const luabridge::LuaRef& info) -> void;
        lua_function(setAction, Available_0_8) lua_setter(action, Available_0_8) auto set_action(const luabridge::LuaRef& body) -> void;
        lua_setter(mouseEnter, Available_0_8) auto set_mouse_enter(const luabridge::LuaRef& body) -> void;
        lua_setter(mouseExit, Available_0_8) auto set_mouse_exit(const luabridge::LuaRef& body) -> void;
        lua_setter(userInfo, Available_0_8) auto set_user_info(const luabridge::LuaRef& info) -> void;
        lua_setter(frame, Available_0_8) auto set_frame(const math::rect& r) -> void;
        lua_setter(disabled, Available_0_8) auto set_disabled(bool disabled) -> void;
        lua_setter(continuous, Available_0_8) auto set_continuous_action(bool continuous) -> void;
        lua_setter(hidden, Available_0_9) auto set_hidden(bool hidden) -> void;

        auto set_ui_action(const ui::action& action) -> void;

        auto receive_event(const event& e) -> bool override;
        auto mouse_down() -> void;
        auto mouse_up() -> void;
        auto mouse_enter() -> void;
        auto mouse_exit() -> void;

    private:
        bool m_dirty { true };
        std::string m_label;
        font::reference::lua_reference m_font { nullptr };
        bool m_pressed { false };
        bool m_disabled { false };
        bool m_inside { false };
        bool m_continuous { false };
        graphics::color m_label_normal { graphics::color(255, 255) };
        graphics::color m_label_pressed { graphics::color(100, 255) };
        graphics::color m_label_disabled { graphics::color(200, 255) };
        std::shared_ptr<graphics::canvas> m_canvas;
        scene_entity::lua_reference m_entity { nullptr };
        image::static_image::lua_reference m_icon { nullptr };
        luabridge::LuaRef m_action { nullptr };
        ui::action m_ui_action;
        luabridge::LuaRef m_mouse_enter { nullptr };
        luabridge::LuaRef m_mouse_exit { nullptr };
        luabridge::LuaRef m_user_info { nullptr };
        stencils::button_stencil::lua_reference m_normal_stencil;
        stencils::button_stencil::lua_reference m_disabled_stencil;
        stencils::button_stencil::lua_reference m_pressed_stencil;

        auto redraw_entity() -> void;

        auto bind_internal_events() -> void;
    };
}

