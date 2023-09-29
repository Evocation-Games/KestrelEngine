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
#include <libKestrel/kestrel.hpp>
#include <libKestrel/math/line.hpp>
#include <libKestrel/graphics/renderer/common/renderer.hpp>
#include <libKestrel/graphics/canvas/canvas.hpp>
#include <libData/writer.hpp>

// MARK: - Construction

kestrel::graphics::canvas::canvas(const math::size& size)
    : m_size(size.round()),
      m_scale(renderer::scale_factor()),
      m_scaled_size((m_size * m_scale).round()),
      m_rgba_buffer(m_scaled_size),
      m_pen_color(graphics::color::white_color()),
      m_typesetter("", m_scale)
{

}

// MARK: - Accessors

auto kestrel::graphics::canvas::get_name() const -> std::string
{
    return m_name;
}

auto kestrel::graphics::canvas::set_name(const std::string &name) -> void
{
    m_name = name;
}

auto kestrel::graphics::canvas::get_pen_color() const -> graphics::color::lua_reference
{
    return { new color(m_pen_color) };
}

auto kestrel::graphics::canvas::set_pen_color(const graphics::color::lua_reference& color) -> void
{
    m_pen_color = *color.get();
    m_typesetter.set_font_color(m_pen_color);
    m_dirty = true;
}

auto kestrel::graphics::canvas::set_font(const kestrel::font::reference::lua_reference& font) -> void
{
    font->load_for_graphics();
    m_typesetter.set_font(*font.get());
    m_dirty = true;
}

auto kestrel::graphics::canvas::font() const -> std::shared_ptr<graphics::font>
{
    return m_typesetter.font();
}

auto kestrel::graphics::canvas::get_bounds() const -> math::rect
{
    return { math::point(0), m_size };
}

// MARK: - Destruction

kestrel::graphics::canvas::~canvas()
{
    m_linked_tex = nullptr;
    m_entity = nullptr;
}

// MARK: - Entity

auto kestrel::graphics::canvas::rebuild_texture() -> void
{
    if (!m_entity || !m_dirty) {
        return;
    }

    // Rebuild the texture.
    if (m_linked_tex) {
        m_linked_tex->set_data(data());
    }
    m_dirty = false;
}

auto kestrel::graphics::canvas::spawn_entity(const math::point& position) -> std::shared_ptr<ecs::entity>
{
    // Create a new bitmap of the text.
    m_linked_tex = renderer::create_texture(m_scaled_size, data());

    m_entity = std::make_shared<ecs::entity>(m_size);
    m_entity->set_sprite_sheet(std::make_shared<graphics::sprite_sheet>(m_linked_tex, m_scaled_size));
    m_entity->set_position(position);
    m_entity->set_render_size(m_size);
    m_entity->set_name(m_name);

    return m_entity;
}

auto kestrel::graphics::canvas::entity() -> std::shared_ptr<ecs::entity>
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

auto kestrel::graphics::canvas::raw() const -> std::uint8_t *
{
    return m_rgba_buffer.data();
}

auto kestrel::graphics::canvas::data() const -> data::block
{
    return std::move(m_rgba_buffer.data_block());
}

// MARK: - Drawing

auto kestrel::graphics::canvas::clear() -> void
{
    m_rgba_buffer.clear(graphics::color::clear_color());
    m_dirty = true;
}

auto kestrel::graphics::canvas::set_clipping_rect(const math::rect &r) -> void
{
    m_rgba_buffer.set_clipping_rect(r);
}

auto kestrel::graphics::canvas::clear_clipping_rect() -> void
{
    m_rgba_buffer.clear_clipping_rect();
}

auto kestrel::graphics::canvas::draw_rect(const math::rect &r) -> void
{
    draw_line(r.origin(), { r.x() + r.width(), r.y() }, 1);
    draw_line(r.origin(), { r.x(), r.y() + r.height() }, 1);
    draw_line({ r.x() + r.width(), r.y() }, { r.x() + r.width(), r.y() + r.height() }, 1);
    draw_line({ r.x(), r.y() + r.height() }, { r.x() + r.width(), r.y() + r.height() }, 1);
    m_dirty = true;
}

