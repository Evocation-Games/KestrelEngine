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

#include <libKestrel/ui/widgets/menu_widget.hpp>
#include <libKestrel/font/manager.hpp>

// MARK: - Construction

kestrel::ui::widgets::menu_widget::menu_widget(const lua::vector<std::string> &items, std::int32_t selected_item)
{
    // Load up a default font.
    m_font = font::manager::shared_manager().default_font();
    m_font->load_for_graphics();

    // Set up sizing defaults
    m_vertical_padding = 10;
    m_horizontal_padding = 10;
    m_row_height = m_font->line_height() + 6;

    // TODO: Calculate the width based on the size of the items...

    m_canvas = std::make_unique<graphics::canvas>(math::size(
        200 + (m_horizontal_padding * 2),
        m_row_height + (m_vertical_padding * 2)
    ));
    m_entity = { new scene_entity(m_canvas->spawn_entity({0, 0})) };
}

// MARK: - Accessors

auto kestrel::ui::widgets::menu_widget::row_height() const -> double
{
    return m_row_height;
}

auto kestrel::ui::widgets::menu_widget::vertical_padding() const -> double
{
    return m_vertical_padding;
}

auto kestrel::ui::widgets::menu_widget::horizontal_padding() const -> double
{
    return m_horizontal_padding;
}

auto kestrel::ui::widgets::menu_widget::index_of_selected_item() const -> std::int32_t
{
    return m_selected_item;
}

auto kestrel::ui::widgets::menu_widget::selected_item() const -> std::string
{
    if (m_selected_item >= 0 && m_selected_item < m_items.size() && !m_items.empty()) {
        return m_items.at(m_selected_item);
    }
    return "";
}

auto kestrel::ui::widgets::menu_widget::items() const -> lua::vector<std::string>
{
    return m_items;
}

auto kestrel::ui::widgets::menu_widget::color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_color) };
}

auto kestrel::ui::widgets::menu_widget::background_color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_background_color) };
}

auto kestrel::ui::widgets::menu_widget::selection_color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_selection_color) };
}

auto kestrel::ui::widgets::menu_widget::border_color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_border_color) };
}

auto kestrel::ui::widgets::menu_widget::font() const -> font::reference::lua_reference
{
    return m_font;
}

auto kestrel::ui::widgets::menu_widget::entity() const -> ui::scene_entity::lua_reference
{
    return m_entity;
}

// MARK: - Setters

auto kestrel::ui::widgets::menu_widget::set_row_height(double height) -> void
{
    m_row_height = height;
    recalculate_entity_size();
}

auto kestrel::ui::widgets::menu_widget::set_vertical_padding(double padding) -> void
{
    m_vertical_padding = padding;
    recalculate_entity_size();
}

auto kestrel::ui::widgets::menu_widget::set_horizontal_padding(double padding) -> void
{
    m_horizontal_padding = padding;
    recalculate_entity_size();
}

auto kestrel::ui::widgets::menu_widget::set_index_of_selected_item(std::int32_t index) -> void
{
    m_selected_item = index;
    m_dirty = true;
}

auto kestrel::ui::widgets::menu_widget::set_items(const lua::vector<std::string> &items) -> void
{
    m_items = items;
    recalculate_entity_size();
}

auto kestrel::ui::widgets::menu_widget::set_color(const graphics::color::lua_reference &v) -> void
{
    m_color = *v.get();
    m_dirty = true;
    redraw_entity();
}

auto kestrel::ui::widgets::menu_widget::set_background_color(const graphics::color::lua_reference &v) -> void
{
    m_background_color = *v.get();
    m_dirty = true;
    redraw_entity();
}

auto kestrel::ui::widgets::menu_widget::set_border_color(const graphics::color::lua_reference &v) -> void
{
    m_border_color = *v.get();
    m_dirty = true;
    redraw_entity();
}

auto kestrel::ui::widgets::menu_widget::set_selection_color(const graphics::color::lua_reference &v) -> void
{
    m_selection_color = *v.get();
    m_dirty = true;
    redraw_entity();
}

auto kestrel::ui::widgets::menu_widget::set_font(const font::reference::lua_reference &font) -> void
{
    m_font = font;
    recalculate_entity_size();
}

// MARK: - Presentation

auto kestrel::ui::widgets::menu_widget::present_from_parent(const ui::scene_entity::lua_reference &entity, const std::function<auto()->void>& callback) -> void
{
    m_parent_entity = entity;
    m_update_callback = callback;
    recalculate_entity_size();
}

auto kestrel::ui::widgets::menu_widget::will_close() -> void
{
    if (m_parent_entity.get()) {
        m_update_callback();
    }
}

