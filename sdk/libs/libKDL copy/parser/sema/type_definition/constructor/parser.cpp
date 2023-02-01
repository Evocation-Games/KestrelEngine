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

#include <libKDL/parser/sema/type_definition/constructor/parser.hpp>
#include <libKDL/exception/unexpected_lexeme_exception.hpp>
#include <libKDL/session/interpreter/expression.hpp>
#include <libKDL/parser/sema/interpreter/expression/parser.hpp>

auto kdl::sema::type_definition::constructor::parse(lexer::lexeme_stream& stream, session::interpreter::scope& scope) -> session::type_constructor
{
    stream.ensure({
        lexer::expectation(lexer::lexeme::identifier, "constructor").be_true(),
        lexer::expectation(lexer::lexeme::l_paren).be_true()
    });

    std::vector<lexer::lexeme> arguments;
    while (stream.expect({ lexer::expectation(lexer::lexeme::r_paren).be_false() })) {
        if (stream.expect({ lexer::expectation(lexer::lexeme::identifier).be_true() })) {
            arguments.emplace_back(stream.read());
        }
        else if (stream.expect({ lexer::expectation(lexer::lexeme::comma).be_true() })) {
            stream.advance();
            continue;
        }
        else {
            throw unexpected_lexeme_exception("Unexpected lexeme encountered in constructor");
        }
    }

    stream.ensure({
        lexer::expectation(lexer::lexeme::r_paren).be_true(),
        lexer::expectation(lexer::lexeme::l_brace).be_true()
    });

    // Start building the actual constructor, and adding expressions.
    session::type_constructor constructor(arguments);

    while (stream.expect({ lexer::expectation(lexer::lexeme::r_brace).be_false() })) {
        auto expr = sema::interpreter::expression::compile(stream);
        constructor.add_expression(expr);
        stream.ensure({ lexer::expectation(lexer::lexeme::semi).be_true() });
    }

    stream.ensure({ lexer::expectation(lexer::lexeme::r_brace).be_true() });

    return constructor;
}
