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

#include <libKDL/parser/sema/type_definition/value_reference/parser.hpp>
#include <libKDL/parser/sema/type_definition/value_reference/symbol_list_parser.hpp>
#include <libKDL/parser/sema/type_descriptor/parser.hpp>
#include <libKDL/parser/sema/decorator/parser.hpp>
#include <libKDL/parser/sema/list/parser.hpp>
#include <libKDL/exception/unexpected_lexeme_exception.hpp>
#include <libKDL/exception/unrecognised_binary_template_field_exception.hpp>
#include <libKDL/parser/sema/interpreter/expression/parser.hpp>

// MARK: - Parsing

auto kdl::sema::type_definition::value_reference::parse(
    lexer::lexeme_stream& stream,
    const session::binary_template& tmpl,
    session::interpreter::scope& scope,
    session::session *session
)
    -> session::type_field_value
{
    // Decorators
    std::vector<session::decorator::instance> decorators;
    while (stream.expect({ lexer::expectation(lexer::lexeme::directive).be_true() })) {
        decorators.emplace_back(decorator::parse(stream, scope));
    }

    // Value name
    if (!stream.expect({ lexer::expectation(lexer::lexeme::identifier).be_true() })) {
        throw unexpected_lexeme_exception("A field value must be an identifier matching one of the binary template fields.", stream.peek());
    }
    const auto& base_name = stream.read();

    // Create the basic structure and apply the decorators.
    session::type_field_value value(base_name);
    value.set_decorators(decorators);

    // Check for any name extensions
    if (stream.expect({ lexer::expectation(lexer::lexeme::l_angle).be_true() })) {
        list::parser list(stream);
        list.set_list_start(lexer::lexeme::l_angle);
        list.set_list_end(lexer::lexeme::r_angle);
        list.set_delimiter(lexer::lexeme::comma);
        list.add_valid_list_item(lexer::lexeme::var);
        value.set_name_extensions(list.parse(scope, false));
    }

    // Validate that the field value exists in the binary template. If we have name extensions, then we need to check
    // each field in the template and find each of the fields that could potentially match.
    enum session::binary_template::data_type binary_type = session::binary_template::DWRD;
    lexer::lexeme nested_type_name(0);
    if (value.has_name_extensions()) {
        std::vector<const session::binary_template::field *> candidates;
        for (const auto& bin_field : tmpl.fields()) {
            const auto& label = bin_field.label.text();
            if (label.starts_with(base_name.text())) {
                candidates.emplace_back(&bin_field);
            }
        }
        if (candidates.empty()) {
            throw unrecognised_binary_template_field_exception("Field name does not match any binary template fields.", base_name);
        }

        // We have candidates. Ensure that they _all_ have the same type.
        binary_type = candidates.front()->type;
        nested_type_name = candidates.front()->nested_type.name;
        for (const auto *candidate : candidates) {
            if (binary_type != candidate->type && nested_type_name.is(candidate->nested_type.name)) {
                throw unrecognised_binary_template_field_exception("Matching binary template fields have differing types.", base_name);
            }
        }
    }
    else if (tmpl.has_field(base_name)) {
        binary_type = tmpl.at(tmpl.index(base_name)).type;
        nested_type_name = tmpl.at(tmpl.index(base_name)).nested_type.name;
    }
    else {
        throw unrecognised_binary_template_field_exception("Field name does not match any binary template fields.", base_name);
    }

    // Check for an explicit type. If no explicit type exists, then infer the type from the Binary Template
    // field type.
    if (stream.expect({ lexer::expectation(lexer::lexeme::identifier, "as").be_true() })) {
        stream.advance();

        auto explicit_type = type_descriptor::parse(stream, scope);
        explicit_type.verify_binary_type(binary_type, nested_type_name);
        value.set_explicit_type(explicit_type);
    }
    else {
        // Infer...
        value.set_explicit_type(infer_type(stream, binary_type, nested_type_name, scope));
    }

    // Check for a default value
    if (stream.expect({ lexer::expectation(lexer::lexeme::equals).be_true() })) {
        stream.advance();

        // Compile the expression that produces a default value. This expression should be executed later by
        // default so any variables can be evaluated by the applicable scope.
        // If the value has been decorated by '@constexpr' then it should be evaluated now.
        auto default_value_expr = sema::interpreter::expression::compile(stream);

        if (value.decorators().has_decorator_named("constexpr")) {
            auto default_value_scope = scope.shadow();
            value.explicit_type().inject(default_value_scope, session);
            auto result = default_value_expr.evaluate(default_value_scope);
            default_value_expr = session::interpreter::expression(lexer::lexeme_stream({ result }));
        }

        value.set_default_value(default_value_expr);
    }

    // Check for symbols
    if (stream.expect({ lexer::expectation(lexer::lexeme::l_bracket).be_true() })) {
        const auto symbols = symbol_list::parse(stream, scope);
        for (const auto& [name, symbol_value] : symbols) {
            value.add_symbol(name, symbol_value);
        }
    }

    return value;
}

// MARK: - Type Inference

auto kdl::sema::type_definition::value_reference::infer_type(
    lexer::lexeme_stream &stream,
    session::binary_template::data_type binary_type,
    const lexer::lexeme& nested_type_name,
    session::interpreter::scope &scope
)
    -> session::type_descriptor
{
    switch (binary_type) {
        case session::binary_template::NESTED: {
            return { false, nested_type_name };
        }
        case session::binary_template::DBYT:
        case session::binary_template::DWRD:
        case session::binary_template::DLNG:
        case session::binary_template::DQAD:
        case session::binary_template::HBYT:
        case session::binary_template::HWRD:
        case session::binary_template::HLNG:
        case session::binary_template::HQAD: {
            return { false, lexer::lexeme(session::type_descriptor::built_in_type_names::integer, lexer::lexeme::identifier) };
        }
        case session::binary_template::CSTR:
        case session::binary_template::PSTR:
        case session::binary_template::Cnnn: {
            return { false, lexer::lexeme(session::type_descriptor::built_in_type_names::string, lexer::lexeme::identifier) };
        }
        case session::binary_template::RSRC: {
            return { true, nested_type_name };
        }
        default: {
            throw std::runtime_error("");
        }
    }
}