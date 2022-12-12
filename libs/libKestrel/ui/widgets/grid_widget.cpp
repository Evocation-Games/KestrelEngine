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
#include <libKestrel/ui/widgets/grid_widget.hpp>
#include <libKestrel/ui/entity/scene_entity.hpp>
#include <libKestrel/font/manager.hpp>

// MARK: - Constants

namespace kestrel::ui::widgets::constants
{
    constexpr std::size_t grid_width = 4;
}

// MARK: - Construction

kestrel::ui::widgets::grid_widget::grid_widget(const math::rect &frame)
{
    setup(frame);
}

auto kestrel::ui::widgets::grid_widget::setup(const math::rect &frame) -> void
{
    m_label_font = font::manager::shared_manager().default_font();
    m_label_font->load_for_graphics();

    m_canvas = { new graphics::canvas(frame.size) };
    m_entity = { new scene_entity(m_canvas->spawn_entity(frame.origin)) };
    m_entity->set_clipping_area(frame.size);

    m_entity->internal_entity()->set_position(frame.origin);
    m_entity->set_position(frame.origin);

    redraw_entity();
    bind_internal_events();

    m_cell_size = math::size((m_entity->size().width - 1) / m_grid.width, (m_entity->size().height - 1) / m_grid.height);
}

// MARK: - Accessors

auto kestrel::ui::widgets::grid_widget::entity() const -> scene_entity::lua_reference
{
    return m_entity;
}

auto kestrel::ui::widgets::grid_widget::selected_item() const -> std::int32_t
{
    return m_selected_item;
}

auto kestrel::ui::widgets::grid_widget::select_item(std::int32_t item) -> void
{
    m_selected_item = item;
    m_dirty = true;
}

auto kestrel::ui::widgets::grid_widget::frame() const -> math::rect
{
    return { m_entity->position(), m_entity->size() };
}

auto kestrel::ui::widgets::grid_widget::font() const -> font::reference::lua_reference
{
    return m_label_font;
}

auto kestrel::ui::widgets::grid_widget::text_color() const -> graphics::color::lua_reference
{
    return m_text_color;
}

auto kestrel::ui::widgets::grid_widget::secondary_text_color() const -> graphics::color::lua_reference
{
    return m_secondary_text_color;
}

auto kestrel::ui::widgets::grid_widget::background_color() const -> graphics::color::lua_reference
{
    return m_background_color;
}

auto kestrel::ui::widgets::grid_widget::hilite_color() const -> graphics::color::lua_reference
{
    return m_hilite_color;
}

auto kestrel::ui::widgets::grid_widget::outline_color() const -> graphics::color::lua_reference
{
    return m_outline_color;
}

auto kestrel::ui::widgets::grid_widget::dimensions() const -> math::size
{
    return m_grid;
}

auto kestrel::ui::widgets::grid_widget::set_frame(const math::rect &frame) -> void
{
    setup(frame);
}

auto kestrel::ui::widgets::grid_widget::set_font(const font::reference::lua_reference& font) -> void
{
    m_label_font = font;
    m_label_font->load_for_graphics();
    m_dirty = true;
}

auto kestrel::ui::widgets::grid_widget::set_text_color(const graphics::color::lua_reference& color) -> void
{
    m_text_color = color;
    m_dirty = true;
}

auto kestrel::ui::widgets::grid_widget::set_secondary_text_color(const graphics::color::lua_reference& color) -> void
{
    m_secondary_text_color = color;
    m_dirty = true;
}

auto kestrel::ui::widgets::grid_widget::set_background_color(const graphics::color::lua_reference& color) -> void
{
    m_background_color = color;
    m_dirty = true;
}

auto kestrel::ui::widgets::grid_widget::set_hilite_color(const graphics::color::lua_reference& color) -> void
{
    m_hilite_color = color;
    m_dirty = true;
}

auto kestrel::ui::widgets::grid_widget::set_outline_color(const graphics::color::lua_reference& color) -> void
{
    m_outline_color = color;
    m_dirty = true;
}

auto kestrel::ui::widgets::grid_widget::set_cell_drawing_function(const luabridge::LuaRef &drawing_function) -> void
{
    m_cell_drawing_function = drawing_function;
    m_dirty = true;
}

auto kestrel::ui::widgets::grid_widget::set_dimensions(const math::size &dimensions) -> void
{
    m_grid = dimensions;
    m_cell_size = math::size((m_entity->size().width - 1) / m_grid.width, (m_entity->size().height - 1) / m_grid.height);
}

// MARK: - Calculations

auto kestrel::ui::widgets::grid_widget::item_index_at_point(const math::point &p) -> std::int32_t
{
    auto cell = math::point(p.x / m_cell_size.width, p.y / m_cell_size.height).floor();
    return static_cast<std::int32_t>(1 + ((m_scroll_offset + cell.y) * m_grid.width) + cell.x);
}

// MARK: - Item Management

auto kestrel::ui::widgets::grid_widget::set_items(const luabridge::LuaRef &items) -> void
{
    if (items.state() && items.isTable()) {
        m_items.clear();
        for (auto i = 1; i <= items.length(); ++i) {
            auto item = items[i];
            if (item.isUserdata()) {
                auto grid_item = item.cast<ui::widgets::grid_item::lua_reference>();
                m_items.emplace_back(grid_item);
            }
        }
    }
    m_dirty = true;
}


