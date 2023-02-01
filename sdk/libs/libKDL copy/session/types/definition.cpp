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

#include <libKDL/session/types/definition.hpp>
#include <libKDL/exception/unknown_field_exception.hpp>

// MARK: - Construction

kdl::session::type_definition::type_definition(const std::string &name, const std::string &code)
    : m_name(name), m_code(code), m_template(code), m_decorators({})
{}

// MARK: - Accessors

auto kdl::session::type_definition::name() const -> std::string
{
    return m_name;
}

auto kdl::session::type_definition::code() const -> std::string
{
    return m_code;
}

// MARK: - Decorators

auto kdl::session::type_definition::decorators() const -> const decorator::collection&
{
    return m_decorators;
}

auto kdl::session::type_definition::set_decorators(const decorator::collection &collection) -> void
{
    m_decorators = collection;
}

auto kdl::session::type_definition::set_decorators(const std::vector<decorator::instance> &decorators) -> void
{
    m_decorators = decorator::collection(decorators);
}

auto kdl::session::type_definition::is_format_compatible(const format& format) const -> bool
{
    if (m_decorators.has_decorator_named(decorator::key::format_if)) {
        const auto& format_decorator = m_decorators.decorator_named(decorator::key::format_if);
        if (format_decorator.has_associated_values()) {
            for (auto i = 0; i < format_decorator.associated_value_count(); ++i) {
                if (format_named(format_decorator.associated_value(i).text()) == format) {
                    return true;
                }
            }
        }
    }
    return true;
}

// MARK: - Constructor Management

auto kdl::session::type_definition::constructors() const -> const std::vector<type_constructor>&
{
    return m_constructors;
}

auto kdl::session::type_definition::add_constructor(const type_constructor& constructor) -> void
{
    m_constructors.emplace_back(constructor);
}

// MARK: - Binary Template

auto kdl::session::type_definition::binary_template() -> struct binary_template&
{
    return m_template;
}

auto kdl::session::type_definition::set_binary_template(const struct binary_template& tmpl) -> void
{
    m_template = tmpl;
}

// MARK: - Field Management

auto kdl::session::type_definition::add_field(const lexer::lexeme& name) -> type_field&
{
    m_fields.emplace_back(name);
    return m_fields.back();
}

auto kdl::session::type_definition::add_field(const type_field& field) -> void
{
    m_fields.emplace_back(field);
}

auto kdl::session::type_definition::field_named(const lexer::lexeme& name) const -> const struct type_field&
{
    for (const auto& field : m_fields) {
        if (field.name().is(name)) {
            return field;
        }
    }
    throw unknown_field_exception("The field '" + name.text() +"' could not be found in type '" + m_name + "'.", name);
}

auto kdl::session::type_definition::all_fields() const -> const std::vector<type_field>&
{
    return m_fields;
}