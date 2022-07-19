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
#include "core/ui/dialog/dialog_layout.hpp"

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

static auto s_control_definition_anchor_top = static_cast<uint32_t>(ui::control_definition::anchor::top);
static auto s_control_definition_anchor_bottom = static_cast<uint32_t>(ui::control_definition::anchor::bottom);
static auto s_control_definition_anchor_left = static_cast<uint32_t>(ui::control_definition::anchor::left);
static auto s_control_definition_anchor_right = static_cast<uint32_t>(ui::control_definition::anchor::right);
static auto s_control_definition_anchor_vertical = static_cast<uint32_t>(ui::control_definition::anchor::vertical);
static auto s_control_definition_anchor_horizontal = static_cast<uint32_t>(ui::control_definition::anchor::horizontal);


// MARK: - Lua

auto ui::control_definition::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("UI")
            .beginNamespace("Anchor")
                .addProperty("Top", &s_control_definition_anchor_top, false)
                .addProperty("Bottom", &s_control_definition_anchor_bottom, false)
                .addProperty("Left", &s_control_definition_anchor_left, false)
                .addProperty("Right", &s_control_definition_anchor_right, false)
                .addProperty("Vertical", &s_control_definition_anchor_vertical, false)
                .addProperty("Horizontal", &s_control_definition_anchor_horizontal, false)
            .endNamespace()
            .beginClass<control_definition>("ControlDefinition")
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
                .addProperty("type", &control_definition::type, &control_definition::set_type)
                .addProperty("name", &control_definition::name)
                .addProperty("frame", &control_definition::frame, &control_definition::set_frame)
                .addProperty("anchor", &control_definition::anchor, &control_definition::set_anchor)
                .addProperty("suggestedValue", &control_definition::suggested_value)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

ui::control_definition::control_definition(const dialog_layout *layout, const std::string &name, enum type type)
    : m_name(name), m_type(type), m_layout(layout)
{
    recalculate_frame();
}

// MARK: - Element Indexes

auto ui::control_definition::set_element_index_vector(const std::vector<std::uint16_t> &v) -> void
{
    m_element_index_vec = v;
    recalculate_frame();
}

auto ui::control_definition::add_element_index_vector(std::uint16_t idx) -> void
{
    m_element_index_vec.emplace_back(idx);
    recalculate_frame();
}

// MARK: - Framing

auto ui::control_definition::recalculate_frame() -> void
{
    if (m_element_index_vec.empty()) {
        return;
    }

    double left = 10'000;
    double top = 10'000;
    double right = 0;
    double bottom = 0;

    auto target_size = m_layout->size();

    for (auto idx : m_element_index_vec) {
        auto item = m_layout->element_at(idx);

        left = std::min(left, item->frame.get_x());
        top = std::min(top, item->frame.get_y());
        right = std::max(right, item->frame.get_max_x());
        bottom = std::max(bottom, item->frame.get_max_y());
    }

    // Vertical Anchors
    if ((m_anchor & anchor::vertical) == anchor::vertical) {
        auto diff = m_layout->size().height - bottom;
        bottom = target_size.height - diff;
    }
    else if ((m_anchor & anchor::bottom) == anchor::bottom) {
        auto top_diff = m_layout->size().height - top;
        auto bottom_diff = m_layout->size().height - bottom;
        top = target_size.height - top_diff;
        bottom = target_size.height - bottom_diff;
    }

    // Horizontal Anchors
    if ((m_anchor & anchor::horizontal) == anchor::horizontal) {
        auto diff = m_layout->size().width - left;
        left = target_size.height - diff;
    }
    else if ((m_anchor & anchor::right) == anchor::right) {
        auto right_diff = m_layout->size().width - right;
        auto left_diff = m_layout->size().width - left;
        right = target_size.width - right_diff;
        left = target_size.width - left_diff;
    }

    m_frame = math::rect(left, top, right - left, bottom - top);
}

// MARK: - Accessors

auto ui::control_definition::name() const -> std::string
{
    return m_name;
}

auto ui::control_definition::suggested_value() const -> std::string
{
    if (m_element_index_vec.empty()) {
        return "";
    }
    auto element = m_layout->element_at(m_element_index_vec[0]);
    return element->value;
}

auto ui::control_definition::anchor() const -> std::uint8_t
{
    return static_cast<std::uint8_t>(m_anchor);
}

auto ui::control_definition::type() const -> std::uint8_t
{
    return static_cast<std::uint8_t>(m_type);
}

auto ui::control_definition::frame() const -> math::rect
{
    return m_frame;
}

auto ui::control_definition::set_type(std::uint8_t type) -> void
{
    m_type = static_cast<enum type>(type);
}

auto ui::control_definition::set_anchor(std::uint8_t anchor) -> void
{
    m_anchor = static_cast<enum anchor>(anchor);
    recalculate_frame();
}

auto ui::control_definition::set_frame(const math::rect &frame) -> void
{
    m_frame = frame;
}
