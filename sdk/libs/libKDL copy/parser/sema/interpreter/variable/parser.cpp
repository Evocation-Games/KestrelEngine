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
#include <libKDL/parser/sema/interpreter/variable/parser.hpp>

auto kdl::sema::interpreter::variable::resolve(lexer::lexeme_stream &stream, const session::interpreter::scope &scope) -> lexer::lexeme
{
    if (!stream.expect_any({
        lexer::expectation(lexer::lexeme::var).be_true(),
        lexer::expectation(lexer::lexeme::identifier).be_true()
    })) {
        throw unexpected_lexeme_exception("Variable name must be a variable lexeme type.", stream.peek());
    }
    auto var_name = stream.read();
    return scope.variable(var_name);
}
