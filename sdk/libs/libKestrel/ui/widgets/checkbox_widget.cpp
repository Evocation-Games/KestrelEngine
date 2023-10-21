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

#include <libKestrel/ui/widgets/checkbox_widget.hpp>
#include <libKestrel/ui/entity/scene_entity.hpp>
#include <libKestrel/font/manager.hpp>
#include <libKestrel/ui/scene/game_scene.hpp>

// MARK: - Construction

kestrel::ui::widgets::checkbox_widget::checkbox_widget()
{
    m_canvas = std::make_unique<graphics::canvas>(math::size(30, 30));
    m_entity = { new scene_entity(m_canvas->spawn_entity({0, 0})) };
}

// MARK: - Accessors

auto kestrel::ui::widgets::checkbox_widget::entity() const -> scene_entity::lua_reference
{
    return m_entity;
}

auto kestrel::ui::widgets::checkbox_widget::user_info() const -> luabridge::LuaRef
{
    return m_user_info;
}

auto kestrel::ui::widgets::checkbox_widget::disabled() const -> bool
{
    return m_disabled;
}

auto kestrel::ui::widgets::checkbox_widget::frame() const -> math::rect
{
    return { m_entity->position(), m_entity->size() };
}

auto kestrel::ui::widgets::checkbox_widget::value() const -> bool
{
    return m_value;
}

auto kestrel::ui::widgets::checkbox_widget::color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_color) };
}

auto kestrel::ui::widgets::checkbox_widget::border_color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_border_color) };
}

auto kestrel::ui::widgets::checkbox_widget::background_color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_background_color) };
}

auto kestrel::ui::widgets::checkbox_widget::set_user_info(const luabridge::LuaRef &info) -> void
{
    m_user_info = info;
    m_dirty = true;
}

auto kestrel::ui::widgets::checkbox_widget::set_disabled(bool disabled) -> void
{
    m_disabled = disabled;
    m_dirty = true;
}

auto kestrel::ui::widgets::checkbox_widget::set_frame(const math::rect &r) -> void
{
    m_entity->set_position(r.origin());
    m_canvas = std::make_unique<graphics::canvas>(r.size());
    m_entity->change_internal_entity(m_canvas->spawn_entity(r.origin()));
    redraw_entity();
}

auto kestrel::ui::widgets::checkbox_widget::set_value(bool value) -> void
{
    m_value = value;
    m_dirty = true;
}

auto kestrel::ui::widgets::checkbox_widget::set_color(const graphics::color::lua_reference &v) -> void
{
    m_color = { *v.get() };
    m_dirty = true;
}

auto kestrel::ui::widgets::checkbox_widget::set_background_color(const graphics::color::lua_reference &v) -> void
{
    m_background_color = { *v.get() };
    m_dirty = true;
}

auto kestrel::ui::widgets::checkbox_widget::set_border_color(const graphics::color::lua_reference &v) -> void
{
    m_border_color = { *v.get() };
    m_dirty = true;
}

auto kestrel::ui::widgets::checkbox_widget::anchor_point() const -> layout::axis_origin
{
    return m_entity->anchor_point();
}

auto kestrel::ui::widgets::checkbox_widget::set_anchor_point(const layout::axis_origin& origin) -> void
{
    m_entity->set_anchor_point(origin);
}


// MARK: - Events

auto kestrel::ui::widgets::checkbox_widget::receive_event(const event &e) -> bool
{
    if (e.is_mouse_event() && entity()->hit_test(e.location())) {
        if (e.has(::ui::event::mouse_move) && !m_inside) {
            m_inside = true;
            mouse_enter();
        }

        if (e.has(::ui::event::any_mouse_down) && !m_pressed) {
            mouse_down();
        }

        if (e.has(::ui::event::any_mouse_up) && m_pressed) {
            mouse_up();
        }
        return true;
    }
    else if (e.is_mouse_event()) {
        if (e.has(::ui::event::mouse_move) && m_inside) {
            m_inside = false;
            mouse_exit();
            return true;
        }
    }
    return false;
}

auto kestrel::ui::widgets::checkbox_widget::mouse_down() -> void
{
    if (m_disabled) {
        return;
    }
    m_pressed = true;
    m_dirty = true;
    draw();
}

auto kestrel::ui::widgets::checkbox_widget::mouse_up() -> void
{
    if (m_disabled) {
        return;
    }
    m_pressed = false;
    m_value = !m_value;
    m_dirty = true;
    draw();
}

auto kestrel::ui::widgets::checkbox_widget::mouse_enter() -> void
{

}

auto kestrel::ui::widgets::checkbox_widget::mouse_exit() -> void
{

}

// MARK: - Drawing

auto kestrel::ui::widgets::checkbox_widget::draw() -> void
{
    if (m_dirty) {
        redraw_entity();
    }
}

auto kestrel::ui::widgets::checkbox_widget::redraw_entity() -> void
{
    if (!m_dirty) {
        return;
    }

    const auto size = math::size(m_entity->size().width() - 1, m_entity->size().height());
    const auto center = math::point(size.width() / 2, size.height() / 2);
    math::rect frame { {0, 0}, size };

    m_canvas->clear();
    m_canvas->set_pen_color({ new graphics::color(m_border_color) });
    m_canvas->fill_rect(frame);

    m_canvas->set_pen_color({ new graphics::color(m_background_color) });
    m_canvas->fill_rect(frame.inset(1));

    if (m_value) {
        m_canvas->set_pen_color({ new graphics::color(m_color) });
        m_canvas->draw_line({ center.x() - 7, center.y() + 2 }, { center.x() - 2, center.y() + 8 }, 2);
        m_canvas->draw_line({ center.x() - 2, center.y() + 8 }, { center.x() + 7, center.y() - 8 }, 2);
    }

    m_canvas->rebuild_texture();
    m_dirty = false;
}