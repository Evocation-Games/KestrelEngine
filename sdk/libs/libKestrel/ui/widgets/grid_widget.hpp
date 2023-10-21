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

namespace kestrel::ui::widgets
{
    struct lua_api(Widget.Grid, Available_0_8) grid_widget: public responder_chain::mouse_responder, public responder_chain::responder
    {
    public:
        has_constructable_lua_api(grid_widget);

        grid_widget() = default;

        luatool_type_fix(const math::rect&, frame)
        lua_constructor(Available_0_8) explicit grid_widget(const math::rect& frame);

        // Framing & Position
        lua_getter(frame, Available_0_8) [[nodiscard]] auto frame() const -> math::rect;
        lua_setter(frame, Available_0_8) auto set_frame(const math::rect& frame) -> void;

        // Selection & State
        lua_getter(selectedCell, Available_0_8) [[nodiscard]] auto selected_item() const -> std::int32_t;
        lua_setter(selectedCell, Available_0_8) auto select_item(std::int32_t item) -> void;

        // Appearance
        lua_getter(textColor, Available_0_8) [[nodiscard]] auto text_color() const -> graphics::color::lua_reference;
        lua_setter(textColor, Available_0_8) auto set_text_color(const graphics::color::lua_reference& color) -> void;
        lua_getter(secondaryTextColor, Available_0_8) [[nodiscard]] auto secondary_text_color() const -> graphics::color::lua_reference;
        lua_setter(secondaryTextColor, Available_0_8) auto set_secondary_text_color(const graphics::color::lua_reference& color) -> void;
        lua_getter(backgroundColor, Available_0_8) [[nodiscard]] auto background_color() const -> graphics::color::lua_reference;
        lua_setter(backgroundColor, Available_0_8) auto set_background_color(const graphics::color::lua_reference& color) -> void;
        lua_getter(hiliteColor, Available_0_8) [[nodiscard]] auto hilite_color() const -> graphics::color::lua_reference;
        lua_setter(hiliteColor, Available_0_8) auto set_hilite_color(const graphics::color::lua_reference& color) -> void;
        lua_getter(outlineColor, Available_0_8) [[nodiscard]] auto outline_color() const -> graphics::color::lua_reference;
        lua_setter(outlineColor, Available_0_8) auto set_outline_color(const graphics::color::lua_reference& color) -> void;
        lua_getter(font, Available_0_8) [[nodiscard]] auto font() const -> font::reference::lua_reference;
        lua_setter(font, Available_0_8) auto set_font(const font::reference::lua_reference& font) -> void;

        // Calculations & Transformations
        lua_function(cellIndexAtPoint, Available_0_8) [[nodiscard]] auto cell_index_at_point(const math::point& p) const -> std::int32_t;
        lua_function(cellAtPoint, Available_0_8) auto cell_at_point(const math::point& p) -> luabridge::LuaRef;
        lua_function(scrollUp, Available_0_8) auto scroll_up() -> void;
        lua_function(scrollDown, Available_0_8) auto scroll_down() -> void;

        // Drawing
        lua_function(draw, Available_0_8) auto draw() -> void;

        // Entity
        lua_getter(entity, Available_0_8) [[nodiscard]] auto entity() const -> scene_entity::lua_reference;

        // Delegate & DataSource
        lua_function(reloadData, Available_0_9) auto reload_data() -> void;
        lua_function(cellOrigin, Available_0_9) [[nodiscard]] auto cell_origin(std::int32_t cell) const -> math::point;
        lua_function(cellSize, Available_0_9) [[nodiscard]] auto cell_size(std::int32_t cell) const -> math::size;
        lua_getter(dataSource, Available_0_9) [[nodiscard]] auto data_source() const -> luabridge::LuaRef;
        lua_setter(dataSource, Available_0_9) auto set_data_source(const luabridge::LuaRef& data_source) -> void;
        lua_getter(delegate, Available_0_9) [[nodiscard]] auto delegate() const -> luabridge::LuaRef;
        lua_setter(delegate, Available_0_9) auto set_delegate(const luabridge::LuaRef& delegate) -> void;

        // Events
        auto receive_event(const event& e) -> bool override;

        lua_getter(anchorPoint, Available_0_9) [[nodiscard]] auto anchor_point() const -> layout::axis_origin;
        lua_setter(anchorPoint, Available_0_9) auto set_anchor_point(const layout::axis_origin& origin) -> void;
        
    private:
        struct {
            scene_entity::lua_reference entity { nullptr };
            graphics::canvas::lua_reference canvas { nullptr };
        } m_entity;

        struct {
            font::reference::lua_reference label_font { nullptr };
            graphics::color::lua_reference background_color { nullptr };
            graphics::color::lua_reference hilite_color { nullptr };
            graphics::color::lua_reference text_color { nullptr };
            graphics::color::lua_reference secondary_text_color { nullptr };
            graphics::color::lua_reference outline_color { nullptr };
        } m_appearance;

        struct {
            bool dirty { true };
            bool pressed { false };
            struct {
                std::int32_t item { -1 };
            } selection;
            struct {
                math::point offset { 0 };
            } scroll;
        } m_state;

        struct {
            luabridge::LuaRef on_cell_select { nullptr };
            luabridge::LuaRef should_select_cell { nullptr };
            luabridge::LuaRef size_for_cell { nullptr };
            luabridge::LuaRef draw_cell { nullptr };
            luabridge::LuaRef title_key { nullptr };
            luabridge::LuaRef subtitle_key { nullptr };
            luabridge::LuaRef icon_key { nullptr };
        } m_delegate;

        struct {
            luabridge::LuaRef number_of_items { nullptr };
            luabridge::LuaRef value_for_cell { nullptr };
        } m_data_source;

        auto setup(const math::rect& frame) -> void;
        auto redraw_entity() -> void;
        auto bind_internal_events() -> void;

        // DataSource Methods
        [[nodiscard]] auto number_of_items() const -> std::uint32_t;
        [[nodiscard]] auto value_for_cell(std::int32_t index) const -> luabridge::LuaRef;

        // Delegate Method
        [[nodiscard]] auto should_select_cell(std::int32_t cell) const -> bool;
        [[nodiscard]] auto title_key() const -> std::string;
        [[nodiscard]] auto subtitle_key() const -> std::string;
        [[nodiscard]] auto icon_key() const -> std::string;
        auto draw_cell(math::rect bounds, const luabridge::LuaRef& value) -> void;
        auto draw_default_cell(math::rect bounds, const luabridge::LuaRef& value) -> void;
        auto cell_selected(std::int32_t i) -> void;

    };
}