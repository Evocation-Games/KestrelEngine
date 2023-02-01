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
#include <libKestrel/event/responder/responder_chain.hpp>
#include <libKestrel/math/point.hpp>
#include <libKestrel/math/size.hpp>
#include <libKestrel/math/rect.hpp>
#include <libKestrel/ui/entity/scene_entity.hpp>
#include <libKestrel/font/font.hpp>

namespace kestrel::ui::widgets
{
    struct lua_api(Widget.ListRow, Available_0_8) list_row
    {
    public:
        has_constructable_lua_api(list_row);

        luatool_type_fix(const luabridge::LuaRef&, values)
        lua_constructor(Available_0_8) explicit list_row(const luabridge::LuaRef& values);

        lua_function(columnValue, Available_0_8) [[nodiscard]] auto column_value(std::int32_t column) const -> std::string;

    private:
        std::vector<std::string> m_columns;
    };

    struct lua_api(Widget.List, Available_0_8) list_widget: public responder_chain::mouse_responder, public responder_chain::responder
    {
    public:
        has_constructable_lua_api(list_widget);

        list_widget() = default;
        lua_constructor(Available_0_8) explicit list_widget(const math::rect& frame);

        lua_function(rowAtPoint, Available_0_8) auto row_at_point(const math::point& p) -> list_row::lua_reference;
        lua_function(rowIndexAtPoint, Available_0_8) auto row_index_at_point(const math::point& p) -> std::int32_t;
        lua_function(onRowSelect, Available_0_8) auto on_row_select(const luabridge::LuaRef& callback) -> void;

        lua_function(columnWidth, Available_0_8) auto column_width(std::int32_t column) const -> std::int32_t;
        lua_function(columnHeading, Available_0_8) auto column_heading(std::int32_t column) const -> std::string;
        lua_function(setColumnWidths, Available_0_8) auto set_column_widths(const luabridge::LuaRef& columns) -> void;
        lua_function(setColumnHeadings, Available_0_8) auto set_column_headings(const luabridge::LuaRef& columns) -> void;
        lua_function(setItems, Available_0_8) auto set_row_items(const luabridge::LuaRef& rows) -> void;

        lua_function(scrollUp, Available_0_8) auto scroll_up() -> void;
        lua_function(scrollDown, Available_0_8) auto scroll_down() -> void;

        lua_getter(borders, Available_0_8) [[nodiscard]] auto borders() const -> bool;
        lua_getter(headers, Available_0_8) [[nodiscard]] auto headers() const -> bool;
        lua_getter(currentRow, Available_0_8) [[nodiscard]] auto current_row() const -> list_row::lua_reference;
        lua_getter(selectedRow, Available_0_8) [[nodiscard]] auto selected_row() const -> std::int32_t;
        lua_getter(frame, Available_0_8) [[nodiscard]] auto frame() const -> math::rect;
        lua_getter(font, Available_0_8) [[nodiscard]] auto font() const -> font::reference::lua_reference;
        lua_getter(textColor, Available_0_8) [[nodiscard]] auto text_color() const -> graphics::color::lua_reference;
        lua_getter(backgroundColor, Available_0_8) [[nodiscard]] auto background_color() const -> graphics::color::lua_reference;
        lua_getter(hiliteColor, Available_0_8) [[nodiscard]] auto hilite_color() const -> graphics::color::lua_reference;
        lua_getter(outlineColor, Available_0_8) [[nodiscard]] auto outline_color() const -> graphics::color::lua_reference;
        lua_getter(headingTextColor, Available_0_8) [[nodiscard]] auto heading_text_color() const -> graphics::color::lua_reference;

        lua_setter(borders, Available_0_8) auto set_borders(bool f) -> void;
        lua_setter(headers, Available_0_8) auto set_headers(bool f) -> void;
        lua_setter(selectedRow, Available_0_8) auto select_row(std::int32_t row) -> void;
        lua_setter(frame, Available_0_8) auto set_frame(const math::rect& frame) -> void;

        lua_setter(textColor, Available_0_8) auto set_text_color(const graphics::color::lua_reference& color) -> void;
        lua_setter(backgroundColor, Available_0_8) auto set_background_color(const graphics::color::lua_reference& color) -> void;
        lua_setter(hiliteColor, Available_0_8) auto set_hilite_color(const graphics::color::lua_reference& color) -> void;
        lua_setter(outlineColor, Available_0_8) auto set_outline_color(const graphics::color::lua_reference& color) -> void;
        lua_setter(headingTextColor, Available_0_8) auto set_heading_text_color(const graphics::color::lua_reference& color) -> void;
        lua_setter(font, Available_0_8) auto set_font(const font::reference::lua_reference& font) -> void;

        lua_function(draw, Available_0_8) auto draw() -> void;
        lua_getter(entity, Available_0_8) [[nodiscard]] auto entity() const -> scene_entity::lua_reference;

        auto receive_event(const event& e) -> bool override;

    private:
        bool m_dirty { true };
        bool m_has_header { false };
        bool m_borders { false };
        bool m_pressed { false };
        math::size m_row_size;
        std::uint32_t m_visible_rows { 0 };
        font::reference::lua_reference m_label_font { nullptr };
        lua::vector<list_row::lua_reference> m_rows;
        std::vector<std::string> m_headings;
        std::vector<double> m_column_widths;
        std::int32_t m_selected_row { -1 };
        math::point m_scroll_offset;
        luabridge::LuaRef m_row_select_callback { nullptr };
        std::shared_ptr<graphics::canvas> m_canvas;
        scene_entity::lua_reference m_entity { nullptr };
        graphics::color::lua_reference m_background_color { nullptr };
        graphics::color::lua_reference m_hilite_color { nullptr };
        graphics::color::lua_reference m_text_color { nullptr };
        graphics::color::lua_reference m_outline_color { nullptr };
        graphics::color::lua_reference m_heading_text_color { nullptr };

        auto setup(const math::rect& frame) -> void;
        auto redraw_entity() -> void;
        auto bind_internal_events() -> void;
    };
}