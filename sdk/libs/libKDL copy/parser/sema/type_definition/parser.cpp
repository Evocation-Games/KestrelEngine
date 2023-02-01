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

#include <libKDL/parser/sema/decorator/parser.hpp>
#include <libKDL/parser/sema/type_definition/parser.hpp>
#include <libKDL/parser/sema/type_definition/binary_template/parser.hpp>
#include <libKDL/parser/sema/type_definition/field_definition/parser.hpp>
#include <libKDL/parser/sema/type_definition/constructor/parser.hpp>
#include <libKDL/exception/unexpected_lexeme_exception.hpp>

// MARK: - Parsing

auto kdl::sema::type_definition::parse(lexer::lexeme_stream& stream, session::session *session, session::interpreter::scope& scope) -> session::type_definition
{
    stream.ensure({ lexer::expectation(lexer::lexeme::identifier, "type").be_true() });

    // Type Name
    if (!stream.expect({ lexer::expectation(lexer::lexeme::identifier).be_true() })) {
        throw unexpected_lexeme_exception("Type name must be an identifier", stream.peek());
    }
    auto name = stream.read();
    stream.ensure({ lexer::expectation(lexer::lexeme::colon).be_true() });

    // Type Code
    if (!stream.expect({ lexer::expectation(lexer::lexeme::string).be_true() })) {
        throw unexpected_lexeme_exception("Type code must be an identifier", stream.peek());
    }
    auto code = stream.read();

    // Type Definition Body
    session::type_definition type(name.text(), code.text());

    std::vector<session::decorator::instance> decorators;
    stream.ensure({ lexer::expectation(lexer::lexeme::l_brace).be_true() });
    while (stream.expect({ lexer::expectation(lexer::lexeme::r_brace).be_false() })) {
        if (stream.expect({ lexer::expectation(lexer::lexeme::directive).be_true() })) {
            decorators.emplace_back(sema::decorator::parse(stream, scope));

            // We need to short circuit the current iteration, as decorators are not followed by ';'
            continue;
        }
        else if (stream.expect({ lexer::expectation(lexer::lexeme::identifier, "template").be_true() })) {
            type.set_binary_template(binary_template::parse(stream, type, session));
            decorators.clear();
        }
        else if (stream.expect({ lexer::expectation(lexer::lexeme::identifier, "constructor").be_true() })) {
            auto constructor = sema::type_definition::constructor::parse(stream, scope);
            type.add_constructor(constructor);
            decorators.clear();
        }
        else if (stream.expect({ lexer::expectation(lexer::lexeme::identifier, "field").be_true() })) {
            auto field = field_definition::parse(stream, type.binary_template(), scope, session);
            field.set_decorators(decorators);
            type.add_field(field);
            decorators.clear();
        }
        else {
            throw unexpected_lexeme_exception("Unexpected lexeme found in type definition", stream.peek());
        }

        stream.ensure({ lexer::expectation(lexer::lexeme::semi).be_true() });
    }
    stream.ensure({ lexer::expectation(lexer::lexeme::r_brace).be_true() });
    return type;
}
