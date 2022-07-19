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

#include "core/ui/widgets/textarea_widget.hpp"
#include "core/graphics/common/text/typesetter.hpp"
#include "core/ui/entity/scene_entity.hpp"

// MARK: - Lua

auto ui::widgets::textarea_widget::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("Widget")
            .beginClass<ui::widgets::textarea_widget>("TextArea")
                .addConstructor<auto(*)(const std::string&)->void, lua_reference>()
                .addProperty("text", &textarea_widget::text, &textarea_widget::set_text)
                .addProperty("font", &textarea_widget::font, &textarea_widget::set_font)
                .addProperty("fontSize", &textarea_widget::font_size, &textarea_widget::set_font_size)
                .addProperty("color", &textarea_widget::color, &textarea_widget::set_color)
                .addProperty("textColor", &textarea_widget::color, &textarea_widget::set_color)
                .addProperty("backgroundColor", &textarea_widget::background_color, &textarea_widget::set_background_color)
                .addProperty("offset", &textarea_widget::offset, &textarea_widget::set_offset)
                .addProperty("position", &textarea_widget::position, &textarea_widget::set_position)
                .addProperty("size", &textarea_widget::size, &textarea_widget::set_size)
                .addProperty("frame", &textarea_widget::frame, &textarea_widget::set_frame)
                .addProperty("scrollOffset", &textarea_widget::scroll_offset, &textarea_widget::set_scroll_offset)
                .addProperty("canScrollDown", &textarea_widget::can_scroll_down)
                .addProperty("canScrollUp", &textarea_widget::can_scroll_up)
                .addFunction("draw", &textarea_widget::draw)
                .addFunction("scrollDown", &textarea_widget::scroll_down)
                .addFunction("scrollUp", &textarea_widget::scroll_up)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

ui::widgets::textarea_widget::textarea_widget(const std::string &text)
    : m_text(text)
{
    graphics::typesetter ts(text);
    ts.layout();

    m_canvas = std::make_unique<graphics::canvas>(ts.get_bounding_size());
    m_entity = { new scene_entity(m_canvas->spawn_entity({ 0, 0 })) };
}

ui::widgets::textarea_widget::textarea_widget(const math::rect &frame, const std::string &text)
    : m_text(text), m_clipping_size(frame.size)
{
    graphics::typesetter ts(text);
    ts.set_margins(frame.size.round());
    ts.layout();

    m_canvas = std::make_unique<graphics::canvas>(math::size(frame.size.width, std::max(frame.size.height, ts.get_bounding_size().height)));
    m_entity = { new scene_entity(m_canvas->spawn_entity(frame.origin)) };
    m_entity->set_clipping_area(m_clipping_size);
}

// MARK: - Accessors


auto ui::widgets::textarea_widget::entity() const -> ui::scene_entity::lua_reference
{
    return m_entity;
}

auto ui::widgets::textarea_widget::text() const -> std::string
{
    return m_text;
}

auto ui::widgets::textarea_widget::font() const -> std::string
{
    return m_font_face;
}

auto ui::widgets::textarea_widget::font_size() const -> int16_t
{
    return m_font_size;
}

auto ui::widgets::textarea_widget::lua_safe_font_size() const -> int
{
    return static_cast<int>(m_font_size);
}

auto ui::widgets::textarea_widget::color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_color) };
}

auto ui::widgets::textarea_widget::background_color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_background) };
}

auto ui::widgets::textarea_widget::offset() const -> math::size
{
    return m_offset;
}

auto ui::widgets::textarea_widget::position() const -> math::point
{
    return m_entity->position();
}

auto ui::widgets::textarea_widget::size() const -> math::size
{
    return m_entity->size();
}

auto ui::widgets::textarea_widget::frame() const -> math::rect
{
    return { position(), size() };
}

auto ui::widgets::textarea_widget::scroll_offset() const -> int32_t
{
    return m_scroll_offset;
}

// MARK: - Setters

auto ui::widgets::textarea_widget::set_text(const std::string& v) -> void
{
    m_text = v;
    m_dirty = true;
}

