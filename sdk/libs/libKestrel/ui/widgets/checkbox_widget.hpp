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

namespace kestrel::ui::widgets
{
    struct lua_api(Widget.Checkbox, Available_0_9) checkbox_widget: public responder_chain::mouse_responder
    {
    public:
        has_constructable_lua_api(checkbox_widget);

        lua_constructor(Available_0_9) explicit checkbox_widget();

        lua_function(draw, Available_0_9) auto draw() -> void;
        lua_function(entity, Available_0_9) [[nodiscard]] auto entity() const -> scene_entity::lua_reference;

        lua_getter(userInfo, Available_0_8) [[nodiscard]] auto user_info() const -> luabridge::LuaRef;
        lua_getter(frame, Available_0_8) [[nodiscard]] auto frame() const -> math::rect;
        lua_getter(disabled, Available_0_8) [[nodiscard]] auto disabled() const -> bool;
        lua_getter(value, Available_0_8) [[nodiscard]] auto value() const -> bool;
        lua_getter(color, Available_0_8) [[nodiscard]] auto color() const -> graphics::color::lua_reference;
        lua_getter(backgroundColor, Available_0_8) [[nodiscard]] auto background_color() const -> graphics::color::lua_reference;
        lua_getter(borderColor, Available_0_8) [[nodiscard]] auto border_color() const -> graphics::color::lua_reference;

        lua_setter(userInfo, Available_0_8) auto set_user_info(const luabridge::LuaRef& info) -> void;
        lua_setter(frame, Available_0_8) auto set_frame(const math::rect& r) -> void;
        lua_setter(disabled, Available_0_8) auto set_disabled(bool disabled) -> void;
        lua_setter(value, Available_0_8) auto set_value(bool value) -> void;
        lua_setter(color, Available_0_8) auto set_color(const graphics::color::lua_reference& v) -> void;
        lua_setter(backgroundColor, Available_0_8) auto set_background_color(const graphics::color::lua_reference& v) -> void;
        lua_setter(borderColor, Available_0_8) auto set_border_color(const graphics::color::lua_reference& v) -> void;

        lua_getter(anchorPoint, Available_0_9) [[nodiscard]] auto anchor_point() const -> layout::axis_origin;
        lua_setter(anchorPoint, Available_0_9) auto set_anchor_point(const layout::axis_origin& origin) -> void;

        auto receive_event(const event& e) -> bool override;
        auto mouse_down() -> void;
        auto mouse_up() -> void;
        auto mouse_enter() -> void;
        auto mouse_exit() -> void;

    private:
        bool m_inside { false };
        bool m_pressed { false };
        bool m_dirty { true };
        bool m_value { false };
        bool m_disabled { false };
        luabridge::LuaRef m_user_info { nullptr };
        std::shared_ptr<graphics::canvas> m_canvas;
        scene_entity::lua_reference m_entity { nullptr };
        graphics::color m_color { graphics::color::black_color() };
        graphics::color m_border_color { graphics::color::black_color() };
        graphics::color m_background_color { graphics::color::white_color() };

        auto redraw_entity() -> void;
    };
}