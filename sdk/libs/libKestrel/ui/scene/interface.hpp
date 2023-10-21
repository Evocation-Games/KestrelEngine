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

#include <vector>
#include <libKestrel/resource/reader.hpp>
#include <libKestrel/resource/descriptor.hpp>
#include <libKestrel/ui/scene/control_definition.hpp>
#include <libKestrel/ui/alignment.hpp>
#include <libKestrel/ui/types/value/value.hpp>
#include <libKestrel/ui/types/action/action.hpp>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/math/point.hpp>
#include <libKestrel/math/size.hpp>
#include <libKestrel/math/rect.hpp>
#include <libKestrel/resource/macro.hpp>
#include <libUI/format/interface/interface.hpp>

namespace kestrel::ui
{
    enum lua_api(UI.SceneInterface.Flags, Available_0_8) scene_interface_flags : std::uint16_t
    {
        // ImGui Flags
        use_imgui = ::ui::format::interface::flags::imgui,
        imgui_show_title = ::ui::format::interface::flags::shows_title_bar,
        imgui_close_button = ::ui::format::interface::flags::shows_close_button,

        // Scene
        scene_passthrough = ::ui::format::interface::flags::passthrough,
        vertical_flow_layout = ::ui::format::interface::flags::vertical_flow_layout,
        is_dialog = ::ui::format::interface::flags::dialog,
        centered = ::ui::format::interface::flags::centered,

        // Misc
        force_macintosh_dialog = 0xF00F,
    };

    enum lua_api(UI.SceneInterface.AnchorPoint, Available_0_9) scene_interface_item_anchor_point : std::uint8_t
    {
        top_left = 0x00,
        top_center = 0x01,
        top_right = 0x02,
        middle_left = 0x10,
        center = 0x11,
        middle_right = 0x12,
        bottom_left = 0x20,
        bottom_center = 0x21,
        bottom_right = 0x22
    };

    enum lua_api(UI.SceneInterface.ScalingMode, Available_0_9) scene_interface_item_scaling_mode : std::uint8_t
    {
        normal = 0x00,
        aspect_fill = 0x01,
        aspect_fit = 0x02,
        scaling_fill = 0x10,
        scaling_fit = 0x11,
    };

    struct lua_api(UI.SceneInterface.Item, Available_0_8) scene_interface_item
    {
    public:
        has_constructable_lua_api(scene_interface_item);

        scene_interface_item() = default;

        lua_getter(type, Available_0_8) [[nodiscard]] auto type() const -> enum ui::control_type { return m_type; }
        auto set_type(enum ui::control_type type) -> void { m_type = type; }

        lua_getter(identifier, Available_0_8) [[nodiscard]] auto identifier() const -> std::string { return m_identifier; }
        auto set_identifier(const std::string& id) -> void { m_identifier = id; }

        lua_getter(frame, Available_0_8) [[nodiscard]] auto frame() const -> math::rect { return m_frame; }
        auto set_frame(const math::rect& frame) -> void { m_frame = frame; }

        lua_getter(fillsParentContainer, Available_0_8) [[nodiscard]] auto fills_parent_container() const -> bool { return m_fills_parent_container; }
        auto set_fills_parent_container() -> void { m_fills_parent_container = true; }

        lua_getter(childCount, Available_0_8) [[nodiscard]] auto child_count() const -> std::size_t { return m_items.size(); }
        lua_function(childAt, Available_0_8) [[nodiscard]] auto child_at(std::int32_t idx) const -> lua_reference { return m_items[idx]; }
        auto add_child(const lua_reference& item) -> void { m_items.emplace_back(item); }
        auto set_children(std::vector<lua_reference> children) -> void { m_items = std::move(children); }

        lua_getter(value, Available_0_8) [[nodiscard]] auto value() const -> ui::value { return m_value; }
        auto set_value(const ui::value& value) -> void { m_value = value; }

