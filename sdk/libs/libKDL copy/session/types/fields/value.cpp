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

#include <libKDL/session/types/fields/value.hpp>
#include <libKDL/exception/module_exception.hpp>

// MARK: - Construction

kdl::session::type_field_value::type_field_value(const lexer::lexeme& name)
    : m_base_name(name), m_decorators({})
{}

// MARK: - Decorators

auto kdl::session::type_field_value::decorators() const -> const decorator::collection&
{
    return m_decorators;
}

auto kdl::session::type_field_value::set_decorators(const std::vector<decorator::instance> &decorators) -> void
{
    m_decorators = decorator::collection(decorators);
}

auto kdl::session::type_field_value::set_decorators(const decorator::collection &collection) -> void
{
    m_decorators = collection;
}

// MARK: - Naming

auto kdl::session::type_field_value::base_name() const -> lexer::lexeme
{
    return m_base_name;
}

auto kdl::session::type_field_value::extended_name(const kdl::session::interpreter::scope& scope) const -> lexer::lexeme
{
    std::string name(m_base_name.text());
    for (const auto& ext : m_name_extensions) {
        if (scope.has_variable(ext)) {
            name.append(scope.variable(ext).text());
        }
    }
    return { name, lexer::lexeme::identifier };
}

auto kdl::session::type_field_value::has_name_extensions() const -> bool
{
    return !m_name_extensions.empty();
}

auto kdl::session::type_field_value::has_export_name() const -> bool
{
    return m_decorators.has_decorator_named(decorator::key::api_export_name);
}

auto kdl::session::type_field_value::export_name() const -> lexer::lexeme
{
    if (has_export_name()) {
        if (m_decorators.decorator_named(decorator::key::api_export_name).has_associated_values()) {
            return m_decorators.decorator_named(decorator::key::api_export_name).associated_value(0);
        }
    }
    return m_base_name;
}

auto kdl::session::type_field_value::set_export_name(const lexer::lexeme &name) -> void
{
    m_decorators.inject(decorator::instance(
        { decorator::key::api_export_name, lexer::lexeme::identifier },
        { 1, name }
    ));
}

auto kdl::session::type_field_value::set_name_extensions(const std::vector<lexer::lexeme> &extensions) -> void
{
    m_name_extensions = extensions;
}

auto kdl::session::type_field_value::add_name_extension(const lexer::lexeme &extension) -> void
{
    m_name_extensions.emplace_back(extension);
}

// MARK: - Explicit Types

auto kdl::session::type_field_value::has_explicit_type() const -> bool
{
    return m_explicit_type.is_reference() || m_explicit_type.has_name();
}

auto kdl::session::type_field_value::set_explicit_type(const type_descriptor &type) -> void
{
    m_explicit_type = type;
}

auto kdl::session::type_field_value::set_explicit_type(bool reference, const lexer::lexeme &name, const std::vector<lexer::lexeme> &hints) -> void
{
    m_explicit_type = type_descriptor(reference, name, hints);
}

auto kdl::session::type_field_value::explicit_type() const -> type_descriptor
{
    return m_explicit_type;
}

// MARK: - Default Values

auto kdl::session::type_field_value::has_default_value() const -> bool
{
    return m_default_value.has_value();
}

auto kdl::session::type_field_value::default_value() const -> interpreter::expression
{
    return m_default_value.value();
}

auto kdl::session::type_field_value::set_default_value(const interpreter::expression &expression) -> void
{
    m_default_value = { expression };
}

auto kdl::session::type_field_value::set_default_value(const lexer::lexeme &value) -> void
{
    set_default_value(kdl::session::interpreter::expression(lexer::lexeme_stream({ value })));
}

auto kdl::session::type_field_value::set_default_value(const std::string &value) -> void
{
    set_default_value(lexer::lexeme(value, lexer::lexeme::string));
}

auto kdl::session::type_field_value::set_default_value(std::int64_t value) -> void
{
    set_default_value(lexer::lexeme(std::to_string(value), lexer::lexeme::integer));
}

// MARK: - Symbols

auto kdl::session::type_field_value::symbols() const -> std::vector<lexer::lexeme>
{
    std::vector<lexer::lexeme> symbol_names;
    for (const auto& [name, value] : m_symbols) {
        symbol_names.emplace_back(name);
    }
    return symbol_names;
}

