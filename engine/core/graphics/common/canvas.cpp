// Copyright (c) 2020 Tom Hancocks
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

#include <iostream>
#include <algorithm>
#include <cmath>
#include <utility>
#include "core/graphics/common/canvas.hpp"
#include "core/environment.hpp"
#include "math/line.hpp"

// MARK: - Lua

auto graphics::canvas::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginClass<graphics::canvas>("Canvas")
            .addConstructor<auto(*)(const math::size&)->void, graphics::canvas::lua_reference>()
            .addProperty("penColor", &graphics::canvas::get_pen_color, &graphics::canvas::set_pen_color)
            .addProperty("bounds", &graphics::canvas::get_bounds)
            .addFunction("entity", &graphics::canvas::entity)
            .addFunction("rebuildEntityTexture", &graphics::canvas::rebuild_texture)
            .addFunction("drawRect", &graphics::canvas::draw_rect)
            .addFunction("fillRect", &graphics::canvas::fill_rect)
            .addFunction("drawLine", &graphics::canvas::draw_line)
            .addFunction("drawCircle", &graphics::canvas::draw_circle)
            .addFunction("fillCircle", &graphics::canvas::fill_circle)
            .addFunction("setFont", &graphics::canvas::set_font)
            .addFunction("layoutText", &graphics::canvas::layout_text)
            .addFunction("layoutTextInBounds", &graphics::canvas::layout_text_in_bounds)
            .addFunction("drawText", &graphics::canvas::draw_text)
            .addFunction("drawMacintoshPicture", &graphics::canvas::draw_picture)
            .addFunction("drawImage", &graphics::canvas::draw_image)
            .addFunction("drawColorIcon", &graphics::canvas::draw_color_icon)
            .addFunction("spawnEntity", &graphics::canvas::spawn_entity)
            .addFunction("clear", &graphics::canvas::clear)
            .addFunction("applyMaskUsingCanvas", &graphics::canvas::apply_mask)
        .endClass();
}

// MARK: - Construction

graphics::canvas::canvas(const math::size& size)
    : m_size(std::round(size.width), std::round(size.height)),
      m_rgba_buffer(m_size),
      m_pen_color(graphics::color::white_color()),
      m_typesetter(""),
      m_left(math::point(0), math::point(0, m_size.height)),
      m_top(math::point(0), math::point(m_size.width, 0)),
      m_right(math::point(m_size.width, 0), math::point(m_size.width, m_size.height)),
      m_bottom(math::point(0, m_size.height), math::point(m_size.width, m_size.height))
{

}

// MARK: - Accessors

auto graphics::canvas::get_pen_color() const -> graphics::color
{
    return m_pen_color;
}

auto graphics::canvas::set_pen_color(const graphics::color& color) -> void
{
    m_pen_color = color;
    m_typesetter.set_font_color(color);
}

auto graphics::canvas::set_font(const std::string &name, const int &size) -> void
{
    m_typesetter.set_font(name);
    m_typesetter.set_font_size(size);
}

// MARK: - Entity

auto graphics::canvas::rebuild_texture() -> void
{
    if (m_entity.get() == nullptr) {
        return;
    }

    if (auto env = environment::active_environment().lock()) {
        // Rebuild the texture.
        if (auto existing_texture = m_linked_tex.lock()) {
            existing_texture->destroy();
        }

        auto tex = env->create_texture(m_size, raw());
        m_entity->set_spritesheet(std::make_shared<graphics::spritesheet>(tex, m_size));
    }
}

auto graphics::canvas::spawn_entity(const math::vector &position) -> graphics::entity::lua_reference
{
    // Create a new bitmap of the text.
    if (auto env = environment::active_environment().lock()) {
        auto tex = env->create_texture(m_size, raw());

        auto entity = graphics::entity::lua_reference(new graphics::entity(m_size));
        entity->set_spritesheet(std::make_shared<graphics::spritesheet>(tex, m_size));
        entity->set_position(position);

        return (m_entity = entity);
    }

    return nullptr;
}

auto graphics::canvas::entity() -> graphics::entity::lua_reference
{
    if (m_entity.get() == nullptr) {
        // Construct a new entity as we don't currently have one.
        m_entity = spawn_entity({0, 0});
    }
    else {
        rebuild_texture();
    }
    return m_entity;
}

// MARK: - Backing Data

auto graphics::canvas::raw() const -> uint8_t *
{
    return m_rgba_buffer.data();
}

// MARK: - Indicies

// MARK: - Drawing

auto graphics::canvas::clear() -> void
{
    m_rgba_buffer.clear(graphics::color::clear_color());
}

