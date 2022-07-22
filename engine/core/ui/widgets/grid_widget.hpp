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
#include "core/ui/entity/scene_entity.hpp"

namespace ui::widgets
{
    struct grid_widget: public scripting::lua::object, public responder_chain::mouse_responder, public responder_chain::responder
    {
    public:
        typedef luabridge::RefCountedPtr<grid_widget> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

        struct item: public scripting::lua::object
        {
        public:
            typedef luabridge::RefCountedPtr<struct item> lua_reference;

        public:
            lua_api item(const luabridge::LuaRef& icon, const std::string& title, const std::string& subtitle, const std::string& corner);

            [[nodiscard]] lua_api auto has_icon() const -> bool { return m_icon.get() != nullptr; }
            [[nodiscard]] lua_api auto icon() const -> asset::static_image::lua_reference { return m_icon; }
            [[nodiscard]] lua_api auto title() const -> std::string { return m_title; }
            [[nodiscard]] lua_api auto subtitle() const -> std::string { return m_subtitle; }
            [[nodiscard]] lua_api auto corner() const -> std::string { return m_corner; }

            [[nodiscard]] auto lines() const -> const std::vector<std::string>& { return m_lines; }

        private:
            asset::static_image::lua_reference m_icon { nullptr };
            std::string m_title;
            std::string m_subtitle;
            std::string m_corner;
            std::vector<std::string> m_lines;
        };

    public:
        grid_widget() = default;
        lua_api explicit grid_widget(const math::rect& frame);

        [[nodiscard]] lua_api auto item_index_at_point(const math::point& p) -> std::int32_t;

        [[nodiscard]] lua_api auto selected_item() const -> std::int32_t;
        [[nodiscard]] lua_api auto frame() const -> math::rect;
        [[nodiscard]] lua_api auto font() const -> ui::font::reference::lua_reference;
        [[nodiscard]] lua_api auto text_color() const -> graphics::color::lua_reference;
        [[nodiscard]] lua_api auto secondary_text_color() const -> graphics::color::lua_reference;
        [[nodiscard]] lua_api auto background_color() const -> graphics::color::lua_reference;
        [[nodiscard]] lua_api auto hilite_color() const -> graphics::color::lua_reference;
        [[nodiscard]] lua_api auto outline_color() const -> graphics::color::lua_reference;

        lua_api auto on_item_select(const luabridge::LuaRef& callback) -> void;

        lua_api auto select_item(std::int32_t item) -> void;
        lua_api auto set_frame(const math::rect& frame) -> void;

        lua_api auto set_text_color(const graphics::color::lua_reference& color) -> void;
        lua_api auto set_secondary_text_color(const graphics::color::lua_reference& color) -> void;
        lua_api auto set_background_color(const graphics::color::lua_reference& color) -> void;
        lua_api auto set_hilite_color(const graphics::color::lua_reference& color) -> void;
        lua_api auto set_outline_color(const graphics::color::lua_reference& color) -> void;
        lua_api auto set_font(const ui::font::reference::lua_reference& font) -> void;

        lua_api auto set_items(const luabridge::LuaRef& items) -> void;

        lua_api auto draw() -> void;
        [[nodiscard]] lua_api auto entity() const -> scene_entity::lua_reference;

        auto receive_event(const event& e) -> bool override;

        lua_api auto scroll_up() -> void;
        lua_api auto scroll_down() -> void;

    private:
        bool m_dirty { true };
        bool m_pressed { false };
        ui::font::reference::lua_reference m_label_font { nullptr };
        luabridge::LuaRef m_item_select_callback { nullptr };
        std::vector<item::lua_reference> m_items;
        std::shared_ptr<graphics::canvas> m_canvas;
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