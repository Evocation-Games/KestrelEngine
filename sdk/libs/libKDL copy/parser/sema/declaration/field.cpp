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

#include <libKDL/parser/sema/declaration/field.hpp>
#include <libKDL/session/types/fields/repeatable.hpp>
#include <libKDL/exception/unexpected_lexeme_exception.hpp>
#include <libKDL/parser/sema/declaration/types/implicit_value_parser.hpp>

// MARK: - Field Parsing

auto kdl::sema::declaration::field::parse(
    session::resources::instance &instance,
    lexer::lexeme_stream &stream,
    session::interpreter::scope &scope,
    session::session *session
)
    -> void
{
    if (!stream.expect({ lexer::expectation(lexer::lexeme::identifier).be_true() })) {
        throw unexpected_lexeme_exception("Expected identifier for field name.", stream.peek());
    }
    auto field_name = stream.read();
    auto& field = const_cast<session::type_field&>(instance.type()->field_named(field_name));

    if (field.repeatable().enabled() && field.repeatable().has_count_field()) {
        field_name = field.repeatable().count_field();
    }

    // Acquire a new lock on the field.
//    auto lock = instance.acquire_field(field);
}

// MARK: - Default Values

auto kdl::sema::declaration::field::apply_defaults(
    session::resources::instance &instance,
    session::type_field& field,
    lexer::lexeme_stream &stream,
    session::interpreter::scope &scope,
    session::session *session
)
    -> void
{
    auto field_name = field.name();
    std::int32_t lower = 0;
    std::int32_t upper = 0;

    const auto& repeatable = field.repeatable();
    if (repeatable.enabled()) {
        lower = upper
              = repeatable.lower_bound();
        if (repeatable.has_count_field()) {
            field_name = repeatable.count_field();
        }
        else {
            upper = repeatable.upper_bound();
        }
    }

    // Iterate for the total number of field repetitions, starting at the lower bound and finishing at the upper.
    for (std::int32_t field_number = lower; field_number <= upper; ++field_number) {
        auto shadow_scope = scope.shadow();
        shadow_scope.add_variable("FieldNumber", field_number, lexer::lexeme::integer);

        // Work through the values.
        for (std::int32_t n = 0; n < field.expected_values(); ++n) {
            const auto& field_value = field.value_at(n);
            auto shadow_value_scope = shadow_scope.shadow();
            field_value.explicit_type().inject(shadow_value_scope, session);

            if (!field_value.has_default_value()) {
                goto next_field_repeat;
            }

            // Joined/Merged values
            std::vector<const session::binary_template::field *> binary_fields;
            for (std::int32_t i = 0; i <= field_value.joined_values_count(); ++i) {
                const auto& value = (i == 0 ? field_value : field_value.joined_value_at(i - 1));
                const auto& bin_field = instance.binary_field(value.extended_name(shadow_value_scope));
                binary_fields.emplace_back(&bin_field);
            }

            // Push the default value in preparation for inserting a default value.
            auto default_value_scope = shadow_value_scope.shadow();
            stream.push({
                field_value.default_value().evaluate(default_value_scope),
                lexer::lexeme(";", lexer::lexeme::semi)
            });

            if (binary_fields.back()->type == session::binary_template::NESTED) {
                auto& instance_field = instance.field(binary_fields.back()->label);
                const auto vars = default_value_scope.local_variables();
                for (const auto& var : vars) {
                    auto& nested_field = instance_field.named(var.name());
                    nested_field.set_value(var.value());
                }
            }
            else {

            }

        }

        next_field_repeat:
        return;
    }
}
