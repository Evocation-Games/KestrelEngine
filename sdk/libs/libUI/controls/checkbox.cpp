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

#include <libUI/controls/checkbox.hpp>

// MARK: - Construction

ui::checkbox::checkbox(const ui::rect &frame, const std::string &label, bool value)
    : ui::control(frame), m_label(label), m_value(value)
{}

// MARK: - Accessors

auto ui::checkbox::label() const -> std::string
{
    return m_label;
}

auto ui::checkbox::set_label(const std::string &label) -> void
{
    m_label = label;
    needs_redraw();
}

auto ui::checkbox::value() const -> bool
{
    return m_value;
}

auto ui::checkbox::set_value(bool value) -> void
{
    m_value = value;
    needs_redraw();
}

// MARK: - Actions

auto ui::checkbox::toggle() -> void
{
    m_value = !m_value;
    needs_redraw();
}

// MARK: - Events

auto ui::checkbox::mouse_down(const ui::event &event) -> void
{
    needs_redraw();
}

auto ui::checkbox::mouse_up(const ui::event &event) -> void
{
    toggle();
}