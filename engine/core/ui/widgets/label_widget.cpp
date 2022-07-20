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

#include <cmath>
#include "core/ui/widgets/label_widget.hpp"
#include "core/graphics/common/text/typesetter.hpp"
#include "core/ui/entity/scene_entity.hpp"

// MARK: - Lua

auto ui::widgets::label_widget::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("Widget")
            .beginClass<ui::widgets::label_widget>("Label")
                .addConstructor<auto(*)(const std::string&)->void, lua_reference>()
                .addProperty("text", &label_widget::text, &label_widget::set_text)
                .addProperty("font", &label_widget::font, &label_widget::set_font)
                .addProperty("color", &label_widget::color, &label_widget::set_color)
                .addProperty("backgroundColor", &label_widget::background_color, &label_widget::set_background_color)
                .addProperty("offset", &label_widget::offset, &label_widget::set_offset)
                .addProperty("position", &label_widget::position, &label_widget::set_position)
                .addProperty("size", &label_widget::size, &label_widget::set_size)
                .addProperty("frame", &label_widget::frame, &label_widget::set_frame)
                .addProperty("verticalAlignment", &label_widget::vertical_alignment, &label_widget::set_vertical_alignment)
                .addProperty("horizontalAlignment", &label_widget::horizontal_alignment, &label_widget::set_horizontal_alignment)
                .addFunction("draw", &label_widget::draw)
            .endClass()
        .endNamespace();
}

// MARK: - Construction

ui::widgets::label_widget::label_widget(const std::string &text)
    : m_text(text)
{
    // Perform an estimation of the text size to get a basic label size.
    // TODO: We need to get the current scale here so the size is estimated correctly!
    graphics::typesetter ts { text };
    ts.layout();

    m_canvas = std::make_unique<graphics::canvas>(ts.get_bounding_size());
    m_entity = { new scene_entity(m_canvas->spawn_entity({0, 0})) };

    m_min_height = static_cast<int16_t>(ts.get_bounding_size().height);
}

// MARK: - Accessors

auto ui::widgets::label_widget::entity() const -> ui::scene_entity::lua_reference
{
    return m_entity;
}

auto ui::widgets::label_widget::text() const -> std::string
{
    return m_text;
}

auto ui::widgets::label_widget::font() const -> ui::font::reference::lua_reference
{
    return m_font;
}

auto ui::widgets::label_widget::color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_color) };
}

auto ui::widgets::label_widget::background_color() const -> graphics::color::lua_reference
{
    return { new graphics::color(m_background) };
}

auto ui::widgets::label_widget::offset() const -> math::size
{
    return m_offset;
}

auto ui::widgets::label_widget::position() const -> math::point
{
    return m_entity->position();
}

auto ui::widgets::label_widget::size() const -> math::size
{
    return m_entity->size();
}

auto ui::widgets::label_widget::frame() const -> math::rect
{
    return { position(), size() };
}

auto ui::widgets::label_widget::vertical_alignment() const -> enum vertical_alignment
{
    return m_vertical;
}

auto ui::widgets::label_widget::horizontal_alignment() const -> enum horizontal_alignment
{
    return m_horizontal;
}

auto ui::widgets::label_widget::lua_safe_vertical_alignment() const -> int
{
    return static_cast<int>(m_vertical);
}

auto ui::widgets::label_widget::lua_safe_horizontal_alignment() const -> int
{
    return static_cast<int>(m_horizontal);
}

// MARK: - Setters

auto ui::widgets::label_widget::set_text(const std::string& v) -> void
{
    m_text = v;
    m_dirty = true;
}

auto ui::widgets::label_widget::set_font(const ui::font::reference::lua_reference& font) -> void
{
    m_font = font;
    m_dirty = true;

    graphics::typesetter ts { m_text };
    ts.set_font(*m_font.get());
    ts.layout();
    m_min_height = static_cast<int16_t>(ts.get_bounding_size().height);
}


auto ui::widgets::label_widget::set_color(const graphics::color::lua_reference& v) -> void
{
    m_color = { *v.get() };
    m_dirty = true;
}

auto ui::widgets::label_widget::set_background_color(const graphics::color::lua_reference& v) -> void
{
    m_background = { *v.get() };
    m_dirty = true;
}

auto ui::widgets::label_widget::set_offset(const math::size& v) -> void
{
    m_offset = v;
    m_dirty = true;
}

auto ui::widgets::label_widget::set_position(const math::point& v) -> void
{
    m_entity->set_position(v);
}

auto ui::widgets::label_widget::set_size(const math::size& v) -> void
{
    auto position = this->position();
    m_canvas = std::make_unique<graphics::canvas>(v);
    m_entity->change_internal_entity(m_canvas->spawn_entity(position));
    redraw_entity();
}

auto ui::widgets::label_widget::set_frame(const math::rect& v) -> void
{
    m_entity->internal_entity()->set_position(v.origin);
    m_entity->set_position(v.origin);

    set_size({ v.size.width, std::max(m_min_height, static_cast<int16_t>(v.size.height)) });
}

auto ui::widgets::label_widget::set_vertical_alignment(enum vertical_alignment v) -> void
{
    m_vertical = v;
    m_dirty = true;
}

auto ui::widgets::label_widget::set_horizontal_alignment(enum horizontal_alignment v) -> void
{
    m_horizontal = v;
    m_dirty = true;
}

auto ui::widgets::label_widget::set_lua_safe_vertical_alignment(int v) -> void
{
    set_vertical_alignment(static_cast<enum vertical_alignment>(v));
}

auto ui::widgets::label_widget::set_lua_safe_horizontal_alignment(int v) -> void
{
    set_horizontal_alignment(static_cast<enum horizontal_alignment>(v));
}

// MARK: - Drawing

auto ui::widgets::label_widget::redraw_entity() -> void
{
    if (!m_dirty) {
        return;
    }

    const auto size = m_entity->size();

    m_canvas->clear();
    m_canvas->set_pen_color(m_background);
    m_canvas->fill_rect({ {0, 0}, size});

    m_canvas->set_pen_color(m_color);
    m_canvas->set_font(m_font);

    const auto text_size = m_canvas->layout_text_in_bounds(m_text, size);
    auto x = m_offset.width;
    auto y = m_offset.height - 2;

    if (text_size.height < size.height) {
        switch (m_vertical) {
            case vertical_alignment::middle: {
                y = (size.height - text_size.height) / 2.0;
                break;
            }
            case vertical_alignment::bottom: {
                y = (size.height - text_size.height);
                break;
            }
            default: break;
        }
    }

    if (text_size.width < size.width) {
        switch (m_horizontal) {
            case horizontal_alignment::center: {
                x = (size.width - text_size.width) / 2.0;
                break;
            }
            case horizontal_alignment::right: {
                x = (size.width - text_size.width);
                break;
            }
            default: break;
        }
    }

    m_canvas->draw_text({ x, y });
    m_canvas->rebuild_texture();
    m_dirty = false;
}

auto ui::widgets::label_widget::draw() -> void
{
    if (m_dirty) {
        redraw_entity();
    }
}
