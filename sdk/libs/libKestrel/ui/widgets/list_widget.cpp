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

#include <algorithm>
#include <libKestrel/kestrel.hpp>
#include <libKestrel/ui/widgets/list_widget.hpp>
#include <libKestrel/ui/entity/scene_entity.hpp>
#include <libKestrel/font/manager.hpp>

// MARK: - Constants

namespace kestrel::ui::widgets::constants
{
    constexpr std::size_t default_row_height = 25;
}

// MARK: - Construction

kestrel::ui::widgets::list_widget::list_widget(const math::rect& frame)
{
    m_appearance.hilite_color = { new graphics::color(255, 0, 150) };
    m_appearance.text_color = { new graphics::color(255, 255, 255) };
    m_appearance.background_color = { new graphics::color(0, 0, 0) };
    m_appearance.outline_color = { new graphics::color(220, 220, 220) };

    m_appearance.label_font = font::manager::shared_manager().default_font();
    m_appearance.label_font->load_for_graphics();

    setup(frame);
}

auto kestrel::ui::widgets::list_widget::setup(const math::rect& frame) -> void
{
    m_entity.canvas = std::make_shared<graphics::canvas>(frame.size());
    m_entity.entity = { new scene_entity(m_entity.canvas->spawn_entity(frame.origin())) };
    m_entity.entity->set_clipping_area(frame.size());

    m_entity.entity->internal_entity()->set_position(frame.origin());
    m_entity.entity->set_position(frame.origin());

    m_appearance.label_font = font::manager::shared_manager().default_font();
    m_appearance.label_font->load_for_graphics();

    redraw_entity();
    bind_internal_events();
}

// MARK: - Accessors

auto kestrel::ui::widgets::list_widget::borders() const -> bool
{
    return m_borders;
}

auto kestrel::ui::widgets::list_widget::headers() const -> bool
{
    return m_has_header;
}

auto kestrel::ui::widgets::list_widget::selected_row() const -> std::int32_t
{
    return m_state.selection.row;
}

auto kestrel::ui::widgets::list_widget::frame() const -> math::rect
{
    return { m_entity.entity->position(), m_entity.entity->size() };
}

auto kestrel::ui::widgets::list_widget::set_borders(bool f) -> void
{
    m_borders = f;
    m_state.dirty = true;
}

auto kestrel::ui::widgets::list_widget::set_headers(bool f) -> void
{
    m_has_header = f;
    m_state.dirty = true;
}

auto kestrel::ui::widgets::list_widget::select_row(std::int32_t row) -> void
{
    m_state.selection.row = row;
    m_state.dirty = true;
}

auto kestrel::ui::widgets::list_widget::set_frame(const math::rect &frame) -> void
{
    setup(frame);
}

auto kestrel::ui::widgets::list_widget::entity() const -> scene_entity::lua_reference
{
    return m_entity.entity;
}

auto kestrel::ui::widgets::list_widget::scroll_up() -> void
{
    if (m_state.scroll.first_visible_row <= 1) {
        return;
    }
    const auto height = static_cast<float>(height_for_row(m_state.scroll.first_visible_row--));
    m_state.scroll.offset.set_y(m_state.scroll.offset.y() - height);
    m_entity.entity->set_clipping_offset(m_state.scroll.offset);
}

auto kestrel::ui::widgets::list_widget::scroll_down() -> void
{
    if (m_state.scroll.first_visible_row >= number_of_rows()) {
        return;
    }
    const auto height = static_cast<float>(height_for_row(m_state.scroll.first_visible_row++));
    m_state.scroll.offset.set_y(m_state.scroll.offset.y() + height);
    m_entity.entity->set_clipping_offset(m_state.scroll.offset);
}

auto kestrel::ui::widgets::list_widget::set_text_color(const graphics::color::lua_reference& color) -> void
{
    m_appearance.text_color = color;
    m_state.dirty = true;
}

auto kestrel::ui::widgets::list_widget::set_background_color(const graphics::color::lua_reference& color) -> void
{
    m_appearance.background_color = color;
    m_state.dirty = true;
}

auto kestrel::ui::widgets::list_widget::set_hilite_color(const graphics::color::lua_reference& color) -> void
{
    m_appearance.hilite_color = color;
    m_state.dirty = true;
}

auto kestrel::ui::widgets::list_widget::set_outline_color(const graphics::color::lua_reference& color) -> void
{
    m_appearance.outline_color = color;
    m_state.dirty = true;
}

