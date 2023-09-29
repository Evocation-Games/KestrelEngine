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

#include <stdexcept>
#include <utility>
#include <libLexer/lexeme.hpp>

namespace kdl
{
    class unrecognised_function_exception : public std::exception
    {
    public:
        explicit unrecognised_function_exception(std::string reason)
            : m_reason(std::move(reason)), m_lexeme("none", lexer::lexeme_type::any)
        {}

        unrecognised_function_exception(std::string reason, lexer::lexeme lx)
            : m_reason(std::move(reason)), m_lexeme(std::move(lx))
        {};

        [[nodiscard]] auto reason() const -> std::string
        {
            return m_reason;
        }

        [[nodiscard]] auto lexeme() const -> lexer::lexeme
        {
            return m_lexeme;
        }

    private:
        std::string m_reason;
        lexer::lexeme m_lexeme;
    };
}