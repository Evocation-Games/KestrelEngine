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

#include <libUI/format/interface/background.hpp>

// MARK: - Construction

ui::format::background::background(const resource::reference& fill)
    : m_type(type::is_static), m_fill(fill)
{}

ui::format::background::background(const resource::reference& top, const resource::reference& fill, const resource::reference& bottom)
    : m_type(type::is_dynamic), m_top(top), m_fill(fill), m_bottom(bottom)
{}

// MARK: - Accessors

auto ui::format::background::type() const -> enum type
{
    return m_type;
}

auto ui::format::background::top() const -> resource::reference
{
    return m_top;
}

auto ui::format::background::fill() const -> resource::reference
{
    return m_fill;
}

auto ui::format::background::bottom() const -> resource::reference
{
    return m_bottom;
}
