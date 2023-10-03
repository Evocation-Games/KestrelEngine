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

#include <libKestrel/ui/widgets/textarea_widget.hpp>
#include <libKestrel/font/typesetter.hpp>
#include <libKestrel/ui/entity/scene_entity.hpp>
#include <libKestrel/font/manager.hpp>

// MARK: - Construction

kestrel::ui::widgets::textarea_widget::textarea_widget(const std::string &text)
    : m_text(text)
{
    m_font = font::manager::shared_manager().default_font();
    m_font->load_for_graphics();

    font::typesetter ts(text);
    ts.set_font(*m_font.get());
    ts.layout();

    m_canvas = std::make_unique<graphics::canvas>(ts.get_bounding_size());
    m_entity = { new scene_entity(m_canvas->spawn_entity({ 0, 0 })) };
}

kestrel::ui::widgets::textarea_widget::textarea_widget(const math::rect &frame, const std::string &text)
    : m_text(text), m_clipping_size(frame.size())
{
    m_font = font::manager::shared_manager().default_font();
    m_font->load_for_graphics();

    font::typesetter ts(text);
    ts.set_margins(frame.size().round());
    ts.set_font(*m_font.get());
    ts.layout();

    m_canvas = std::make_unique<graphics::canvas>(math::size(frame.size().width(), std::max(frame.size().height(), ts.get_bounding_size().height())));
    m_entity = { new scene_entity(m_canvas->spawn_entity(frame.origin())) };
    m_entity->set_clipping_area(m_clipping_size);
}

// MARK: - Accessors


auto kestrel::ui::widgets::textarea_widget::entity() const -> ui::scene_entity::lua_reference
{
    return m_entity;
}

auto kestrel::ui::widgets::textarea_widget::text() const -> std::string
{
    return m_text;
}

auto kestrel::ui::widgets::textarea_widget::font() const -> font::reference::lua_reference
{
    return m_font;
}

auto kestrel::ui::widgets::textarea_widget::color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_color) };
}

auto kestrel::ui::widgets::textarea_widget::background_color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_background) };
}

auto kestrel::ui::widgets::textarea_widget::offset() const -> math::size
{
    return m_offset;
}

auto kestrel::ui::widgets::textarea_widget::position() const -> math::point
{
    return m_entity->position();
}

auto kestrel::ui::widgets::textarea_widget::size() const -> math::size
{
    return m_entity->size();
}

auto kestrel::ui::widgets::textarea_widget::frame() const -> math::rect
{
    return { position(), size() };
}

auto kestrel::ui::widgets::textarea_widget::scroll_offset() const -> std::int32_t
{
    return m_scroll_offset;
}

// MARK: - Setters

auto kestrel::ui::widgets::textarea_widget::set_text(const std::string& v) -> void
{
    m_text = v;
    m_dirty = true;
}

auto kestrel::ui::widgets::textarea_widget::set_font(const font::reference::lua_reference& font) -> void
{
    m_font = font;
    m_font->load_for_graphics();
    m_dirty = true;
}

auto kestrel::ui::widgets::textarea_widget::set_color(const graphics::color::lua_reference& v) -> void
{
    m_color = { *v.get() };
    m_dirty = true;
}

auto kestrel::ui::widgets::textarea_widget::set_background_color(const graphics::color::lua_reference& v) -> void
{
    m_background = { *v.get() };
    m_dirty = true;
}

auto kestrel::ui::widgets::textarea_widget::set_offset(const math::size& v) -> void
{
    m_offset = v;
    m_dirty = true;
}

auto kestrel::ui::widgets::textarea_widget::set_position(const math::point& v) -> void
{
    m_entity->set_position(v);
}

auto kestrel::ui::widgets::textarea_widget::set_size(const math::size& v) -> void
{
    m_entity->set_size(v);
    m_entity->internal_entity()->set_clipping_area(v);
    m_entity->set_clipping_area(v);
    m_clipping_size = v;
}

auto kestrel::ui::widgets::textarea_widget::set_frame(const math::rect& v) -> void
{
    set_size(v.size());

    m_entity->internal_entity()->set_position(v.origin());
    m_entity->set_position(v.origin());
}

auto kestrel::ui::widgets::textarea_widget::set_scroll_offset(std::int32_t offset) -> void
{
    // Check to make sure we aren't at the end of the scroll bounds
    m_can_scroll_up = true;
    if (offset < 0) {
        offset = 0;
        m_can_scroll_up = false;
    }

    m_can_scroll_down = true;
    if (offset + m_entity->clipping_area().height() > m_canvas->get_bounds().size().height()) {
        offset = m_canvas->get_bounds().size().height() - m_entity->size().height();
        m_can_scroll_down = false;
    }

    m_scroll_offset = offset;
    m_entity->set_clipping_offset({ 0, static_cast<float>(m_scroll_offset) });
}

auto kestrel::ui::widgets::textarea_widget::can_scroll_up() const -> bool
{
    return m_can_scroll_up;
}

auto kestrel::ui::widgets::textarea_widget::can_scroll_down() const -> bool
{
    return m_can_scroll_down;
}

// MARK: - Drawing

auto kestrel::ui::widgets::textarea_widget::redraw_entity() -> void
{
    auto size = m_entity->size();
    m_canvas->set_font(m_font);

    auto text_size = m_canvas->layout_text_in_bounds(m_text, size);
    auto x = m_offset.width();
    auto y = m_offset.height() - 2 - m_scroll_offset;

    auto height = std::max(text_size.height(), m_entity->size().height());
    m_canvas = std::make_unique<graphics::canvas>(math::size(m_entity->size().width(), height));

    m_canvas->set_pen_color({ new graphics::color(m_background) });
    m_canvas->fill_rect({{0, 0}, size});

    m_canvas->set_pen_color({ new graphics::color(m_color) });
    m_canvas->set_font(m_font);

    m_canvas->layout_text_in_bounds(m_text, size);
    m_canvas->draw_text({ x, y });
    m_canvas->rebuild_texture();

    m_entity->change_internal_entity(m_canvas->spawn_entity({0, 0}));
    m_entity->set_size(m_clipping_size);
    m_entity->set_clipping_area(m_clipping_size);
    m_dirty = false;
}

auto kestrel::ui::widgets::textarea_widget::draw() -> void
{
    if (m_dirty) {
        redraw_entity();
    }
}

// MARK: - Scrolling

auto kestrel::ui::widgets::textarea_widget::scroll_up() -> void
{
    set_scroll_offset(m_scroll_offset - 2);
}

auto kestrel::ui::widgets::textarea_widget::scroll_down() -> void
{
    set_scroll_offset(m_scroll_offset + 2);
}
