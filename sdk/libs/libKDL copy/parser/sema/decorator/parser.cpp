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

#include <iostream>
#include <libKDL/parser/sema/decorator/parser.hpp>
#include <libKDL/parser/sema/list/parser.hpp>
#include <libKDL/exception/unexpected_lexeme_exception.hpp>

auto kdl::sema::decorator::parse(lexer::lexeme_stream &stream, session::interpreter::scope &scope) -> session::decorator::instance
{
    if (!stream.expect({ lexer::expectation(lexer::lexeme::directive).be_true() })) {
        throw unexpected_lexeme_exception("A decorator must be denoted by a '@'", stream.peek());
    }
    const auto& name = stream.read();

    std::vector<lexer::lexeme> values;
    if (stream.expect({ lexer::expectation(lexer::lexeme::l_paren).be_true() })) {
        list::parser list(stream);
        list.set_list_start(lexer::lexeme::l_paren);
        list.set_list_end(lexer::lexeme::r_paren);
        list.set_delimiter(lexer::lexeme::comma);
        list.add_valid_list_item(lexer::lexeme::integer);
        list.add_valid_list_item(lexer::lexeme::percentage);
        list.add_valid_list_item(lexer::lexeme::reference);
        list.add_valid_list_item(lexer::lexeme::identifier);
        list.add_valid_list_item(lexer::lexeme::string);
        values = list.parse(scope, false);
    }

    return session::decorator::instance(name, values);
}
