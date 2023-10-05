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
#include <libKestrel/font/font.hpp>
#include <libKestrel/ui/entity/scene_entity.hpp>

namespace kestrel::ui::widgets
{
    struct lua_api(Widget.TextArea, Available_0_8) textarea_widget
    {
    public:
        has_constructable_lua_api(textarea_widget);

        textarea_widget(const math::rect& frame, const std::string& text);
        lua_constructor(Available_0_8) explicit textarea_widget(const std::string& text);

        lua_function(draw, Available_0_8) auto draw() -> void;
        lua_getter(entity, Available_0_8) [[nodiscard]] auto entity() const -> ui::scene_entity::lua_reference;

        lua_getter(text, Available_0_8) [[nodiscard]] auto text() const -> std::string;
        lua_getter(font, Available_0_8) [[nodiscard]] auto font() const -> font::reference::lua_reference;
        lua_getter(textColor, Available_0_8) [[nodiscard]] auto color() const -> graphics::color::lua_reference;
        lua_getter(backgroundColor, Available_0_8) [[nodiscard]] auto background_color() const -> graphics::color::lua_reference;
        lua_getter(offset, Available_0_8) [[nodiscard]] auto offset() const -> math::size;
        lua_getter(position, Available_0_8) [[nodiscard]] auto position() const -> math::point;
        lua_getter(size, Available_0_8) [[nodiscard]] auto size() const -> math::size;
        lua_getter(frame, Available_0_8) [[nodiscard]] auto frame() const -> math::rect;
        lua_getter(scrollOffset, Available_0_8) [[nodiscard]] auto scroll_offset() const -> std::int32_t;

        lua_setter(text, Available_0_8) auto set_text(const std::string& v) -> void;
        lua_setter(font, Available_0_8) auto set_font(const font::reference::lua_reference& font) -> void;
        lua_setter(textColor, Available_0_8) auto set_color(const graphics::color::lua_reference& v) -> void;
        lua_setter(backgroundColor, Available_0_8) auto set_background_color(const graphics::color::lua_reference& v) -> void;
        lua_setter(offset, Available_0_8) auto set_offset(const math::size& v) -> void;
        lua_setter(position, Available_0_8) auto set_position(const math::point& v) -> void;
        lua_setter(size, Available_0_8) auto set_size(const math::size& v) -> void;
        lua_setter(frame, Available_0_8) auto set_frame(const math::rect& v) -> void;
        lua_setter(scrollOffset, Available_0_8) auto set_scroll_offset(std::int32_t offset) -> void;

        lua_function(scrollUp, Available_0_8) auto scroll_up() -> void;
        lua_function(scrollDown, Available_0_8) auto scroll_down() -> void;

        lua_getter(canScrollUp, Available_0_8) [[nodiscard]] auto can_scroll_up() const -> bool;
        lua_getter(canScrollDown, Available_0_8) [[nodiscard]] auto can_scroll_down() const -> bool;

        lua_getter(anchorPoint, Available_0_9) [[nodiscard]] auto anchor_point() const -> layout::axis_origin;
        lua_setter(anchorPoint, Available_0_9) auto set_anchor_point(const layout::axis_origin& origin) -> void;

    private:
        bool m_dirty { true };
        std::string m_text;
        font::reference::lua_reference m_font { nullptr };
        math::size m_clipping_size;
        graphics::color m_color { graphics::color::white_color() };
        graphics::color m_background { graphics::color::clear_color() };
        math::size m_offset { 0 };
        std::unique_ptr<graphics::canvas> m_canvas;
        ui::scene_entity::lua_reference m_entity { nullptr };
        std::int32_t m_scroll_offset { 0 };
        bool m_can_scroll_up { false };
        bool m_can_scroll_down { false };

        auto redraw_entity() -> void;
    };
}
