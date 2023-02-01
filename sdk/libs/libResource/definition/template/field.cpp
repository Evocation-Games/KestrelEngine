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

#include <libResource/definition/template/field.hpp>

// MARK: - Construction

resource::definition::binary_template::field::field(const struct type &type, const std::string &label)
    : m_type(type), m_label(label)
{}

// MARK: - Nested Types

auto resource::definition::binary_template::field::has_nested_type() const -> bool
{
    return (m_type.nested_type() != nullptr) && (m_type.value() == type::NESTED);
}

auto resource::definition::binary_template::field::nested_type() const -> const instance *
{
    return m_type.nested_type();
}

// MARK: - Accessors

auto resource::definition::binary_template::field::type() const -> const struct type&
{
    return m_type;
}

auto resource::definition::binary_template::field::label() const -> std::string
{
    return m_label;
}

// MARK: - Hashing

auto resource::definition::binary_template::field::hash() const -> hash_value
{
    return foundation::hashing::string(m_label);
}
