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

#include <algorithm>
#include <cmath>
#include <utility>
#include "math/line.hpp"
#include "core/environment.hpp"
#include "renderer/common/renderer.hpp"
#include "core/graphics/common/canvas.hpp"
#include <libGraphite/data/writer.hpp>

// MARK: - Lua

auto graphics::canvas::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginClass<canvas>("Canvas")
            .addConstructor<auto(*)(const math::size&)->void, canvas::lua_reference>()
            .addProperty("penColor", &canvas::get_pen_color, &canvas::set_pen_color)
            .addProperty("bounds", &canvas::get_bounds)
            .addFunction("entity", &canvas::entity)
            .addFunction("rebuildEntityTexture", &canvas::rebuild_texture)
            .addFunction("drawRect", &canvas::draw_rect)
            .addFunction("fillRect", &canvas::fill_rect)
            .addFunction("drawLine", &canvas::draw_line)
            .addFunction("drawCircle", &canvas::draw_circle)
            .addFunction("fillCircle", &canvas::fill_circle)
            .addFunction("setFont", &canvas::set_font)
            .addFunction("layoutText", &canvas::layout_text)
            .addFunction("layoutTextInBounds", &canvas::layout_text_in_bounds)
            .addFunction("drawText", &canvas::draw_text)
            .addFunction("drawMacintoshPicture", &canvas::draw_picture)
            .addFunction("drawImage", &canvas::draw_image)
            .addFunction("drawStaticImage", &canvas::draw_static_image)
            .addFunction("drawColorIcon", &canvas::draw_color_icon)
            .addFunction("spawnEntity", &canvas::spawn_entity)
            .addFunction("clear", &canvas::clear)
            .addFunction("applyMaskUsingCanvas", &canvas::apply_mask)
            .addFunction("drawMask", &canvas::draw_mask)
        .endClass();
}

// MARK: - Construction

graphics::canvas::canvas(const math::size& size)
    : m_size(std::round(size.width), std::round(size.height)),
      m_scale(renderer::scale_factor()),
      m_scaled_size(m_size * m_scale),
      m_rgba_buffer(m_scaled_size),
      m_pen_color(graphics::color::white_color()),
      m_typesetter("", m_scale)
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

auto graphics::canvas::font() const -> std::shared_ptr<graphics::font>
{
    return m_typesetter.font();
}

auto graphics::canvas::get_bounds() const -> math::rect
{
    return { math::point(0), m_size };
}

// MARK: - Entity

auto graphics::canvas::rebuild_texture() -> void
{
    if (!m_entity) {
        return;
    }

    // Rebuild the texture.
    if (auto existing_texture = m_linked_tex.lock()) {
        existing_texture->destroy();
    }

    auto tex = renderer::create_texture(m_scaled_size, raw());
    m_entity->set_sprite_sheet(std::make_shared<graphics::spritesheet>(tex, m_scaled_size));
    m_entity->set_render_size(m_size);
}

auto graphics::canvas::spawn_entity(const math::point& position) -> std::shared_ptr<graphics::entity>
{
    // Create a new bitmap of the text.
    auto tex = renderer::create_texture(m_scaled_size, raw());

    m_entity = std::make_shared<graphics::entity>(m_size);
    m_entity->set_sprite_sheet(std::make_shared<graphics::spritesheet>(tex, m_scaled_size));
    m_entity->set_position(position);
    m_entity->set_render_size(m_size);

    return m_entity;
}

