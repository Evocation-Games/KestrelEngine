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

#include <cmath>
#include <vector>
#include <libKDL/exception/unexpected_lexeme_exception.hpp>
#include <libKDL/exception/unrecognised_binary_template_field_exception.hpp>
#include <libKDL/exception/incorrect_arguments_exception.hpp>
#include <libKDL/parser/sema/type_definition/field_definition/parser.hpp>
#include <libKDL/parser/sema/type_definition/value_reference/parser.hpp>
#include <libKDL/parser/sema/list/parser.hpp>
#include <libKDL/parser/sema/decorator/parser.hpp>
#include <libKDL/session/decorators/decorator.hpp>

// MARK: - Parsing

auto kdl::sema::type_definition::field_definition::parse(
    lexer::lexeme_stream& stream,
    const session::binary_template& tmpl,
    session::interpreter::scope& scope,
    session::session *session
)
    -> session::type_field
{
    // Consume any leading decorator on the field.
    std::vector<session::decorator::instance> decorators;
    while (stream.expect({ lexer::expectation(lexer::lexeme::directive).be_true() })) {
        decorators.emplace_back(decorator::parse(stream, scope));
    }

    // Field Name
    stream.ensure({
        lexer::expectation(lexer::lexeme::identifier, "field").be_true(),
        lexer::expectation(lexer::lexeme::l_paren).be_true()
    });
    if (!stream.expect({ lexer::expectation(lexer::lexeme::string).be_true() })) {
        throw unexpected_lexeme_exception("Field name must be a string.", stream.peek());
    }

    const auto& field_name = stream.read();
    session::type_field field(field_name);
    field.set_decorators(decorators);
    stream.ensure({ lexer::expectation(lexer::lexeme::r_paren).be_true() });

    // Does the body specify a repeatable?
    if (stream.expect({ lexer::expectation(lexer::lexeme::identifier, "repeatable").be_true() })) {
        auto lx = stream.read();

        list::parser list(stream);
        list.set_list_start(lexer::lexeme::l_angle);
        list.set_list_end(lexer::lexeme::r_angle);
        list.set_delimiter(lexer::lexeme::comma);
        list.add_valid_list_item(lexer::lexeme::integer);
        list.add_valid_list_item(lexer::lexeme::identifier);
        list.add_valid_list_item(lexer::lexeme::var);
        auto items = list.parse(scope, false);

        if (items.size() >= 2) {
            const auto& lower_bound = scope.resolve_or_passthrough(items[0]);
            const auto& upper_bound = scope.resolve_or_passthrough(items[1]);
            const auto& count_field = items[2];
            auto& repeatable = field.make_repeatable(lower_bound.value<std::int32_t>(), upper_bound.value<std::int32_t>());

            if (items.size() == 3) {
                if (!count_field.is(lexer::lexeme::identifier)) {
                    throw unexpected_lexeme_exception("Count field name for field repeatable clause should be an identifier", count_field);
                }
                else if (!tmpl.has_field(count_field)) {
                    throw unrecognised_binary_template_field_exception("The specified count field name, does not exist in binary template.", count_field);
                }
                repeatable.set_count_field(count_field);
            }
        }
        else if (items.size() == 1 && items[0].is(lexer::lexeme::identifier)) {
            const auto& count_field = items[0];
            if (!tmpl.has_field(count_field)) {
                throw unrecognised_binary_template_field_exception("The specified count field name, does not exist in binary template.", count_field);
            }

            const auto& bin_field = tmpl.named(count_field);
            auto upper_bound = static_cast<std::int32_t>(std::pow(2, session::binary_template::data_type_base_size(bin_field.type) << 3));
            field.make_repeatable(1, upper_bound)
                 .set_count_field(count_field);
        }
        else if (items.size() == 1) {
            const auto& upper_bound = scope.resolve_or_passthrough(items[0]);
            field.make_repeatable(0, upper_bound.value<std::int32_t>());
        }
        else {
            throw incorrect_arguments_exception("Field repeatable clause has incorrect number of arguments.", field_name);
        }
    }

    // The body of the field is enclosed in '{' braces '}'
    if (stream.expect({ lexer::expectation(lexer::lexeme::l_brace).be_true() })) {
        stream.advance();
        while (stream.expect({ lexer::expectation(lexer::lexeme::r_brace).be_false() })) {
            field.add_value(value_reference::parse(stream, tmpl, scope, session));
            stream.ensure({ lexer::expectation(lexer::lexeme::semi).be_true() });
        }
        stream.ensure({ lexer::expectation(lexer::lexeme::r_brace).be_true() });
    }
    else if (
        stream.expect({ lexer::expectation(lexer::lexeme::semi).be_true() }) &&
        !field.decorators().has_decorator_named(session::decorator::key::synthesize_name)
    ) {
        throw std::runtime_error("Field definition must be explicitly specify the binary fields that compose it, or be decorated by @synthesize.");
    }
    else if (stream.expect({ lexer::expectation(lexer::lexeme::semi).be_true() })) {
        if (!tmpl.has_field(field_name)) {
            throw unrecognised_binary_template_field_exception("Field name does not match any binary template fields.", field_name);
        }

        session::type_field_value value(field_name);
        field.add_value(value);
    }
    else {
        throw unexpected_lexeme_exception("Missing field values after field definition.", field_name);
    }

    return field;
}