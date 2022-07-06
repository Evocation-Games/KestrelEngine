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

#include <string>
#include <sstream>
#include <iterator>
#include "core/ui/control_definition.hpp"
#include "core/environment.hpp"
#include "core/ui/imgui/imgui.hpp"

#include "core/ui/widgets/button_widget.hpp"
#include "core/ui/widgets/label_widget.hpp"
#include "core/ui/widgets/image_widget.hpp"
#include "core/ui/widgets/textarea_widget.hpp"

// MARK: - Enumeration Types

static auto s_control_definition_none = static_cast<uint32_t>(ui::control_definition::type::none);
static auto s_control_definition_button = static_cast<uint32_t>(ui::control_definition::type::button);
static auto s_control_definition_label = static_cast<uint32_t>(ui::control_definition::type::label);
static auto s_control_definition_text_area = static_cast<uint32_t>(ui::control_definition::type::text_area);
static auto s_control_definition_image = static_cast<uint32_t>(ui::control_definition::type::image);
static auto s_control_definition_text_field = static_cast<uint32_t>(ui::control_definition::type::text_field);
static auto s_control_definition_checkbox = static_cast<uint32_t>(ui::control_definition::type::checkbox);
static auto s_control_definition_list = static_cast<uint32_t>(ui::control_definition::type::list);
static auto s_control_definition_scroll_area = static_cast<uint32_t>(ui::control_definition::type::scroll_area);
static auto s_control_definition_grid = static_cast<uint32_t>(ui::control_definition::type::grid);
static auto s_control_definition_labeled_list = static_cast<uint32_t>(ui::control_definition::type::labeled_list);
static auto s_control_definition_canvas = static_cast<uint32_t>(ui::control_definition::type::canvas);
static auto s_control_definition_sprite = static_cast<uint32_t>(ui::control_definition::type::sprite);
static auto s_control_definition_popup_button = static_cast<uint32_t>(ui::control_definition::type::popup_button);
static auto s_control_definition_slider = static_cast<uint32_t>(ui::control_definition::type::slider);
static auto s_control_definition_table = static_cast<uint32_t>(ui::control_definition::type::table);
static auto s_control_definition_box = static_cast<uint32_t>(ui::control_definition::type::box);
static auto s_control_definition_radio = static_cast<uint32_t>(ui::control_definition::type::radio);
static auto s_control_definition_tabbar = static_cast<uint32_t>(ui::control_definition::type::tabbar);
static auto s_control_definition_separator = static_cast<uint32_t>(ui::control_definition::type::separator);

static auto s_control_definition_mode_scene = static_cast<uint32_t>(ui::control_definition::mode::scene);
static auto s_control_definition_mode_imgui = static_cast<uint32_t>(ui::control_definition::mode::imgui);

// MARK: - Lua

