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

#include <libUI/format/interface/element.hpp>

// MARK: - Construction

ui::format::element::element(enum ui::format::element::type type)
    : m_type(type)
{}

// MARK: - Attributes

auto ui::format::element::add_attribute(enum attribute::code code, const std::string &name, const ui::format::attribute_value &value) -> void
{
    m_attributes.emplace_back(code, name, value);
}

auto ui::format::element::add_attribute(enum attribute::code code, const std::string &name, const std::vector<attribute_value> &values) -> void
{
    m_attributes.emplace_back(code, name, values);
}

auto ui::format::element::add_attribute(const struct ui::format::attribute &attribute) -> void
{
    m_attributes.emplace_back(attribute);
}

// MARK: - Accessors

auto ui::format::element::type() const -> enum type
{
    return m_type;
}

auto ui::format::element::attribute_count() const -> std::size_t
{
    return m_attributes.size();
}

auto ui::format::element::attribute() const -> struct attribute
{
    return m_attributes.front();
}

auto ui::format::element::attributes() const -> std::vector<struct attribute>
{
    return m_attributes;
}