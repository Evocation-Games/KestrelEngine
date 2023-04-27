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
    m_appearance.label_font = font::manager::shared_manager().default_font();
    m_appearance.label_font->load_for_graphics();

    m_entity.canvas = { new graphics::canvas(frame.size()) };
    m_entity.entity = { new scene_entity(m_entity.canvas->spawn_entity(frame.origin())) };
    m_entity.entity->set_clipping_area(frame.size());

    m_entity.entity->internal_entity()->set_position(frame.origin());
    m_entity.entity->set_position(frame.origin());

    redraw_entity();
    bind_internal_events();
}

// MARK: - Framing & Position

auto kestrel::ui::widgets::grid_widget::frame() const -> math::rect
{
    return { m_entity.entity->position(), m_entity.entity->size() };
}

auto kestrel::ui::widgets::grid_widget::set_frame(const math::rect &frame) -> void
{
    setup(frame);
}

// MARK: - Entity

auto kestrel::ui::widgets::grid_widget::entity() const -> scene_entity::lua_reference
{
    return m_entity.entity;
}

// MARK: - Selection & State

auto kestrel::ui::widgets::grid_widget::selected_item() const -> std::int32_t
{
    return m_state.selection.item;
}

auto kestrel::ui::widgets::grid_widget::select_item(std::int32_t item) -> void
{
    m_state.selection.item = item;
    m_state.dirty = true;
}

// MARK: - Appearance

auto kestrel::ui::widgets::grid_widget::font() const -> font::reference::lua_reference
{
    return m_appearance.label_font;
}

auto kestrel::ui::widgets::grid_widget::text_color() const -> graphics::color::lua_reference
{
    return m_appearance.text_color;
}

auto kestrel::ui::widgets::grid_widget::secondary_text_color() const -> graphics::color::lua_reference
{
    return m_appearance.secondary_text_color;
}

auto kestrel::ui::widgets::grid_widget::background_color() const -> graphics::color::lua_reference
{
    return m_appearance.background_color;
}

auto kestrel::ui::widgets::grid_widget::hilite_color() const -> graphics::color::lua_reference
{
    return m_appearance.hilite_color;
}

auto kestrel::ui::widgets::grid_widget::outline_color() const -> graphics::color::lua_reference
{
    return m_appearance.outline_color;
}

auto kestrel::ui::widgets::grid_widget::set_font(const font::reference::lua_reference& font) -> void
{
    m_appearance.label_font = font;
    m_appearance.label_font->load_for_graphics();
    m_state.dirty = true;
}

auto kestrel::ui::widgets::grid_widget::set_text_color(const graphics::color::lua_reference& color) -> void
{
    m_appearance.text_color = color;
    m_state.dirty = true;
}

auto kestrel::ui::widgets::grid_widget::set_secondary_text_color(const graphics::color::lua_reference& color) -> void
{
    m_appearance.secondary_text_color = color;
    m_state.dirty = true;
}

auto kestrel::ui::widgets::grid_widget::set_background_color(const graphics::color::lua_reference& color) -> void
{
    m_appearance.background_color = color;
    m_state.dirty = true;
}

auto kestrel::ui::widgets::grid_widget::set_hilite_color(const graphics::color::lua_reference& color) -> void
{
    m_appearance.hilite_color = color;
    m_state.dirty = true;
}

auto kestrel::ui::widgets::grid_widget::set_outline_color(const graphics::color::lua_reference& color) -> void
{
    m_appearance.outline_color = color;
    m_state.dirty = true;
}

// MARK: - Calculations & Transformations

auto kestrel::ui::widgets::grid_widget::cell_index_at_point(const math::point &p) const -> std::int32_t
{
    const auto cell_size = this->cell_size(0);
    const auto grid_cols = static_cast<std::int32_t>(std::ceil(frame().size().width() / cell_size.width()));
    auto cell = (math::vec2(p) / math::vec2(cell_size)).to_point().floor();
    return static_cast<std::int32_t>(1 + ((m_state.scroll.offset.y() + cell.y()) * grid_cols) + cell.x());
}

auto kestrel::ui::widgets::grid_widget::cell_at_point(const math::point &p) -> luabridge::LuaRef
{
    const auto idx = cell_index_at_point(p);
    if (idx < 1 || idx > number_of_items()) {
        return { nullptr };
    }
    return value_for_cell(idx);
}