auto kdl::session::type_field_value::has_symbol(const lexer::lexeme &name) const -> bool
{
    return has_symbol(name.text());
}

auto kdl::session::type_field_value::has_symbol(const std::string &name) const -> bool
{
    for (const auto& [symbol_hash, symbol] : m_symbols) {
        if (symbol.is(name)) {
            return true;
        }
    }
    return false;
}

auto kdl::session::type_field_value::value_for_symbol(const lexer::lexeme &name) const -> lexer::lexeme
{
    return value_for_symbol(name.text());
}

auto kdl::session::type_field_value::value_for_symbol(const std::string &name) const -> lexer::lexeme
{
    for (const auto& value : m_symbols) {
        if (std::get<0>(value).is(name)) {
            return std::get<1>(value);
        }
    }
    throw module_exception("The symbol '" + name + "' could not be found on field value '" + m_base_name.text() + "'");
}

auto kdl::session::type_field_value::add_symbol(const std::string &name, std::int64_t value) -> void
{
    add_symbol(lexer::lexeme(name, lexer::lexeme::identifier), lexer::lexeme(value));
}

auto kdl::session::type_field_value::add_symbol(const std::string &name, const std::string &value) -> void
{
    add_symbol(lexer::lexeme(name, lexer::lexeme::identifier), lexer::lexeme(value, lexer::lexeme::string));
}

auto kdl::session::type_field_value::add_symbol(const lexer::lexeme &name, const lexer::lexeme &value) -> void
{
    m_symbols.emplace_back(name, value);
}

// MARK: - Conversions

auto kdl::session::type_field_value::has_conversion() const -> bool
{
    return m_decorators.has_decorator_named(decorator::key::conversion_name);
}

auto kdl::session::type_field_value::conversion() const -> kdl::session::conversion
{
    if (m_decorators.has_decorator_named(decorator::key::conversion_name)) {
        const auto& conversion_decorator = m_decorators.decorator_named(decorator::key::conversion_name);
        if (conversion_decorator.associated_value_count() >= 2) {
            return { conversion_decorator.associated_value(0), conversion_decorator.associated_value(1) };
        }
    }
    return {};
}

auto kdl::session::type_field_value::set_conversion(const lexer::lexeme &input, const lexer::lexeme &output) -> void
{
    m_decorators.inject(decorator::instance(
        { decorator::key::conversion_name, lexer::lexeme::string },
        { input, output }
    ));
}

// MARK: - Joined Values

auto kdl::session::type_field_value::has_joined_values() const -> bool
{
    return !m_joined_values.empty();
}

auto kdl::session::type_field_value::joined_values_count() const -> std::size_t
{
    return m_joined_values.size();
}

auto kdl::session::type_field_value::joined_value_at(std::int32_t idx) const -> const type_field_value &
{
    return m_joined_values.at(idx);
}

auto kdl::session::type_field_value::has_joined_value_for(const lexer::lexeme &symbol) const -> bool
{
    return has_joined_value_for(symbol.text());
}

auto kdl::session::type_field_value::has_joined_value_for(const std::string &symbol) const -> bool
{
    // Check if the symbol is in this, and if it is return false as the value is _not_ joined.
    if (has_symbol(symbol)) {
        return false;
    }

    // Check through each of the joined values for the symbol.
    for (const auto& field : m_joined_values) {
        if (field.has_symbol(symbol)) {
            return true;
        }
    }

    return false;
}

auto kdl::session::type_field_value::joined_value_for(const lexer::lexeme &symbol) const -> std::tuple<std::int32_t, lexer::lexeme>
{
    return joined_value_for(symbol.text());
}

auto kdl::session::type_field_value::joined_value_for(const std::string &symbol) const -> std::tuple<std::int32_t, lexer::lexeme>
{
    if (!has_symbol(symbol)) {
        std::int32_t i = 0;
        for (const auto& field : m_joined_values) {
            if (field.has_symbol(symbol)) {
                return { i, field.value_for_symbol(symbol) };
            }
            ++i;
        }
    }
    throw module_exception("The symbol '" + symbol + "' could not be found on any joined values for field value '" + m_base_name.text() + "'");
}
