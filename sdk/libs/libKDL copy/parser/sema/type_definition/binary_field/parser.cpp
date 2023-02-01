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
#include <libKDL/session/types/binary_template/binary_template.hpp>
#include <libKDL/parser/sema/type_definition/binary_field/parser.hpp>
#include <libKDL/session/session.hpp>

// MARK: - Parsing

auto kdl::sema::type_definition::binary_field::parse(lexer::lexeme_stream &stream, const session::session *session) -> session::binary_template::field
{
    if (!stream.expect({ lexer::expectation(lexer::lexeme::identifier).be_true() })) {
        throw unexpected_lexeme_exception("Type name for a binary field must be an identifier.", stream.peek());
    }
    auto type = session::binary_template::data_type_for_name(stream.read().text());

    // Nested types need some special processing.
    lexer::lexeme nested(0);
    const session::type_definition *nested_definition = nullptr;
    if (type == session::binary_template::data_type::NESTED) {
        stream.ensure({ lexer::expectation(lexer::lexeme::l_angle).be_true() });

        if (!stream.expect({ lexer::expectation(lexer::lexeme::identifier).be_true() })) {
            throw unexpected_lexeme_exception("Nested type name for a binary field must be an identifier.", stream.peek());
        }
        nested = stream.read();
        if (!session->has_type_definition(nested.text())) {
            throw std::runtime_error("");
        }
        nested_definition = session->type_definition(nested.text());

        stream.ensure({ lexer::expectation(lexer::lexeme::r_angle).be_true() });
    }

    if (!stream.expect({ lexer::expectation(lexer::lexeme::identifier).be_true() })) {
        throw unexpected_lexeme_exception("Label for a binary field must be an identifier.", stream.peek());
    }

    return { stream.read(), type, nested, nested_definition };
}