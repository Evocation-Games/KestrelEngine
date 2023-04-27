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

#include <libUI/format/interface/indexed_list_lookup.hpp>

// MARK: - Construction

ui::format::indexed_list_lookup::indexed_list_lookup(const resource::reference &reference, const std::vector<std::uint16_t> &list, const std::string &tnam)
    : m_type(tnam), m_reference(reference), m_list(list)
{}

// MARK: - Accessors

auto ui::format::indexed_list_lookup::reference() const -> resource::reference
{
    return m_reference;
}

auto ui::format::indexed_list_lookup::type() const -> std::string
{
    return m_type;
}

auto ui::format::indexed_list_lookup::list() const -> std::vector<std::uint16_t>
{
    return m_list;
}
