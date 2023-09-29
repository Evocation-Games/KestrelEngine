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

#include <libResource/definition/type/value.hpp>
// MARK: - Construction

resource::definition::type::field_value::field_value(const std::string &base_name)
    : m_base_name(base_name)
{
}

resource::definition::type::field_value::field_value(const binary_template::field *field)
    : m_field(field), m_base_name(field->label())
{
}

resource::definition::type::field_value::field_value(const binary_template::field *field, const std::string &base_name)
    : m_field(field), m_base_name(base_name)
{
}

// MARK: - Name

auto resource::definition::type::field_value::base_name() const -> std::string
{
    return m_base_name;
}

auto resource::definition::type::field_value::add_name_extension(const std::string &name) -> void
{
    if (std::find(m_name_extensions.begin(), m_name_extensions.end(), name) == m_name_extensions.end()) {
        m_name_extensions.emplace_back(name);
    }
}

auto resource::definition::type::field_value::extended_name(const interpreter::scope &scope) const -> std::string
{
    auto name = m_base_name;
    if (!m_name_extensions.empty()) {
        for (const auto& extension : m_name_extensions) {
            if (!scope.has_variable(extension)) {
                continue;
            }
            const auto& extension_replacement = scope.variable(extension);
            name += extension_replacement.string_value();
        }
    }
    else if (scope.has_variable("FieldNumber")) {
        const auto& extension_replacement = scope.variable("FieldNumber");
        name += extension_replacement.string_value();
    }
    return name;
}

auto resource::definition::type::field_value::has_extended_name() const -> bool
{
    return !m_name_extensions.empty();
}

auto resource::definition::type::field_value::has_export_name() const -> bool
{
    return has_decorator("api");
}

auto resource::definition::type::field_value::export_name() const -> std::string
{
    if (has_export_name()) {
        const auto& decorator = decorators_named("api");
        if (decorator.back().has_associated_values()) {
            return decorator.back().associated_value_at(0);
        }
    }
    return m_base_name;
}

auto resource::definition::type::field_value::set_export_name(const std::string &name) -> void
{
    add_decorator("api", { name });
}

// MARK: - Explicit Type

auto resource::definition::type::field_value::is_type_explicit() const -> bool
{
    return has_decorator("__explicit");
}

auto resource::definition::type::field_value::type() const -> const descriptor&
{
    return m_type_descriptor;
}

auto resource::definition::type::field_value::set_type(const descriptor& type, bool is_explicit) -> void
{
    m_type_descriptor = type;

    if (is_explicit) {
        add_decorator("__explicit", std::vector<std::string>({}));
    }
}

// MARK: - Symbols

auto resource::definition::type::field_value::has_symbols() const -> bool
{
    return !m_symbols.empty();
}

auto resource::definition::type::field_value::symbols() const -> const std::unordered_map<std::string, symbol>&
{
    return m_symbols;
}

auto resource::definition::type::field_value::symbol_names() const -> std::vector<std::string>
{
    std::vector<std::string> names;
    for (const auto& it : m_symbols) {
        names.emplace_back(it.first);
    }
    return std::move(names);
}

auto resource::definition::type::field_value::has_symbol_named(const std::string &name) const -> bool
{
    return (m_symbols.find(name) != m_symbols.end());
}

auto resource::definition::type::field_value::symbol_named(const std::string &name) const -> const symbol &
{
    return m_symbols.find(name)->second;
}

auto resource::definition::type::field_value::add_symbol(const std::string &name, const value_container &value) -> symbol&
{
    auto it = m_symbols.find(name);
    if (it == m_symbols.end()) {
        m_symbols.emplace(name, symbol(name, value));
        it = m_symbols.find(name);
    }
    else {
        it->second = symbol(name, value);
    }
    return it->second;
}

// MARK: - Joined Values

auto resource::definition::type::field_value::has_joined_values() const -> bool
{
    return !m_joined_values.empty();
}

auto resource::definition::type::field_value::joined_values() const -> const std::vector<field_value> &
{
    return m_joined_values;
}

auto resource::definition::type::field_value::add_joined_value(const field_value &value) -> void
{
    m_joined_values.emplace_back(value);
}

// MARK: - Default Values

auto resource::definition::type::field_value::has_default_value() const -> bool
{
    return m_default_value.has_value();
}

auto resource::definition::type::field_value::default_value() const -> interpreter::script::statement
{
    return m_default_value.value();
}

auto resource::definition::type::field_value::set_default_value(const interpreter::script::statement &stmt) -> void
{
    m_default_value = stmt;
}