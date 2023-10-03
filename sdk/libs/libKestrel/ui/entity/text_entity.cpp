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

#include <libKestrel/ui/entity/text_entity.hpp>
#include <libKestrel/graphics/renderer/common/renderer.hpp>
#include <libKestrel/ui/alignment.hpp>

// MARK: - Construction

kestrel::ui::text_entity::text_entity(const std::string &text)
    : m_text(text), m_position(0)
{
    // Load up a default font.
    m_font = font::manager::shared_manager().default_font();
    m_font->load_for_graphics();

    // Perform an estimation of the text size to get a basic label size.
    // TODO: We need to get the current scale here so the size is estimated correctly!
    font::typesetter ts(text, renderer::scale_factor());
    ts.set_font(*m_font.get());
    ts.layout();

    m_canvas = std::make_unique<graphics::canvas>(ts.get_bounding_size() / renderer::scale_factor());
    m_entity = m_canvas->spawn_entity({0, 0});

    m_min_height = static_cast<std::int16_t>(m_canvas->get_bounds().height());
    m_parent_bounds = { math::point(0), renderer::window_size() };
}

// MARK: - Accessors

auto kestrel::ui::text_entity::text() const -> std::string
{
    return m_text;
}

auto kestrel::ui::text_entity::font() const -> font::reference::lua_reference
{
    return m_font;
}

auto kestrel::ui::text_entity::text_color() const -> graphics::color::lua_reference
{
    return m_text_color;
}

auto kestrel::ui::text_entity::background_color() const -> graphics::color::lua_reference
{
    return m_background_color;
}

auto kestrel::ui::text_entity::anchor_point() const -> enum layout::axis_origin
{
    return m_anchor;
}

auto kestrel::ui::text_entity::lua_anchor_point() const -> std::int32_t
{
    return static_cast<std::int32_t>(anchor_point());
}

auto kestrel::ui::text_entity::position() const -> math::point
{
    return m_position;
}

auto kestrel::ui::text_entity::size() const -> math::size
{
    return m_entity->get_size();
}

auto kestrel::ui::text_entity::half_size() const -> math::size
{
    return m_entity->get_size() / 2;
}

auto kestrel::ui::text_entity::alpha() const -> double
{
    return m_entity->get_alpha();
}

auto kestrel::ui::text_entity::blend_mode() const -> std::int32_t
{
    return m_entity->get_blend_lua();
}

auto kestrel::ui::text_entity::clipping_area() const -> math::size
{
    return m_entity->clipping_area();
}

auto kestrel::ui::text_entity::clipping_offset() const -> math::point
{
    return m_entity->clipping_offset();
}

auto kestrel::ui::text_entity::ignore_positioning_frame_scaler() const -> bool
{
    return m_ignore_positioning_frame_scaler;
}

auto kestrel::ui::text_entity::lua_safe_alignment() const -> std::int32_t
{
    return static_cast<std::int32_t>(m_alignment);
}

// MARK: - Setters

auto kestrel::ui::text_entity::set_text(const std::string &v) -> void
{
    m_text = v;
    m_dirty = true;
    update_position();
}

auto kestrel::ui::text_entity::set_font(const font::reference::lua_reference &v) -> void
{
    m_font = v;
    m_font->load_for_graphics();
    m_dirty = true;

    font::typesetter ts(m_text, renderer::scale_factor());
    ts.set_font(*m_font.get());
    ts.layout();

    auto size = ts.get_bounding_size() / renderer::scale_factor();
    m_min_height = static_cast<std::int16_t>(size.height());
    set_size(size);
}

auto kestrel::ui::text_entity::set_text_color(const graphics::color::lua_reference &v) -> void
{
    m_text_color = v;
    m_dirty = true;
}

auto kestrel::ui::text_entity::set_background_color(const graphics::color::lua_reference &v) -> void
{
    m_background_color = v;
    m_dirty = true;
}

auto kestrel::ui::text_entity::set_anchor_point(enum layout::axis_origin v) -> void
{
    m_anchor = v;
    update_position();
}

auto kestrel::ui::text_entity::set_lua_anchor_point(std::int32_t v) -> void
{
    set_anchor_point(static_cast<enum layout::axis_origin>(v));
}

auto kestrel::ui::text_entity::set_position(const math::point &v) -> void
{
    m_position = v;
    update_position();
}

auto kestrel::ui::text_entity::update_position() -> void
{
    auto position = entity_position(renderer::window_size(), this->anchor_point(), this->position(), this->size());
    m_entity->set_position(position + m_parent_bounds.origin());
}

auto kestrel::ui::text_entity::set_size(const math::size &v) -> void
{
    m_entity->set_size(v);
    update_position();

    m_canvas = std::make_unique<graphics::canvas>(v);
    m_entity = m_canvas->spawn_entity({});

    redraw();
}

auto kestrel::ui::text_entity::set_alpha(double v) -> void
{
    m_entity->set_alpha(v);
}

auto kestrel::ui::text_entity::set_blend_mode(std::int32_t v) -> void
{
    m_entity->set_blend_lua(v);
}

auto kestrel::ui::text_entity::set_clipping_area(const math::size &v) -> void
{
    m_entity->set_clipping_area(v);
}

auto kestrel::ui::text_entity::set_clipping_offset(const math::point &v) -> void
{
    m_entity->set_clipping_offset(v);
}

auto kestrel::ui::text_entity::set_ignore_positioning_frame_scaler(bool f) -> void
{
    m_ignore_positioning_frame_scaler = f;
}

auto kestrel::ui::text_entity::set_lua_safe_alignment(std::int32_t v) -> void
{
    m_alignment = static_cast<enum horizontal_alignment>(v);
    m_dirty = true;
}

// MARK: - Layout

auto kestrel::ui::text_entity::layout() -> void
{

}

auto kestrel::ui::text_entity::on_layout(const luabridge::LuaRef &callback) -> void
{

}

// MARK: - Drawing

auto kestrel::ui::text_entity::draw() -> void
{
    if (m_dirty) {
        redraw();
    }

    const auto size = m_entity->get_size();
    if (size.area() <= 1) {
        return;
    }

    m_entity->draw();
}

auto kestrel::ui::text_entity::redraw() -> void
{
    const auto size = m_entity->get_size() / renderer::scale_factor();
    if (size.area() <= 1) {
        return;
    }

    m_canvas->clear();
    if (m_background_color.get()) {
        m_canvas->set_pen_color(*m_background_color);
        m_canvas->fill_rect({ {0, 0}, size});
    }

    if (m_text_color.get()) {
        m_canvas->set_pen_color(*m_text_color);
    }

    m_canvas->set_font(m_font);

    const auto text_size = m_canvas->layout_text_in_bounds(m_text, size).round();
    auto x = 0.f;
    auto y = (size.height() - text_size.height()) / 2.f;

    if (text_size.width() < size.width()) {
        switch (m_alignment) {
            case horizontal_alignment::center: {
                x = (size.width() - text_size.width()) / 2.f;
                break;
            }
            case horizontal_alignment::right: {
                x = (size.width() - text_size.width());
                break;
            }
            default: break;
        }
    }


    m_canvas->draw_text({ x, std::max(y, 0.f) });
    m_canvas->rebuild_texture();
    m_dirty = false;
}