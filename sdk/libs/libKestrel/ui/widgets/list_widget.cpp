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
#include <libKestrel/ui/widgets/list_widget.hpp>
#include <libKestrel/ui/entity/scene_entity.hpp>
#include <libKestrel/font/manager.hpp>

// MARK: - Constants

namespace kestrel::ui::widgets::constants
{
    constexpr std::size_t default_row_height = 14;
}

// MARK: - Construction

kestrel::ui::widgets::list_widget::list_widget(const math::rect& frame)
    : m_visible_rows(frame.size().height() / constants::default_row_height), m_column_widths({ 1.0 })
{
    m_hilite_color = { new graphics::color(255, 0, 150) };
    m_text_color = { new graphics::color(255, 255, 255) };
    m_background_color = { new graphics::color(0, 0, 0) };
    m_outline_color = { new graphics::color(220, 220, 220) };

    m_label_font = font::manager::shared_manager().default_font();
    m_label_font->load_for_graphics();

    setup(frame);
}

auto kestrel::ui::widgets::list_widget::setup(const math::rect& frame) -> void
{
    m_row_size = math::size(frame.size().width(), constants::default_row_height);

    m_canvas = std::make_shared<graphics::canvas>(frame.size());
    m_entity = { new scene_entity(m_canvas->spawn_entity(frame.origin())) };
    m_entity->set_clipping_area(frame.size());

    m_entity->internal_entity()->set_position(frame.origin());
    m_entity->set_position(frame.origin());

    m_label_font = font::manager::shared_manager().default_font();
    m_label_font->load_for_graphics();
    m_row_size = math::size(frame.size().width(), m_label_font->graphics_font()->line_height() * 6.f);

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

auto kestrel::ui::widgets::list_widget::current_row() const -> list_row::lua_reference
{
    if (m_selected_row < 1 || m_selected_row >= m_rows.size()) {
        return { nullptr };
    }
    else {
        return m_rows.at(m_selected_row - 1);
    }
}

auto kestrel::ui::widgets::list_widget::selected_row() const -> std::int32_t
{
    return m_selected_row;
}

auto kestrel::ui::widgets::list_widget::frame() const -> math::rect
{
    return { m_entity->position(), m_entity->size() };
}

auto kestrel::ui::widgets::list_widget::set_borders(bool f) -> void
{
    m_borders = f;
    m_dirty = true;
}

auto kestrel::ui::widgets::list_widget::set_headers(bool f) -> void
{
    m_has_header = f;
    m_dirty = true;
}

auto kestrel::ui::widgets::list_widget::select_row(std::int32_t row) -> void
{
    m_selected_row = row;
    m_dirty = true;
}

auto kestrel::ui::widgets::list_widget::set_frame(const math::rect &frame) -> void
{
    setup(frame);
}

auto kestrel::ui::widgets::list_widget::set_column_widths(const luabridge::LuaRef &columns) -> void
{
    if (columns.state() && columns.isTable()) {
        m_column_widths.clear();
        for (auto i = 1; i <= columns.length(); ++i) {
            auto column = columns[i];
            if (column.isNumber()) {
                m_column_widths.emplace_back(column.cast<double>());
            }
            else {
                m_column_widths.emplace_back(1.0 / columns.length());
            }
        }
    }
}

auto kestrel::ui::widgets::list_widget::set_column_headings(const luabridge::LuaRef &columns) -> void
{
    if (columns.state() && columns.isTable()) {
        m_headings.clear();
        for (auto i = 1; i <= columns.length(); ++i) {
            auto column = columns[i];
            if (column.isString()) {
                m_headings.emplace_back(column.tostring());
            }
            else {
                m_headings.emplace_back("");
            }
        }
        m_has_header = !m_headings.empty();
    }
}

auto kestrel::ui::widgets::list_widget::set_row_items(const luabridge::LuaRef &rows) -> void
{
    if (rows.state() && rows.isTable()) {
        m_rows.clear();
        for (auto i = 1; i <= rows.length(); ++i) {
            auto row = rows[i];
            if (row.isUserdata()) {
                auto list_row = row.cast<ui::widgets::list_row::lua_reference>();
                m_rows.emplace_back(list_row);
            }
        }
    }
}

auto kestrel::ui::widgets::list_widget::entity() const -> scene_entity::lua_reference
{
    return m_entity;
}

auto kestrel::ui::widgets::list_widget::scroll_up() -> void
{
    m_scroll_offset.set_y(m_scroll_offset.y() - m_row_size.height());
    m_entity->set_clipping_offset(m_scroll_offset);
}

auto kestrel::ui::widgets::list_widget::scroll_down() -> void
{
    m_scroll_offset.set_y(m_scroll_offset.y() + m_row_size.height());
    m_entity->set_clipping_offset(m_scroll_offset);
}


auto kestrel::ui::widgets::list_widget::set_text_color(const graphics::color::lua_reference& color) -> void
{
    m_text_color = color;
    m_dirty = true;
}

auto kestrel::ui::widgets::list_widget::set_background_color(const graphics::color::lua_reference& color) -> void
{
    m_background_color = color;
    m_dirty = true;
}

auto kestrel::ui::widgets::list_widget::set_hilite_color(const graphics::color::lua_reference& color) -> void
{
    m_hilite_color = color;
    m_dirty = true;
}

auto kestrel::ui::widgets::list_widget::set_outline_color(const graphics::color::lua_reference& color) -> void
{
    m_outline_color = color;
    m_dirty = true;
}

auto kestrel::ui::widgets::list_widget::set_heading_text_color(const graphics::color::lua_reference& color) -> void
{
    m_heading_text_color = color;
    m_dirty = true;
}

auto kestrel::ui::widgets::list_widget::set_font(const font::reference::lua_reference& font) -> void
{
    m_label_font = font;
    m_row_size = math::size(m_row_size.width(), m_label_font->graphics_font()->line_height() + 6.f);
    m_dirty = true;
}

auto kestrel::ui::widgets::list_widget::font() const -> font::reference::lua_reference
{
    return m_label_font;
}

auto kestrel::ui::widgets::list_widget::text_color() const -> graphics::color::lua_reference
{
    return m_text_color;
}

auto kestrel::ui::widgets::list_widget::background_color() const -> graphics::color::lua_reference
{
    return m_background_color;
}

auto kestrel::ui::widgets::list_widget::hilite_color() const -> graphics::color::lua_reference
{
    return m_hilite_color;
}

auto kestrel::ui::widgets::list_widget::outline_color() const -> graphics::color::lua_reference
{
    return m_outline_color;
}

auto kestrel::ui::widgets::list_widget::heading_text_color() const -> graphics::color::lua_reference
{
    return m_heading_text_color;
}


// MARK: - Drawing

auto kestrel::ui::widgets::list_widget::draw() -> void
{
    if (m_dirty) {
        redraw_entity();
    }
}

auto kestrel::ui::widgets::list_widget::redraw_entity() -> void
{
    m_canvas->clear();
    m_canvas->set_font(m_label_font);

    math::point row_offset(0);
    if (m_has_header) {
        m_canvas->set_pen_color(*m_background_color);
        m_canvas->fill_rect({ { 1, 0 }, { m_row_size.width() - 2, m_row_size.height() } });
        m_canvas->set_pen_color(*m_heading_text_color);

        math::point row_position(0);
        for (auto j = 1; j <= m_headings.size(); ++j) {
            const auto& column = m_headings[j - 1];
            auto column_width = this->column_width(j);

            auto text_size = m_canvas->layout_text(column);
            m_canvas->draw_text({
               row_position.x() + 5,
               row_position.y() + ((m_row_size.height() - text_size.height()) / 2)
           });

            row_position.set_x(row_position.x() + column_width);
        }
    }

    for (auto i = 0; i < m_rows.size(); ++i) {
        const auto& row = m_rows.at(i);

        if (m_selected_row == (i + 1)) {
            // Draw selection background
            m_canvas->set_pen_color(*m_hilite_color);
        }
        else {
            m_canvas->set_pen_color(*m_background_color);
        }

        math::point row_position(0, i * m_row_size.height());
        if (m_has_header) {
            row_position.set_y(row_position.y() + m_row_size.height());
        }

        m_canvas->fill_rect({
            { row_position.x() + 1, row_position.y() },
            { m_row_size.width() - 2, m_row_size.height() }
        });

        m_canvas->set_pen_color(*m_text_color);
        m_canvas->set_font(m_label_font);

        if (m_column_widths.empty()) {
            const auto& column = row->column_value(1);
            auto text_size = m_canvas->layout_text(column);
            m_canvas->draw_text({
                row_position.x() + 5,
                row_position.y() + ((m_row_size.height() - text_size.height()) / 2.f)
            });
        }
        else {
            for (auto j = 1; j <= m_column_widths.size(); ++j) {
                const auto& column = row->column_value(j);
                auto column_width = this->column_width(j);

                auto text_size = m_canvas->layout_text(column);
                m_canvas->draw_text({
                    row_position.x() + 5,
                    row_position.y() + ((m_row_size.height() - text_size.height()) / 2.f)
                });

                row_position.set_x(row_position.x() + column_width);
            }
        }


        row_position.set_y(row_position.y() + m_row_size.height());
    }

    if (m_borders) {
        m_canvas->set_pen_color(*m_outline_color);

        if (m_has_header) {
            m_canvas->draw_line({ 0, m_row_size.height() - 1.f }, { m_entity->size().width(), m_row_size.height() - 1.f }, 1);
        }
        else {
            m_canvas->draw_line({ 0, 0 }, { m_entity->size().width(), 0 }, 1);
        }

        m_canvas->draw_line({ 0, m_entity->size().height() - 1.f }, { m_entity->size().width(), m_entity->size().height() - 1.f }, 1);
    }

    m_canvas->rebuild_texture();
}

// MARK: - Calculations

auto kestrel::ui::widgets::list_widget::row_index_at_point(const math::point &p) -> std::int32_t
{
    auto y = p.y();
    if (m_has_header) {
        y -= m_row_size.height();
    }
    return static_cast<std::int32_t>(std::floor(y / m_row_size.height()) + 1);
}

auto kestrel::ui::widgets::list_widget::row_at_point(const math::point &p) -> list_row::lua_reference
{
    auto index = row_index_at_point(p);
    return m_rows.at(index);
}

auto kestrel::ui::widgets::list_widget::column_width(int column) const -> std::int32_t
{
    return m_column_widths.at(column - 1) * m_entity->size().width();
}

auto kestrel::ui::widgets::list_widget::column_heading(int column) const -> std::string
{
    return m_headings.at(column);
}

auto kestrel::ui::widgets::list_widget::on_row_select(const luabridge::LuaRef &callback) -> void
{
    m_row_select_callback = callback;
}

// MARK: - Events

auto kestrel::ui::widgets::list_widget::receive_event(const event &e) -> bool
{
    auto local_position = e.location() - entity()->position();
    if (e.is_mouse_event() && entity()->hit_test(local_position)) {
        if (e.has(event_type::any_mouse_down) && !m_pressed) {
            m_pressed = true;
        }

        if (e.has(event_type::any_mouse_up) && m_pressed) {
            auto row_number = row_index_at_point(local_position);
            if (row_number > m_rows.size()) {
                row_number = m_rows.size();
            }

            auto row = m_rows[row_number - 1];
            m_dirty = true;

            if (!row.get() || row->column_value(1).empty()) {
                m_selected_row = -1;
            }
            else {
                m_selected_row = row_number;
            }

            if (m_row_select_callback.state() && m_row_select_callback.isFunction()) {
                m_row_select_callback(m_selected_row);
            }

            redraw_entity();
            m_pressed = false;
        }

        return true;
    }
    return false;
}

auto kestrel::ui::widgets::list_widget::bind_internal_events() -> void
{
}

// MARK: - Rows

kestrel::ui::widgets::list_row::list_row(const luabridge::LuaRef &values)
{
    if (values.state() && values.isTable()) {
        for (auto i = 1; i <= values.length(); ++i) {
            auto str = values[i].tostring();
            m_columns.emplace_back(str);
        }
    }
}

auto kestrel::ui::widgets::list_row::column_value(std::int32_t column) const -> std::string
{
    return m_columns.at(column - 1);
}