auto ui::control_definition::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("UI")
            .beginClass<control_definition>("ControlDefinition")
                .addConstructor<auto(*)(const math::rect&, uint32_t)->void, lua_reference>()

                .addStaticProperty("Scene", &s_control_definition_mode_scene, false)
                .addStaticProperty("ImGui", &s_control_definition_mode_imgui, false)
                .addStaticProperty("None", &s_control_definition_none, false)
                .addStaticProperty("Button", &s_control_definition_button, false)
                .addStaticProperty("Label", &s_control_definition_label, false)
                .addStaticProperty("TextArea", &s_control_definition_text_area, false)
                .addStaticProperty("Image", &s_control_definition_image, false)
                .addStaticProperty("TextField", &s_control_definition_text_field, false)
                .addStaticProperty("CheckBox", &s_control_definition_checkbox, false)
                .addStaticProperty("List", &s_control_definition_list, false)
                .addStaticProperty("ScrollArea", &s_control_definition_scroll_area, false)
                .addStaticProperty("Grid", &s_control_definition_grid, false)
                .addStaticProperty("LabeledList", &s_control_definition_labeled_list, false)
                .addStaticProperty("Canvas", &s_control_definition_canvas, false)
                .addStaticProperty("Sprite", &s_control_definition_sprite, false)
                .addStaticProperty("PopupButton", &s_control_definition_popup_button, false)
                .addStaticProperty("Slider", &s_control_definition_slider, false)
                .addStaticProperty("Table", &s_control_definition_table, false)
                .addStaticProperty("Box", &s_control_definition_box, false)
                .addStaticProperty("Radio", &s_control_definition_radio, false)
                .addStaticProperty("TabBar", &s_control_definition_tabbar, false)
                .addStaticProperty("Separator", &s_control_definition_separator, false)

                .addProperty("type", &control_definition::type)
                .addProperty("entity", &control_definition::entity)
                .addProperty("control", &control_definition::control)
                .addProperty("action", &control_definition::m_action, true)

                // Framing
                .addProperty("frame", &control_definition::frame, &control_definition::set_frame)
                .addProperty("contentOffset", &control_definition::content_offset, &control_definition::set_content_offset)
                .addProperty("contentSize", &control_definition::content_size, &control_definition::set_content_size)
                .addProperty("gridSize", &control_definition::grid_size, &control_definition::set_grid_size)
                .addProperty("borders", &control_definition::borders, &control_definition::set_borders)
                .addProperty("canScrollUp", &control_definition::can_scroll_up)
                .addProperty("canScrollDown", &control_definition::can_scroll_down)

                // General
                .addProperty("disabled", &control_definition::disabled, &control_definition::set_disabled)

                // Strings
                .addProperty("title", &control_definition::string_value, &control_definition::set_string_value)
                .addProperty("bodyText", &control_definition::string_value, &control_definition::set_string_value)

                // Font
                .addProperty("textColor", &control_definition::text_color, &control_definition::set_text_color)
                .addProperty("secondaryTextColor", &control_definition::secondary_text_color, &control_definition::set_secondary_text_color)
                .addProperty("textFont", &control_definition::text_font, &control_definition::set_text_font)
                .addProperty("textSize", &control_definition::text_size, &control_definition::set_text_size)
                .addProperty("alignment", &control_definition::alignment, &control_definition::set_alignment)

                // Images
                .addProperty("image", &control_definition::image, &control_definition::set_image)
                .addProperty("icon", &control_definition::image, &control_definition::set_image)
                .addProperty("sprite", &control_definition::image, &control_definition::set_image)

                // Action
                .addProperty("action", &control_definition::action, &control_definition::set_action)

                // Tables / Lists
                .addProperty("selectedIndex", &control_definition::selected_index, &control_definition::set_selected_index)
                .addProperty("columnWidths", &control_definition::column_widths, &control_definition::set_column_widths)
                .addProperty("columnHeadings", &control_definition::column_headings, &control_definition::set_column_headings)
                .addProperty("columnSpacings", &control_definition::column_spacing, &control_definition::set_column_spacing)

                // Methods
                .addFunction("construct", &control_definition::construct)
                .addFunction("useImGuiFlowLayout", &control_definition::disable_absolute_frame)
                .addFunction("setTitle", &control_definition::set_string_value)
                .addFunction("setBodyText", &control_definition::set_body_text_value)
                .addFunction("setBodyDescription", &control_definition::set_body_description)
                .addFunction("setAction", &control_definition::set_action)
                .addFunction("setColumnSpacings", &control_definition::set_column_spacing)
                .addFunction("setTextColor", &control_definition::set_text_color)
                .addFunction("setSecondaryTextColor", &control_definition::set_secondary_text_color)
                .addFunction("setTextFont", &control_definition::set_text_font_and_size)
                .addFunction("setAlignment", &control_definition::set_alignment)
                .addFunction("setImage", &control_definition::set_image)
                .addFunction("setIcon", &control_definition::set_image)
                .addFunction("setSprite", &control_definition::set_image)
                .addFunction("setFrameSize", &control_definition::set_frame_size)
                .addFunction("setContentOffset", &control_definition::set_content_offset)
                .addFunction("setContentSize", &control_definition::set_content_size)
                .addFunction("setItemCount", &control_definition::set_item_count)
                .addFunction("selectIndex", &control_definition::set_selected_index)
                .addFunction("setRowItems", &control_definition::set_items)
                .addFunction("setGridItems", &control_definition::set_items)
                .addFunction("setItems", &control_definition::set_items)
                .addFunction("setDisabled", &control_definition::set_disabled)
                .addFunction("setColumns", &control_definition::set_columns)
                .addFunction("setBorders", &control_definition::set_borders)
                .addFunction("setRender", &control_definition::set_render)
                .addFunction("setActionScript", &control_definition::set_action)
                .addFunction("setContinuous", &control_definition::set_continuous)
                .addFunction("scrollUp", &control_definition::scroll_up)
                .addFunction("scrollDown", &control_definition::scroll_down)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