auto kestrel::ui::widgets::list_widget::set_heading_text_color(const graphics::color::lua_reference& color) -> void
{
    m_appearance.heading_text_color = color;
    m_state.dirty = true;
}

auto kestrel::ui::widgets::list_widget::set_font(const font::reference::lua_reference& font) -> void
{
    m_appearance.label_font = font;
    m_appearance.label_font->load_for_graphics();
    m_state.dirty = true;
}

auto kestrel::ui::widgets::list_widget::font() const -> font::reference::lua_reference
{
    return m_appearance.label_font;
}

auto kestrel::ui::widgets::list_widget::text_color() const -> graphics::color::lua_reference
{
    return m_appearance.text_color;
}

auto kestrel::ui::widgets::list_widget::background_color() const -> graphics::color::lua_reference
{
    return m_appearance.background_color;
}

auto kestrel::ui::widgets::list_widget::hilite_color() const -> graphics::color::lua_reference
{
    return m_appearance.hilite_color;
}

auto kestrel::ui::widgets::list_widget::outline_color() const -> graphics::color::lua_reference
{
    return m_appearance.outline_color;
}

auto kestrel::ui::widgets::list_widget::heading_text_color() const -> graphics::color::lua_reference
{
    return m_appearance.heading_text_color;
}


// MARK: - Drawing

auto kestrel::ui::widgets::list_widget::draw() -> void
{
    if (m_state.dirty) {
        redraw_entity();
    }
    m_state.dirty = false;
}

auto kestrel::ui::widgets::list_widget::redraw_entity() -> void
{
    auto& canvas = m_entity.canvas;
    canvas->clear();
    canvas->set_font(m_appearance.label_font);

    const auto row_x = 1.f;
    const auto row_width = frame().size().width() - (row_x * 2.f);
    const auto column_count = number_of_columns();
    const auto heading_height = static_cast<float>(height_of_header());

    math::point row_offset(0);
    if (m_has_header) {
        canvas->set_pen_color(*m_appearance.background_color);
        canvas->fill_rect({ { row_x, 0 }, { row_width, heading_height } });
        canvas->set_pen_color(*m_appearance.heading_text_color);

        math::point row_position(0);
        for (auto j = 1; j <= column_count; ++j) {
            const auto& heading_text = column_heading(j);
            const auto column_width = static_cast<float>(width_for_column(j));

            const auto text_size = canvas->layout_text(heading_text);
            canvas->draw_text({
               row_position.x() + 5.f,
               row_position.y() + ((heading_height - text_size.height()) / 2.f)
           });

            row_position.set_x(row_position.x() + column_width);
        }
    }

    const auto row_count = number_of_rows();
    for (auto i = 1; i <= row_count; ++i) {
        const auto row_height = static_cast<float>(height_for_row(i));
        math::point row_position(row_x, static_cast<float>(i - 1) * row_height);

        canvas->set_pen_color(
            (m_state.selection.row == i) ? *m_appearance.hilite_color : m_appearance.background_color
        );

        if (m_has_header) {
            row_position.set_y(row_position.y() + heading_height);
        }

        canvas->fill_rect({
            { row_position.x(), row_position.y() },
            { row_width, row_height }
        });

        canvas->set_pen_color(*m_appearance.text_color);
        canvas->set_font(m_appearance.label_font);

        for (auto j = 1; j <= column_count; ++j) {
            const auto column_width = static_cast<float>(width_for_column(j));
            const auto value = value_for_cell(i, j);
            const auto text_size = canvas->layout_text(value);

            canvas->draw_text({
                row_position.x() + 5,
                row_position.y() + ((row_height - text_size.height()) / 2.f)
            });

            row_position.set_x(row_position.x() + column_width);
        }

        row_position.set_y(row_position.y() + row_height);
    }

    if (m_borders) {
        canvas->set_pen_color(*m_appearance.outline_color);

        if (m_has_header) {
            canvas->draw_line({ 0, heading_height - 1.f }, { m_entity.entity->size().width(), heading_height - 1.f }, 1.f);
        }
        else {
            canvas->draw_line({ 0, 0 }, { m_entity.entity->size().width(), 0 }, 1);
        }

        canvas->draw_line({ 0, m_entity.entity->size().height() - 1.f },
                          { m_entity.entity->size().width(), m_entity.entity->size().height() - 1.f }, 1);
    }

    canvas->rebuild_texture();
}

// MARK: - Calculations