auto kestrel::ui::widgets::grid_widget::scroll_up() -> void
{
    m_state.scroll.offset.set_y(m_state.scroll.offset.y() - 1);
    m_state.dirty = true;
}

auto kestrel::ui::widgets::grid_widget::scroll_down() -> void
{
    const auto cell_size = this->cell_size(0);
    const auto grid_cols = static_cast<std::int32_t>(std::ceil(frame().size().width() / cell_size.width()));

    auto rows = static_cast<float>(std::ceil(number_of_items() / grid_cols));
    m_state.scroll.offset.set_y(std::min(m_state.scroll.offset.y() + 1, rows));
    m_state.dirty = true;
}

// MARK: - Events

auto kestrel::ui::widgets::grid_widget::bind_internal_events() -> void
{
}

auto kestrel::ui::widgets::grid_widget::receive_event(const event &e) -> bool
{
    auto local_position = e.location() - entity()->position();
    if (e.is_mouse_event() && entity()->hit_test(local_position)) {
        if (e.has(::ui::event::any_mouse_down) && !m_state.pressed) {
            m_state.pressed = true;
        }

        if (e.has(::ui::event::any_mouse_up) && m_state.pressed) {
            auto item_number = cell_index_at_point(local_position);
            m_state.dirty = true;

            if (item_number > number_of_items() || item_number <= 0) {
                m_state.selection.item = -1;
            }
            else {
                m_state.selection.item = item_number;
            }
            cell_selected(item_number);

            redraw_entity();
            m_state.pressed = false;
        }

        return true;
    }
    return false;
}

// MARK: - Drawing

auto kestrel::ui::widgets::grid_widget::draw() -> void
{
    if (m_state.dirty) {
        redraw_entity();
    }
}

auto kestrel::ui::widgets::grid_widget::redraw_entity() -> void
{
    m_entity.canvas->clear();
    m_entity.canvas->set_font(m_appearance.label_font);

    const auto cell_size = this->cell_size(0);
    const auto grid_cols = static_cast<std::uint32_t>(std::ceil(frame().size().width() / cell_size.width()));
    const auto grid_rows = static_cast<std::uint32_t>(std::ceil(frame().size().width() / grid_cols));
    const auto item_count = number_of_items();

    if (item_count > 0) {
        const auto count = static_cast<std::uint32_t>(grid_cols * grid_rows);
        const auto first_idx = static_cast<std::int32_t>(m_state.scroll.offset.y() * static_cast<float>(grid_cols));
        const auto last_idx = std::min(first_idx + count, item_count);

        bool selection_visible = false;
        math::point selected_cell(0);

        for (auto i = first_idx; i < last_idx; ++i) {
            auto k = i - first_idx;
            auto item = value_for_cell(i + 1);
            math::point position(k % grid_cols * cell_size.width(), k / grid_cols * cell_size.height());

            if ((i + 1) == m_state.selection.item) {
                selection_visible = true;
                selected_cell = position;
            }

            m_entity.canvas->set_pen_color(*m_appearance.outline_color);
            m_entity.canvas->draw_rect({ position, cell_size });

            draw_cell(math::rect(position, cell_size), item);
        }

        if (selection_visible) {
            m_entity.canvas->set_pen_color(*m_appearance.hilite_color);
            m_entity.canvas->draw_rect({ selected_cell, cell_size });
        }
    }

    m_entity.canvas->rebuild_texture();
}

auto kestrel::ui::widgets::grid_widget::draw_cell(math::rect bounds, const luabridge::LuaRef &value) -> void
{
    if (!m_delegate.draw_cell.state() || !m_delegate.draw_cell.isFunction()) {
        draw_default_cell(bounds, value);
        return;
    }

    m_delegate.draw_cell(m_entity.canvas, bounds, value);
}