ui::control_definition::control_definition(const math::rect &frame, uint32_t type)
    : m_frame(frame), m_type(static_cast<enum type>(type))
{

}

// MARK: -

auto ui::control_definition::construct(uint32_t mode) -> void
{
    switch (static_cast<enum mode>(mode)) {
        case mode::scene: {
            construct_scene_entity();
            break;
        }
        case mode::imgui: {
            construct_imgui_control();
            break;
        }
    }
}

auto ui::control_definition::construct_scene_entity() -> void
{
    auto env = environment::active_environment().lock();
    if (!env) {
        return;
    }
    auto state = env->lua_runtime()->internal_state();

    switch (m_type) {
        case type::button: {
            widgets::button_widget::lua_reference button(new widgets::button_widget(m_string_value));
            button->set_frame(m_frame);
            button->set_continuous_action(m_continuous);

            if (m_image.state()) {
                button->set_icon(m_image);
            }

            if (m_action.state()) {
                button->set_action(m_action);
            }

            button->draw();
            m_widget = luabridge::LuaRef(state, button);
            m_entity = scene_entity::lua_reference(new scene_entity(button->entity()));
            m_entity->internal_entity()->set_position(m_frame.origin);
            m_entity->set_position(m_frame.origin);
            break;
        }
        case type::label: {
            widgets::label_widget::lua_reference label(new widgets::label_widget(m_string_value));
            label->set_frame(m_frame);
            m_widget = luabridge::LuaRef(state, label);
            m_entity = scene_entity::lua_reference(new scene_entity(label->entity()));
            break;
        }
        case type::image: {
            widgets::image_widget::lua_reference image(new widgets::image_widget(m_image));
            image->set_frame(m_frame);
            m_widget = luabridge::LuaRef(state, image);
            m_entity = scene_entity::lua_reference(new scene_entity(image->entity()));
            break;
        }
        case type::text_area: {
            widgets::textarea_widget::lua_reference text(new widgets::textarea_widget(m_body_text));
            text->set_frame(m_frame);
            text->set_font(m_font_name);
            text->set_font_size(static_cast<int16_t>(m_font_size));
            text->set_color(m_text_color);
            text->set_scroll_offset(static_cast<int16_t>(m_content_offset.y));
            text->draw();
            m_widget = luabridge::LuaRef(state, text);
            m_entity = scene_entity::lua_reference(new scene_entity(text->entity()));
            m_entity->internal_entity()->set_position(m_frame.origin);
            m_entity->set_position(m_frame.origin);

            m_can_scroll_up = text->can_scroll_up();
            m_can_scroll_down = text->can_scroll_down();

            break;
        }
    }
}