auto graphics::canvas::entity() -> std::shared_ptr<graphics::entity>
{
    if (!m_entity) {
        // Construct a new entity as we don't currently have one.
        m_entity = spawn_entity({ 0, 0 });
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

// MARK: - Drawing

auto graphics::canvas::clear() -> void
{
    m_rgba_buffer.clear(graphics::color::clear_color());
}

auto graphics::canvas::set_clipping_rect(const math::rect &r) -> void
{
    m_rgba_buffer.set_clipping_rect(r);
}

auto graphics::canvas::clear_clipping_rect() -> void
{
    m_rgba_buffer.clear_clipping_rect();
}

auto graphics::canvas::draw_rect(const math::rect &r) -> void
{
    draw_line(r.origin, { r.origin.x + r.size.width, r.origin.y }, 1);
    draw_line(r.origin, { r.origin.x, r.origin.y + r.size.height }, 1);
    draw_line({ r.origin.x + r.size.width, r.origin.y }, { r.origin.x + r.size.width, r.origin.y + r.size.height }, 1);
    draw_line({ r.origin.x, r.origin.y + r.size.height }, { r.origin.x + r.size.width, r.origin.y + r.size.height }, 1);
}

auto graphics::canvas::fill_rect(const math::rect &r) -> void
{
    m_rgba_buffer.fill_rect(m_pen_color, (r * m_scale).round());
}

auto graphics::canvas::draw_line(const math::point &pp, const math::point &qq, const double& thickness) -> void
{
    auto x0 = static_cast<long>(floor(pp.x * m_scale));
    auto y0 = static_cast<long>(floor(pp.y * m_scale));
    auto x1 = static_cast<long>(floor(qq.x * m_scale));
    auto y1 = static_cast<long>(floor(qq.y * m_scale));

    long dx = std::abs(x1 - x0);
    long dy = std::abs(y1 - y0);
    long sx = x0 < x1 ? 1 : -1;
    long sy = y0 < y1 ? 1 : -1;

    long err = dx - dy;
    long e2 = 0;
    long x2 = 0;
    long y2 = 0;

    auto ed = dx + dy == 0 ? 1 : std::sqrt((dx * dx) + (dy * dy));
    auto wd = thickness * m_scale;

    auto set_pixel = [this] (const long& x, const long& y, const double& intensity) {
        m_rgba_buffer.draw_pixel(
            m_pen_color.with_alpha(std::max(0, std::min(255, static_cast<int>(255.0 * (1.0 - intensity))))),
            { static_cast<double>(x), static_cast<double>(y) }
        );
    };


    for (wd = (wd + 1) / 2;;) {
        set_pixel(x0, y0, (std::fabs(err - dx + dy) / ed - wd + 1));
        e2 = err;
        x2 = x0;
        if ((e2 << 1) >= -dx) {
            for (e2 += dy, y2 = y0; (e2 < ed * wd) && (y1 != y2 || dx > dy); e2 += dx) {
                set_pixel(x0, y2 += sy, (std::fabs(e2) / ed - wd + 1));
            }
            if (x0 == x1) {
                break;
            }
            e2 = err;
            err -= dy;
            x0 += sx;
        }
        if ((e2 << 1) <= dy) {
            for (e2 = dx - e2; (e2 < ed * wd) && (x1 != x2 || dx < dy); e2 += dy) {
                set_pixel(x2 += sx, y0, (std::fabs(e2) / ed - wd + 1));
            }
            if (y0 == y1) {
                break;
            }
            err += dx;
            y0 += sy;
        }
    }
}

auto graphics::canvas::draw_circle(const math::point &p, const double &r) -> void
{
    auto inner_draw_circle = [this] (const math::point& p, const double& r) {
        // Check if the circle intersects the bounds of the canvas. If it doesn't, then don't draw.
        auto extended = math::rect( -r, -r, m_scaled_size.width + r + r, m_scaled_size.height + r + r );
        if (!extended.contains_point(p)) {
            return;
        }

        auto x = static_cast<int>(std::round(r));
        typeof(x) y = 0;
        double err = 0;

        while (x >= y) {
            m_rgba_buffer.draw_pixel(m_pen_color, { static_cast<double>(p.x) + x, static_cast<double>(p.y) + y });
            m_rgba_buffer.draw_pixel(m_pen_color, { static_cast<double>(p.x) + y, static_cast<double>(p.y) + x });
            m_rgba_buffer.draw_pixel(m_pen_color, { static_cast<double>(p.x) - y, static_cast<double>(p.y) + x });
            m_rgba_buffer.draw_pixel(m_pen_color, { static_cast<double>(p.x) - x, static_cast<double>(p.y) + y });
            m_rgba_buffer.draw_pixel(m_pen_color, { static_cast<double>(p.x) - x, static_cast<double>(p.y) - y });
            m_rgba_buffer.draw_pixel(m_pen_color, { static_cast<double>(p.x) - y, static_cast<double>(p.y) - x });
            m_rgba_buffer.draw_pixel(m_pen_color, { static_cast<double>(p.x) + y, static_cast<double>(p.y) - x });
            m_rgba_buffer.draw_pixel(m_pen_color, { static_cast<double>(p.x) + x, static_cast<double>(p.y) - y });

            if (err <= 0) {
                y += 1;
                err += (2 * y) + 1;
            }
            else {
                x -= 1;
                err -= (2 * x) + 1;
            }
        }
    };

    inner_draw_circle((p * m_scale).round(), std::round(r * m_scale));
}

auto graphics::canvas::fill_circle(const math::point &p, const double &r) -> void
{
    auto inner_fill_circle = [this] (const math::point& p, const double& r) {
        // Check if the circle intersects the bounds of the canvas. If it doesn't, then don't draw.
        auto extended = math::rect( -r, -r, m_scaled_size.width + r + r, m_scaled_size.height + r + r );
        if (!extended.contains_point(p)) {
            return;
        }

        for (int y = -static_cast<int>(r); y < r; ++y) {
            auto ww = static_cast<int>(std::sqrt((r * r) - (y * y)));
            auto ry = p.y + y;
            m_rgba_buffer.fill_rect(m_pen_color, { p.x - ww, ry, static_cast<double>(ww << 1), 1 });
        }
    };
    inner_fill_circle((p * m_scale).round(), std::round(r * m_scale));
}

// MARK: - Text

auto graphics::canvas::layout_text(const std::string &text) -> math::size
{
    m_typesetter.set_text(text);
    m_typesetter.layout();
    return m_typesetter.get_bounding_size() / m_scale;
}

auto graphics::canvas::layout_text_in_bounds(const std::string &text, const math::size& bounds) -> math::size
{
    m_typesetter.set_margins((bounds * m_scale).round());
    m_typesetter.set_text(text);
    m_typesetter.layout();
    return m_typesetter.get_bounding_size() / m_scale;
}

auto graphics::canvas::character_point_in_text(const int &position) const -> math::point
{
    return m_typesetter.get_point_at_location(position);
}

auto graphics::canvas::draw_text(const math::point &point) -> void
{
    auto inner_draw_text = [this] (const math::point& point) {
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
            else if (y >= m_scaled_size.height) {
                break;
            }

            auto bmp_line_offset = static_cast<int64_t>(y * text_size.width);

            auto vstart = text_bmp.cbegin() + bmp_line_offset + bmp_line_start;
            auto vend = vstart + bmp_line_len;
            std::vector<graphics::color> cv { vstart, vend };

            m_rgba_buffer.apply_run(cv, start, dy);
        }

        m_typesetter.reset();
    };
    inner_draw_text((point * m_scale).round());
}


auto graphics::canvas::draw_static_image(const asset::static_image::lua_reference &image, const math::rect &rect) -> void
{
    auto inner_draw_image = [this] (const asset::static_image::lua_reference& image, const math::point& point, const math::size& sz) {
        math::rect bounds(math::point(0), m_scaled_size);
        math::rect img_bounds(math::point(0), image->size());
        math::rect img_frame(point, { std::round(sz.width), std::round(sz.height) });

        if (!img_frame.intersects(bounds)) {
            return;
        }

        auto img_data = image->sprite_sheet()->texture()->data();
        auto scaled_len = static_cast<uint32_t>(img_frame.size.width * img_frame.size.height);

        graphite::data::block scaled_data(scaled_len * 4);
        scaled_data.set(static_cast<uint32_t>(0), scaled_data.size());

        // Perform some initial calculations in order to determine how the scaling should be performed
        auto x_scale = img_frame.size.width / img_bounds.size.width;
        auto y_scale = img_frame.size.height / img_bounds.size.height;

        for (auto y = 0; y < img_frame.size.height; ++y) {
            auto ry = static_cast<uint32_t>(std::floor(y / y_scale));
            if (ry >= img_bounds.size.height) {
                break;
            }

            auto src_offset = static_cast<uint32_t>(ry * img_bounds.size.width);
            auto dst_offset = static_cast<uint32_t>(y * img_frame.size.width);
            for (auto x = 0; x < img_frame.size.width; ++x) {
                auto rx = static_cast<uint32_t>(std::floor(x / x_scale));
                if (rx >= img_bounds.size.width) {
                    break;
                }

                auto color = img_data.get<uint32_t>((src_offset + rx) * 4);
                scaled_data.set(color, 4, (dst_offset + x) * 4);
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
            else if (y >= m_scaled_size.height) {
                break;
            }

            auto bmp_line_offset = static_cast<int64_t>(y * img_frame.size.width);
            auto cv = scaled_data.slice((bmp_line_offset + bmp_line_start) * 4, bmp_line_len * 4);
            m_rgba_buffer.apply_run(cv, start, dy);
        }
    };
    if (rect.size.width <= 0 || rect.size.height <= 0) {
        return;
    }
    inner_draw_image(image, (rect.origin * m_scale).round(), (rect.size * m_scale).round());
}

auto graphics::canvas::draw_image(const asset::legacy::macintosh::quickdraw::picture::lua_reference& image, const math::point& point, const math::size& sz) -> void
{
    auto inner_draw_image = [this] (const asset::legacy::macintosh::quickdraw::picture::lua_reference& image, const math::point& point, const math::size& sz) {
        math::rect bounds(math::point(0), m_scaled_size);
        math::rect img_bounds(math::point(0), image->size());
        math::rect img_frame(point, { std::round(sz.width), std::round(sz.height) });

        if (!img_frame.intersects(bounds)) {
            return;
        }

        auto img_data = image->sprite_sheet()->texture()->data();
        auto scaled_len = static_cast<uint32_t>(img_frame.size.width * img_frame.size.height);

        graphite::data::block scaled_data(scaled_len * 4);
        scaled_data.set(static_cast<uint32_t>(0), scaled_data.size());

        // Perform some initial calculations in order to determine how the scaling should be performed
        auto x_scale = img_frame.size.width / img_bounds.size.width;
        auto y_scale = img_frame.size.height / img_bounds.size.height;

        for (auto y = 0; y < img_frame.size.height; ++y) {
            auto ry = static_cast<uint32_t>(std::floor(y / y_scale));
            if (ry >= img_bounds.size.height) {
                break;
            }

            auto src_offset = static_cast<uint32_t>(ry * img_bounds.size.width) * 4;
            auto dst_offset = static_cast<uint32_t>(y * img_frame.size.width) * 4;
            for (auto x = 0; x < img_frame.size.width; ++x) {
                auto rx = static_cast<uint32_t>(std::floor(x / x_scale));
                if (rx >= img_bounds.size.width) {
                    break;
                }
                scaled_data.set(img_data.get<uint32_t>(src_offset), 4, dst_offset);
            }
        }

        // Drawing
        auto bmp_line_start = std::max<int64_t>(0LL, static_cast<int64_t>(-point.x));
        auto bmp_line_len = static_cast<int64_t>(img_frame.size.width) - bmp_line_start - 1;
        auto start = std::max<int64_t>(0LL, static_cast<int64_t>(point.x));

        for (auto y = 0; y < img_frame.size.height; ++y) {
            auto dy = std::floor(y + point.y);
            if (dy < 0) {
                continue;
            }
            else if (y >= m_scaled_size.height) {
                break;
            }

            auto bmp_line_offset = static_cast<int64_t>(y * img_frame.size.width);
            auto cv = scaled_data.slice((bmp_line_offset + bmp_line_start) * 4, bmp_line_len * 4);
            m_rgba_buffer.apply_run(cv, start, dy);
        }
    };
    inner_draw_image(image, (point * m_scale).round(), (sz * m_scale).round());
}

auto graphics::canvas::draw_picture_at_point(const asset::legacy::macintosh::quickdraw::picture::lua_reference &pict, const math::point &point) -> void
{
    auto inner_draw = [this] (const asset::legacy::macintosh::quickdraw::picture::lua_reference &pict, const math::point &point) {
        math::rect bounds(math::point(0), m_scaled_size);
        math::rect pict_bounds(point, pict->size());
        if (!pict_bounds.intersects(bounds)) {
            return;
        }

        auto raw_pict_data = pict->sprite_sheet()->texture()->data();

        auto bmp_line_start = std::max<int64_t>(0LL, static_cast<int64_t>(-point.x));
        auto bmp_line_len = static_cast<int64_t>(pict_bounds.size.width) - bmp_line_start;
        auto start = std::max<int64_t>(0LL, static_cast<int64_t>(point.x));

        for (auto y = 0; y < pict->size().height; ++y) {
            auto dy = std::floor(y + point.y);
            if (dy < 0) {
                continue;
            }
            else if (y >= m_scaled_size.height) {
                break;
            }

            auto bmp_line_offset = static_cast<int64_t>(y * pict_bounds.size.width);
            auto cv = raw_pict_data.slice((bmp_line_offset + bmp_line_start) * 4, bmp_line_len * 4);
            m_rgba_buffer.apply_run(cv, start, dy);
        }
    };
    inner_draw(pict, (point * m_scale).round());
}

auto graphics::canvas::draw_picture(const asset::legacy::macintosh::quickdraw::picture::lua_reference &pict, const math::rect &rect) -> void
{
    if (rect.size.width <= 0 || rect.size.height <= 0) {
        return;
    }

    draw_image(pict, rect.origin, rect.size);
}

auto graphics::canvas::draw_color_icon(const asset::legacy::macintosh::quickdraw::color_icon::lua_reference &icon, const math::point &point, const math::size &sz) -> void
{
    auto inner_draw = [this] (const asset::legacy::macintosh::quickdraw::color_icon::lua_reference &icon, const math::point &point, const math::size &sz) {
        math::rect bounds(math::point(0), m_scaled_size);
        math::rect icon_bounds(point, icon->size());
        if (!icon_bounds.intersects(bounds)) {
            return;
        }

        auto raw_icon_data = icon->sprite_sheet()->texture()->data();

        auto bmp_line_start = std::max(static_cast<int64_t>(0LL), static_cast<int64_t>(-point.x));
        auto bmp_line_len = static_cast<int64_t>(icon_bounds.size.width) - bmp_line_start;
        auto start = std::max(static_cast<int64_t>(0LL), static_cast<int64_t>(point.x));

        for (auto y = 0; y < icon->size().height; ++y) {
            auto dy = std::floor(y + point.y);
            if (dy < 0) {
                continue;
            }
            else if (y >= m_scaled_size.height) {
                break;
            }

            auto bmp_line_offset = static_cast<int64_t>(y * icon_bounds.size.width);
            auto cv = raw_icon_data.slice((bmp_line_offset + bmp_line_start) * 4, bmp_line_len * 4);

            m_rgba_buffer.apply_run(cv, start, dy);
        }
    };
    inner_draw(icon, (point * m_scale).round(), (sz * m_scale).round());
}


// MARK: - Masking

auto graphics::canvas::apply_mask(const graphics::canvas::lua_reference &c) -> void
{
    m_rgba_buffer.apply_mask(c->m_rgba_buffer);
}

auto graphics::canvas::draw_mask(const luabridge::LuaRef &mask_function) -> void
{
    // Construct a canvas that is equal in size to the current canvas for the mask to be drawn in
    // to.
    auto mask = lua_reference(new canvas(m_size));
    mask_function(mask);
    apply_mask(mask);
}