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

#include <iostream>
#include <unordered_map>
#include <libKestrel/graphics/canvas/canvas.hpp>
#include <libKestrel/device/console.hpp>
#include <libKestrel/graphics/renderer/common/renderer.hpp>
#include <libKestrel/font/font.hpp>
#include <libKestrel/event/text_entry.hpp>

// MARK: - Storage

static struct {
    bool dirty { true };
    kestrel::math::size canvas_size;
    std::shared_ptr<kestrel::graphics::canvas> canvas;
    std::shared_ptr<kestrel::ecs::entity> entity;
    kestrel::font::reference::lua_reference font { nullptr };
    std::vector<struct kestrel::device::console::record> history;
    std::unordered_map<kestrel::util::uid, std::function<auto(const kestrel::device::console::record&)->void>> pipes;
    kestrel::text_entry_event text_entry;
} s_console;

// MARK: - History

auto kestrel::device::console::write(const std::string &text, enum status status) -> void
{
    struct record item = {
        .status = status,
        .text = text,
    };

    if (s_console.history.size() >= 10000) {
        s_console.history.erase(s_console.history.begin());
    }

    std::cout << item.text << std::endl;
    s_console.history.emplace_back(std::move(item));

    for (const auto& pipe : s_console.pipes) {
        pipe.second(s_console.history.back());
    }

    s_console.dirty = true;
    redraw();
}

auto kestrel::device::console::history() -> const std::vector<struct record>&
{
    return s_console.history;
}

// MARK: - Pipe

auto kestrel::device::console::pipe_output(const std::function<auto(const struct record &) -> void> &handler) -> util::uid
{
    util::uid uid;
    s_console.pipes.insert(std::pair(uid, handler));
    return uid;
}

auto kestrel::device::console::destroy_pipe(const util::uid &uid) -> void
{
    auto it = s_console.pipes.find(uid);
    if (it != s_console.pipes.end()) {
        s_console.pipes.erase(it);
    }
}

// MARK: - Drawing & Entity

auto kestrel::device::console::entity() -> std::shared_ptr<ecs::entity>
{
    return s_console.entity;
}

auto kestrel::device::console::show_console() -> void
{
    if (!s_console.entity) {
        s_console.canvas_size = kestrel::renderer::window_size();
        s_console.canvas_size.set_height(s_console.canvas_size.height() * 0.5);

        s_console.canvas = std::make_shared<graphics::canvas>(s_console.canvas_size);
        s_console.entity = s_console.canvas->entity();

        s_console.font = { new kestrel::font::reference("Menlo", 11) };
        s_console.font->load_for_graphics();

        s_console.text_entry.on_enter([&] (const std::string& command) {
            write(command, status::input);
            s_console.text_entry.set_string_value("");
            s_console.text_entry.set_cursor_position(0);
        });

        s_console.dirty = true;
        redraw();
    }
}

auto kestrel::device::console::hide_console() -> void
{
    s_console.font->unload();
    s_console.entity = nullptr;
    s_console.font = { nullptr };
}

auto kestrel::device::console::toggle_console() -> void
{
    if (s_console.entity) {
        hide_console();
    }
    else {
        show_console();
    }
}

auto kestrel::device::console::send_event(const event &e) -> bool
{
    if (s_console.entity) {
        s_console.text_entry.receive(e);
        s_console.dirty = true;
        redraw();
        return true;
    }
    return false;
}

auto kestrel::device::console::redraw() -> void
{
    if (!s_console.entity || !s_console.dirty) {
        return;
    }

    s_console.canvas->clear();
    s_console.canvas->set_pen_color(graphics::color::white_ref(0, 200));
    s_console.canvas->fill_rect({ math::point(0), s_console.canvas_size });

    s_console.canvas->set_font(s_console.font);

    auto y = s_console.canvas_size.height() - 5;

    // Draw user input
    s_console.canvas->set_pen_color(graphics::color::rgb_ref(94, 168, 104, 255));
    auto inset = s_console.canvas->layout_text_in_bounds("> ", { 50, 20 });
    y -= inset.height();
    s_console.canvas->draw_text({ 5, y });

    s_console.canvas->set_pen_color(graphics::color::white_color_ref());
    auto size = s_console.canvas->layout_text_in_bounds(s_console.text_entry.string_value(), { s_console.canvas_size.width() - 10 - inset.width(), 9999 });
    const auto& cursor = s_console.canvas->character_point_in_text(s_console.text_entry.cursor_position());
    s_console.canvas->draw_text({ 5 + inset.width(), y });
    s_console.canvas->fill_rect({ 5 + inset.width() + cursor.x(), y + cursor.y(), 2, size.height() });

    // Draw the output and history...
    for (auto it = s_console.history.rbegin(); it != s_console.history.rend() && y >= 0; ++it) {
        inset = math::size(0);

        if (it->status == device::console::status::error) {
            s_console.canvas->set_pen_color(graphics::color::rgb_ref(255, 0, 0, 255));
        }
        else if (it->status == device::console::status::warning) {
            s_console.canvas->set_pen_color(graphics::color::rgb_ref(255, 128, 0, 255));
        }
        else if (it->status == device::console::status::input) {
            s_console.canvas->set_pen_color(graphics::color::rgb_ref(94, 168, 104, 255));
            inset = s_console.canvas->layout_text_in_bounds("> ", { 50, 20 });
            s_console.canvas->draw_text({ 5, y - inset.height() });
            s_console.canvas->set_pen_color(graphics::color::white_color_ref());
        }
        else {
            s_console.canvas->set_pen_color(graphics::color::rgb_ref(204, 204, 204, 255));
        }

        size = s_console.canvas->layout_text_in_bounds(it->text, { s_console.canvas_size.width() - 10, 20 });
        y -= size.height();
        s_console.canvas->draw_text({ 5 + inset.width(), y });
    }

    s_console.canvas->rebuild_texture();
    s_console.dirty = false;
}