        lua_getter(action, Available_0_9) [[nodiscard]] auto action() const -> ui::action { return m_action; }
        auto set_action(const ui::action& action) -> void { m_action = action; }

        lua_getter(script, Available_0_8) [[nodiscard]] auto lua_script() const -> std::string { return m_lua_script; }
        auto set_lua_script(const std::string& script) -> void { m_lua_script = script; }

        lua_getter(anchorPoint, Available_0_9) [[nodiscard]] auto anchor_point() const -> enum ui::scene_interface_item_anchor_point { return m_anchor_point; }
        auto set_anchor_point(std::uint8_t anchor_point) -> void { m_anchor_point = static_cast<enum ui::scene_interface_item_anchor_point>(anchor_point); }

        lua_getter(scalingMode, Available_0_9) [[nodiscard]] auto scaling_mode() const -> enum ui::scene_interface_item_scaling_mode { return m_scaling_mode; }
        auto set_scaling_mode(std::uint8_t mode) -> void { m_scaling_mode = static_cast<enum ui::scene_interface_item_scaling_mode>(mode); }

        lua_getter(condition, Available_0_8) [[nodiscard]] auto condition() const -> std::string { return m_condition; }
        auto set_condition(const std::string& script) -> void { m_condition = script; }

        lua_getter(backgroundColor, Available_0_9) [[nodiscard]] auto background_color() const -> graphics::color::lua_reference { return m_background_color; }
        auto set_background_color(const graphics::color::lua_reference& value) -> void { m_background_color = value; }

        lua_getter(borderColor, Available_0_9) [[nodiscard]] auto border_color() const -> graphics::color::lua_reference { return m_border_color; }
        auto set_border_color(const graphics::color::lua_reference& value) -> void { m_border_color = value; }

        lua_getter(secondaryColor, Available_0_9) [[nodiscard]] auto secondary_color() const -> graphics::color::lua_reference { return m_secondary_color; }
        auto set_secondary_color(const graphics::color::lua_reference& value) -> void { m_secondary_color = value; }

        lua_getter(color, Available_0_9) [[nodiscard]] auto color() const -> graphics::color::lua_reference { return m_color; }
        auto set_color(const graphics::color::lua_reference& value) -> void { m_color = value; }

        lua_getter(selectionColor, Available_0_9) [[nodiscard]] auto selection_color() const -> graphics::color::lua_reference { return m_selection_color; }
        auto set_selection_color(const graphics::color::lua_reference& value) -> void { m_selection_color = value; }

        lua_getter(font, Available_0_9) [[nodiscard]] auto font() const -> std::string { return m_font; }
        auto set_font(const std::string& value) -> void { m_font = value; }

        lua_getter(fontSize, Available_0_9) [[nodiscard]] auto font_size() const -> std::int16_t { return m_font_size; }
        auto set_font_size(std::int16_t value) -> void { m_font_size = value; }

        lua_getter(alignment, Available_0_9) [[nodiscard]] auto alignment() const -> ui::horizontal_alignment { return m_alignment; }
        auto set_alignment(ui::horizontal_alignment value) -> void { m_alignment = value; }

        lua_getter(weight, Available_0_9) [[nodiscard]] auto weight() const -> std::int16_t { return m_weight; }
        auto set_weight(std::int16_t weight) -> void { m_weight = weight; }

        lua_getter(hidden, Available_0_9) [[nodiscard]] auto hidden() const -> bool { return m_hidden; }
        auto set_hidden(bool hidden) -> void { m_hidden = hidden; }

        lua_getter(disabled, Available_0_9) [[nodiscard]] auto disabled() const -> bool { return m_disabled; }
        auto set_disabled(bool disabled) -> void { m_disabled = disabled; }

