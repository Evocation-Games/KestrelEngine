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
#include "scripting/state.hpp"
#include "util/hint.hpp"
#include "core/graphics/common/canvas.hpp"
#include "core/graphics/common/color.hpp"
#include "math/point.hpp"
#include "math/size.hpp"
#include "math/rect.hpp"
#include "core/ui/stencils/button_stencil.hpp"
#include "core/event/responder/responder_chain.hpp"

namespace ui
{
    struct scene_entity;
}

namespace ui::widgets
{
    struct button_widget: public scripting::lua::object, public responder_chain::mouse_responder
    {
    public:
        typedef luabridge::RefCountedPtr<button_widget> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

        static auto set_global_normal_stencil(const stencils::button_stencil::lua_reference& stencil) -> void;
        static auto set_global_pressed_stencil(const stencils::button_stencil::lua_reference& stencil) -> void;
        static auto set_global_disabled_stencil(const stencils::button_stencil::lua_reference& stencil) -> void;

    public:
        lua_api explicit button_widget(const std::string& label);

        lua_api auto draw() -> void;
        [[nodiscard]] lua_api auto entity() const -> std::shared_ptr<scene_entity>;

        [[nodiscard]] lua_api auto normal_stencil() const -> stencils::button_stencil::lua_reference;
        [[nodiscard]] lua_api auto pressed_stencil() const -> stencils::button_stencil::lua_reference;
        [[nodiscard]] lua_api auto disabled_stencil() const -> stencils::button_stencil::lua_reference;
        [[nodiscard]] lua_api auto label() const -> std::string;
        [[nodiscard]] lua_api auto font() const -> std::string;
        [[nodiscard]] lua_api auto font_size() const -> int16_t;
        [[nodiscard]] lua_api auto is_pressed() const -> bool;
        [[nodiscard]] lua_api auto is_disabled() const -> bool;
        [[nodiscard]] lua_api auto label_color() const -> graphics::color::lua_reference;
        [[nodiscard]] lua_api auto label_pressed_color() const -> graphics::color::lua_reference;
        [[nodiscard]] lua_api auto label_disabled_color() const -> graphics::color::lua_reference;
        [[nodiscard]] lua_api auto icon() const -> asset::static_image::lua_reference;
        [[nodiscard]] lua_api auto action_body() const -> luabridge::LuaRef;
        [[nodiscard]] lua_api auto mouse_enter_body() const -> luabridge::LuaRef;
        [[nodiscard]] lua_api auto mouse_exit_body() const -> luabridge::LuaRef;
        [[nodiscard]] lua_api auto user_info() const -> luabridge::LuaRef;
        [[nodiscard]] lua_api auto frame() const -> math::rect;
        [[nodiscard]] lua_api auto disabled() const -> bool;

        auto set_normal_stencil(const stencils::button_stencil::lua_reference& stencil) -> void;
        auto set_pressed_stencil(const stencils::button_stencil::lua_reference& stencil) -> void;
        auto set_disabled_stencil(const stencils::button_stencil::lua_reference& stencil) -> void;
        auto set_label(const std::string& label) -> void;
        auto set_font(const std::string& font) -> void;
        auto set_font_size(int16_t font_size) -> void;
        auto set_label_color(const graphics::color::lua_reference& color) -> void;
        auto set_label_pressed_color(const graphics::color::lua_reference& color) -> void;
        auto set_label_disabled_color(const graphics::color::lua_reference& color) -> void;
        auto set_icon(const luabridge::LuaRef& info) -> void;
        auto set_action(const luabridge::LuaRef& body) -> void;
        auto set_mouse_enter(const luabridge::LuaRef& body) -> void;
        auto set_mouse_exit(const luabridge::LuaRef& body) -> void;
        auto set_user_info(const luabridge::LuaRef& info) -> void;
        auto set_frame(const math::rect& r) -> void;
        auto set_disabled(bool disabled) -> void;
        auto set_continuous_action(bool continuous) -> void;

        auto receive_event(const event& e) -> bool override;
        auto mouse_down() -> void;
        auto mouse_up() -> void;
        auto mouse_enter() -> void;
        auto mouse_exit() -> void;

    private:
        bool m_dirty { true };
        std::string m_label;
        std::string m_label_font { "Geneva" };
        int16_t m_font_size { 12 };
        bool m_pressed { false };
        bool m_disabled { false };
        bool m_inside { false };
        bool m_continuous { false };
        graphics::color m_label_normal { graphics::color(255, 255) };
        graphics::color m_label_pressed { graphics::color(100, 255) };
        graphics::color m_label_disabled { graphics::color(200, 255) };
        std::shared_ptr<graphics::canvas> m_canvas;
        std::shared_ptr<scene_entity> m_entity;
        asset::static_image::lua_reference m_icon { nullptr };
        luabridge::LuaRef m_action { nullptr };
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

