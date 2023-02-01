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
#include <libKDL/parser/sema/directive/command_directive.hpp>
#include <libKDL/exception/unexpected_lexeme_exception.hpp>

// MARK: - Base

auto kdl::sema::directive::is_command(lexer::lexeme_stream &stream) -> bool
{
    return command<function::out>::accepts(stream);
}

auto kdl::sema::directive::run(lexer::lexeme_stream &stream, session::session *session, session::interpreter::scope &scope) -> void
{
    if (command<function::out>::accepts(stream)) {
        command<function::out>::run(stream, session, scope);
    }
}

// MARK: - Command

template<const char *K>
auto kdl::sema::directive::command<K>::accepts(lexer::lexeme_stream &stream) -> bool
{
    return stream.expect({ lexer::expectation(lexer::lexeme::directive, std::string(K)).be_true() });
}

template<const char *K>
auto kdl::sema::directive::command<K>::run(lexer::lexeme_stream &stream, session::session *session, session::interpreter::scope &scope) -> void
{
    stream.ensure({lexer::expectation(lexer::lexeme::directive, std::string(K)).be_true()});

    if (K == function::out) {
        while (stream.expect({ lexer::expectation(lexer::lexeme::semi).be_false() })) {
            std::cout << stream.read().text() << std::endl;
        }
    }
}