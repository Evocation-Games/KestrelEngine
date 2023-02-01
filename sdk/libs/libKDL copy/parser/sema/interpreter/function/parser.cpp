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
#include <libKDL/exception/unrecognised_function_exception.hpp>
#include <libKDL/parser/sema/interpreter/function/parser.hpp>
#include <libKDL/parser/sema/interpreter/argument/parser.hpp>
#include <libKDL/parser/sema/interpreter/variable//parser.hpp>

// MARK: - Function Parsing

auto kdl::sema::interpreter::function::call(lexer::lexeme_stream &stream, session::interpreter::scope& scope) -> lexer::lexeme
{
    std::vector<lexer::lexeme> arguments;
    const auto& function_name = stream.read();

    stream.ensure({ lexer::expectation(lexer::lexeme::l_paren).be_true() });

    // Extract the arguments
    while (stream.expect({ lexer::expectation(lexer::lexeme::r_paren).be_false() })) {
        if (stream.expect({ lexer::expectation(lexer::lexeme::identifier).be_true(), lexer::expectation(lexer::lexeme::l_paren).be_true() })) {
            arguments.emplace_back(call(stream, scope));
        }
        else if (stream.expect_any({ lexer::expectation(lexer::lexeme::var).be_true(), lexer::expectation(lexer::lexeme::identifier).be_true() })) {
            arguments.emplace_back(variable::resolve(stream, scope));
        }
        else {
            arguments.emplace_back(argument::parse(stream, scope));
        }

        if (stream.expect({ lexer::expectation(lexer::lexeme::comma).be_true() })) {
            stream.advance();
            continue;
        }
        else if (stream.expect({ lexer::expectation(lexer::lexeme::r_paren).be_true() })) {
            break;
        }
        else {
            throw unexpected_lexeme_exception("Unexpected lexeme encountered in expression. Expected ',' or ')'.", stream.peek());
        }
    }
    stream.ensure({ lexer::expectation(lexer::lexeme::r_paren).be_true() });

    // Find the function in the scope.
    if (!scope.has_function(function_name, true)) {
        throw unrecognised_function_exception("Unrecognised function called.", function_name);
    }

    return scope.call(function_name, scope, arguments);
}
