// Copyright (c) 2023 Tom Hancocks
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

#include <stdexcept>
#include <libUI/controls/control.hpp>
#include <libUI/cells/cell.hpp>

// MARK: - Construction

ui::control::control(const ui::rect &frame)
    : ui::responder(), m_frame(frame)
{}

auto ui::control::prepare_for_use() -> void
{
    throw std::runtime_error("This must be override in a subclass");
}

// MARK: - Accessors

auto ui::control::frame() const -> ui::rect
{
    return m_frame;
}

auto ui::control::set_frame(const ui::rect& frame) -> void
{
    m_frame = frame;
}

auto ui::control::disabled() const -> bool
{
    return m_disabled;
}

auto ui::control::set_disabled(bool disabled) -> void
{
    m_disabled = disabled;
}

auto ui::control::hidden() const -> bool
{
    return m_hidden;
}

auto ui::control::set_hidden(bool hidden) -> void
{
    m_hidden = hidden;
}

auto ui::control::mouse_inside() const -> bool
{
    return m_inside;
}

auto ui::control::mouse_pressed() const -> bool
{
    return m_pressed;
}

auto ui::control::continuous_action() const -> bool
{
    return m_continuous;
}

auto ui::control::set_continuous(bool continuous) -> void
{
    m_continuous = continuous;
}

auto ui::control::action() const -> ui::format::action
{
    return m_action;
}

auto ui::control::set_action(const ui::format::action &action) -> void
{
    m_action = action;
}

// MARK: - Events

auto ui::control::hit_test(const ui::point &point) const -> bool
{
    return (point.x >= m_frame.origin.x && point.x <= (m_frame.origin.x + m_frame.size.width))
        && (point.y >= m_frame.origin.y && point.y <= (m_frame.origin.y + m_frame.size.height));
}

auto ui::control::send_event(const ui::event &event) -> bool
{
    if (m_disabled || m_hidden) {
        return false;
    }

    if (event.is_mouse_event() && hit_test(event.location())) {
        if (event.has(ui::event::type::mouse_move) && !m_inside) {
            m_inside = true;
            mouse_enter(event);
        }

        if (event.has(ui::event::type::any_mouse_down) && !m_pressed) {
            m_pressed = true;
            mouse_down(event);
        }

        if (event.has(ui::event::type::any_mouse_up) && m_pressed) {
            m_pressed = false;
            mouse_up(event);
        }

        return true;
    }
    else if (event.is_mouse_event()) {
        if (event.has(ui::event::type::mouse_move) && m_inside) {
            m_inside = false;
            mouse_exit(event);
            return true;
        }
    }

    // Fallback to the responder.
    return ui::responder::send_event(event);
}

// MARK: - Event Notifications

auto ui::control::mouse_enter(const ui::event &event) -> void
{}

auto ui::control::mouse_exit(const ui::event &event) -> void
{}

auto ui::control::mouse_down(const ui::event &event) -> void
{}

auto ui::control::mouse_up(const ui::event &event) -> void
{}

auto ui::control::key_down(const ui::event &event) -> void
{}

auto ui::control::key_repeat(const ui::event &event) -> void
{}

auto ui::control::key_up(const ui::event &event) -> void
{}

// MARK: - Drawing

auto ui::control::draw() -> void
{
    if (m_dirty && m_cell) {
        m_cell->draw(this, ui::rect(ui::point(0, 0), m_frame.size));
    }
    m_dirty = false;
}

auto ui::control::needs_redraw() -> void
{
    m_dirty = true;
}

// MARK: - Actions

auto ui::control::perform_action() -> void
{

}