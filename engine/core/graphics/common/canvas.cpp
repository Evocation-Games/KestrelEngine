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
#include "core/graphics/common/canvas.hpp"
#include "core/environment.hpp"

// MARK: - Lua

auto graphics::canvas::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginClass<graphics::canvas>("Canvas")
            .addConstructor<auto(*)(const math::size&)->void, graphics::canvas::lua_reference>()
            .addProperty("penColor", &graphics::canvas::get_pen_color, &graphics::canvas::set_pen_color)
            .addFunction("entity", &graphics::canvas::entity)
            .addFunction("rebuildEntityTexture", &graphics::canvas::rebuild_texture)
            .addFunction("fillRect", &graphics::canvas::fill_rect)
            .addFunction("drawCircle", &graphics::canvas::draw_circle)
            .addFunction("setFont", &graphics::canvas::set_font)
            .addFunction("sizeOfText", &graphics::canvas::text_size)
            .addFunction("drawText", &graphics::canvas::draw_text)
            .addFunction("drawMacintoshPicture", &graphics::canvas::draw_picture)
            .addFunction("spawnEntity", &graphics::canvas::spawn_entity)
            .addFunction("clear", &graphics::canvas::clear)
        .endClass();
}

// MARK: - Construction

graphics::canvas::canvas(const math::size& size)
    : m_size(size),
      m_buffer(static_cast<int>(size.width * size.height), graphics::color(0, 0, 0, 0)),
      m_pen_color(graphics::color::white_color()),
      m_font(std::make_shared<graphics::font>())
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
}

auto graphics::canvas::set_font(const std::string &name, const int &size) -> void
{
    m_font_size = size;
    m_font = std::make_shared<graphics::font>(name);
}

// MARK: - Entity

auto graphics::canvas::rebuild_texture() -> void
{
    if (m_entity.get() == nullptr) {
        return;
    }

    if (auto env = environment::active_environment().lock()) {
        // Rebuild the texture.
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

auto graphics::canvas::raw() const -> std::vector<uint32_t>
{
    auto out = std::vector<uint32_t>();
    for (const auto& i : m_buffer) {
        uint32_t color = (i.get_alpha() << 24UL)
                         | i.get_red()
                         | (i.get_green() << 8UL)
                         | (i.get_blue() << 16UL);
        out.push_back(color);
    }
    return out;
}

// MARK: - Indicies

auto graphics::canvas::index_at(const double& x, const double& y) const -> int
{
    auto i = static_cast<int>(std::floor((m_size.width * y) + x));
    if (i < 0 || i >= m_buffer.size()) {
        return {};
    }
    return i;
}

// MARK: - Drawing

auto graphics::canvas::clear() -> void
{
    for (auto & i : m_buffer) {
        i = graphics::color(0, 0, 0, 0);
    }
}

auto graphics::canvas::draw_pixel(const double &x, const double &y) -> void
{
    if (y < 0 || y >= m_size.height) {
        return;
    }

    auto line_start = index_at(0, y);
    auto line_end = index_at(m_size.width - 1, y);

    auto i = index_at(x, y);

    if (i < line_start || i >= line_end) {
        return;
    }

    m_buffer[i].blend_in_place(m_pen_color);
}

auto graphics::canvas::fill_rect(const math::rect &r) -> void
{
    for (auto y = static_cast<int>(r.origin.y); y < r.origin.y + r.size.height; ++y) {
        if (y < 0) {
            y = 0;
        }
        else if (y >= m_size.height) {
            break;
        }

        auto run_start = index_at(r.origin.x, y);
        auto run_end = index_at(r.origin.x + r.size.width, y);

        auto line_start = index_at(0, y);
        auto line_end = index_at(m_size.width, y);

        for (auto i = std::max(run_start, line_start); i < std::min(run_end, line_end); ++i) {
            m_buffer[i].blend_in_place(m_pen_color);
        }
    }
}

auto graphics::canvas::draw_circle(const math::point &p, const double &r) -> void
{
    auto x = std::round(r);
    typeof(x) y = 0;
    double err = 0;

    while (x >= y) {
        draw_pixel(p.x + x, p.y + y);
        draw_pixel(p.x + y, p.y + x);
        draw_pixel(p.x - y, p.y + x);
        draw_pixel(p.x - x, p.y + y);
        draw_pixel(p.x - x, p.y - y);
        draw_pixel(p.x - y, p.y - x);
        draw_pixel(p.x + y, p.y - x);
        draw_pixel(p.x + x, p.y - y);

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

// MARK: - Text

auto graphics::canvas::text_size(const std::string &text) const -> math::size
{
    return m_font->text_size(text, m_font_size);
}

auto graphics::canvas::draw_text(const std::string &text, const math::point &point) -> void
{
    auto text_size = this->text_size(text);
    auto text_bmp = m_font->render_text(text, text_size, m_font_size, m_pen_color);

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

        auto run_start = index_at(std::floor(point.x), dy);
        auto run_end = index_at(std::floor(point.x + text_size.width), dy);

        auto line_start = index_at(0, dy);
        auto line_end = index_at(std::floor(m_size.width), dy);

        auto src_i = static_cast<int>(std::floor(y * text_size.width));
        for (auto i = std::max(run_start, line_start); i < std::min(run_end, line_end); ++i) {
            auto color = graphics::color::color_value(text_bmp[src_i++]);
            m_buffer[i].blend_in_place(color);
        }
    }
}

auto graphics::canvas::draw_picture(const asset::macintosh_picture::lua_reference &pict, const math::rect &rect) -> void
{
    auto raw_pict_data = pict->spritesheet()->texture()->data();

    for (auto y = 0; y < pict->size().height; ++y) {
        auto dy = std::floor(y + rect.origin.y);
        if (dy < 0) {
            continue;
        }
        else if (y >= m_size.height) {
            break;
        }

        auto run_start = index_at(std::floor(rect.origin.x), dy);
        auto run_end = index_at(std::floor(rect.origin.x + pict->size().width), dy);

        auto line_start = index_at(0, dy);
        auto line_end = index_at(std::floor(m_size.width), dy);

        auto src_i = static_cast<int>(std::floor(y * pict->size().width));
        for (auto i = std::max(run_start, line_start); i < std::min(run_end, line_end); ++i) {
            auto color = graphics::color::color_value(raw_pict_data[src_i++]);
            m_buffer[i].blend_in_place(color);
        }
    }
}