auto kestrel::ui::widgets::list_widget::row_index_at_point(const math::point &p) -> std::int32_t
{
    // Add the scroll offset to the point provided to find the absolute position.
    // Then step through each of the rows (excluding the header) and determine what row the point
    // falls into.
    // TODO: Memoize the outputs of row heights??
    auto q = p + m_state.scroll.offset;

    // If we have a header to consider, then add the height of the header to the position.
    if (m_has_header) {
        q.set_y(q.y() - static_cast<float>(height_of_header()));
    }

    const auto& row_count = number_of_rows();
    for (auto n = 1; n <= row_count; ++n) {
        q.set_y(q.y() - static_cast<float>(height_for_row(n)));
        if (q.y() <= 0) {
            // We have found the row.
            return n;
        }
    }

    // We could not identify the row.
    return -1;
}


auto kestrel::ui::widgets::list_widget::column_width(std::int32_t column) const -> std::int32_t
{
    return width_for_column(column);
}

auto kestrel::ui::widgets::list_widget::column_heading(std::int32_t column) const -> std::string
{
    return heading_for_column(column);
}

auto kestrel::ui::widgets::list_widget::reload_data() -> void
{
    m_state.dirty = true;
    redraw_entity();
}

// MARK: - Events

auto kestrel::ui::widgets::list_widget::lua_receive_event(const event::lua_reference &e) -> void
{
    if (e.get()) {
        receive_event(*e.get());
    }
}

auto kestrel::ui::widgets::list_widget::receive_event(const event &e) -> bool
{
    auto local_position = e.location() - entity()->position();
    if (e.is_mouse_event() && entity()->hit_test(local_position)) {
        if (e.has(::ui::event::any_mouse_down) && !m_pressed) {
            m_pressed = true;
            return true;
        }
        else if (e.has(::ui::event::any_mouse_up) && m_pressed) {
            auto row_number = row_index_at_point(local_position);
            if (row_number > number_of_rows()) {
                row_number = number_of_rows();
            }

            m_state.dirty = true;
            m_state.selection.row = should_select_row(row_number) ? row_number : -1;
            row_selected(row_number);
            redraw_entity();
            m_pressed = false;
            return true;
        }
    }
    return false;
}

auto kestrel::ui::widgets::list_widget::bind_internal_events() -> void
{
}

// MARK: - Data Source

auto kestrel::ui::widgets::list_widget::data_source() const -> luabridge::LuaRef
{
    // Create a blank table to pass as the data source.
    return luabridge::LuaRef::newTable(kestrel::lua_runtime()->internal_state());
}

auto kestrel::ui::widgets::list_widget::set_data_source(const luabridge::LuaRef &data_source) -> void
{
    if (!data_source.state() || !data_source.isTable()) {
        return;
    }

    const auto& number_of_rows = data_source["numberOfRows"];
    const auto& number_of_columns = data_source["numberOfColumns"];
    const auto& heading_for_column = data_source["headingForColumn"];
    const auto& value_for_cell = data_source["valueForCell"];

    if (number_of_rows.isFunction() || number_of_rows.isNumber()) {
        m_data_source.number_of_rows = number_of_rows;
    }

    if (number_of_columns.isFunction() || number_of_columns.isNumber()) {
        m_data_source.number_of_columns = number_of_columns;
    }

    if (heading_for_column.isFunction() || value_for_cell.isString()) {
        m_data_source.heading_for_column = heading_for_column;
    }

    if (value_for_cell.isFunction() || value_for_cell.isString()) {
        m_data_source.value_for_cell = value_for_cell;
    }

    m_state.dirty = true;
}

auto kestrel::ui::widgets::list_widget::number_of_rows() const -> std::int32_t
{
    if (m_data_source.number_of_rows.state() && m_data_source.number_of_rows.isFunction()) {
        auto count = m_data_source.number_of_rows();
        if (count.state() && count.isNumber()) {
            return count.cast<std::int32_t>();
        }
    }
    else if (m_data_source.number_of_rows.state() && m_data_source.number_of_rows.isNumber()) {
        return m_data_source.number_of_rows.cast<std::int32_t>();
    }
    return 0;
}

auto kestrel::ui::widgets::list_widget::number_of_columns() const -> std::int32_t
{
    if (m_data_source.number_of_columns.state() && m_data_source.number_of_columns.isFunction()) {
        auto count = m_data_source.number_of_columns();
        if (count.state() && count.isNumber()) {
            return count.cast<std::int32_t>();
        }
    }
    else if (m_data_source.number_of_columns.state() && m_data_source.number_of_columns.isNumber()) {
        return m_data_source.number_of_columns.cast<std::int32_t>();
    }
    return 1;
}

