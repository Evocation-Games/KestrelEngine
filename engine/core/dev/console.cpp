// Copyright (c) 2021 Tom Hancocks
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

#include "core/dev/console.hpp"

// MARK: - Construction

dev::console::console()
    : m_size({ 600, 300 }), m_dirty(true), m_visible(true)
{
    m_history.emplace_back("Kestrel v0.5");
}

// MARK: - Drawing Update

auto dev::console::update() -> void
{
    if (!m_canvas) {
        set_size({600, 400});
    }

    if (!m_dirty && m_visible) {
        return;
    }

    m_canvas->clear();
    m_canvas->set_pen_color(graphics::color::black_color().with_alpha(200));
    m_canvas->fill_rect({ 0, 0, m_size.width, m_size.height });

    m_canvas->set_font("Menlo", 12);
    m_canvas->set_pen_color(graphics::color::white_color());

    auto y = m_size.height - 5;
    // Draw user input...
    auto size = m_canvas->layout_text_in_bounds("> " + m_input.string_value(), { m_size.width - 10, 20 });
    y -= size.height;
    m_canvas->draw_text({ 5, y });
    m_canvas->fill_rect({ 7 + size.width, y, 3, size.height });

    // Draw the output and history...
    for (auto it = m_history.rbegin(); it != m_history.rend() && y >= 0; ++it) {
        if ((*it).substr(0, 3) == "***") {
            m_canvas->set_pen_color(graphics::color::red_color());
        }
        else {
            m_canvas->set_pen_color(graphics::color::light_grey_color());
        }

        size = m_canvas->layout_text_in_bounds(*it, { m_size.width - 10, 20 });
        y -= size.height;
        m_canvas->draw_text({ 5, y });
    }

    m_canvas->rebuild_texture();
}

// MARK: - Setters

auto dev::console::set_size(const math::size &sz) -> void
{
    m_size = sz;
    m_canvas = std::make_unique<graphics::canvas>(sz);
    m_entity = m_canvas->entity();
    m_dirty = true;
}

// MARK: - Accessors

auto dev::console::entity() -> graphics::entity::lua_reference
{
    return m_entity;
}

auto dev::console::set_visible(bool f) -> void
{
    m_visible = f;
}

auto dev::console::toggle_visibility() -> void
{
    m_visible = !m_visible;
}

auto dev::console::is_visible() const -> bool
{
    return m_visible;
}

// MARK: - Events

auto dev::console::receive(const event::key &key) -> void
{
    m_input.receive(key);
}
