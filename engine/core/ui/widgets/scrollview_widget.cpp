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

#include "core/ui/widgets/scrollview_widget.hpp"

// MARK: - Lua

auto ui::widgets::scrollview_widget::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("Widget")
            .beginClass<scrollview_widget>("ScrollView")
                .addProperty("scrollOffset", &scrollview_widget::scroll_offset, &scrollview_widget::set_scroll_offset)
                .addProperty("frame", &scrollview_widget::frame, &scrollview_widget::set_frame)
                .addProperty("position", &scrollview_widget::position, &scrollview_widget::set_position)
                .addProperty("size", &scrollview_widget::size, &scrollview_widget::set_size)
                .addFunction("setContent", &scrollview_widget::set_content_entity)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

ui::widgets::scrollview_widget::scrollview_widget()
{
    m_canvas = { new graphics::canvas({ 100, 100 }) };
    m_entity = { new scene_entity(m_canvas) };

    bind_internal_events();
}

// MARK: - Accessors

auto ui::widgets::scrollview_widget::entity() const -> scene_entity::lua_reference
{
    return m_entity;
}

auto ui::widgets::scrollview_widget::position() const -> math::point
{
    return m_entity->position();
}

auto ui::widgets::scrollview_widget::size() const -> math::size
{
    return m_entity->render_size();
}

auto ui::widgets::scrollview_widget::frame() const -> math::rect
{
    return { position(), size() };
}

auto ui::widgets::scrollview_widget::scroll_offset() const -> math::point
{
    return m_scroll_offset;
}

auto ui::widgets::scrollview_widget::set_position(const math::point &v) -> void
{
    m_entity->set_position(v);
    m_dirty = true;
}

auto ui::widgets::scrollview_widget::set_size(const math::size &v) -> void
{
    m_entity->set_size(v);
    m_entity->set_render_size(v);
    m_dirty = true;
}

auto ui::widgets::scrollview_widget::set_frame(const math::rect &v) -> void
{
    set_position(v.origin);
    set_size(v.size);
    m_dirty = true;
}

auto ui::widgets::scrollview_widget::set_scroll_offset(const math::point &v) -> void
{
    m_scroll_offset = v;
    m_dirty = true;
}

auto ui::widgets::scrollview_widget::bind_internal_events() -> void
{

}

auto ui::widgets::scrollview_widget::receive_event(const event &e) -> bool
{
    if (e.is_mouse_event()) {
        if (e.was_dragged()) {
            auto delta = e.location() - m_drag_mouse_origin;
            set_scroll_offset(m_drag_scroll_origin - delta);
        }
        else if (e.is_pressed()) {
            m_drag_mouse_origin = e.location();
            m_drag_scroll_origin = m_scroll_offset;
        }
    }

    if (m_content_entity.get()) {
        m_content_entity->send_event(e);
    }
}

// MARK: - Content

auto ui::widgets::scrollview_widget::set_content_entity(const scene_entity::lua_reference &entity) -> void
{
    // TODO: Remove current child entity
    m_content_entity = entity;
    m_entity->add_child_entity(m_content_entity);
    m_dirty = true;
}

// MARK: - Drawing

auto ui::widgets::scrollview_widget::draw() -> void
{
    redraw_entity();
}

auto ui::widgets::scrollview_widget::redraw_entity() -> void
{
    if (!m_dirty || !m_content_entity.get()) {
        return;
    }

    math::point scroll_offset(0);
    math::size clip_size(0);
    math::point origin(0);
    math::size size(0);

    if (m_content_entity->render_size().width >= m_entity->render_size().width) {
        clip_size.width = size.width = m_entity->render_size().width;
        origin.x = 0;
        scroll_offset.x = m_scroll_offset.x - (m_content_entity->render_size().width / 2);
    }
    else {
        clip_size.width = size.width = m_content_entity->render_size().width;
        origin.x = ((m_entity->render_size().width - size.width) / 2.0);
    }

    if (m_content_entity->render_size().height >= m_entity->render_size().height) {
        clip_size.height = size.height = m_entity->render_size().height;
        origin.y = 0;
        scroll_offset.y = m_scroll_offset.y - (m_content_entity->render_size().height / 2);
    }
    else {
        clip_size.height = size.height = m_content_entity->render_size().height;
        origin.y = ((m_entity->render_size().height - size.height) / 2.0);
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