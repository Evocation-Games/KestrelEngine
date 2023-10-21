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

#include <libUI/format/interface/value.hpp>

// MARK: - Construction

ui::format::value::value(const std::string& value)
    : m_value(value), m_type(type::string)
{}

ui::format::value::value(std::int64_t value)
    : m_value(value), m_type(type::integer)
{}

ui::format::value::value(std::uint64_t value)
    : m_value(value), m_type(type::unsigned_integer)
{}

ui::format::value::value(const indexed_lookup& value)
    : m_value(value), m_type(type::indexed_lookup)
{}

ui::format::value::value(const indexed_list_lookup& value)
    : m_value(value), m_type(type::indexed_list_lookup)
{}

ui::format::value::value(const named_lookup& value)
    : m_value(value), m_type(type::named_lookup)
{}

ui::format::value::value(const lookup& value)
    : m_value(value), m_type(type::lookup)
{}

ui::format::value::value(std::uint32_t value)
    : m_value(value), m_type(type::color)
{}

ui::format::value::value(const std::vector<std::string>& value)
    : m_value(value), m_type(type::string_list)
{}

ui::format::value::value(const std::vector<std::int64_t>& value)
    : m_value(value), m_type(type::integer_list)
{}

ui::format::value::value(const std::vector<std::uint64_t>& value)
    : m_value(value), m_type(type::unsigned_integer_list)
{}

ui::format::value::value(const resource::reference& value)
    : m_value(value), m_type(type::image)
{}

ui::format::value::value(const std::vector<resource::reference>& value)
    : m_value(value), m_type(type::image_list)
{}

// MARK: - Accessors

auto ui::format::value::type() const -> enum type
{
    return m_type;
};