// MARK: - Drawing

auto kestrel::ui::widgets::menu_widget::recalculate_entity_size() -> void
{
    // Determine the width of the largest item.
    auto max_width = 0.f;
    graphics::canvas canvas(math::size(1, 1));
    for (const auto& item : m_items) {
        const auto& size = canvas.layout_text(item);
        max_width = std::max(max_width, size.width());
    }

    // Determine the maximum number of items to display at once.
    m_displayed_items = std::min(m_max_items, static_cast<std::uint32_t>(m_items.size()));

    // Calculate the ideal size.
    math::size menu_size(
        std::max(100.f, max_width) + (m_horizontal_padding * 2.f) + m_indicator_spacing,
        (m_displayed_items * m_row_height) + (m_vertical_padding * 2.f)
    );

    // Calculate the position of the menu.
    math::point menu_position(
        m_parent_entity->position().x() + 5,
        m_parent_entity->position().y() + m_parent_entity->size().height()
    );

    // Resize the entity with the updated information and flag the drawing state as dirty.
    m_canvas = std::make_unique<graphics::canvas>(menu_size);
    m_entity = { new scene_entity(m_canvas->spawn_entity({0, 0})) };
    m_entity->set_position(menu_position);

    m_dirty = true;
    redraw_entity();
}

auto kestrel::ui::widgets::menu_widget::redraw_entity() -> void
{
    if (!m_dirty || !m_parent_entity.get()) {
        return;
    }

    const auto size = math::size(m_entity->size().width() - 1, m_entity->size().height());
    math::rect frame { {0, 0}, size };

    m_canvas->clear();
    m_canvas->set_pen_color({ new graphics::color(m_border_color) });
    m_canvas->fill_rect(frame);

    m_canvas->set_pen_color({ new graphics::color(m_background_color) });
    m_canvas->fill_rect(frame.inset(1));

    m_canvas->set_font(m_font);
    m_canvas->set_pen_color({ new graphics::color(m_color) });

    auto row_y = m_vertical_padding;
    for (auto i = 0; i < m_displayed_items; ++i) {
        const auto item_idx = m_start_item + i;
        const auto& item = m_items.at(item_idx);
        auto text_size = m_canvas->layout_text_in_bounds(item, { frame.size().width() - (m_horizontal_padding * 2), m_row_height });

        if (m_mouse_over && (m_hover_item == i)) {
            // Hover
            m_canvas->set_pen_color({ new graphics::color(m_selection_color) });
            m_canvas->fill_rect(math::rect(frame.origin().x() + 3, row_y, frame.size().width() - 6, m_row_height));
        }
        m_canvas->set_pen_color({ new graphics::color(m_color) });

        m_canvas->set_clipping_rect(math::rect(0, row_y, frame.size().width(), m_row_height));
        m_canvas->draw_text({ m_horizontal_padding, row_y + ((m_row_height - text_size.height()) / 2.f) });
        m_canvas->clear_clipping_rect();

        row_y += m_row_height;
    }

    m_canvas->rebuild_texture();
    m_dirty = false;
}

auto kestrel::ui::widgets::menu_widget::draw() -> void
{
    if (m_dirty && m_parent_entity.get()) {
        redraw_entity();
    }
}

// MARK: - Events

auto kestrel::ui::widgets::menu_widget::receive_event(const event &e) -> bool
{
    if (e.is_mouse_event()) {
        const auto point = e.location() - entity()->position();
        const auto hover_row = static_cast<std::int32_t>((point.y() - m_vertical_padding) / m_row_height);

        if (!m_mouse_over && e.has(event_type::mouse_move) && entity()->hit_test(point)) {
            m_mouse_over = true;
            if (m_hover_item != hover_row) {
                m_hover_item = hover_row;
                m_dirty = true;
                redraw_entity();
            }
        }
        else if (m_mouse_over && !e.has(event_type::mouse_move) && !entity()->hit_test(point)) {
            m_mouse_over = false;
            m_dirty = true;
            redraw_entity();
        }
        else if (m_mouse_over && e.has(event_type::mouse_move) && entity()->hit_test(point)) {
            m_mouse_over = true;
            m_dirty = true;
            if (m_hover_item != hover_row) {
                m_hover_item = hover_row;
                m_dirty = true;
                redraw_entity();
            }
        }

        if (m_mouse_over && e.has(event_type::any_mouse_down)) {
            m_selected_item = hover_row;
            m_hover_item = -1;
            m_dirty = true;
            return true;
        }
    }
    return false;
}

