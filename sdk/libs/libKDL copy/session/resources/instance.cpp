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

#include <iostream>
#include <libKDL/session/resources/instance.hpp>
#include <libKDL/session/types/fields/definition.hpp>
#include <libKDL/session/resources/field.hpp>
#include <libKDL/session/types/binary_template/binary_template.hpp>

// MARK: - Construction

kdl::session::resources::instance::instance(
    const type_definition *type,
    reference::identifier id,
    const std::string& name,
    const interpreter::scope& scope
)
    : m_type(type), m_id(id), m_name(name)
{
    if (m_type->all_fields().empty()) {
        // Attempt to use binary template
        synthesize_fields(const_cast<type_definition *>(type)->binary_template().fields(), scope);
    }
    else {
        synthesize_fields(const_cast<std::vector<kdl::session::type_field>&>(m_type->all_fields()), scope);
    }
}

// MARK: - Accessors

auto kdl::session::resources::instance::type() const -> const type_definition *
{
    return m_type;
}

// MARK: - Field Management

auto kdl::session::resources::instance::synthesize_fields(std::vector<kdl::session::type_field>& fields, const interpreter::scope& scope) -> void
{
    for (auto& field : fields) {
        std::int32_t lower = 0;
        std::int32_t upper = 0;

        if (field.repeatable().enabled()) {
            lower = field.repeatable().lower_bound();
            upper = field.repeatable().upper_bound();
        }

        for (auto n = lower; n <= upper; ++n) {
            auto shadow_scope = scope.shadow();
            shadow_scope.add_variable("FieldNumber", n, lexer::lexeme::integer);

            for (auto i = 0; i < field.expected_values(); ++i) {
                const auto& value = field.value_at(i);
                const auto& binary_field_name = value.extended_name(shadow_scope);

                resources::field bin_field(&field, &value);

                // If this a nested type, then we need to synthesize the fields for it.
                auto underlying_field = const_cast<type_definition *>(m_type)->binary_template().named(binary_field_name);
                if (underlying_field.type == binary_template::NESTED && underlying_field.nested_type.definition) {
                    instance nested(underlying_field.nested_type.definition, m_id, m_name, shadow_scope);
                    bin_field.set_nested_fields(nested.m_fields);
                }

                m_fields.emplace(hashing::string(binary_field_name.text()), bin_field);
            }
        }
    }
}

auto kdl::session::resources::instance::synthesize_fields(const std::vector<kdl::session::binary_template::field>& fields, const interpreter::scope& scope) -> void
{
    for (const auto& underlying_field: fields) {
        auto field = new type_field(underlying_field.label);
        auto value = new type_field_value(underlying_field.label);

        resources::field bin_field(field, value);
        m_fields.emplace(hashing::string(underlying_field.label.text()), bin_field);

        // If this a nested type, then we need to synthesize the fields for it.
        if (underlying_field.type == binary_template::NESTED && underlying_field.nested_type.definition) {
            instance nested(underlying_field.nested_type.definition, m_id, m_name, scope);
            bin_field.set_nested_fields(nested.m_fields);
        }
    }
}

auto kdl::session::resources::instance::binary_field(const lexer::lexeme &binary_field) -> const binary_template::field&
{
    auto tmpl = const_cast<type_definition *>(m_type)->binary_template();
    if (!tmpl.has_field(binary_field)) {
        throw std::runtime_error("");
    }
    return tmpl.named(binary_field);
}

auto kdl::session::resources::instance::field(const lexer::lexeme& name) -> resources::field&
{
    auto it = m_fields.find(hashing::string(name.text()));
    if (it == m_fields.end()) {
        throw std::runtime_error("");
    }
    return it->second;
}