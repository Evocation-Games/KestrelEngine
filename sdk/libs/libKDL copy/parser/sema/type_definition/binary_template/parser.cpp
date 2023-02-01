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

#include <libKDL/exception/unexpected_lexeme_exception.hpp>
#include <libKDL/parser/sema/type_definition/binary_template/parser.hpp>
#include <libKDL/parser/sema/type_definition/binary_field/parser.hpp>

// MARK: - Parsing

auto kdl::sema::type_definition::binary_template::parse(lexer::lexeme_stream& stream, const session::type_definition& type, const session::session *session) -> session::binary_template
{
    session::binary_template tmpl(type.code());

    stream.ensure({
        lexer::expectation(lexer::lexeme::identifier, "template").be_true(),
        lexer::expectation(lexer::lexeme::l_brace).be_true()
    });

    while (stream.expect({ lexer::expectation(lexer::lexeme::r_brace).be_false() })) {
        auto field = binary_field::parse(stream, session);
        stream.ensure({ lexer::expectation(lexer::lexeme::semi).be_true() });

        // We need to handle lists slightly differently.
        if (field.type == session::binary_template::data_type::OCNT) {
            // The next field must be an LSTB typed field, with all subsequent fields being attach to the original
            // OCNT field.
            auto list_start_field = binary_field::parse(stream, session);
            stream.ensure({ lexer::expectation(lexer::lexeme::semi).be_true() });

            if (list_start_field.type != session::binary_template::data_type::LSTC) {
                throw unexpected_lexeme_exception("First template field after an OCNT field should be LSTC");
            }

            do {
                auto list_field = binary_field::parse(stream, session);
                stream.ensure({ lexer::expectation(lexer::lexeme::semi).be_true() });

                if (list_field.type == session::binary_template::data_type::LSTE) {
                    break;
                }
                else {
                    field.fields.emplace_back(list_field);
                }
            } while(true);
        }

        tmpl.add_field(field);
    }

    stream.ensure({
        lexer::expectation(lexer::lexeme::r_brace).be_true()
    });

    return tmpl;
}
