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

#include <libUI/format/interface/attribute.hpp>

// MARK: - Construction

ui::format::attribute::attribute(enum ui::format::attribute::code code, const std::string& name, const ui::format::attribute_value& value)
    : m_code(code), m_name(name), m_values({ value })
{}

ui::format::attribute::attribute(enum ui::format::attribute::code code, const std::string& name, const std::vector<attribute_value>& values)
    : m_code(code), m_name(name), m_values(values)
{}

// MARK: - Accessors


auto ui::format::attribute::code() const -> enum code
{
    return m_code;
}

auto ui::format::attribute::name() const -> std::string
{
    return m_name;
}

auto ui::format::attribute::value_count() const -> std::size_t
{
    return m_values.size();
}

auto ui::format::attribute::value() const -> attribute_value
{
    return m_values.front();
}

auto ui::format::attribute::values() const -> std::vector<attribute_value>
{
    return m_values;
}

// MARK: - Value Management

auto ui::format::attribute::add_value(const ui::format::attribute_value &value) -> void
{
    m_values.emplace_back(value);
}