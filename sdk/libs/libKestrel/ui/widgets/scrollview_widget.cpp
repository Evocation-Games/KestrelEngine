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

#include <libKestrel/ui/widgets/scrollview_widget.hpp>

// MARK: - Construction

kestrel::ui::widgets::scrollview_widget::scrollview_widget()
{
    m_canvas = { new graphics::canvas({ 100, 100 }) };
    m_entity = { new scene_entity(m_canvas) };

    bind_internal_events();
}

// MARK: - Accessors

auto kestrel::ui::widgets::scrollview_widget::entity() const -> scene_entity::lua_reference
{
    return m_entity;
}

auto kestrel::ui::widgets::scrollview_widget::position() const -> math::point
{
    return m_entity->position();
}

auto kestrel::ui::widgets::scrollview_widget::size() const -> math::size
{
    return m_entity->render_size();
}

auto kestrel::ui::widgets::scrollview_widget::frame() const -> math::rect
{
    return { position(), size() };
}

auto kestrel::ui::widgets::scrollview_widget::scroll_offset() const -> math::point
{
    return m_scroll_offset;
}

auto kestrel::ui::widgets::scrollview_widget::set_position(const math::point &v) -> void
{
    m_entity->set_position(v);
    m_dirty = true;
}

auto kestrel::ui::widgets::scrollview_widget::set_size(const math::size &v) -> void
{
    m_entity->set_size(v);
    m_entity->set_render_size(v);
    m_dirty = true;
}

auto kestrel::ui::widgets::scrollview_widget::set_frame(const math::rect &v) -> void
{
    set_position(v.origin());
    set_size(v.size());
    m_dirty = true;
}

auto kestrel::ui::widgets::scrollview_widget::set_scroll_offset(const math::point &v) -> void
{
    m_scroll_offset = v;
    m_dirty = true;
}

auto kestrel::ui::widgets::scrollview_widget::bind_internal_events() -> void
{

}

auto kestrel::ui::widgets::scrollview_widget::receive_event(const event &e) -> bool
{
    bool result = false;

    if (e.is_mouse_event()) {
        if (e.has(::ui::event::mouse_drag)) {
            auto delta = e.location() - m_drag_mouse_origin;
            set_scroll_offset(m_drag_scroll_origin - delta);
            return true;
        }
        else if (e.has(::ui::event::any_mouse_down)) {
            m_drag_mouse_origin = e.location();
            m_drag_scroll_origin = m_scroll_offset;

            result = true;
        }
        else if (e.has(::ui::event::any_mouse_up)) {
            auto delta = e.location() - m_drag_mouse_origin;
            if (delta.magnitude() > 2.0) {
                return true;
            }
        }
    }

    if (m_content_entity.get()) {
        m_content_entity->send_event(e);
        result = true;
    }

    return false;
}

// MARK: - Content

auto kestrel::ui::widgets::scrollview_widget::set_content_entity(const scene_entity::lua_reference &entity) -> void
{
    if (m_content_entity.get()) {
        m_entity->remove_entity(m_content_entity);
    }

    m_content_entity = entity;
    m_entity->add_child_entity(m_content_entity);
    m_dirty = true;
}

// MARK: - Drawing

auto kestrel::ui::widgets::scrollview_widget::draw() -> void
{
    redraw_entity();
}

auto kestrel::ui::widgets::scrollview_widget::redraw_entity() -> void
{
    if (!m_dirty || !m_content_entity.get()) {
        return;
    }

    math::point scroll_offset(0);
    math::size clip_size(0);
    math::point origin(0);
    math::size size(0);

    if (m_content_entity->render_size().width() >= m_entity->render_size().width()) {
        clip_size.set_width(m_entity->render_size().width());
        size.set_width(m_entity->render_size().width());
        origin.set_x(0);
        scroll_offset.set_x(m_scroll_offset.x() - (m_content_entity->render_size().width() / 2.f));
    }
    else {
        clip_size.set_width(m_content_entity->render_size().width());
        size.set_width(m_content_entity->render_size().width());
        origin.set_x((m_entity->render_size().width() - size.width()) / 2.f);
    }

    if (m_content_entity->render_size().height() >= m_entity->render_size().height()) {
        clip_size.set_height(m_entity->render_size().height());
        size.set_height(m_entity->render_size().height());
        origin.set_y(0);
        scroll_offset.set_y(m_scroll_offset.y() - (m_content_entity->render_size().height() / 2.f));
    }
    else {
        clip_size.set_height(m_entity->render_size().height());
        size.set_height(m_entity->render_size().height());
        origin.set_y((m_entity->render_size().height() - size.height()) / 2.f);
    }

    origin = origin.round();
    size = size.round();
    scroll_offset = scroll_offset.round();
    clip_size = clip_size.round();

    m_content_entity->set_position(origin);
    m_content_entity->set_size(size);
    m_content_entity->set_render_size(size);
    m_content_entity->set_clipping_offset(scroll_offset);
    m_content_entity->set_clipping_area(clip_size);

    m_dirty = false;
}