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

#include <variant>
#include <libLexer/lexeme.hpp>
#include <libKDL/tokenizer/token_type.hpp>
#include <libResource/reference.hpp>

namespace kdl::tokenizer
{
    struct token
    {
    public:
        token(token_type type = error) : m_type(type) {}

        token(const lexer::lexeme& lx, token_type type)
            : m_source(lx), m_value(lx.text()), m_type(type)
        {}

        token(const lexer::lexeme& lx, const resource::reference& ref)
            : m_source(lx), m_value(ref), m_type(token_type::reference)
        {}

        token(const lexer::lexeme& lx, const std::vector<lexer::lexeme>& values, token_type type)
            : m_source(lx), m_value(values), m_type(token_type::reference)
        {}

        // MARK: - Look Ups
        [[nodiscard]] auto string_value() const -> std::string
        {
            if (m_value.index() == value_lut::string) {
                return std::get<std::string>(m_value);
            }
            throw std::runtime_error("");
        }

        [[nodiscard]] auto reference_value() const -> resource::reference
        {
            if (m_type != token_type::reference || m_value.index() != value_lut::reference) {
                throw std::runtime_error("");
            }
            return std::get<resource::reference>(m_value);
        }

    private:
        enum value_lut { string, reference, values };

        token_type m_type;
        lexer::lexeme m_source { 0 };
        std::variant<std::string, resource::reference, std::vector<lexer::lexeme>> m_value;
    };
}