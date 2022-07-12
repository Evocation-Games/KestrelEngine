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

#include "core/ui/entity/scene_entity.hpp"
#include "core/ui/imgui/widget.hpp"
#include "scripting/state.hpp"
#include "util/hint.hpp"
#include "math/rect.hpp"
#include "core/graphics/common/color.hpp"

namespace ui
{
    class control_definition : public scripting::lua::object
    {
    public:
        enum class mode
        {
            scene, imgui
        };

        enum class type : uint8_t
        {
            none, button, label, text_area, image, text_field, checkbox, list,
            scroll_area, grid, labeled_list, canvas, sprite, popup_button, slider,
            table, box, radio, tabbar, separator
        };

        enum class anchor : uint8_t
        {
            left = 0x01,
            top = 0x02,
            right = 0x04,
            bottom = 0x08,

            vertical = 0x0A,
            horizontal = 0x05
        };

        enum class alignment : uint8_t
        {
            horizontal_left = 0x01, horizontal_center = 0x02, horizontal_right = 0x04,
            vertical_top = 0x10, vertical_middle = 0x20, vertical_bottom = 0x40,
        };

        typedef luabridge::RefCountedPtr<control_definition> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        lua_api explicit control_definition(const math::rect& frame, uint32_t type);

        lua_api auto construct(uint32_t mode) -> void;

        auto set_access_flag(bool access) -> void { m_accessed = access; };
        [[nodiscard]] auto access_flag() const -> bool { return m_accessed; };

        [[nodiscard]] lua_api auto type() const -> enum type { return m_type; }

        [[nodiscard]] lua_api auto frame() const -> math::rect { return m_frame; }
        lua_api auto set_frame(const math::rect& frame) -> void { m_frame = frame; }
        lua_api auto set_frame_size(double width, double height) -> void { m_frame.size = { width, height }; }

        [[nodiscard]] lua_api auto entity() const -> scene_entity::lua_reference  { return m_entity; }
        [[nodiscard]] lua_api auto control() const -> luabridge::LuaRef { return m_control; }
        [[nodiscard]] auto has_entity() const -> bool { return m_entity.get() != nullptr; }
        [[nodiscard]] auto has_control() const -> bool { return m_control.state() != nullptr; }

        [[nodiscard]] lua_api auto string_value() const -> std::string { return m_string_value; }
        lua_api auto set_string_value(const std::string& title) -> void { m_string_value = title; }

        [[nodiscard]] lua_api auto body_text_value() const -> std::string { return m_body_text; }
        lua_api auto set_body_text_value(const std::string& body_text) -> void { m_body_text = body_text; update(); }

        [[nodiscard]] lua_api auto text_color() const -> graphics::color::lua_reference { return m_text_color; }
        lua_api auto set_text_color(const graphics::color::lua_reference& color) -> void { m_text_color = color; }

        [[nodiscard]] lua_api auto secondary_text_color() const -> graphics::color::lua_reference  { return m_secondary_text_color; }
        lua_api auto set_secondary_text_color(const graphics::color::lua_reference& color) -> void { m_secondary_text_color = color; }

        [[nodiscard]] lua_api auto text_font() const -> std::string { return m_font_name; }
        lua_api auto set_text_font(const std::string& font) -> void { m_font_name = font; }

        [[nodiscard]] lua_api auto text_size() const -> uint32_t { return m_font_size; }
        lua_api auto set_text_size(uint32_t size) -> void { m_font_size = size; }

        lua_api auto set_text_font_and_size(const std::string& font, uint32_t size) -> void;

        [[nodiscard]] lua_api auto alignment() const -> uint8_t { return static_cast<uint8_t>(m_alignment); }
        lua_api auto set_alignment(uint8_t size) -> void { m_alignment = static_cast<enum alignment>(size); }

        [[nodiscard]] lua_api auto action() const -> luabridge::LuaRef { return m_action; }
        lua_api auto set_action(const luabridge::LuaRef& action) -> void { m_action = action; }

        [[nodiscard]] lua_api auto image() const -> luabridge::LuaRef { return m_image; }
        lua_api auto set_image(const luabridge::LuaRef& image) -> void { m_image = image; }

        [[nodiscard]] lua_api auto content_offset() const -> math::point { return m_content_offset; }
        lua_api auto set_content_offset(const math::point& content_offset) -> void { m_content_offset = content_offset; update(); }

        [[nodiscard]] lua_api auto content_size() const -> math::size { return m_content_size; }
        lua_api auto set_content_size(const math::size& content_size) -> void { m_content_size = content_size; }

