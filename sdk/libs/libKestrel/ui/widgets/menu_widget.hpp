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
#include <libKestrel/event/responder/responder_chain.hpp>
#include <libKestrel/ui/entity/scene_entity.hpp>
#include <libKestrel/graphics/canvas/canvas.hpp>
#include <libKestrel/graphics/types/color.hpp>
#include <libKestrel/math/point.hpp>
#include <libKestrel/math/size.hpp>
#include <libKestrel/math/rect.hpp>
#include <libKestrel/font/font.hpp>

namespace kestrel::ui::widgets
{
    struct lua_api(Widget.Menu, Available_0_9) menu_widget: public responder_chain::mouse_responder
    {
    public:
        has_constructable_lua_api(menu_widget);

        lua_constructor(Available_0_9) explicit menu_widget(const lua::vector<std::string>& items, std::int32_t selected_item = -1);

        lua_function(draw, Available_0_9) auto draw() -> void;
        lua_getter(entity, Available_0_9) [[nodiscard]] auto entity() const -> ui::scene_entity::lua_reference;

        lua_getter(rowHeight, Available_0_9) [[nodiscard]] auto row_height() const -> double;
        lua_getter(verticalPadding, Available_0_9) [[nodiscard]] auto vertical_padding() const -> double;
        lua_getter(horizontalPadding, Available_0_9) [[nodiscard]] auto horizontal_padding() const -> double;
        lua_getter(indexOfSelectedItem, Available_0_9) [[nodiscard]] auto index_of_selected_item() const -> std::int32_t;
        lua_getter(selectedItem, Available_0_9) [[nodiscard]] auto selected_item() const -> std::string;
        lua_getter(items, Available_0_9) [[nodiscard]] auto items() const -> lua::vector<std::string>;
        lua_getter(textColor, Available_0_9) [[nodiscard]] auto color() const -> graphics::color::lua_reference;
        lua_getter(backgroundColor, Available_0_9) [[nodiscard]] auto background_color() const -> graphics::color::lua_reference;
        lua_getter(borderColor, Available_0_9) [[nodiscard]] auto border_color() const -> graphics::color::lua_reference;
        lua_getter(selectionColor, Available_0_9) [[nodiscard]] auto selection_color() const -> graphics::color::lua_reference;
        lua_getter(font, Available_0_9) [[nodiscard]] auto font() const -> font::reference::lua_reference;

        lua_setter(rowHeight, Available_0_9) auto set_row_height(double height) -> void;
        lua_setter(verticalPadding, Available_0_9) auto set_vertical_padding(double padding) -> void;
        lua_setter(horizontalPadding, Available_0_9) auto set_horizontal_padding(double padding) -> void;
        lua_setter(indexOfSelectedItem, Available_0_9) auto set_index_of_selected_item(std::int32_t index) -> void;
        lua_setter(items, Available_0_9) auto set_items(const lua::vector<std::string>& items) -> void;
        lua_setter(textColor, Available_0_9) auto set_color(const graphics::color::lua_reference& v) -> void;
        lua_setter(backgroundColor, Available_0_9) auto set_background_color(const graphics::color::lua_reference& v) -> void;
        lua_setter(borderColor, Available_0_9) auto set_border_color(const graphics::color::lua_reference& v) -> void;
        lua_setter(selectionColor, Available_0_9) auto set_selection_color(const graphics::color::lua_reference& v) -> void;
        lua_setter(font, Available_0_9) auto set_font(const font::reference::lua_reference& font) -> void;

        auto present_from_parent(const ui::scene_entity::lua_reference& entity, const std::function<auto()->void>& callback) -> void;
        auto will_close() -> void;

        auto receive_event(const event& e) -> bool override;

    private:
        bool m_dirty { true };
        float m_row_height { 30.f };
        float m_vertical_padding { 10.f };
        float m_horizontal_padding { 7.f };
        float m_indicator_spacing { 8.f };
        std::int32_t m_start_item { 0 };
        std::uint32_t m_max_items { 10 };
        std::uint32_t m_displayed_items { 10 };
        std::int32_t m_selected_item { -1 };
        std::int32_t m_hover_item { -1 };
        lua::vector<std::string> m_items;
        std::unique_ptr<graphics::canvas> m_canvas;
        scene_entity::lua_reference m_entity { nullptr };
        scene_entity::lua_reference m_parent_entity { nullptr };
        bool m_mouse_over { false };
        math::point m_mouse_point { 0, 0 };
        std::function<auto()->void> m_update_callback;

        font::reference::lua_reference m_font { nullptr };
        graphics::color m_color { graphics::color::black_color() };
        graphics::color m_border_color { graphics::color::black_color() };
        graphics::color m_selection_color { graphics::color::blue_color() };
        graphics::color m_background_color { graphics::color::white_color() };

        auto recalculate_entity_size() -> void;
        auto redraw_entity() -> void;
    };
}