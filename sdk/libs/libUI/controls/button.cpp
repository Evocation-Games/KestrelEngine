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

#include <libUI/controls/button.hpp>

// MARK: - Construction

ui::button::button(const ui::rect &frame, const std::string& title)
    : ui::control(frame), m_title(title)
{}

// MARK: - Actions

auto ui::button::press() -> void
{
    perform_action();
}

// MARK: - Accessors

auto ui::button::is_default() const -> bool
{
    return m_default;
}

auto ui::button::set_is_default(bool is_default) -> void
{
    m_default = is_default;
}

auto ui::button::title() const -> std::string
{
    return m_title;
}

auto ui::button::set_title(const std::string &title) -> void
{
    m_title = title;
}

// MARK: - Events

auto ui::button::mouse_down(const ui::event &event) -> void
{
    needs_redraw();
}

auto ui::button::mouse_up(const ui::event &event) -> void
{
    needs_redraw();
    press();
}

auto ui::button::key_up(const ui::event &event) -> void
{
    if (m_default && (event.is(ui::hid::key::enter) || event.is(ui::hid::key::kp_enter))) {
        press();
    }
}