    private:
        enum ui::control_type m_type { ui::control_type::none };
        enum ui::scene_interface_item_anchor_point m_anchor_point { ui::scene_interface_item_anchor_point::top_left };
        enum ui::scene_interface_item_scaling_mode m_scaling_mode { ui::scene_interface_item_scaling_mode::normal };
        std::string m_identifier;
        math::rect m_frame;
        bool m_fills_parent_container { false };
        std::vector<lua_reference> m_items;
        ui::value m_value;
        ui::action m_action;
        std::string m_lua_script;
        std::string m_condition;
        std::string m_font;
        std::int16_t m_font_size { 12 };
        std::int16_t m_weight { 1 };
        bool m_hidden { false };
        bool m_disabled { false };
        graphics::color::lua_reference m_background_color { graphics::color::black_color_ref() };
        graphics::color::lua_reference m_border_color { graphics::color::white_color_ref() };
        graphics::color::lua_reference m_color { graphics::color::white_color_ref() };
        graphics::color::lua_reference m_selection_color { graphics::color::green_color_ref() };
        graphics::color::lua_reference m_secondary_color { graphics::color::dark_grey_color_ref() };
        ui::horizontal_alignment m_alignment { ui::horizontal_alignment::left };
    };

    struct lua_api(UI.SceneInterface.Dialog, Available_0_8) scene_interface
    {
    public:
        has_constructable_lua_api(scene_interface);
        is_resource_type("scïn");

        lua_constructor(Available_0_8) explicit scene_interface(const resource::descriptor::lua_reference& ref);

        lua_getter(itemCount, Available_0_8) [[nodiscard]] auto child_count() const -> std::uint32_t { return m_items.size(); }
        lua_function(itemAt, Available_0_8) [[nodiscard]] auto child_at(std::uint32_t idx) const -> scene_interface_item::lua_reference { return m_items[idx]; }

        lua_getter(name, Available_0_8) [[nodiscard]] auto name() const -> std::string { return m_name; }
        lua_getter(flags, Available_0_8) [[nodiscard]] auto flags() const -> enum scene_interface_flags { return m_flags; }
        lua_getter(size, Available_0_8) [[nodiscard]] auto scene_size() const -> math::size { return m_scene_size; }

        lua_getter(usesImGui, Available_0_8) [[nodiscard]] auto uses_imgui() const -> bool { return m_flags & scene_interface_flags::use_imgui; }
        lua_getter(showsTitle, Available_0_8) [[nodiscard]] auto shows_title_in_imgui() const -> bool { return m_flags & scene_interface_flags::imgui_show_title; }
        lua_getter(showsCloseButton, Available_0_8) [[nodiscard]] auto shows_close_button_in_imgui() const -> bool { return m_flags & scene_interface_flags::imgui_close_button; }
        lua_getter(hasPassthrough, Available_0_8) [[nodiscard]] auto has_scene_passthrough() const -> bool { return m_flags & scene_interface_flags::scene_passthrough; }

        lua_getter(hasStretchBackground, Available_0_9) [[nodiscard]] auto has_stretch_background() const -> bool { return m_stretch_background; }
        lua_getter(background, Available_0_9) [[nodiscard]] auto background() const -> resource::descriptor::lua_reference { return m_background; }
        lua_getter(backgroundTop, Available_0_9) [[nodiscard]] auto background_top() const -> resource::descriptor::lua_reference { return m_background_top; }
        lua_getter(backgroundBottom, Available_0_9) [[nodiscard]] auto background_bottom() const -> resource::descriptor::lua_reference { return m_background_bottom; }

    private:
        std::string m_name;
        enum scene_interface_flags m_flags;
        math::size m_scene_size;
        std::vector<scene_interface_item::lua_reference> m_items;
        bool m_has_background { false };
        bool m_stretch_background { false };
        resource::descriptor::lua_reference m_background { nullptr };
        resource::descriptor::lua_reference m_background_top { nullptr };
        resource::descriptor::lua_reference m_background_bottom { nullptr };

        auto parse_item_list(resource::reader& reader, std::uint32_t count = UINT32_MAX) -> std::vector<scene_interface_item::lua_reference>;
    };

}