auto kestrel::ui::widgets::grid_widget::draw_default_cell(math::rect bounds, const luabridge::LuaRef &value) -> void
{
    if (!value.state() || !value.isTable()) {
        return;
    }

    // Gather the information to display in the cell.
    const auto& title = value[title_key()];
    const auto& subtitle = value[subtitle_key()];
    const auto& icon = value[icon_key()];

    // Draw the icon if it exists.
    if (icon.state() && !icon.isNil()) {
        image::static_image::lua_reference icon_image { nullptr };
        if (lua::ref_isa<resource::descriptor::lua_reference>(icon)) {
            icon_image = {
                image::static_image::lua_reference(new image::static_image(icon.cast<resource::descriptor::lua_reference>()))
            };
        }
        else if (lua::ref_isa<image::static_image::lua_reference>(icon)) {
            icon_image = icon.cast<image::static_image::lua_reference>();
        }
        if (icon_image.get()) {
            const auto icon_scale = (1.f / icon_image->size().height()) * bounds.height() * 0.6f;
            const auto icon_size = icon_image->size() * icon_scale;
            math::point icon_position((bounds.size().width() - icon_size.width()) / 2.f, 3.f);
            const auto draw_position = icon_position + bounds.origin();
            math::rect icon_frame(draw_position.round(), icon_size.round());
            m_entity.canvas->draw_static_image(icon_image, icon_frame);
        }
    }

    std::vector<std::string> lines;
    if (title.state() && title.isString()) {
        lines.emplace_back(title.tostring());
    }
    if (subtitle.state() && subtitle.isString()) {
        lines.emplace_back(subtitle.tostring());
    }

    if (!lines.empty()) {
        auto base = bounds.size().height() - 7;
        const auto center = bounds.size().width() / 2.f;
        for (auto it = lines.rbegin(); it != lines.rend(); ++it) {
            if (std::isalnum((*it)[0])) {
                m_entity.canvas->set_pen_color(m_appearance.text_color);
            }
            else {
                m_entity.canvas->set_pen_color(m_appearance.secondary_text_color);
            }

            const auto title_size = m_entity.canvas->layout_text_in_bounds(*it, bounds.size() - 6.f);
            base -= title_size.height() - 8.f;
            const auto title_origin = bounds.origin() + math::point(center - (title_size.width() / 2.f), base);
            m_entity.canvas->draw_text(title_origin);
        }
    }
}

// MARK: - Data Source

auto kestrel::ui::widgets::grid_widget::data_source() const -> luabridge::LuaRef
{
    return luabridge::LuaRef::newTable(kestrel::lua_runtime()->internal_state());
}

auto kestrel::ui::widgets::grid_widget::set_data_source(const luabridge::LuaRef &data_source) -> void
{
    if (!data_source.state() || !data_source.isTable()) {
        return;
    }

    const auto& number_of_items = data_source["numberOfItems"];
    const auto& value_for_cell = data_source["valueForCell"];

    if (number_of_items.isFunction() || number_of_items.isNumber()) {
        m_data_source.number_of_items = number_of_items;
    }

    if (value_for_cell.isFunction()) {
        m_data_source.value_for_cell = value_for_cell;
    }
}

auto kestrel::ui::widgets::grid_widget::reload_data() -> void
{
    m_state.dirty = true;
    redraw_entity();
}

auto kestrel::ui::widgets::grid_widget::number_of_items() const -> std::uint32_t
{
    if (m_data_source.number_of_items.state() && m_data_source.number_of_items.isFunction()) {
        auto value = m_data_source.number_of_items();
        if (value.state() && value.isNumber()) {
            return value.cast<std::uint32_t>();
        }
    }
    else if (m_data_source.number_of_items.state() && m_data_source.number_of_items.isNumber()) {
        return m_data_source.number_of_items.cast<std::uint32_t>();
    }
    return 0;
}

auto kestrel::ui::widgets::grid_widget::value_for_cell(std::int32_t index) const -> luabridge::LuaRef
{
    if (m_data_source.value_for_cell.state() && m_data_source.value_for_cell.isFunction()) {
        return m_data_source.value_for_cell(index);
    }
    return { kestrel::lua_runtime()->internal_state() };
}

// MARK: - Delegate

auto kestrel::ui::widgets::grid_widget::delegate() const -> luabridge::LuaRef
{
    return luabridge::LuaRef::newTable(kestrel::lua_runtime()->internal_state());
}