auto kestrel::graphics::canvas::fill_rect(const math::rect &r) -> void
{
    m_rgba_buffer.fill_rect(m_pen_color, (r * m_scale).round());
    m_dirty = true;
}

auto kestrel::graphics::canvas::draw_line(const math::point &pp, const math::point &qq, double thickness) -> void
{
    auto ll = math::line(pp, qq) * m_scale;
    auto x0 = static_cast<long>(ll.p.x());
    auto y0 = static_cast<long>(ll.p.y());
    auto x1 = static_cast<long>(ll.q.x());
    auto y1 = static_cast<long>(ll.q.y());

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

    auto set_pixel = [this] (long x, long y, double intensity) {
        m_rgba_buffer.draw_pixel(
            *m_pen_color.with_alpha(std::max(0, std::min(255, static_cast<int>(255.0 * (1.0 - intensity))))).get(),
            { static_cast<float>(x), static_cast<float>(y) }
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

    m_dirty = true;
}

auto kestrel::graphics::canvas::draw_circle(const math::point &p, double r) -> void
{
    auto inner_draw_circle = [this] (const math::point& p, double r) {
        // Check if the circle intersects the bounds of the canvas. If it doesn't, then don't draw.
        auto extended = math::rect( -r, -r, m_scaled_size.width() + r + r, m_scaled_size.height() + r + r );
        if (!extended.contains_point(p)) {
            return;
        }

        auto x = static_cast<int>(std::round(r));
        decltype(x) y = 0;
        double err = 0;

        while (x >= y) {
            m_rgba_buffer.draw_pixel(m_pen_color, { static_cast<float>(p.x()) + x, static_cast<float>(p.y()) + y });
            m_rgba_buffer.draw_pixel(m_pen_color, { static_cast<float>(p.x()) + y, static_cast<float>(p.y()) + x });
            m_rgba_buffer.draw_pixel(m_pen_color, { static_cast<float>(p.x()) - y, static_cast<float>(p.y()) + x });
            m_rgba_buffer.draw_pixel(m_pen_color, { static_cast<float>(p.x()) - x, static_cast<float>(p.y()) + y });
            m_rgba_buffer.draw_pixel(m_pen_color, { static_cast<float>(p.x()) - x, static_cast<float>(p.y()) - y });
            m_rgba_buffer.draw_pixel(m_pen_color, { static_cast<float>(p.x()) - y, static_cast<float>(p.y()) - x });
            m_rgba_buffer.draw_pixel(m_pen_color, { static_cast<float>(p.x()) + y, static_cast<float>(p.y()) - x });
            m_rgba_buffer.draw_pixel(m_pen_color, { static_cast<float>(p.x()) + x, static_cast<float>(p.y()) - y });

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
    m_dirty = true;
}

auto kestrel::graphics::canvas::fill_circle(const math::point &p, double r) -> void
{
    auto inner_fill_circle = [this] (const math::point& p, double r) {
        // Check if the circle intersects the bounds of the canvas. If it doesn't, then don't draw.
        auto extended = math::rect( -r, -r, m_scaled_size.width() + r + r, m_scaled_size.height() + r + r );
        if (!extended.contains_point(p)) {
            return;
        }

        for (int y = -static_cast<int>(r); y < r; ++y) {
            auto ww = static_cast<int>(std::sqrt((r * r) - (y * y)));
            auto ry = p.y() + y;
            m_rgba_buffer.fill_rect(m_pen_color, { p.x() - ww, ry, static_cast<float>(ww << 1), 1 });
        }
    };
    inner_fill_circle((p * m_scale).round(), std::round(r * m_scale));
    m_dirty = true;
}

// MARK: - Text

auto kestrel::graphics::canvas::layout_text(const std::string &text) -> math::size
{
    m_typesetter.set_text(text);
    m_typesetter.layout();
    return m_typesetter.get_bounding_size() / m_scale;
}

auto kestrel::graphics::canvas::layout_text_in_bounds(const std::string &text, const math::size& bounds) -> math::size
{
    m_typesetter.set_margins((bounds * m_scale).round());
    m_typesetter.set_text(text);
    m_typesetter.layout();
    return m_typesetter.get_bounding_size() / m_scale;
}

auto kestrel::graphics::canvas::character_point_in_text(std::uint32_t position) const -> math::point
{
    return m_typesetter.get_point_at_location(position) / m_scale;
}

auto kestrel::graphics::canvas::draw_text(const math::point &point) -> void
{
    auto inner_draw_text = [this] (const math::point& point) {
        auto text_bmp = m_typesetter.render();
        auto text_size = m_typesetter.get_bounding_size();

        auto bmp_line_start = std::max<std::int64_t>(0LL, static_cast<std::int64_t>(-point.x()));
        auto bmp_line_len = static_cast<std::int64_t>(text_size.width()) - bmp_line_start;
        auto start = std::max<std::int64_t>(0LL, static_cast<std::int64_t>(point.x()));

        // Drawing the text into the canvas buffer at the appropriate point.
        for (auto y = 0; y < text_size.height(); ++y) {
            auto dy = std::floor(y + point.y());
            if (dy < 0) {
                // TODO: Calculate the correct Y to be on, if it exists.
                continue;
            }
            else if (y >= m_scaled_size.height()) {
                break;
            }

            auto bmp_line_offset = static_cast<std::int64_t>(y * text_size.width());

            auto vstart = text_bmp.cbegin() + bmp_line_offset + bmp_line_start;
            auto vend = vstart + bmp_line_len;
            std::vector<graphics::color> cv { vstart, vend };

            m_rgba_buffer.apply_run(cv, start, dy);
        }

        m_typesetter.reset();
    };
    inner_draw_text((point * m_scale).round());
    m_dirty = true;
}

auto kestrel::graphics::canvas::draw_static_image(const image::static_image::lua_reference &image, const math::rect &rect) -> void
{
    auto inner_draw_image = [this] (const image::static_image::lua_reference& image, const math::point& point, const math::size& sz) {
        math::rect bounds(math::point(0), m_scaled_size);
        math::rect img_bounds(math::point(0), image->size());
        math::rect img_frame(point, { std::round(sz.width()), std::round(sz.height()) });

        if (!img_frame.intersects(bounds)) {
            return;
        }

        auto img_data = image->sprite_sheet()->texture()->data();
        auto scaled_len = static_cast<std::uint32_t>(img_frame.size().area());

        data::block scaled_data(scaled_len * 4);
        scaled_data.set(static_cast<std::uint32_t>(0), scaled_data.size());

        // Perform some initial calculations in order to determine how the scaling should be performed
        auto scale = img_frame.size() / img_bounds.size();
        auto x_scale = scale.width();
        auto y_scale = scale.height();

        for (auto y = 0; y < img_frame.height(); ++y) {
            auto ry = static_cast<std::uint32_t>(std::floor(y / y_scale));
            if (ry >= img_bounds.height()) {
                break;
            }

            auto src_offset = static_cast<std::uint32_t>(ry * img_bounds.width());
            auto dst_offset = static_cast<std::uint32_t>(y * img_frame.width());
            for (auto x = 0; x < img_frame.width(); ++x) {
                auto rx = static_cast<std::uint32_t>(std::floor(x / x_scale));
                if (rx >= img_bounds.width()) {
                    break;
                }

                auto color = img_data.get<std::uint32_t>((src_offset + rx) * 4);
                scaled_data.set(color, 4, (dst_offset + x) * 4);
            }
        }

        // Drawing
        auto bmp_line_start = std::max<std::int64_t>(0LL, static_cast<std::int64_t>(-point.x()));
        auto bmp_line_len = static_cast<std::int64_t>(img_frame.width()) - bmp_line_start;
        auto start = std::max<std::int64_t>(0LL, static_cast<std::int64_t>(point.x()));

        for (auto y = 0; y < img_frame.height(); ++y) {
            auto dy = std::floor(y + point.y());
            if (dy < 0) {
                continue;
            }
            else if (y >= m_scaled_size.height()) {
                break;
            }

            auto bmp_line_offset = static_cast<std::int64_t>(y * img_frame.width());
            auto cv = scaled_data.slice((bmp_line_offset + bmp_line_start) * 4, bmp_line_len * 4);
            m_rgba_buffer.apply_run(cv, start, dy);
        }
    };
    if (rect.width() <= 0 || rect.height() <= 0) {
        return;
    }
    auto scaled = (rect * m_scale).round();
    inner_draw_image(image, scaled.origin(), scaled.size());
    m_dirty = true;
}

auto kestrel::graphics::canvas::draw_image(const image::legacy::macintosh::quickdraw::picture::lua_reference& image, const math::point& point, const math::size& sz) -> void
{
    auto inner_draw_image = [this] (const image::legacy::macintosh::quickdraw::picture::lua_reference& image, const math::point& point, const math::size& sz) {
        math::rect bounds(math::point(0), m_scaled_size);
        math::rect img_bounds(math::point(0), image->size());
        math::rect img_frame(point, sz.round());

        if (!img_frame.intersects(bounds)) {
            return;
        }

        auto img_data = image->sprite_sheet()->texture()->data();
        auto scaled_len = static_cast<std::uint32_t>(img_frame.size().area());

        data::block scaled_data(scaled_len * 4);
        scaled_data.set(static_cast<std::uint32_t>(0), scaled_data.size());

        // Perform some initial calculations in order to determine how the scaling should be performed
        auto scale = img_frame.size() / img_bounds.size();
        auto x_scale = scale.width();
        auto y_scale = scale.height();

        for (auto y = 0; y < img_frame.height(); ++y) {
            auto ry = static_cast<std::uint32_t>(std::floor(y / y_scale));
            if (ry >= img_bounds.height()) {
                break;
            }

            auto src_offset = static_cast<std::uint32_t>(ry * img_bounds.width()) << 2;
            auto dst_offset = static_cast<std::uint32_t>(y * img_frame.width()) << 2;
            for (auto x = 0; x < img_frame.width(); ++x) {
                auto rx = static_cast<std::uint32_t>(std::floor(x / x_scale));
                if (rx >= img_bounds.width()) {
                    break;
                }
                scaled_data.set(img_data.get<std::uint32_t>(src_offset), 4, dst_offset);
            }
        }

        // Drawing
        auto bmp_line_start = std::max<std::int64_t>(0LL, static_cast<std::int64_t>(-point.x()));
        auto bmp_line_len = static_cast<std::int64_t>(img_frame.width()) - bmp_line_start - 1;
        auto start = std::max<std::int64_t>(0LL, static_cast<std::int64_t>(point.x()));

        for (auto y = 0; y < img_frame.height(); ++y) {
            auto dy = std::floor(y + point.y());
            if (dy < 0) {
                continue;
            }
            else if (y >= m_scaled_size.height()) {
                break;
            }

            auto bmp_line_offset = static_cast<std::int64_t>(y * img_frame.width());
            auto cv = scaled_data.slice((bmp_line_offset + bmp_line_start) * 4, bmp_line_len * 4);
            m_rgba_buffer.apply_run(cv, start, dy);
        }
    };
    inner_draw_image(image, (point * m_scale).round(), (sz * m_scale).round());
    m_dirty = true;
}

auto kestrel::graphics::canvas::draw_picture_at_point(const image::legacy::macintosh::quickdraw::picture::lua_reference& pict, const math::point& point) -> void
{
    auto inner_draw = [this] (const image::legacy::macintosh::quickdraw::picture::lua_reference& pict, const math::point& point) {
        math::rect bounds(math::point(0), m_scaled_size);
        math::rect pict_bounds(point, pict->size());
        if (!pict_bounds.intersects(bounds)) {
            return;
        }

        auto raw_pict_data = pict->sprite_sheet()->texture()->data();

        auto bmp_line_start = std::max<std::int64_t>(0LL, static_cast<std::int64_t>(-point.x()));
        auto bmp_line_len = static_cast<std::int64_t>(pict_bounds.width()) - bmp_line_start;
        auto start = std::max<std::int64_t>(0LL, static_cast<std::int64_t>(point.x()));

        for (auto y = 0; y < pict->size().height(); ++y) {
            auto dy = std::floor(y + point.y());
            if (dy < 0) {
                continue;
            }
            else if (y >= m_scaled_size.height()) {
                break;
            }

            auto bmp_line_offset = static_cast<std::int64_t>(y * pict_bounds.width());
            auto cv = raw_pict_data.slice((bmp_line_offset + bmp_line_start) * 4, bmp_line_len * 4);
            m_rgba_buffer.apply_run(cv, start, dy);
        }
    };
    inner_draw(pict, (point * m_scale).round());
    m_dirty = true;
}

auto kestrel::graphics::canvas::draw_picture(const image::legacy::macintosh::quickdraw::picture::lua_reference& pict, const math::rect& rect) -> void
{
    if (rect.width() <= 0 || rect.height() <= 0) {
        return;
    }
    draw_image(pict, rect.origin(), rect.size());
    m_dirty = true;
}

auto kestrel::graphics::canvas::draw_color_icon(const image::legacy::macintosh::quickdraw::color_icon::lua_reference& icon, const math::point &point, const math::size &sz) -> void
{
    auto inner_draw = [this] (const image::legacy::macintosh::quickdraw::color_icon::lua_reference& icon, const math::point& point, const math::size& sz) {
        math::rect bounds(math::point(0), m_scaled_size);
        math::rect icon_bounds(point, icon->size());
        if (!icon_bounds.intersects(bounds)) {
            return;
        }

        auto raw_icon_data = icon->sprite_sheet()->texture()->data();

        auto bmp_line_start = std::max(static_cast<std::int64_t>(0LL), static_cast<std::int64_t>(-point.x()));
        auto bmp_line_len = static_cast<std::int64_t>(icon_bounds.width()) - bmp_line_start;
        auto start = std::max(static_cast<std::int64_t>(0LL), static_cast<std::int64_t>(point.x()));

        for (auto y = 0; y < icon->size().height(); ++y) {
            auto dy = std::floor(y + point.y());
            if (dy < 0) {
                continue;
            }
            else if (y >= m_scaled_size.height()) {
                break;
            }

            auto bmp_line_offset = static_cast<std::int64_t>(y * icon_bounds.width());
            auto cv = raw_icon_data.slice((bmp_line_offset + bmp_line_start) * 4, bmp_line_len * 4);

            m_rgba_buffer.apply_run(cv, start, dy);
        }
    };
    inner_draw(icon, (point * m_scale).round(), (sz * m_scale).round());
    m_dirty = true;
}

// MARK: - Masking

auto kestrel::graphics::canvas::apply_mask(const lua_reference& c) -> void
{
    m_rgba_buffer.apply_mask(c->m_rgba_buffer);
    m_dirty = true;
}

auto kestrel::graphics::canvas::draw_mask(const luabridge::LuaRef& mask_function) -> void
{
    // Construct a canvas that is equal in size to the current canvas for the mask to be drawn in
    // to.
    auto mask = lua_reference(new canvas(m_size));
    mask_function(mask);
    apply_mask(mask);
    m_dirty = true;
}