auto ui::widgets::textarea_widget::set_font(const std::string& v) -> void
{
    m_font_face = v;
    m_dirty = true;
}

auto ui::widgets::textarea_widget::set_font_size(int16_t v) -> void
{
    m_font_size = v;
    m_dirty = true;
}

auto ui::widgets::textarea_widget::set_lua_safe_font_size(int v) -> void
{
    set_font_size(static_cast<int16_t>(v));
}

auto ui::widgets::textarea_widget::set_color(const graphics::color::lua_reference& v) -> void
{
    m_color = { *v.get() };
    m_dirty = true;
}

auto ui::widgets::textarea_widget::set_background_color(const graphics::color::lua_reference& v) -> void
{
    m_background = { *v.get() };
    m_dirty = true;
}

auto ui::widgets::textarea_widget::set_offset(const math::size& v) -> void
{
    m_offset = v;
    m_dirty = true;
}

auto ui::widgets::textarea_widget::set_position(const math::point& v) -> void
{
    m_entity->set_position(v);
}

auto ui::widgets::textarea_widget::set_size(const math::size& v) -> void
{
    m_entity->set_size(v);
    m_entity->internal_entity()->set_clipping_area(v);
    m_entity->set_clipping_area(v);
    m_clipping_size = v;
}

auto ui::widgets::textarea_widget::set_frame(const math::rect& v) -> void
{
    set_size(v.size);

    m_entity->internal_entity()->set_position(v.origin);
    m_entity->set_position(v.origin);
}

auto ui::widgets::textarea_widget::set_scroll_offset(int32_t offset) -> void
{
    // Check to make sure we aren't at the end of the scroll bounds
    m_can_scroll_up = true;
    if (offset < 0) {
        offset = 0;
        m_can_scroll_up = false;
    }

    m_can_scroll_down = true;
    if (offset + m_entity->clipping_area().height > m_canvas->get_bounds().size.height) {
        offset = m_canvas->get_bounds().size.height - m_entity->size().height;
        m_can_scroll_down = false;
    }
    
    m_scroll_offset = offset;
    m_entity->set_clipping_offset({ 0, static_cast<double>(m_scroll_offset) });
}

auto ui::widgets::textarea_widget::can_scroll_up() const -> bool
{
    return m_can_scroll_up;
}

auto ui::widgets::textarea_widget::can_scroll_down() const -> bool
{
    return m_can_scroll_down;
}

// MARK: - Drawing

auto ui::widgets::textarea_widget::redraw_entity() -> void
{
    auto size = m_entity->size();

    m_canvas->set_font(m_font_face, m_font_size);

    auto text_size = m_canvas->layout_text_in_bounds(m_text, size);
    auto x = m_offset.width;
    auto y = m_offset.height - 2 - m_scroll_offset;

    auto height = std::max(text_size.height, m_entity->size().height);
    m_canvas = std::make_unique<graphics::canvas>(math::size(m_entity->size().width, height));

    m_canvas->set_pen_color(m_background);
    m_canvas->fill_rect({{0, 0}, size});

    m_canvas->set_pen_color(m_color);
    m_canvas->set_font(m_font_face, m_font_size);

    m_canvas->layout_text_in_bounds(m_text, size);
    m_canvas->draw_text({ x, y });
    m_canvas->rebuild_texture();

    m_entity->change_internal_entity(m_canvas->spawn_entity({0, 0}));
    m_entity->set_size(m_clipping_size);
    m_entity->set_clipping_area(m_clipping_size);
    m_dirty = false;
}

auto ui::widgets::textarea_widget::draw() -> void
{
    if (m_dirty) {
        redraw_entity();
    }
}

// MARK: - Scrolling

auto ui::widgets::textarea_widget::scroll_up() -> void
{
    set_scroll_offset(m_scroll_offset - 2);
}

auto ui::widgets::textarea_widget::scroll_down() -> void
{
    set_scroll_offset(m_scroll_offset + 2);
}
