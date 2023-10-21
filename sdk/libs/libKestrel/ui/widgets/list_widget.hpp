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
    struct lua_api(Widget.List, Available_0_8) list_widget: public responder_chain::mouse_responder, public responder_chain::responder
    {
    public:
        has_constructable_lua_api(list_widget);

        list_widget() = default;

        luatool_type_fix(const math::rect&, frame)
        lua_constructor(Available_0_8) explicit list_widget(const math::rect& frame);

        // Framing & Position
        lua_getter(frame, Available_0_8) [[nodiscard]] auto frame() const -> math::rect;
        lua_setter(frame, Available_0_8) auto set_frame(const math::rect& frame) -> void;

        // Selection & State
        lua_getter(selectedRow, Available_0_8) [[nodiscard]] auto selected_row() const -> std::int32_t;
        lua_setter(selectedRow, Available_0_8) auto select_row(std::int32_t row) -> void;

        // Appearance
        lua_getter(borders, Available_0_8) [[nodiscard]] auto borders() const -> bool;
        lua_setter(borders, Available_0_8) auto set_borders(bool f) -> void;
        lua_getter(headers, Available_0_8) [[nodiscard]] auto headers() const -> bool;
        lua_setter(headers, Available_0_8) auto set_headers(bool f) -> void;
        lua_getter(font, Available_0_8) [[nodiscard]] auto font() const -> font::reference::lua_reference;
        lua_setter(font, Available_0_8) auto set_font(const font::reference::lua_reference& font) -> void;
        lua_getter(textColor, Available_0_8) [[nodiscard]] auto text_color() const -> graphics::color::lua_reference;
        lua_setter(textColor, Available_0_8) auto set_text_color(const graphics::color::lua_reference& color) -> void;
        lua_getter(backgroundColor, Available_0_8) [[nodiscard]] auto background_color() const -> graphics::color::lua_reference;
        lua_setter(backgroundColor, Available_0_8) auto set_background_color(const graphics::color::lua_reference& color) -> void;
        lua_getter(hiliteColor, Available_0_8) [[nodiscard]] auto hilite_color() const -> graphics::color::lua_reference;
        lua_setter(hiliteColor, Available_0_8) auto set_hilite_color(const graphics::color::lua_reference& color) -> void;
        lua_getter(outlineColor, Available_0_8) [[nodiscard]] auto outline_color() const -> graphics::color::lua_reference;
        lua_setter(outlineColor, Available_0_8) auto set_outline_color(const graphics::color::lua_reference& color) -> void;
        lua_getter(headingTextColor, Available_0_8) [[nodiscard]] auto heading_text_color() const -> graphics::color::lua_reference;
        lua_setter(headingTextColor, Available_0_8) auto set_heading_text_color(const graphics::color::lua_reference& color) -> void;

        // Calculations & Transformations
        lua_function(rowIndexAtPoint, Available_0_8) auto row_index_at_point(const math::point& p) -> std::int32_t;
        lua_function(scrollUp, Available_0_8) auto scroll_up() -> void;
        lua_function(scrollDown, Available_0_8) auto scroll_down() -> void;

        // Drawing
        lua_function(draw, Available_0_8) auto draw() -> void;

        // Entity
        lua_getter(entity, Available_0_8) [[nodiscard]] auto entity() const -> scene_entity::lua_reference;

        // Delegate & Data Source
        lua_function(reloadData, Available_0_9) auto reload_data() -> void;
        lua_function(columnWidth, Available_0_8) auto column_width(std::int32_t column) const -> std::int32_t;
        lua_function(columnHeading, Available_0_8) auto column_heading(std::int32_t column) const -> std::string;
        lua_getter(dataSource, Available_0_9) auto data_source() const -> luabridge::LuaRef;
        lua_setter(dataSource, Available_0_9) auto set_data_source(const luabridge::LuaRef& data_source) -> void;
        lua_getter(delegate, Available_0_9) auto delegate() const -> luabridge::LuaRef;
        lua_setter(delegate, Available_0_9) auto set_delegate(const luabridge::LuaRef& delegate) -> void;

        // Events
        auto receive_event(const event& e) -> bool override;
        lua_function(receiveEvent, Available_0_9) auto lua_receive_event(const event::lua_reference& e) -> void;

        lua_getter(anchorPoint, Available_0_9) [[nodiscard]] auto anchor_point() const -> layout::axis_origin;
        lua_setter(anchorPoint, Available_0_9) auto set_anchor_point(const layout::axis_origin& origin) -> void;

    private:
        bool m_has_header { false };
        bool m_borders { false };
        bool m_pressed { false };

        struct {
            scene_entity::lua_reference entity { nullptr };
            std::shared_ptr<graphics::canvas> canvas;
        } m_entity;

        struct {
            graphics::color::lua_reference background_color { nullptr };
            graphics::color::lua_reference hilite_color { nullptr };
            graphics::color::lua_reference text_color { nullptr };
            graphics::color::lua_reference outline_color { nullptr };
            graphics::color::lua_reference heading_text_color { nullptr };
            font::reference::lua_reference label_font { nullptr };
        } m_appearance;

        struct {
            bool dirty { true };
            struct {
                std::int32_t row { -1 };
            } selection;
            struct {
                std::int32_t first_visible_row { 1 };
                math::point offset;
            } scroll;
        } m_state;

        struct {
            luabridge::LuaRef on_row_select { nullptr };
            luabridge::LuaRef should_select_row { nullptr };
            luabridge::LuaRef width_for_column { nullptr };
            luabridge::LuaRef height_for_row { nullptr };
            luabridge::LuaRef height_of_header { nullptr };
        } m_delegate;

        struct {
            luabridge::LuaRef number_of_rows { nullptr };
            luabridge::LuaRef number_of_columns { nullptr };
            luabridge::LuaRef heading_for_column { nullptr };
            luabridge::LuaRef value_for_cell { nullptr };
        } m_data_source;

        auto setup(const math::rect& frame) -> void;
        auto redraw_entity() -> void;
        auto bind_internal_events() -> void;

        // DataSource Methods
        [[nodiscard]] auto number_of_rows() const -> std::int32_t;
        [[nodiscard]] auto number_of_columns() const -> std::int32_t;
        [[nodiscard]] auto heading_for_column(std::int32_t column) const -> std::string;
        [[nodiscard]] auto value_for_cell(std::int32_t row, std::int32_t column) const -> std::string;

        // Delegate Methods
        [[nodiscard]] auto should_select_row(std::int32_t row) const -> bool;
        [[nodiscard]] auto width_for_column(std::int32_t column) const -> std::int32_t;
        [[nodiscard]] auto height_for_row(std::int32_t row) const -> std::int32_t;
        [[nodiscard]] auto height_of_header() const -> std::int32_t;
        auto row_selected(std::int32_t i) -> void;
    };
}