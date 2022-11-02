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
    struct lua_api(Widget.GridItem, Available_0_8) grid_item
    {
    public:
        has_constructable_lua_api(grid_item);

        luatool_type_fix(const luabridge::LuaRef&, icon)
        lua_constructor(Available_0_8) grid_item(const luabridge::LuaRef& icon, const std::string& title, const std::string& subtitle, const std::string& corner);

        lua_getter(hasIcon, Available_0_8) [[nodiscard]] auto has_icon() const -> bool { return m_icon.get() != nullptr; }
        lua_getter(icon, Available_0_8) [[nodiscard]] auto icon() const -> image::static_image::lua_reference { return m_icon; }
        lua_getter(title, Available_0_8) [[nodiscard]] auto title() const -> std::string { return m_title; }
        lua_getter(subtitle, Available_0_8) [[nodiscard]] auto subtitle() const -> std::string { return m_subtitle; }
        lua_getter(corner, Available_0_8) [[nodiscard]] auto corner() const -> std::string { return m_corner; }

        lua_getter(lines, Available_0_8) [[nodiscard]] auto lines() const -> lua::vector<std::string> { return m_lines; }

    private:
        image::static_image::lua_reference m_icon { nullptr };
        std::string m_title;
        std::string m_subtitle;
        std::string m_corner;
        lua::vector<std::string> m_lines;
    };

    struct lua_api(Widget.Grid, Available_0_8) grid_widget: public responder_chain::mouse_responder, public responder_chain::responder
    {
    public:
        has_constructable_lua_api(grid_widget);

        grid_widget() = default;
        lua_constructor(Available_0_8) explicit grid_widget(const math::rect& frame);

        lua_function(itemAtPoint, Available_0_8) [[nodiscard]] auto item_index_at_point(const math::point& p) -> std::int32_t;

        lua_getter(selectedItem, Available_0_8) [[nodiscard]] auto selected_item() const -> std::int32_t;
        lua_getter(frame, Available_0_8) [[nodiscard]] auto frame() const -> math::rect;
        lua_getter(font, Available_0_8) [[nodiscard]] auto font() const -> font::reference::lua_reference;
        lua_getter(textColor, Available_0_8) [[nodiscard]] auto text_color() const -> graphics::color::lua_reference;
        lua_getter(secondaryTextColor, Available_0_8) [[nodiscard]] auto secondary_text_color() const -> graphics::color::lua_reference;
        lua_getter(backgroundColor, Available_0_8) [[nodiscard]] auto background_color() const -> graphics::color::lua_reference;
        lua_getter(hiliteColor, Available_0_8) [[nodiscard]] auto hilite_color() const -> graphics::color::lua_reference;
        lua_getter(outlineColor, Available_0_8) lua_getter(gridColor, Available_0_8) [[nodiscard]] auto outline_color() const -> graphics::color::lua_reference;

        lua_function(onItemSelect, Available_0_8) auto on_item_select(const luabridge::LuaRef& callback) -> void;

        lua_setter(selectedItem, Available_0_8) auto select_item(std::int32_t item) -> void;
        lua_setter(frame, Available_0_8) auto set_frame(const math::rect& frame) -> void;
        lua_setter(textColor, Available_0_8) auto set_text_color(const graphics::color::lua_reference& color) -> void;
        lua_setter(secondaryTextColor, Available_0_8) auto set_secondary_text_color(const graphics::color::lua_reference& color) -> void;
        lua_setter(backgroundColor, Available_0_8) auto set_background_color(const graphics::color::lua_reference& color) -> void;
        lua_setter(hiliteColor, Available_0_8) auto set_hilite_color(const graphics::color::lua_reference& color) -> void;
        lua_setter(outlineColor, Available_0_8) lua_setter(gridColor, Available_0_8) auto set_outline_color(const graphics::color::lua_reference& color) -> void;
        lua_setter(font, Available_0_8) auto set_font(const font::reference::lua_reference& font) -> void;

        lua_function(setCellDrawingFunction, Available_0_8) auto set_cell_drawing_function(const luabridge::LuaRef& drawing_function) -> void;

        lua_function(setItems, Available_0_8) auto set_items(const luabridge::LuaRef& items) -> void;

        lua_function(draw, Available_0_8) auto draw() -> void;
        lua_getter(entity, Available_0_8) [[nodiscard]] auto entity() const -> scene_entity::lua_reference;

        auto receive_event(const event& e) -> bool override;

        lua_function(scrollUp, Available_0_8) auto scroll_up() -> void;
        lua_function(scrollDown, Available_0_8) auto scroll_down() -> void;

    private:
        bool m_dirty { true };
        bool m_pressed { false };
        font::reference::lua_reference m_label_font { nullptr };
        luabridge::LuaRef m_item_select_callback { nullptr };
        luabridge::LuaRef m_cell_drawing_function { nullptr };
        std::vector<grid_item::lua_reference> m_items;
        graphics::canvas::lua_reference m_canvas { nullptr };
        scene_entity::lua_reference m_entity { nullptr };
        std::int32_t m_selected_item { -1 };
        std::int32_t m_scroll_offset { 0 };
        math::size m_grid { 4, 5 };
        graphics::color::lua_reference m_background_color { nullptr };
        graphics::color::lua_reference m_hilite_color { nullptr };
        graphics::color::lua_reference m_text_color { nullptr };
        graphics::color::lua_reference m_secondary_text_color { nullptr };
        graphics::color::lua_reference m_outline_color { nullptr };
        math::size m_cell_size;

        auto setup(const math::rect& frame) -> void;
        auto redraw_entity() -> void;
        auto bind_internal_events() -> void;
    };
}