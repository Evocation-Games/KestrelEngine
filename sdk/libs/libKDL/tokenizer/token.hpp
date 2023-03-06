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
            : m_source(lx), m_value(values), m_type(type)
        {}

        // MARK: - Look Ups
        [[nodiscard]] auto source() const -> const lexer::lexeme&
        {
            return m_source;
        }

        [[nodiscard]] auto string_value() const -> std::string
        {
            if (m_value.index() == value_lut::string) {
                return std::get<std::string>(m_value);
            }
            else if (m_value.index() == value_lut::values) {
                return std::get<std::vector<lexer::lexeme>>(m_value).back().text();
            }
            throw std::runtime_error("");
        }

        [[nodiscard]] auto path_value(const std::string& delimiter = "/") const -> std::string
        {
            if (m_value.index() == value_lut::string) {
                return std::get<std::string>(m_value);
            }
            else if (m_value.index() == value_lut::values) {
                std::string out;
                for (const auto& lx : std::get<std::vector<lexer::lexeme>>(m_value)) {
                    if (!out.empty()) {
                        out += delimiter;
                    }
                    out += lx.text();
                }
                return out;
            }
            throw std::runtime_error("");
        }

        [[nodiscard]] auto associated_values() const -> std::vector<std::string>
        {
            std::vector<lexer::lexeme> associated_values;
            std::vector<std::string> out;

            if (m_value.index() == value_lut::values) {
                associated_values = std::get<std::vector<lexer::lexeme>>(m_value);
            }

            for (const auto& lx : associated_values) {
                out.emplace_back(lx.text());
            }

            return out;
        }

        [[nodiscard]] auto reference_value() const -> resource::reference
        {
            if (m_type != token_type::reference || m_value.index() != value_lut::reference) {
                throw std::runtime_error("");
            }
            return std::get<resource::reference>(m_value);
        }

        // MARK: - Expectations
        [[nodiscard]] auto is(enum token_type type) const -> bool
        {
            return m_type == type;
        }

        [[nodiscard]] auto is(const std::string& value) const -> bool
        {
            return string_value() == value;
        }

        [[nodiscard]] auto is(const std::vector<std::string>& options) const -> bool
        {
            for (const auto& str : options) {
                if (string_value() == str) {
                    return true;
                }
            }
            return false;
        }

        [[nodiscard]] auto is(const enum token_type type, const std::string& value) const -> bool
        {
            return is(type) && is(value);
        }

        [[nodiscard]] auto is(const enum token_type type, const std::vector<std::string>& options) const -> bool
        {
            return is(type) && is(options);
        }

        template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>
        auto value() const -> T
        {
            if (m_type == tokenizer::reference && m_value.index() == value_lut::reference) {
                return std::get<resource::reference>(m_value).id();
            }
            else {
                return m_source.value<T>();
            }
        }

    private:
        enum value_lut { string, reference, values };

        token_type m_type;
        lexer::lexeme m_source { 0 };
        std::variant<std::string, resource::reference, std::vector<lexer::lexeme>> m_value;
    };
}