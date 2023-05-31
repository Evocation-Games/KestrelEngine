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
    class incompatible_conversion_exception : public std::exception
    {
    public:
        explicit incompatible_conversion_exception(std::string reason)
            : m_reason(std::move(reason)), m_input("none", lexer::lexeme_type::any), m_output("none", lexer::lexeme_type::any)
        {}

        incompatible_conversion_exception(std::string reason, lexer::lexeme input, lexer::lexeme output)
            : m_reason(std::move(reason)), m_input(std::move(input)), m_output(std::move(output))
        {};

        [[nodiscard]] auto reason() const -> std::string
        {
            return m_reason;
        }

        [[nodiscard]] auto input_type() const -> lexer::lexeme
        {
            return m_input;
        }

        [[nodiscard]] auto output_type() const -> lexer::lexeme
        {
            return m_output;
        }

    private:
        std::string m_reason;
        lexer::lexeme m_input;
        lexer::lexeme m_output;
    };
}