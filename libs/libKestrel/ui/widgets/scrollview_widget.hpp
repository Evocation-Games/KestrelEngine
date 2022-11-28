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
#include <libKestrel/math/rect.hpp>
#include <libKestrel/ui/entity/scene_entity.hpp>
#include <libKestrel/ui/layout/positioning_frame.hpp>
#include <libKestrel/event/responder/responder_chain.hpp>

namespace kestrel::ui::widgets
{
    struct lua_api(Widget.ScrollView, Available_0_8) scrollview_widget: public responder_chain::mouse_responder, public responder_chain::responder
    {
    public:
        has_constructable_lua_api(scrollview_widget);

        lua_constructor(Available_0_8) explicit scrollview_widget();

        lua_getter(entity, Available_0_8) [[nodiscard]] auto entity() const -> scene_entity::lua_reference;
        lua_function(draw, Available_0_8) auto draw() -> void;

        lua_getter(scrollOffset, Available_0_8) [[nodiscard]] auto scroll_offset() const -> math::point;
        lua_getter(position, Available_0_8) [[nodiscard]] auto position() const -> math::point;
        lua_getter(size, Available_0_8) [[nodiscard]] auto size() const -> math::size;
        lua_getter(frame, Available_0_8) [[nodiscard]] auto frame() const -> math::rect;

        lua_setter(scrollOffset, Available_0_8) auto set_scroll_offset(const math::point& v) -> void;
        lua_setter(position, Available_0_8) auto set_position(const math::point& v) -> void;
        lua_setter(size, Available_0_8) auto set_size(const math::size& v) -> void;
        lua_setter(frame, Available_0_8) auto set_frame(const math::rect& v) -> void;

        lua_function(setContent, Available_0_8) auto set_content_entity(const scene_entity::lua_reference& entity) -> void;

        auto bind_internal_events() -> void;
        auto receive_event(const event& e) -> bool override;

        auto redraw_entity() -> void;

    private:
        bool m_dirty { true };
        math::point m_scroll_offset { 0 };
        math::point m_drag_mouse_origin { 0 };
        math::point m_drag_scroll_origin { 0 };
        graphics::canvas::lua_reference m_canvas;
        ui::scene_entity::lua_reference m_entity { nullptr };
        ui::scene_entity::lua_reference m_content_entity { nullptr };

    };
}