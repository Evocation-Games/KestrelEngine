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

// MARK: - Name

auto resource::definition::type::field_value::base_name() const -> std::string
{
    return m_base_name;
}

auto resource::definition::type::field_value::add_name_extension(const std::string &name) -> void
{
    m_name_extensions.emplace_back(name);
}

auto resource::definition::type::field_value::extended_name(const interpreter::scope &scope) const -> std::string
{
    auto name = m_base_name;
    for (const auto& extension : m_name_extensions) {
        if (!scope.has_variable(extension)) {
            continue;
        }
        const auto& extension_replacement = scope.variable(extension);
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
        add_decorator("__explicit", {});
    }
}