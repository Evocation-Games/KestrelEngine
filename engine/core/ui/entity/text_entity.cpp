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

#include "core/ui/entity/text_entity.hpp"
#include "renderer/common/renderer.hpp"
#include "core/ui/alignment.hpp"

// MARK: - Lua

auto ui::text_entity::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void
{
    lua->global_namespace()
        .beginClass<text_entity>("TextEntity")
            .addConstructor<auto(*)(const std::string&)->void, lua_reference>()
            .addProperty("text", &text_entity::text, &text_entity::set_text)
            .addProperty("font", &text_entity::font, &text_entity::set_font)
            .addProperty("textColor", &text_entity::text_color, &text_entity::set_text_color)
            .addProperty("backgroundColor", &text_entity::background_color, &text_entity::set_background_color)
            .addProperty("axisAnchor", &text_entity::lua_anchor_point, &text_entity::set_lua_anchor_point)
            .addProperty("position", &text_entity::position, &text_entity::set_position)
            .addProperty("drawPosition", &text_entity::draw_position, &text_entity::set_draw_position)
            .addProperty("drawSize", &text_entity::draw_size, &text_entity::set_draw_size)
            .addProperty("size", &text_entity::size, &text_entity::set_size)
            .addProperty("halfSize", &text_entity::half_size)
            .addProperty("renderSize", &text_entity::render_size, &text_entity::set_render_size)
            .addProperty("ignorePositioningFrameScaler", &text_entity::ignore_positioning_frame_scaler, &text_entity::set_ignore_positioning_frame_scaler)
            .addProperty("alpha", &text_entity::alpha, &text_entity::set_alpha)
            .addProperty("blend", &text_entity::blend_mode, &text_entity::set_blend_mode)
            .addProperty("clippingArea", &text_entity::clipping_area, &text_entity::set_clipping_area)
            .addProperty("clippingOffset", &text_entity::clipping_offset, &text_entity::set_clipping_offset)
            .addProperty("alignment", &text_entity::lua_safe_alignment, &text_entity::set_lua_safe_alignment)
            .addFunction("layout", &text_entity::layout)
            .addFunction("onLayout", &text_entity::on_layout)
            .addFunction("draw", &text_entity::draw)
        .endClass();
}

// MARK: - Construction

ui::text_entity::text_entity(const std::string &text)
    : m_text(text), m_position(0)
{
    // Load up a default font.
    m_font = ui::font::manager::shared_manager().default_font();
    m_font->load_for_graphics();

    // Perform an estimation of the text size to get a basic label size.
    // TODO: We need to get the current scale here so the size is estimated correctly!
    graphics::typesetter ts(text);
    ts.set_font(*m_font.get());
    ts.layout();

    m_canvas = std::make_unique<graphics::canvas>(ts.get_bounding_size());
    m_entity = m_canvas->spawn_entity({0, 0});

    m_min_height = static_cast<int16_t>(ts.get_bounding_size().height);
}

// MARK: - Accessors

auto ui::text_entity::text() const -> std::string
{
    return m_text;
}

auto ui::text_entity::font() const -> font::reference::lua_reference
{
    return m_font;
}

auto ui::text_entity::text_color() const -> graphics::color::lua_reference
{
    return m_text_color;
}

auto ui::text_entity::background_color() const -> graphics::color::lua_reference
{
    return m_background_color;
}

auto ui::text_entity::anchor_point() const -> enum layout::axis_origin
{
    return m_anchor;
}

auto ui::text_entity::lua_anchor_point() const -> std::int32_t
{
    return static_cast<std::int32_t>(anchor_point());
}

auto ui::text_entity::position() const -> math::point
{
    return m_position;
}

auto ui::text_entity::draw_position() const -> math::point
{
    auto offset = origin_for_axis(render_size(), m_anchor);
    return m_entity->get_position() + offset;
}

auto ui::text_entity::size() const -> math::size
{
    return m_entity->get_size();
}

auto ui::text_entity::half_size() const -> math::size
{
    return m_entity->get_size() / 2;
}

auto ui::text_entity::render_size() const -> math::size
{
    return m_entity->get_render_size();
}

auto ui::text_entity::draw_size() const -> math::size
{
    return m_entity->get_draw_size();
}

auto ui::text_entity::alpha() const -> double
{
    return m_entity->get_alpha();
}

auto ui::text_entity::blend_mode() const -> int
{
    return m_entity->get_blend_lua();
}