auto graphics::canvas::draw_rect(const math::rect &r) -> void
{
    draw_line(r.origin, { r.origin.x + r.size.width, r.origin.y });
    draw_line(r.origin, { r.origin.x, r.origin.y + r.size.height });
    draw_line({ r.origin.x + r.size.width, r.origin.y }, { r.origin.x + r.size.width, r.origin.y + r.size.height });
    draw_line({ r.origin.x, r.origin.y + r.size.height }, { r.origin.x + r.size.width, r.origin.y + r.size.height });
}

auto graphics::canvas::fill_rect(const math::rect &r) -> void
{
    m_rgba_buffer.fill_rect(m_pen_color, r);
}

auto graphics::canvas::draw_line(const math::point &p, const math::point &q) -> void
{
    math::line l(p, q);
    if (!(l.intersects(m_left) || l.intersects(m_top) || l.intersects(m_right) || l.intersects(m_bottom))) {
        if (!((p.x > 0 && p.x < m_size.width) && (p.y > 0 && p.y < m_size.height))) {
            return;
        }
    }

    // This implementation of Xiaolin Wu's line algorithm is based on the implementation found at
    // https://rosettacode.org/wiki/Xiaolin_Wu%27s_line_algorithm.

    // Helper functions
    auto ipart =  [](const double& n) -> int { return static_cast<int>(std::floor(n)); };
    auto round =  [](const double& n) -> double { return std::round(n); };
    auto fpart =  [](const double& n) -> double { return n - std::floor(n); };
    auto rfpart = [=](const double& n) -> double { return 1 - fpart(n); };

    auto x0 = p.x;
    auto y0 = p.y;
    auto x1 = q.x;
    auto y1 = q.y;

    const bool steep = std::abs(y1 - y0) > std::abs(x1 - x0);
    if (steep) {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    const auto dx = x1 - x0;
    const auto dy = y1 - y0;
    const auto gradient = (dx == 0) ? 1 : dy/dx;

    int xpx11;
    double intery;
    {
        const auto xend = round(x0);
        const auto yend = y0 + gradient * (xend - x0);
        const auto xgap = rfpart(x0 + 0.5);
        xpx11 = static_cast<int>(xend);
        const int ypx11 = ipart(yend);
        if (steep) {
            m_rgba_buffer.draw_pixel(m_pen_color, {ypx11, xpx11});
            m_rgba_buffer.draw_pixel(m_pen_color, {ypx11 + 1, xpx11});
        }
        else {
            m_rgba_buffer.draw_pixel(m_pen_color, {xpx11, ypx11});
            m_rgba_buffer.draw_pixel(m_pen_color, {xpx11, ypx11 + 1});
        }
        intery = yend + gradient;
    }

    int xpx12;
    {
        const auto xend = round(x1);
        const auto yend = y1 + gradient * (xend - x1);
        const auto xgap = rfpart(x1 + 0.5);
        xpx12 = static_cast<int>(xend);
        const int ypx12 = ipart(yend);
        if (steep) {
            m_rgba_buffer.draw_pixel(m_pen_color, {ypx12, xpx12});
            m_rgba_buffer.draw_pixel(m_pen_color, {ypx12 + 1, xpx12});
        }
        else {
            m_rgba_buffer.draw_pixel(m_pen_color, {xpx12, ypx12});
            m_rgba_buffer.draw_pixel(m_pen_color, {xpx12, ypx12 + 1});
        }
    }

    if (steep) {
        for (auto x = xpx11 + 1; x < xpx12; ++x) {
            auto ip = ipart(intery);
            m_rgba_buffer.draw_pixel(m_pen_color, {ip, x});
            m_rgba_buffer.draw_pixel(m_pen_color, {ip + 1, x});
            intery += gradient;
        }
    }
    else {
        for (auto x = xpx11 + 1; x < xpx12; ++x) {
            auto ip = ipart(intery);
            m_rgba_buffer.draw_pixel(m_pen_color, {x, ip});
            m_rgba_buffer.draw_pixel(m_pen_color, {x, ip + 1});
            intery += gradient;
        }
    }
}

auto graphics::canvas::draw_circle(const math::point &p, const double &r) -> void
{
    // Check if the circle intersects the bounds of the canvas. If it doesn't, then don't draw.
    auto extended = math::rect( -r, -r, m_size.width + r + r, m_size.height + r + r );
    if (!extended.contains_point(p)) {
        return;
    }

    auto x = static_cast<int>(std::round(r));
    typeof(x) y = 0;
    double err = 0;

    while (x >= y) {
        m_rgba_buffer.draw_pixel(m_pen_color, {static_cast<int>(p.x) + x, static_cast<int>(p.y) + y});
        m_rgba_buffer.draw_pixel(m_pen_color, {static_cast<int>(p.x) + y, static_cast<int>(p.y) + x});
        m_rgba_buffer.draw_pixel(m_pen_color, {static_cast<int>(p.x) - y, static_cast<int>(p.y) + x});
        m_rgba_buffer.draw_pixel(m_pen_color, {static_cast<int>(p.x) - x, static_cast<int>(p.y) + y});
        m_rgba_buffer.draw_pixel(m_pen_color, {static_cast<int>(p.x) - x, static_cast<int>(p.y) - y});
        m_rgba_buffer.draw_pixel(m_pen_color, {static_cast<int>(p.x) - y, static_cast<int>(p.y) - x});
        m_rgba_buffer.draw_pixel(m_pen_color, {static_cast<int>(p.x) + y, static_cast<int>(p.y) - x});
        m_rgba_buffer.draw_pixel(m_pen_color, {static_cast<int>(p.x) + x, static_cast<int>(p.y) - y});

        if (err <= 0) {
            y += 1;
            err += (2 * y) + 1;
        }
        else {
            x -= 1;
            err -= (2 * x) + 1;
        }
    }
}

auto graphics::canvas::fill_circle(const math::point &p, const double &r) -> void
{
    // Check if the circle intersects the bounds of the canvas. If it doesn't, then don't draw.
    auto extended = math::rect( -r, -r, m_size.width + r + r, m_size.height + r + r );
    if (!extended.contains_point(p)) {
        return;
    }

    for (int y = -static_cast<int>(r); y < r; ++y) {
        auto ww = static_cast<int>(std::sqrt((r * r) - (y * y)));
        auto ry = p.y + y;
        m_rgba_buffer.fill_rect(m_pen_color, { p.x - ww, ry, ww << 1, 1 });
    }
}

// MARK: - Text

auto graphics::canvas::layout_text(const std::string &text) -> math::size
{
    m_typesetter.set_text(text);
    m_typesetter.layout();
    return m_typesetter.get_bounding_size();
}

auto graphics::canvas::layout_text_in_bounds(const std::string &text, const math::size& bounds) -> math::size
{
    m_typesetter.set_margins(bounds);
    m_typesetter.set_text(text);
    m_typesetter.layout();
    return m_typesetter.get_bounding_size();
}

auto graphics::canvas::draw_text(const math::point &point) -> void
{
    // Check if the circle intersects the bounds of the canvas. If it doesn't, then don't draw.
    const double edge = 20;
    auto extended = math::rect( -edge, -edge, m_size.width + edge + edge, m_size.height + edge + edge );
    if (!extended.contains_point(point)) {
        return;
    }

    auto text_bmp = m_typesetter.render();
    auto text_size = m_typesetter.get_bounding_size();

    auto bmp_line_start = std::max<int64_t>(0LL, static_cast<int64_t>(-point.x));
    auto bmp_line_len = static_cast<int64_t>(text_size.width) - bmp_line_start;
    auto start = std::max<int64_t>(0LL, static_cast<int64_t>(point.x));

    // Drawing the text into the canvas buffer at the appropriate point.
    for (auto y = 0; y < text_size.height; ++y) {
        auto dy = std::floor(y + point.y);
        if (dy < 0) {
            // TODO: Calculate the correct Y to be on, if it exists.
            continue;
        }
        else if (y >= m_size.height) {
            break;
        }

        auto bmp_line_offset = static_cast<int64_t>(y * text_size.width);

        auto vstart = text_bmp.cbegin() + bmp_line_offset + bmp_line_start;
        auto vend = vstart + bmp_line_len;
        std::vector<graphics::color> cv { vstart, vend };

        m_rgba_buffer.apply_run(cv, start, dy);
    }

    m_typesetter.reset();
}

auto graphics::canvas::draw_image(const asset::macintosh_picture::lua_reference& image, const math::point& point, const math::size& sz) -> void
{
    math::rect bounds(math::point(0), m_size);
    math::rect img_bounds(math::point(0), image->size());
    math::rect img_frame(point, { std::round(sz.width), std::round(sz.height) });

    if (!img_frame.intersects(bounds)) {
        return;
    }

    auto raw_img_data = image->spritesheet()->texture()->data();
    auto scaled_len = static_cast<uint32_t>(img_frame.size.width * img_frame.size.height);
    std::vector<uint32_t> scaled_img_data(scaled_len, 0);

    // Perform some initial calculations in order to determine how the scaling should be performed
    auto x_scale = img_frame.size.width / img_bounds.size.width;
    auto y_scale = img_frame.size.height / img_bounds.size.height;

    for (auto y = 0; y < img_frame.size.height; ++y) {
        auto ry = static_cast<uint32_t>(std::round(y / y_scale));
        if (ry >= img_bounds.size.height) {
            break;
        }

        auto src_offset = (ry * img_bounds.size.width);
        auto dst_offset = (y * img_frame.size.width);
        for (auto x = 0; x < img_frame.size.width; ++x) {
            auto rx = static_cast<uint32_t>(std::round(x / x_scale));
            if (rx >= img_bounds.size.width) {
                break;
            }
            scaled_img_data[dst_offset + x] = raw_img_data.at(src_offset + rx);
        }
    }

    // Drawing
    auto bmp_line_start = std::max<int64_t>(0LL, static_cast<int64_t>(-point.x));
    auto bmp_line_len = static_cast<int64_t>(img_frame.size.width) - bmp_line_start;
    auto start = std::max<int64_t>(0LL, static_cast<int64_t>(point.x));

    for (auto y = 0; y < img_frame.size.height; ++y) {
        auto dy = std::floor(y + point.y);
        if (dy < 0) {
            continue;
        }
        else if (y >= m_size.height) {
            break;
        }

        auto bmp_line_offset = static_cast<int64_t>(y * img_frame.size.width);

        auto vstart = scaled_img_data.cbegin() + bmp_line_offset + bmp_line_start;
        auto vend = vstart + bmp_line_len;
        std::vector<graphics::color> cv { vstart, vend };

        m_rgba_buffer.apply_run(cv, start, dy);
    }
}

auto graphics::canvas::draw_picture_at_point(const asset::macintosh_picture::lua_reference &pict, const math::point &point) -> void
{
    math::rect bounds(math::point(0), m_size);
    math::rect pict_bounds(point, pict->size());
    if (!pict_bounds.intersects(bounds)) {
        return;
    }

    auto raw_pict_data = pict->spritesheet()->texture()->data();

    auto bmp_line_start = std::max<int64_t>(0LL, static_cast<int64_t>(-point.x));
    auto bmp_line_len = static_cast<int64_t>(pict_bounds.size.width) - bmp_line_start;
    auto start = std::max<int64_t>(0LL, static_cast<int64_t>(point.x));

    for (auto y = 0; y < pict->size().height; ++y) {
        auto dy = std::floor(y + point.y);
        if (dy < 0) {
            continue;
        }
        else if (y >= m_size.height) {
            break;
        }

        auto bmp_line_offset = static_cast<int64_t>(y * pict_bounds.size.width);

        auto vstart = raw_pict_data.cbegin() + bmp_line_offset + bmp_line_start;
        auto vend = vstart + bmp_line_len;
        std::vector<graphics::color> cv { vstart, vend };

        m_rgba_buffer.apply_run(cv, start, dy);
    }
}

auto graphics::canvas::draw_picture(const asset::macintosh_picture::lua_reference &pict, const math::rect &rect) -> void
{
    for (auto y = static_cast<int>(rect.origin.y); y < rect.origin.y + rect.size.height; y += pict->size().height) {
        for (auto x = static_cast<int>(rect.origin.x); x < rect.origin.x + rect.size.width; x += pict->size().width) {
            draw_picture_at_point(pict, { x, y });
        }
    }
}

auto graphics::canvas::draw_color_icon(const asset::color_icon::lua_reference &icon, const math::point &point, const math::size &sz) -> void
{
    math::rect bounds(math::point(0), m_size);
    math::rect icon_bounds(point, icon->size());
    if (!icon_bounds.intersects(bounds)) {
        return;
    }

    auto raw_icon_data = icon->spritesheet()->texture()->data();

    auto bmp_line_start = std::max(static_cast<int64_t>(0LL), static_cast<int64_t>(-point.x));
    auto bmp_line_len = static_cast<int64_t>(icon_bounds.size.width) - bmp_line_start;
    auto start = std::max(static_cast<int64_t>(0LL), static_cast<int64_t>(point.x));

    for (auto y = 0; y < icon->size().height; ++y) {
        auto dy = std::floor(y + point.y);
        if (dy < 0) {
            continue;
        }
        else if (y >= m_size.height) {
            break;
        }

        auto bmp_line_offset = static_cast<int64_t>(y * icon_bounds.size.width);

        auto vstart = raw_icon_data.cbegin() + bmp_line_offset + bmp_line_start;
        auto vend = vstart + bmp_line_len;
        std::vector<graphics::color> cv { vstart, vend };

        m_rgba_buffer.apply_run(cv, start, dy);
    }
}


// MARK: - Masking

auto graphics::canvas::apply_mask(const graphics::canvas::lua_reference &c) -> void
{
    m_rgba_buffer.apply_mask(c->m_rgba_buffer);
}

auto graphics::canvas::get_bounds() const -> math::rect
{
    return math::rect(math::point(0), m_size);
}
