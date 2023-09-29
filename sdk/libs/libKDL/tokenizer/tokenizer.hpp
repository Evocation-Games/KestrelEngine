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

#pragma once

#include <libLexer/lexeme.hpp>
#include <libFoundation/stream/stream.hpp>
#include <libKDL/tokenizer/token.hpp>

namespace kdl::tokenizer
{
    struct tokenizer
    {
    public:
        tokenizer(const foundation::stream<lexer::lexeme>& input);

        auto process() -> foundation::stream<token>;

    private:
        foundation::stream<lexer::lexeme> m_input;

        static auto directive_named(const lexer::lexeme& name) -> token_type;
        static auto keyword_named(const lexer::lexeme& name) -> token_type;
        static auto binary_type_named(const lexer::lexeme& name) -> token_type;
        static auto type_named(const lexer::lexeme& name) -> token_type;
        static auto decorator_named(const lexer::lexeme& name) -> token_type;
    };
}