auto ui::text_entity::clipping_area() const -> math::size
{
    return m_entity->clipping_area();
}

auto ui::text_entity::clipping_offset() const -> math::point
{
    return m_entity->clipping_offset();
}

auto ui::text_entity::ignore_positioning_frame_scaler() const -> bool
{
    return m_ignore_positioning_frame_scaler;
}

auto ui::text_entity::lua_safe_alignment() const -> int32_t
{
    return static_cast<int32_t>(m_alignment);
}

// MARK: - Setters

auto ui::text_entity::set_text(const std::string &v) -> void
{
    m_text = v;
    m_dirty = true;
}

auto ui::text_entity::set_font(const font::reference::lua_reference &v) -> void
{
    m_font = v;
    m_font->load_for_graphics();
    m_dirty = true;

    graphics::typesetter ts(m_text);
    ts.set_font(*m_font.get());
    ts.layout();

    auto size = ts.get_bounding_size() / renderer::scale_factor();
    m_min_height = static_cast<int16_t>(size.height);
    set_size(size);
}

auto ui::text_entity::set_text_color(const graphics::color::lua_reference &v) -> void
{
    m_text_color = v;
    m_dirty = true;
}

auto ui::text_entity::set_background_color(const graphics::color::lua_reference &v) -> void
{
    m_background_color = v;
    m_dirty = true;
}

auto ui::text_entity::set_anchor_point(enum layout::axis_origin v) -> void
{
    m_anchor = v;
}

auto ui::text_entity::set_lua_anchor_point(std::int32_t v) -> void
{
    set_anchor_point(static_cast<enum layout::axis_origin>(v));
}

auto ui::text_entity::set_position(const math::point &v) -> void
{
    m_position = v;
}

auto ui::text_entity::set_draw_position(const math::point &v) -> void
{
    auto offset = layout::origin_for_axis(render_size(), m_anchor);
    m_entity->set_position(v - offset);
}

auto ui::text_entity::set_size(const math::size &v) -> void
{
    m_entity->set_size(v);

    m_canvas = std::make_unique<graphics::canvas>(v);
    m_entity = m_canvas->spawn_entity({});

    redraw();
}

auto ui::text_entity::set_render_size(const math::size &v) -> void
{
    m_entity->set_render_size(v);
}

auto ui::text_entity::set_draw_size(const math::size &v) -> void
{
    m_entity->set_draw_size(v);
}

auto ui::text_entity::set_alpha(double v) -> void
{
    m_entity->set_alpha(v);
}

auto ui::text_entity::set_blend_mode(int v) -> void
{
    m_entity->set_blend_lua(v);
}

auto ui::text_entity::set_clipping_area(const math::size &v) -> void
{
    m_entity->set_clipping_area(v);
}

auto ui::text_entity::set_clipping_offset(const math::point &v) -> void
{
    m_entity->set_clipping_offset(v);
}

auto ui::text_entity::set_ignore_positioning_frame_scaler(bool f) -> void
{
    m_ignore_positioning_frame_scaler = f;
}

auto ui::text_entity::set_lua_safe_alignment(int32_t v) -> void
{
    m_alignment = static_cast<enum horizontal_alignment>(v);
    m_dirty = true;
}

// MARK: - Layout

auto ui::text_entity::layout() -> void
{

}

auto ui::text_entity::on_layout(const luabridge::LuaRef &callback) -> void
{

}

// MARK: - Drawing

auto ui::text_entity::draw() -> void
{
    if (m_dirty) {
        redraw();
    }

    m_entity->draw();
}

auto ui::text_entity::redraw() -> void
{
    const auto size = m_entity->get_size();

    m_canvas->clear();
    if (m_background_color.get()) {
        m_canvas->set_pen_color(*m_background_color.get());
        m_canvas->fill_rect({ {0, 0}, size});
    }

    if (m_text_color.get()) {
        m_canvas->set_pen_color(*m_text_color.get());
    }

    m_canvas->set_font(m_font);

    const auto text_size = m_canvas->layout_text_in_bounds(m_text, size);
    auto x = 0.0;
    auto y = (size.height - text_size.height) / 2.0;

    if (text_size.width < size.width) {
        switch (m_alignment) {
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


    m_canvas->draw_text({ x, std::max(y, 0.0) });
    m_canvas->rebuild_texture();
    m_dirty = false;
}