auto ui::control_definition::construct_imgui_control() -> void
{
    auto env = environment::active_environment().lock();
    if (!env) {
        return;
    }
    auto state = env->lua_runtime()->internal_state();

    switch (m_type) {
        case type::button: {
            imgui::button::lua_reference button(new imgui::button(m_string_value, { state }));
            if (m_absolute_frame) {
                button->set_position(m_frame.origin);
                button->set_size(m_frame.size);
            }
            m_control = luabridge::LuaRef(state, button);
            break;
        }
        case type::label: {
            imgui::label::lua_reference label { new imgui::label(m_string_value) };
            if (m_absolute_frame) {
                label->set_position(m_frame.origin);
                label->set_size(m_frame.size);
            }
            m_control = luabridge::LuaRef(state, label);
            break;
        }
        case type::image: {
            imgui::image::lua_reference image { new imgui::image(m_image) };
            if (m_absolute_frame) {
                image->set_position(m_frame.origin);
                image->set_size(m_frame.size);
            }
            m_control = luabridge::LuaRef(state, image);
            break;
        }
        case type::text_field: {
            imgui::textfield::lua_reference field { new imgui::textfield(512, m_string_value) };
            if (m_absolute_frame) {
                field->set_position(m_frame.origin);
                field->set_size(m_frame.size);
            }
            m_control = luabridge::LuaRef(state, field);
            break;
        }
        case type::checkbox: {
            imgui::checkbox::lua_reference checkbox { new imgui::checkbox(m_string_value, false) };
            if (m_absolute_frame) {
                checkbox->set_position(m_frame.origin);
                checkbox->set_size(m_frame.size);
            }
            m_control = luabridge::LuaRef(state, checkbox);
            break;
        }
        case type::popup_button: {
            luabridge::LuaRef items {
                luabridge::LuaRef::newTable(env->lua_runtime()->internal_state())
            };

            if (m_items.state() && m_items.isTable()) {
                for (auto i = 0; i < m_items.length(); ++i) {
                    items[i + 1] = m_items[i + 1];
                }
            }
            else if (!m_string_value.empty()) {
                std::istringstream iss { m_string_value };
                std::string item;
                while (std::getline(iss, item, ',')) {
                    items[items.length() + 1] = item;
                }
            }

            imgui::combo::lua_reference combo {
                new imgui::combo(items)
            };

            if (m_absolute_frame) {
                combo->set_position(m_frame.origin);
                combo->set_size(m_frame.size);
            }
            m_control = luabridge::LuaRef(state, combo);
            break;
        }
        case type::separator: {
            imgui::separator::lua_reference separator { new imgui::separator() };
            if (m_absolute_frame) {
                separator->set_position(m_frame.origin);
                separator->set_size(m_frame.size);
            }
            m_control = luabridge::LuaRef(state, separator);
            break;
        }
        case type::slider: {
            imgui::slider::lua_reference slider { new imgui::slider(50, 0, 100) };
            if (m_absolute_frame) {
                slider->set_position(m_frame.origin);
                slider->set_size(m_frame.size);
            }
            m_control = luabridge::LuaRef(state, slider);
            break;
        }
        default: {
            env->lua_out("ControlDefinition type is not supported in ImGui.", true);
            break;
        }
    }
}

auto ui::control_definition::update() -> void
{
    if (scripting::lua::ref_isa<widgets::textarea_widget>(m_widget)) {
        auto text = m_widget.cast<widgets::textarea_widget::lua_reference>();
        text->set_text(m_body_text);
        text->set_scroll_offset(static_cast<int32_t>(m_content_offset.y));
        text->draw();

        m_can_scroll_up = text->can_scroll_up();
        m_can_scroll_down = text->can_scroll_down();
    }
    else if (scripting::lua::ref_isa<widgets::button_widget>(m_widget)) {
        auto button = m_widget.cast<widgets::button_widget::lua_reference>();
        button->set_disabled(m_disabled);
        button->draw();
    }
}

// MARK: - Helper Functions

auto ui::control_definition::set_columns(const luabridge::LuaRef &widths, const luabridge::LuaRef &headings) -> void
{

}

auto ui::control_definition::set_text_font_and_size(const std::string &font, uint32_t size) -> void
{
    set_text_font(font);
    set_text_size(size);
}

auto ui::control_definition::set_body_description(const luabridge::LuaRef &base_reference, int64_t id_offset) -> void
{

}

auto ui::control_definition::scroll_up() -> void
{
    m_content_offset.y -= 2;
    update();
}

auto ui::control_definition::scroll_down() -> void
{
    m_content_offset.y += 2;
    update();
}

auto ui::control_definition::can_scroll_up() const -> bool
{
    return m_can_scroll_up;
}

auto ui::control_definition::can_scroll_down() const -> bool
{
    return m_can_scroll_down;
}