auto kestrel::ui::widgets::grid_widget::set_delegate(const luabridge::LuaRef &delegate) -> void
{
    if (!delegate.state() || !delegate.isTable()) {
        return;
    }

    const auto& on_cell_select = delegate["onCellSelected"];
    const auto& should_select_cell = delegate["shouldSelectCell"];
    const auto& size_for_cell = delegate["sizeForCell"];
    const auto& draw_cell = delegate["drawCell"];
    const auto& title_key = delegate["titleKey"];
    const auto& subtitle_key = delegate["subtitleKey"];
    const auto& icon_key = delegate["iconKey"];

    if (on_cell_select.state() && on_cell_select.isFunction()) {
        m_delegate.on_cell_select = on_cell_select;
    }

    if (should_select_cell.state() && (should_select_cell.isFunction() || should_select_cell.isBool())) {
        m_delegate.should_select_cell = should_select_cell;
    }

    if (size_for_cell.state() && (size_for_cell.isFunction() || size_for_cell.isInstance<math::size>())) {
        m_delegate.size_for_cell = size_for_cell;
    }

    if (draw_cell.state() && draw_cell.isFunction()) {
        m_delegate.draw_cell = draw_cell;
    }

    if (title_key.state() && (title_key.isString() || title_key.isFunction())) {
        m_delegate.title_key = title_key;
    }

    if (subtitle_key.state() && (subtitle_key.isString() || subtitle_key.isFunction())) {
        m_delegate.subtitle_key = subtitle_key;
    }

    if (icon_key.state() && (icon_key.isString() || icon_key.isFunction())) {
        m_delegate.icon_key = icon_key;
    }
}

auto kestrel::ui::widgets::grid_widget::should_select_cell(std::int32_t cell) const -> bool
{
    if (m_delegate.should_select_cell.state() && m_delegate.should_select_cell.isFunction()) {
        auto value = m_delegate.should_select_cell(cell);
        if (value.state() && value.isBool()) {
            return value.cast<bool>();
        }
    }
    else if (m_delegate.should_select_cell.state() && m_delegate.should_select_cell.isBool()) {
        return m_delegate.should_select_cell.cast<bool>();
    }
    return true;
}

auto kestrel::ui::widgets::grid_widget::cell_size(std::int32_t cell) const -> math::size
{
    if (m_delegate.size_for_cell.state() && m_delegate.size_for_cell.isFunction()) {
        auto value = m_delegate.size_for_cell(cell);
        if (value.state() && value.isInstance<math::size>()) {
            return value.cast<math::size>();
        }
    }
    else if (m_delegate.size_for_cell.state() && m_delegate.size_for_cell.isInstance<math::size>()) {
        return m_delegate.size_for_cell.cast<math::size>();
    }
    return frame().size() / 4.f;
}

auto kestrel::ui::widgets::grid_widget::title_key() const -> std::string
{
    if (m_delegate.title_key.state() && m_delegate.title_key.isFunction()) {
        auto value = m_delegate.title_key();
        if (value.state() && value.isString()) {
            return value.tostring();
        }
    }
    else if (m_delegate.title_key.state() && m_delegate.title_key.isString()) {
        return m_delegate.title_key.tostring();
    }
    return "title";
}

auto kestrel::ui::widgets::grid_widget::subtitle_key() const -> std::string
{
    if (m_delegate.subtitle_key.state() && m_delegate.subtitle_key.isFunction()) {
        auto value = m_delegate.subtitle_key();
        if (value.state() && value.isString()) {
            return value.tostring();
        }
    }
    else if (m_delegate.subtitle_key.state() && m_delegate.subtitle_key.isString()) {
        return m_delegate.subtitle_key.tostring();
    }
    return "subtitle";
}

auto kestrel::ui::widgets::grid_widget::icon_key() const -> std::string
{
    if (m_delegate.icon_key.state() && m_delegate.icon_key.isFunction()) {
        auto value = m_delegate.icon_key();
        if (value.state() && value.isString()) {
            return value.tostring();
        }
    }
    else if (m_delegate.icon_key.state() && m_delegate.icon_key.isString()) {
        return m_delegate.icon_key.tostring();
    }
    return "icon";
}

auto kestrel::ui::widgets::grid_widget::cell_selected(std::int32_t i) -> void
{
    if (m_delegate.on_cell_select.state() && m_delegate.on_cell_select.isFunction()) {
        m_delegate.on_cell_select(i);
    }
}

auto kestrel::ui::widgets::grid_widget::cell_origin(std::int32_t cell) const -> math::point
{
    const auto cell_size = this->cell_size(0); // TODO: Convert to the cell index specified.
    const auto grid_cols = static_cast<std::uint32_t>(std::ceil(frame().size().width() / cell_size.width()));
    const auto first_idx = static_cast<std::int32_t>(m_state.scroll.offset.y() * static_cast<float>(grid_cols));
    auto k = cell - first_idx;
    return { k % grid_cols * cell_size.width(), k / grid_cols * cell_size.height() };
}
