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

#include <libKDL/session/interpreter/expression.hpp>
#include <libKDL/lexer/lexeme_stream.hpp>
#include <libKDL/parser/sema/interpreter/argument/parser.hpp>
#include <libKDL/parser/sema/interpreter/function/parser.hpp>

auto kdl::sema::interpreter::argument::parse(lexer::lexeme_stream &stream, session::interpreter::scope &scope) -> lexer::lexeme
{
    std::vector<lexer::lexeme> expression_lexemes;

    while (!stream.expect_any({ lexer::expectation(lexer::lexeme::comma).be_true(), lexer::expectation(lexer::lexeme::r_paren).be_true() })) {
        if (stream.expect({ lexer::expectation(lexer::lexeme::identifier).be_true(), lexer::expectation(lexer::lexeme::l_paren).be_true() })) {
            auto result = function::call(stream, scope);
            expression_lexemes.emplace_back(result);
        }
        else {
            expression_lexemes.emplace_back(stream.read());
        }
    }

    lexer::lexeme_stream expression(expression_lexemes);
    return session::interpreter::expression::evaluate(expression, scope);
}