// MARK: - Events

auto kestrel::ui::widgets::grid_widget::bind_internal_events() -> void
{
}

auto kestrel::ui::widgets::grid_widget::receive_event(const event &e) -> bool
{
    auto local_position = e.location() - entity()->position();
    if (e.is_mouse_event() && entity()->hit_test(local_position)) {
        if (e.has(event_type::any_mouse_down) && !m_pressed) {
            m_pressed = true;
        }

        if (e.has(event_type::any_mouse_up) && m_pressed) {
            auto item_number = item_index_at_point(local_position);
            m_dirty = true;

            if (item_number > m_items.size() || item_number <= 0) {
                m_selected_item = -1;
            }
            else {
                m_selected_item = item_number;
            }

            if (m_item_select_callback.state() && m_item_select_callback.isFunction()) {
                m_item_select_callback(m_selected_item);
            }

            redraw_entity();
            m_pressed = false;
        }

        return true;
    }
    return false;
}

auto kestrel::ui::widgets::grid_widget::scroll_up() -> void
{
    m_scroll_offset = std::max(m_scroll_offset - 1, 0);
    m_dirty = true;
}

auto kestrel::ui::widgets::grid_widget::scroll_down() -> void
{
    auto rows = m_items.size() / constants::grid_width;
    m_scroll_offset = std::min(m_scroll_offset + 1, static_cast<std::int32_t>(rows));
    m_dirty = true;
}

auto kestrel::ui::widgets::grid_widget::on_item_select(const luabridge::LuaRef &callback) -> void
{
    m_item_select_callback = callback;
}

// MARK: - Drawing

auto kestrel::ui::widgets::grid_widget::draw() -> void
{
    if (m_dirty) {
        redraw_entity();
    }
}

auto kestrel::ui::widgets::grid_widget::redraw_entity() -> void
{
    m_canvas->clear();
    m_canvas->set_font(m_label_font);

    if (!m_items.empty()) {
        auto count = static_cast<std::uint32_t>(m_grid.width * m_grid.height);
        auto first_idx = static_cast<std::size_t>(m_scroll_offset * m_grid.width);
        auto last_idx = std::min(first_idx + count, m_items.size());

        auto grid_width = static_cast<std::uint32_t>(m_grid.width);
        bool selection_visible = false;
        math::point selected_cell;

        for (auto i = first_idx; i < last_idx; ++i) {
            auto k = i - first_idx;
            auto item = m_items[i];
            math::point position((k % grid_width) * m_cell_size.width, (k / grid_width) * m_cell_size.height);

            if ((i + 1) == m_selected_item) {
                selection_visible = true;
                selected_cell = position;
            }

            m_canvas->set_pen_color(*m_outline_color);
            m_canvas->draw_rect({ position, m_cell_size });

            if (m_cell_drawing_function.state()) {
                m_cell_drawing_function(m_canvas, math::rect(position, m_cell_size), item);
            }
            else {
                auto icon_scale = (1.0 / item->icon()->size().height) * (m_cell_size.height * 0.6);
                auto icon_size = item->icon()->size() * icon_scale;
                math::point icon_position((m_cell_size.width - icon_size.width) / 2.0, 3.0);

                auto p = icon_position + position;
                math::rect r(p.round(), icon_size.round());
                m_canvas->draw_static_image(item->icon(), r);

                auto base = m_cell_size.height - 7;
                auto center = m_cell_size.width / 2;

                const auto& lines = item->lines();
                for (std::int32_t j = lines.size() - 1; j >= 0; --j) {
                    auto title = lines.at(j);

                    if (std::isalnum(title[0])) {
                        m_canvas->set_pen_color(*m_text_color);
                    }
                    else {
                        m_canvas->set_pen_color(*m_secondary_text_color);
                    }

                    auto title_size = m_canvas->layout_text_in_bounds(title, { m_cell_size.width - 6, m_cell_size.height - 6 });
                    base -= title_size.height - 8;

                    auto title_origin = position + math::point(center - (title_size.width / 2), base);
                    m_canvas->draw_text(title_origin);
                }

                if (!item->corner().empty()) {
                    m_canvas->set_pen_color(*m_text_color);
                    auto count_size = m_canvas->layout_text_in_bounds(item->corner(), { m_cell_size.width - 6, m_cell_size.height - 6 });
                    auto count_origin = position + math::point(m_cell_size.width - 3 - count_size.width, 3);
                    m_canvas->draw_text(count_origin);
                }
            }
        }

        if (selection_visible) {
            m_canvas->set_pen_color(*m_hilite_color);
            m_canvas->draw_rect({ selected_cell, m_cell_size });
        }
    }

    m_canvas->rebuild_texture();
}

// MARK: - Grid Item

kestrel::ui::widgets::grid_item::grid_item(const luabridge::LuaRef &icon, const std::string &title, const std::string &subtitle, const std::string &corner)
    : m_title(title), m_subtitle(subtitle), m_corner(corner)
{
    if (lua::ref_isa<image::static_image>(icon)) {
        m_icon = icon.cast<image::static_image::lua_reference>();
    }

    if (!m_title.empty()) {
        m_lines.emplace_back(m_title);
    }
    if (!m_subtitle.empty()) {
        m_lines.emplace_back(m_subtitle);
    }
}