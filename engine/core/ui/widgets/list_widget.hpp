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
#include "core/event/responder/responder_chain.hpp"
#include "math/point.hpp"
#include "math/size.hpp"
#include "math/rect.hpp"

namespace ui
{
    struct scene_entity;
}

namespace ui::widgets
{
    struct list_widget: public scripting::lua::object, public responder_chain::mouse_responder, public responder_chain::responder
    {
    public:
        typedef luabridge::RefCountedPtr<list_widget> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

        struct row: public scripting::lua::object
        {
        public:
            typedef luabridge::RefCountedPtr<struct row> lua_reference;

        public:
            row(const luabridge::LuaRef& values);

            [[nodiscard]] auto column_value(int column) const -> std::string;

        private:
            std::vector<std::string> m_columns;
        };

    public:
        list_widget() = default;
        lua_api explicit list_widget(const math::rect& frame);

        lua_api auto row_at_point(const math::point& p) -> row::lua_reference;
        lua_api auto row_index_at_point(const math::point& p) -> std::int32_t;
        lua_api auto on_row_select(const luabridge::LuaRef& callback) -> void;

        lua_api auto column_width(int column) const -> std::int32_t;
        lua_api auto column_heading(int column) const -> std::string;
        lua_api auto set_column_widths(const luabridge::LuaRef& columns) -> void;
        lua_api auto set_column_headings(const luabridge::LuaRef& columns) -> void;
        lua_api auto set_row_items(const luabridge::LuaRef& rows) -> void;

        lua_api auto scroll_up() -> void;
        lua_api auto scroll_down() -> void;

        [[nodiscard]] lua_api auto borders() const -> bool;
        [[nodiscard]] lua_api auto headers() const -> bool;
        [[nodiscard]] lua_api auto current_row() const -> row::lua_reference;
        [[nodiscard]] lua_api auto selected_row() const -> std::int32_t;
        [[nodiscard]] lua_api auto frame() const -> math::rect;

        lua_api auto set_borders(bool f) -> void;
        lua_api auto set_headers(bool f) -> void;
        lua_api auto select_row(std::int32_t row) -> void;
        lua_api auto set_frame(const math::rect& frame) -> void;

        lua_api auto set_text_color(const graphics::color::lua_reference& color) -> void;
        lua_api auto set_background_color(const graphics::color::lua_reference& color) -> void;
        lua_api auto set_hilite_color(const graphics::color::lua_reference& color) -> void;
        lua_api auto set_outline_color(const graphics::color::lua_reference& color) -> void;
        lua_api auto set_heading_text_color(const graphics::color::lua_reference& color) -> void;
        lua_api auto set_font(const std::string& font, int16_t size) -> void;

        lua_api auto draw() -> void;
        [[nodiscard]] lua_api auto entity() const -> std::shared_ptr<scene_entity>;

        auto receive_event(const event& e) -> bool override;

    private:
        bool m_dirty { true };
        bool m_has_header { false };
        bool m_borders { false };
        math::size m_row_size;
        std::uint32_t m_visible_rows { 0 };
        std::string m_label_font { "Geneva" };
        std::int16_t m_font_size { 11 };
        std::vector<row::lua_reference> m_rows;
        std::vector<std::string> m_headings;
        std::vector<double> m_column_widths;
        std::int32_t m_selected_row { -1 };
        math::point m_scroll_offset;
        luabridge::LuaRef m_row_select_callback { nullptr };
        std::shared_ptr<graphics::canvas> m_canvas;
        std::shared_ptr<scene_entity> m_entity;
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