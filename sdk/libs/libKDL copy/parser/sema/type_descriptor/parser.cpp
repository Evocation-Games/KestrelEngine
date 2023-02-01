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

#include <libKDL/parser/sema/type_descriptor/parser.hpp>
#include <libKDL/parser/sema/list/parser.hpp>

auto kdl::sema::type_descriptor::parse(lexer::lexeme_stream& stream, session::interpreter::scope& scope) -> session::type_descriptor
{
    session::type_descriptor descriptor;

    // Check for an explicit type name.
    if (stream.expect({ lexer::expectation(lexer::lexeme::identifier).be_true() })) {
        descriptor.set_name(stream.read());

        // Check for an associated value / type hints
        if (stream.expect({ lexer::expectation(lexer::lexeme::l_angle).be_true() })) {
            list::parser list(stream);
            list.set_list_start(lexer::lexeme::l_angle);
            list.set_list_end(lexer::lexeme::r_angle);
            list.set_delimiter(lexer::lexeme::comma);
            list.add_valid_list_item(lexer::lexeme::integer);
            list.add_valid_list_item(lexer::lexeme::percentage);
            list.add_valid_list_item(lexer::lexeme::reference);
            list.add_valid_list_item(lexer::lexeme::identifier);
            list.add_valid_list_item(lexer::lexeme::string);
            list.add_valid_list_item(lexer::lexeme::var);
            descriptor.set_hints(list.parse(scope, false));
        }
    }

    // Check for a reference type.
    if (stream.expect({ lexer::expectation(lexer::lexeme::amp).be_true() })) {
        stream.advance();
        descriptor.set_reference(true);
    }

    return descriptor;
}
