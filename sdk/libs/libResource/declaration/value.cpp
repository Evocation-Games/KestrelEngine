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

#include <libResource/declaration/value.hpp>

// MARK: - Construction

resource::value_container::value_container(const std::string &value)
    : m_type(type::string), m_value(value)
{}

resource::value_container::value_container(const reference& value)
    : m_type(type::reference), m_value(value)
{}

resource::value_container::value_container(const std::vector<std::uint8_t> &bytes)
    : m_type(type::data), m_value(bytes)
{}

resource::value_container::value_container(const std::uint8_t *bytes, std::size_t length)
    : m_type(type::data), m_value(std::vector<std::uint8_t>(bytes, bytes+length))
{}

// MARK: - Accessors

auto resource::value_container::type() const -> enum type
{
    return m_type;
}

auto resource::value_container::string_value() const -> std::string
{
    return std::get<std::string>(m_value);
}

auto resource::value_container::reference_value() const -> reference
{
    return std::get<reference>(m_value);
}

auto resource::value_container::data_value() const -> std::vector<std::uint8_t>
{
    return std::get<std::vector<std::uint8_t>>(m_value);
}