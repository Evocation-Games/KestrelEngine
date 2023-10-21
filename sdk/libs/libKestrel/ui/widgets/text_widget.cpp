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

#include <libKestrel/ui/widgets/text_widget.hpp>
#include <libKestrel/ui/entity/scene_entity.hpp>
#include <libKestrel/font/manager.hpp>
#include <libKestrel/ui/scene/game_scene.hpp>

// MARK: - Construction

kestrel::ui::widgets::text_widget::text_widget(double width)
{
    // Load up a default font.
    m_font = font::manager::shared_manager().default_font();
    m_font->load_for_graphics();

    m_canvas = std::make_unique<graphics::canvas>(math::size(width, 25));
    m_entity = { new scene_entity(m_canvas->spawn_entity({0, 0})) };
}

// MARK: - Accessors

auto kestrel::ui::widgets::text_widget::entity() const -> ui::scene_entity::lua_reference
{
    return m_entity;
}

auto kestrel::ui::widgets::text_widget::text() const -> std::string
{
    return m_input.string_value();
}

auto kestrel::ui::widgets::text_widget::font() const -> font::reference::lua_reference
{
    return m_font;
}

auto kestrel::ui::widgets::text_widget::color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_color) };
}

auto kestrel::ui::widgets::text_widget::background_color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_background_color) };
}

auto kestrel::ui::widgets::text_widget::border_color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_border_color) };
}

auto kestrel::ui::widgets::text_widget::selection_color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_selection_color) };
}

auto kestrel::ui::widgets::text_widget::cursor_color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_cursor_color) };
}

auto kestrel::ui::widgets::text_widget::position() const -> math::point
{
    return m_entity->position();
}

auto kestrel::ui::widgets::text_widget::size() const -> math::size
{
    return m_entity->size();
}

auto kestrel::ui::widgets::text_widget::frame() const -> math::rect
{
    return { position(), size() };
}

// MARK: - Setters

auto kestrel::ui::widgets::text_widget::set_text(const std::string& v) -> void
{
    m_input.set_string_value(v);
    m_dirty = true;
}

auto kestrel::ui::widgets::text_widget::set_font(const font::reference::lua_reference& font) -> void
{
    m_font = font;
    m_dirty = true;
}

auto kestrel::ui::widgets::text_widget::set_color(const graphics::color::lua_reference& v) -> void
{
    m_color = { *v.get() };
    m_dirty = true;
}

auto kestrel::ui::widgets::text_widget::set_background_color(const graphics::color::lua_reference& v) -> void
{
    m_background_color = { *v.get() };
    m_dirty = true;
}

auto kestrel::ui::widgets::text_widget::set_border_color(const graphics::color::lua_reference& v) -> void
{
    m_border_color = { *v.get() };
    m_dirty = true;
}

auto kestrel::ui::widgets::text_widget::set_selection_color(const graphics::color::lua_reference& v) -> void
{
    m_selection_color = { *v.get() };
    m_dirty = true;
}

auto kestrel::ui::widgets::text_widget::set_cursor_color(const graphics::color::lua_reference& v) -> void
{
    m_cursor_color = { *v.get() };
    m_dirty = true;
}

auto kestrel::ui::widgets::text_widget::set_position(const math::point& v) -> void
{
    m_entity->set_position(v);
}

auto kestrel::ui::widgets::text_widget::set_size(const math::size& v) -> void
{
    auto position = this->position();
    m_canvas = std::make_unique<graphics::canvas>(v);
    m_entity->change_internal_entity(m_canvas->spawn_entity(position));
    redraw_entity();
}

auto kestrel::ui::widgets::text_widget::set_frame(const math::rect& v) -> void
{
    set_position(v.origin());
    set_size(v.size());
}

auto kestrel::ui::widgets::text_widget::anchor_point() const -> layout::axis_origin
{
    return m_entity->anchor_point();
}

auto kestrel::ui::widgets::text_widget::set_anchor_point(const layout::axis_origin& origin) -> void
{
    m_entity->set_anchor_point(origin);
}


// MARK: - Drawing

auto kestrel::ui::widgets::text_widget::redraw_entity() -> void
{
    if (!m_dirty) {
        return;
    }

    const auto size = math::size(m_entity->size().width() - 1, m_entity->size().height());
    const auto inset = 3;
    math::size text_area_size = size - (inset << 1);

    math::rect frame { {0, 0}, size };

    m_canvas->clear();
    m_canvas->set_pen_color({ new graphics::color(m_is_first_responder ? m_selection_color : m_border_color) });
    m_canvas->fill_rect(frame);

    m_canvas->set_pen_color({ new graphics::color(m_background_color) });
    m_canvas->fill_rect(frame.inset(1));

    m_canvas->set_font(m_font);
    auto text_size = m_canvas->layout_text_in_bounds(m_input.string_value(), { 100000, 9999});
    const auto& cursor = m_canvas->character_point_in_text(m_input.cursor_position());

    m_canvas->set_pen_color({ new graphics::color(m_color) });
    m_canvas->set_clipping_rect(frame.inset(inset));
    m_canvas->draw_text({inset, inset + ((text_area_size.height() - text_size.height()) / 2.f)});
    m_canvas->clear_clipping_rect();

    if (m_is_first_responder) {
        m_canvas->set_pen_color({ new graphics::color(m_cursor_color) });
        m_canvas->fill_rect({inset + cursor.x(), inset + cursor.y(), 1, text_area_size.height()});
    }

    m_canvas->rebuild_texture();
    m_dirty = false;
}

auto kestrel::ui::widgets::text_widget::draw() -> void
{
    if (m_dirty) {
        redraw_entity();
    }
}

// MARK: - Events

auto kestrel::ui::widgets::text_widget::did_become_first_responder() -> void
{
    m_is_first_responder = true;
    m_dirty = true;
    redraw_entity();
}

auto kestrel::ui::widgets::text_widget::did_resign_first_responder() -> void
{
    m_is_first_responder = false;
    m_dirty = true;
    redraw_entity();
}

auto kestrel::ui::widgets::text_widget::receive_event(const event& e) -> bool
{
    if (e.has(::ui::event::any_mouse_down)) {
        if (entity()->hit_test(e.location())) {
            auto& chain = ui::game_scene::current()->responder_chain();
            chain.set_first_responder(this);
            return true;
        }
    }
    else if (e.is_key_event()) {
        m_input.receive(e);
        m_dirty = true;
        return true;
    }

    return false;
}