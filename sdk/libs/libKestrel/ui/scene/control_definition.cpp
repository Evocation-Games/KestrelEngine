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
#include <libKestrel/ui/scene/control_definition.hpp>
#include <libKestrel/kestrel.hpp>
#include <libKestrel/ui/imgui/imgui.hpp>
#include <libKestrel/ui/dialog/dialog_layout.hpp>
#include <libKestrel/ui/types/value/value.hpp>

// MARK: - Construction

kestrel::ui::control_definition::control_definition(const dialog_layout *layout, const std::string &name, enum control_type type)
    : m_name(name), m_type(type), m_layout(layout)
{
    recalculate_frame();
}

// MARK: - Element Indexes

auto kestrel::ui::control_definition::set_element_index_vector(const std::vector<std::uint16_t> &v) -> void
{
    m_element_index_vec = v;
    recalculate_frame();
}

auto kestrel::ui::control_definition::add_element_index_vector(std::uint16_t idx) -> void
{
    m_element_index_vec.emplace_back(idx);
    recalculate_frame();
}

// MARK: - Framing

auto kestrel::ui::control_definition::recalculate_frame() -> void
{
    if (m_element_index_vec.empty()) {
        return;
    }

    float left = 10'000.f;
    float top = 10'000.f;
    float right = 0.f;
    float bottom = 0.f;

    auto target_size = m_layout->size();

    for (auto idx : m_element_index_vec) {
        auto item = m_layout->element_at(idx);

        left = std::min(left, item->frame.x());
        top = std::min(top, item->frame.y());
        right = std::max(right, item->frame.max_x());
        bottom = std::max(bottom, item->frame.max_y());
    }

    // Vertical Anchors
    if ((m_anchor & anchor::vertical) == anchor::vertical) {
        auto diff = m_layout->size().height() - bottom;
        bottom = target_size.height() - diff;
    }
    else if ((m_anchor & anchor::bottom) == anchor::bottom) {
        auto top_diff = m_layout->size().height() - top;
        auto bottom_diff = m_layout->size().height() - bottom;
        top = target_size.height() - top_diff;
        bottom = target_size.height() - bottom_diff;
    }

    // Horizontal Anchors
    if ((m_anchor & anchor::horizontal) == anchor::horizontal) {
        auto diff = m_layout->size().width() - left;
        left = target_size.height() - diff;
    }
    else if ((m_anchor & anchor::right) == anchor::right) {
        auto right_diff = m_layout->size().width() - right;
        auto left_diff = m_layout->size().width() - left;
        right = target_size.width() - right_diff;
        left = target_size.width() - left_diff;
    }

    m_frame = math::rect(left, top, right - left, bottom - top);
}

// MARK: - Accessors

auto kestrel::ui::control_definition::name() const -> std::string
{
    return m_name;
}

auto kestrel::ui::control_definition::anchor() const -> std::uint8_t
{
    return static_cast<std::uint8_t>(m_anchor);
}

auto kestrel::ui::control_definition::anchor_point() const -> layout::axis_origin
{
    return m_anchor_point;
}

auto kestrel::ui::control_definition::scaling_mode() const -> layout::scaling_mode
{
    return m_scaling_mode;
}

auto kestrel::ui::control_definition::type() const -> std::uint8_t
{
    return static_cast<std::uint8_t>(m_type);
}

auto kestrel::ui::control_definition::frame() const -> math::rect
{
    return m_frame;
}

auto kestrel::ui::control_definition::set_type(std::uint8_t type) -> void
{
    m_type = static_cast<enum control_type>(type);
}

auto kestrel::ui::control_definition::set_anchor(std::uint8_t anchor) -> void
{
    m_anchor = static_cast<enum anchor>(anchor);
    recalculate_frame();
}

auto kestrel::ui::control_definition::set_anchor_point(std::uint8_t anchor) -> void
{
    m_anchor_point = static_cast<layout::axis_origin>(anchor);
}

auto kestrel::ui::control_definition::set_scaling_mode(std::uint8_t mode) -> void
{
    m_scaling_mode = static_cast<layout::scaling_mode>(mode);
}

auto kestrel::ui::control_definition::set_frame(const math::rect &frame) -> void
{
    m_frame = frame;
}

auto kestrel::ui::control_definition::set_script_action(const ui::action& script) -> void
{
    m_script_action = script;
}

auto kestrel::ui::control_definition::script_action() const -> ui::action
{
    return m_script_action;
}

auto kestrel::ui::control_definition::background_color() const -> graphics::color::lua_reference
{
    return m_background_color;
}

auto kestrel::ui::control_definition::border_color() const -> graphics::color::lua_reference
{
    return m_border_color;
}

auto kestrel::ui::control_definition::text_color() const -> graphics::color::lua_reference
{
    return m_text_color;
}

auto kestrel::ui::control_definition::secondary_text_color() const -> graphics::color::lua_reference
{
    return m_secondary_text_color;
}

auto kestrel::ui::control_definition::selection_color() const -> graphics::color::lua_reference
{
    return m_selection_color;
}

auto kestrel::ui::control_definition::font() const -> font::reference::lua_reference
{
    return m_font;
}

auto kestrel::ui::control_definition::value() const -> ui::value
{
    return m_value;
}

auto kestrel::ui::control_definition::set_background_color(const graphics::color::lua_reference& value) -> void
{
    m_background_color = value;
}

auto kestrel::ui::control_definition::set_border_color(const graphics::color::lua_reference& value) -> void
{
   m_border_color = value;
}

auto kestrel::ui::control_definition::set_text_color(const graphics::color::lua_reference& value) -> void
{
    m_text_color = value;
}

auto kestrel::ui::control_definition::set_secondary_text_color(const graphics::color::lua_reference& value) -> void
{
    m_secondary_text_color = value;
}

auto kestrel::ui::control_definition::set_selection_color(const graphics::color::lua_reference& value) -> void
{
    m_selection_color = value;
}

auto kestrel::ui::control_definition::alignment() const -> ui::horizontal_alignment
{
    return m_alignment;
}

auto kestrel::ui::control_definition::set_alignment(ui::horizontal_alignment alignment) -> void
{
    m_alignment = alignment;
}

auto kestrel::ui::control_definition::set_font(const font::reference::lua_reference &font) -> void
{
    m_font = font;
}

auto kestrel::ui::control_definition::set_value(const ui::value& value) -> void
{
    m_value = value;
}
