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

#include "core/ui/widgets/text_widget.hpp"
#include "core/ui/entity/scene_entity.hpp"

// MARK: - Lua

auto ui::widgets::text_widget::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void
{
    lua->global_namespace()
        .beginNamespace("Widget")
            .beginClass<text_widget>("TextField")
                .addConstructor<auto(*)(double)->void, lua_reference>()
                .addProperty("text", &text_widget::text, &text_widget::set_text)
                .addProperty("font", &text_widget::font, &text_widget::set_font)
                .addProperty("backgroundColor", &text_widget::background_color, &text_widget::set_background_color)
                .addProperty("borderColor", &text_widget::border_color, &text_widget::set_border_color)
                .addProperty("selectionColor", &text_widget::selection_color, &text_widget::set_selection_color)
                .addProperty("color", &text_widget::color, &text_widget::set_color)
                .addProperty("cursorColor", &text_widget::cursor_color, &text_widget::set_cursor_color)
                .addProperty("position", &text_widget::position, &text_widget::set_position)
                .addProperty("size", &text_widget::size, &text_widget::set_size)
                .addProperty("frame", &text_widget::frame, &text_widget::set_frame)
                .addFunction("draw", &text_widget::draw)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

ui::widgets::text_widget::text_widget(double width)
{
    m_canvas = std::make_unique<graphics::canvas>(math::size(width, 25));
    m_entity = { new scene_entity(m_canvas->spawn_entity({0, 0})) };
}

// MARK: - Accessors

auto ui::widgets::text_widget::entity() const -> ui::scene_entity::lua_reference
{
    return m_entity;
}

auto ui::widgets::text_widget::text() const -> std::string
{
    return m_input.string_value();
}

auto ui::widgets::text_widget::font() const -> ui::font::reference::lua_reference
{
    return m_font;
}

auto ui::widgets::text_widget::color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_color) };
}

auto ui::widgets::text_widget::background_color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_background_color) };
}

auto ui::widgets::text_widget::border_color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_border_color) };
}

auto ui::widgets::text_widget::selection_color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_selection_color) };
}

auto ui::widgets::text_widget::cursor_color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_cursor_color) };
}

auto ui::widgets::text_widget::position() const -> math::point
{
    return m_entity->position();
}

auto ui::widgets::text_widget::size() const -> math::size
{
    return m_entity->size();
}

auto ui::widgets::text_widget::frame() const -> math::rect
{
    return { position(), size() };
}

// MARK: - Setters

auto ui::widgets::text_widget::set_text(const std::string& v) -> void
{
    m_input.set_string_value(v);
    m_dirty = true;
}

auto ui::widgets::text_widget::set_font(const ui::font::reference::lua_reference& font) -> void
{
    m_font = font;
    m_dirty = true;
}

auto ui::widgets::text_widget::set_color(const graphics::color::lua_reference& v) -> void
{
    m_color = { *v.get() };
    m_dirty = true;
}

auto ui::widgets::text_widget::set_background_color(const graphics::color::lua_reference& v) -> void
{
    m_background_color = { *v.get() };
    m_dirty = true;
}

auto ui::widgets::text_widget::set_border_color(const graphics::color::lua_reference& v) -> void
{
    m_border_color = { *v.get() };
    m_dirty = true;
}

auto ui::widgets::text_widget::set_selection_color(const graphics::color::lua_reference& v) -> void
{
    m_selection_color = { *v.get() };
    m_dirty = true;
}

auto ui::widgets::text_widget::set_cursor_color(const graphics::color::lua_reference& v) -> void
{
    m_cursor_color = { *v.get() };
    m_dirty = true;
}

auto ui::widgets::text_widget::set_position(const math::point& v) -> void
{
    m_entity->set_position(v);
}

auto ui::widgets::text_widget::set_size(const math::size& v) -> void
{
    auto position = this->position();
    m_canvas = std::make_unique<graphics::canvas>(v);
    m_entity->change_internal_entity(m_canvas->spawn_entity(position));
    redraw_entity();
}

auto ui::widgets::text_widget::set_frame(const math::rect& v) -> void
{
    set_position(v.origin);
    set_size(v.size);
}

// MARK: - Drawing

auto ui::widgets::text_widget::redraw_entity() -> void
{
    if (!m_dirty) {
        return;
    }

    const auto size = m_entity->size();
    const auto inset = 3;
    math::size text_area_size { size.width - (inset << 1), size.height - (inset << 1) };

    math::rect frame { {0, 0}, size };

    m_canvas->clear();
    m_canvas->set_pen_color(m_border_color);
    m_canvas->fill_rect(frame);

    m_canvas->set_pen_color(m_background_color);
    m_canvas->fill_rect(frame.inset(1));

    m_canvas->set_font(m_font);
    auto text_size = m_canvas->layout_text_in_bounds(m_input.string_value(), { 100000, 9999});
    const auto& cursor = m_canvas->character_point_in_text(m_input.cursor_position());

    m_canvas->set_pen_color(m_color);
    m_canvas->set_clipping_rect(frame.inset(inset));
    m_canvas->draw_text({inset, inset + ((text_area_size.height - text_size.height) / 2)});
    m_canvas->clear_clipping_rect();

    m_canvas->set_pen_color(m_cursor_color);
    m_canvas->fill_rect({inset + cursor.x, inset + cursor.y, 1, text_area_size.height});

    m_canvas->rebuild_texture();
    m_dirty = false;
}

auto ui::widgets::text_widget::draw() -> void
{
    if (m_dirty) {
        redraw_entity();
    }
}

// MARK: - Events

auto ui::widgets::text_widget::did_become_first_responder() -> void
{

}

auto ui::widgets::text_widget::did_resign_first_responder() -> void
{

}

auto ui::widgets::text_widget::receive_event(const event& e) -> bool
{
    m_input.receive(e);
    m_dirty = true;
    return true;
}