auto kestrel::ui::widgets::list_widget::value_for_cell(std::int32_t row, std::int32_t column) const -> std::string
{
    if (m_data_source.value_for_cell.state() && m_data_source.value_for_cell.isFunction()) {
        auto value = m_data_source.value_for_cell(row, column);
        if (value.state() && (value.isString() || value.isNumber())) {
            return value.tostring();
        }
    }
    else if (m_data_source.value_for_cell.state() && m_data_source.value_for_cell.isString()) {
        return m_data_source.value_for_cell.tostring();
    }
    return "";
}

auto kestrel::ui::widgets::list_widget::heading_for_column(std::int32_t column) const -> std::string
{
    if (m_data_source.heading_for_column.state() && m_data_source.heading_for_column.isFunction()) {
        auto value = m_data_source.heading_for_column(column);
        if (value.state() && (value.isString() || value.isNumber())) {
            return value.tostring();
        }
    }
    else if (m_data_source.heading_for_column.state() && m_data_source.heading_for_column.isString()) {
        return m_data_source.heading_for_column.tostring();
    }
    return "";
}

// MARK: - Delegate

auto kestrel::ui::widgets::list_widget::delegate() const -> luabridge::LuaRef
{
    // Create a blank table to pass as the delegate.
    return luabridge::LuaRef::newTable(kestrel::lua_runtime()->internal_state());
}

auto kestrel::ui::widgets::list_widget::set_delegate(const luabridge::LuaRef &delegate) -> void
{
    if (!delegate.state() || !delegate.isTable()) {
        return;
    }

    const auto& on_row_select = delegate["onRowSelected"];
    const auto& should_select_row = delegate["shouldSelectRow"];
    const auto& width_for_column = delegate["widthForColumn"];
    const auto& height_for_row = delegate["heightForRow"];

    if (on_row_select.isFunction()) {
        m_delegate.on_row_select = on_row_select;
    }

    if (should_select_row.isFunction() || should_select_row.isBool()) {
        m_delegate.should_select_row = should_select_row;
    }

    if (width_for_column.isFunction() || should_select_row.isNumber()) {
        m_delegate.width_for_column = width_for_column;
    }

    if (height_for_row.isFunction() || should_select_row.isNumber()) {
        m_delegate.height_for_row = height_for_row;
    }

    m_state.dirty = true;
}

auto kestrel::ui::widgets::list_widget::should_select_row(std::int32_t row) const -> bool
{
    if (m_delegate.should_select_row.state() && m_delegate.should_select_row.isFunction()) {
        return m_delegate.should_select_row(row).cast<bool>();
    }
    else if (m_delegate.should_select_row.state() && m_delegate.should_select_row.isBool()) {
        return m_delegate.should_select_row.cast<bool>();
    }
    return true;
}

auto kestrel::ui::widgets::list_widget::row_selected(std::int32_t i) -> void
{
    if (m_delegate.on_row_select.state() && m_delegate.on_row_select.isFunction()) {
        m_delegate.on_row_select(i);
    }
}

auto kestrel::ui::widgets::list_widget::width_for_column(std::int32_t column) const -> std::int32_t
{
    if (m_delegate.width_for_column.state() && m_delegate.width_for_column.isFunction()) {
        return m_delegate.width_for_column(column).cast<std::int32_t>();
    }
    else if (m_delegate.width_for_column.state() && m_delegate.width_for_column.isNumber()) {
        return m_delegate.width_for_column.cast<std::int32_t>();
    }
    return static_cast<std::int32_t>(m_entity.entity->size().width() / static_cast<double>(number_of_columns()));
}

auto kestrel::ui::widgets::list_widget::height_for_row(std::int32_t row) const -> std::int32_t
{
    if (m_delegate.height_for_row.state() && m_delegate.height_for_row.isFunction()) {
        return m_delegate.height_for_row(row).cast<std::int32_t>();
    }
    else if (m_delegate.height_for_row.state() && m_delegate.height_for_row.isNumber()) {
        return m_delegate.height_for_row.cast<std::int32_t>();
    }
    return constants::default_row_height;
}

auto kestrel::ui::widgets::list_widget::height_of_header() const -> std::int32_t
{
    if (m_delegate.height_of_header.state() && m_delegate.height_of_header.isFunction()) {
        return m_delegate.height_of_header().cast<std::int32_t>();
    }
    else if (m_delegate.height_of_header.state() && m_delegate.height_of_header.isNumber()) {
        return m_delegate.height_of_header.cast<std::int32_t>();
    }
    return constants::default_row_height;
}
