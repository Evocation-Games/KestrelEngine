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
#include <libKestrel/ui/entity/scene_entity.hpp>
#include <libKestrel/font/font.hpp>
#include <libKestrel/ui/alignment.hpp>

namespace kestrel::ui::widgets
{
    struct lua_api(Widget.Label, Available_0_8) label_widget
    {
    public:
        has_constructable_lua_api(label_widget);

        lua_constructor(Available_0_8) explicit label_widget(const std::string& text);

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
        [[nodiscard]] auto vertical_alignment() const -> enum vertical_alignment;
        [[nodiscard]] auto horizontal_alignment() const -> enum horizontal_alignment;

        lua_setter(text, Available_0_8) auto set_text(const std::string& v) -> void;
        lua_setter(font, Available_0_8) auto set_font(const font::reference::lua_reference& font) -> void;
        lua_setter(textColor, Available_0_8) auto set_color(const graphics::color::lua_reference& v) -> void;
        lua_setter(backgroundColor, Available_0_8) auto set_background_color(const graphics::color::lua_reference& v) -> void;
        lua_setter(offset, Available_0_8) auto set_offset(const math::size& v) -> void;
        lua_setter(position, Available_0_8) auto set_position(const math::point& v) -> void;
        lua_setter(size, Available_0_8) auto set_size(const math::size& v) -> void;
        lua_setter(frame, Available_0_8) auto set_frame(const math::rect& v) -> void;
        auto set_vertical_alignment(enum vertical_alignment v) -> void;
        auto set_horizontal_alignment(enum horizontal_alignment v) -> void;

        lua_getter(anchorPoint, Available_0_9) [[nodiscard]] auto anchor_point() const -> layout::axis_origin;
        lua_setter(anchorPoint, Available_0_9) auto set_anchor_point(const layout::axis_origin& origin) -> void;

    private:
        bool m_dirty { true };
        std::int16_t m_min_height { 0 };
        std::string m_text;
        font::reference::lua_reference m_font { nullptr };
        graphics::color m_color { graphics::color::white_color() };
        graphics::color m_background { graphics::color::clear_color() };
        math::size m_offset { 0 };
        enum vertical_alignment m_vertical { vertical_alignment::middle };
        enum horizontal_alignment m_horizontal { horizontal_alignment::center };
        std::unique_ptr<graphics::canvas> m_canvas;
        ui::scene_entity::lua_reference m_entity { nullptr };

        auto redraw_entity() -> void;

        lua_getter(verticalAlignment, Available_0_8) [[nodiscard]] auto lua_safe_vertical_alignment() const -> std::int32_t;
        lua_getter(horizontalAlignment, Available_0_8) [[nodiscard]] auto lua_safe_horizontal_alignment() const -> std::int32_t;

        lua_setter(verticalAlignment, Available_0_8) auto set_lua_safe_vertical_alignment(std::int32_t v) -> void;
        lua_setter(horizontalAlignment, Available_0_8) auto set_lua_safe_horizontal_alignment(std::int32_t v) -> void;

    };
}