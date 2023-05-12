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

#include "codegen/documentation/structure/heading.hpp"

// MARK: - Construction

luatool::codegen::documentation::heading::heading(const std::string &title, std::int8_t size)
    : m_title(new text(title)), m_size(size)
{}

luatool::codegen::documentation::heading::heading(text *title, std::int8_t size)
    : m_title(title), m_size(size)
{}

// MARK: - Value

auto luatool::codegen::documentation::heading::value() const -> std::string
{
    std::string out;
    for (auto n = 0; n < m_size; ++n) {
        out += "#";
    }
    out += " " + m_title->value();
    return std::move(out);
}