        [[nodiscard]] lua_api auto grid_size() const -> math::size { return m_grid_size; }
        lua_api auto set_grid_size(const math::size& grid_size) -> void { m_grid_size = grid_size; }

        [[nodiscard]] lua_api auto item_count() const -> uint32_t { return m_item_count; }
        lua_api auto set_item_count(uint32_t item_count) -> void { m_item_count = item_count; }

        [[nodiscard]] lua_api auto selected_index() const -> int32_t { return m_selected_index; }
        lua_api auto set_selected_index(int32_t index) -> void { m_selected_index = index; }

        [[nodiscard]] lua_api auto column_spacing() const -> float { return m_column_spacings; }
        lua_api auto set_column_spacing(float spacing) -> void { m_column_spacings = spacing; }

        [[nodiscard]] lua_api auto disabled() const -> bool { return m_disabled; }
        lua_api auto set_disabled(bool f) -> void { m_disabled = f; update(); }

        [[nodiscard]] lua_api auto continuous() const -> bool { return m_continuous; }
        lua_api auto set_continuous(bool f) -> void { m_continuous = f; }

        [[nodiscard]] lua_api auto column_widths() const -> luabridge::LuaRef { return m_column_widths; }
        lua_api auto set_column_widths(const luabridge::LuaRef& widths) -> void { m_column_widths = widths; }

        [[nodiscard]] lua_api auto column_headings() const -> luabridge::LuaRef { return m_column_headings; }
        lua_api auto set_column_headings(const luabridge::LuaRef& headings) -> void { m_column_headings = headings; }

        lua_api auto set_columns(const luabridge::LuaRef& widths, const luabridge::LuaRef& headings) -> void;

        [[nodiscard]] lua_api auto borders() const -> bool { return m_borders; }
        lua_api auto set_borders(bool f) -> void { m_borders = f; }

        [[nodiscard]] lua_api auto render() const -> luabridge::LuaRef { return m_render; }
        lua_api auto set_render(const luabridge::LuaRef& callback) -> void { m_render = callback; }

        [[nodiscard]] auto entity_index() const -> uint32_t { return m_entity_index; }
        auto set_entity_index(uint32_t index) -> void { m_entity_index = index; }

        auto set_items(const luabridge::LuaRef& items) -> void { m_items = items; update(); }

        lua_api auto update() -> void;

        lua_api auto scroll_up() -> void;
        lua_api auto scroll_down() -> void;

        [[nodiscard]] lua_api auto can_scroll_up() const -> bool;
        [[nodiscard]] lua_api auto can_scroll_down() const -> bool;

    private:
        bool m_accessed { false };
        enum type m_type { type::none };
        math::rect m_frame;
        scene_entity::lua_reference m_entity { nullptr };
        luabridge::LuaRef m_control { nullptr };
        luabridge::LuaRef m_widget { nullptr };
        std::string m_string_value;
        std::string m_body_text;
        std::string m_font_name { "Geneva" };
        uint32_t m_font_size { 11 };
        luabridge::LuaRef m_image { nullptr };
        luabridge::LuaRef m_items { nullptr };
        luabridge::LuaRef m_action { nullptr };
        graphics::color::lua_reference m_text_color { new graphics::color(255, 255) };
        graphics::color::lua_reference m_secondary_text_color { new graphics::color(255, 255) };
        math::point m_content_offset;
        math::size m_content_size;
        bool m_can_scroll_up { false };
        bool m_can_scroll_down { false };
        math::size m_grid_size;
        uint32_t m_item_count { 0 };
        int32_t m_selected_index { 1 };
        float m_column_spacings { 0 };
        bool m_disabled { false };
        bool m_continuous { false };
        luabridge::LuaRef m_column_widths { nullptr };
        luabridge::LuaRef m_column_headings { nullptr };
        bool m_borders { false };
        luabridge::LuaRef m_render { nullptr };
        bool m_absolute_frame { true };
        uint32_t m_entity_index { UINT32_MAX };
        enum alignment m_alignment {
            static_cast<enum alignment>(static_cast<int>(alignment::horizontal_left) | static_cast<int>(alignment::vertical_middle))
        };

        auto construct_scene_entity() -> void;
        auto construct_imgui_control() -> void;

        auto disable_absolute_frame() -> void { m_absolute_frame = false; }
    };

    // MARK: - Bitwise Operators

    inline auto operator& (control_definition::anchor lhs, control_definition::anchor rhs) -> control_definition::anchor
    {
        return static_cast<control_definition::anchor>(static_cast<int>(lhs) & static_cast<int>(rhs));
    }

}
