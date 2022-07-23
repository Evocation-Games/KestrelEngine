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

#include <iostream>
#include "core/dev/console.hpp"
#include "core/ui/font/font.hpp"

// MARK: - Construction

dev::console::console()
    : m_size({ 600, 300 }), m_dirty(true), m_visible(false)
{
    m_history.emplace_back("Kestrel v0.7");

    m_font = { new ui::font::reference("Menlo", 11) };
    m_font->load_for_graphics();

    m_input.on_enter([&] (const std::string& input) {
        send_command(input);
    });

    m_on_command = [] (const std::string&) {};
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

    m_canvas->set_font(m_font);
    m_canvas->set_pen_color(graphics::color::white_color());

    auto y = m_size.height - 5;
    // Draw user input...
    m_canvas->set_pen_color(graphics::color::teal_color());
    auto inset = m_canvas->layout_text_in_bounds("> ", { 50, 20 });
    y -= inset.height;
    m_canvas->draw_text({ 5, y });

    auto size = m_canvas->layout_text_in_bounds(m_input.string_value(), { m_size.width - 10 - inset.width, 9999 });
    const auto& cursor = m_canvas->character_point_in_text(m_input.cursor_position());
    m_canvas->set_pen_color(graphics::color::white_color());
    m_canvas->draw_text({ 5 + inset.width, y });
    m_canvas->fill_rect({ 5 + inset.width + cursor.x, y + cursor.y, 2, size.height });

    // Draw the output and history...
    for (auto it = m_history.rbegin(); it != m_history.rend() && y >= 0; ++it) {
        auto line = *it;
        if (line.substr(0, 3) == "***") {
            line = line.substr(3);
            m_canvas->set_pen_color(graphics::color::red_color());
        }
        else if (line.substr(0, 4) == "&IN>") {
            line = line.substr(4);
            m_canvas->set_pen_color(graphics::color::white_color());
        }
        else {
            m_canvas->set_pen_color(graphics::color::grey_color());
        }

        size = m_canvas->layout_text_in_bounds(line, { m_size.width - 10, 20 });
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

auto dev::console::entity() -> std::shared_ptr<graphics::entity>
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

auto dev::console::entries() const -> const std::vector<std::string> &
{
    return m_history;
}

// MARK: - Events

auto dev::console::receive(const event& e) -> void
{
    m_input.receive(e);
}

auto dev::console::write(const std::string &message) -> void
{
#if DEBUG
    std::cout << message << std::endl;
#endif
    m_history.emplace_back(message);
}

// MARK: - Commands

auto dev::console::on_command(std::function<auto(const std::string &)->void> callback) -> void
{
    m_on_command = std::move(callback);
}

auto dev::console::send_command(const std::string& command) -> void
{
    m_history.emplace_back("&IN>" + command);
    m_on_command(command);
    m_input.set_string_value("");
    m_input.set_cursor_position(0);
}
