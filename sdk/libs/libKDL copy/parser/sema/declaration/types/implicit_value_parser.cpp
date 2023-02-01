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

#include <libKDL/parser/sema/declaration/types/implicit_value_parser.hpp>
#include <libKDL/parser/sema/interpreter/expression/parser.hpp>

auto kdl::sema::declaration::implicit_type::parse(
    lexer::lexeme_stream &stream,
    session::interpreter::scope &scope,
    const session::type_field_value &field,
    std::vector<const session::binary_template::field *>& binary_fields
)
    -> lexer::lexeme
{
    // Build a shadow scope, that includes all symbols as variables.
    auto shadow_scope = scope.shadow();
    for (auto n = 0; n <= field.joined_values_count(); ++n) {
        auto& value = (n == 0) ? field : field.joined_value_at(n - 1);
        for (const auto& symbol : value.symbols()) {
            const auto& symbol_value = value.value_for_symbol(symbol);
            shadow_scope.add_variable(symbol, symbol_value);
        }
    }

    // Parse out an expression, and then evaluate it.
    auto value_expr = interpreter::expression::compile(stream);
    auto result = value_expr.evaluate(shadow_scope);

    // Ensure that all the binary fields have the same type.
    auto binary_type = binary_fields.back()->type;
    for (const auto& bin : binary_fields) {
        if (binary_type != bin->type) {
            throw std::runtime_error("");
        }
    }

    // Does the result type and binary type match?
    switch (result.type()) {
        case lexer::lexeme::integer:
        case lexer::lexeme::percentage: {
            switch (binary_type & ~0xFFFUL) {
                case session::binary_template::data_type::DBYT:
                case session::binary_template::data_type::DWRD:
                case session::binary_template::data_type::DLNG:
                case session::binary_template::data_type::DQAD:
                case session::binary_template::data_type::HBYT:
                case session::binary_template::data_type::HWRD:
                case session::binary_template::data_type::HLNG:
                case session::binary_template::data_type::HQAD: {

                }
                default: {
                    throw std::runtime_error("");
                }
            }
        }

        case lexer::lexeme::identifier: {
            switch (binary_type & ~0xFFFUL) {
                case session::binary_template::data_type::RSRC:
                case session::binary_template::data_type::DBYT:
                case session::binary_template::data_type::DWRD:
                case session::binary_template::data_type::DLNG:
                case session::binary_template::data_type::DQAD: {

                }
                default: {
                    throw std::runtime_error("");
                }
            }
        }
        case lexer::lexeme::string: {
            switch (binary_type & ~0xFFFUL) {
                case session::binary_template::data_type::CSTR:
                case session::binary_template::data_type::Cnnn:
                case session::binary_template::data_type::PSTR: {

                }
                default: {
                    throw std::runtime_error("");
                }
            }
        }
        default: {
            // The result type is completely invalid!
            throw std::runtime_error("");
        }
    }
}
