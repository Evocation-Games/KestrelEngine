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

#include <libKDL/session/resources/field.hpp>

// MARK: - Construction

kdl::session::resources::field::field(const kdl::session::type_field *definition, const kdl::session::type_field_value *value)
    : m_definition(definition), m_value_definition(value)
{}

// MARK: - Accessors

auto kdl::session::resources::field::name() const -> lexer::lexeme
{
    return m_value_definition->base_name();
}

// MARK: - Nesting

auto kdl::session::resources::field::named(const lexer::lexeme &name) -> field&
{
    auto it = m_nested_fields.find(hashing::string(name.text()));
    return it->second;
}

auto kdl::session::resources::field::set_nested_fields(const std::unordered_map<hashing::value, resources::field> &fields) -> void
{
    m_nested_fields = fields;
}

// MARK: - Values

auto kdl::session::resources::field::set_value(const lexer::lexeme &value) -> void
{
    m_value = value;
}