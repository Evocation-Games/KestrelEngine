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
#include <libKDL/parser/sema/type_definition/value_reference/symbol_list_parser.hpp>

auto kdl::sema::type_definition::value_reference::symbol_list::parse(
    lexer::lexeme_stream &stream,
    session::interpreter::scope &scope
)
    -> std::vector<std::tuple<lexer::lexeme, lexer::lexeme>>
{
    std::vector<std::tuple<lexer::lexeme, lexer::lexeme>> symbols;
    stream.ensure({ lexer::expectation(lexer::lexeme::l_bracket).be_true() });

    while (stream.expect({ lexer::expectation(lexer::lexeme::r_bracket).be_false() })) {
        if (!stream.expect({ lexer::expectation(lexer::lexeme::identifier).be_true() })) {
            throw unexpected_lexeme_exception("Symbol name must be an identifier", stream.peek());
        }
        auto symbol = stream.read();

        stream.ensure({ lexer::expectation(lexer::lexeme::equals).be_true() });

        if (stream.expect_any({
            lexer::expectation(lexer::lexeme::integer).be_true(), lexer::expectation(lexer::lexeme::percentage).be_true(),
            lexer::expectation(lexer::lexeme::reference).be_true(), lexer::expectation(lexer::lexeme::string).be_true()
        })) {
            symbols.emplace_back(symbol, stream.read());
        }
        else if (stream.expect({ lexer::expectation(lexer::lexeme::var).be_true() })) {
            symbols.emplace_back(symbol, scope.resolve_or_passthrough(stream.read()));
        }
        else {
            throw unexpected_lexeme_exception("Symbol value must be a string, integer, resource reference or percentage.");
        }

        if (stream.expect({ lexer::expectation(lexer::lexeme::r_bracket).be_false() })) {
            stream.ensure({ lexer::expectation(lexer::lexeme::comma).be_true() });
        }
    }

    stream.ensure({ lexer::expectation(lexer::lexeme::r_bracket).be_true() });
    return symbols;
}
