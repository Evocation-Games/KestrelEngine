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

#include <vector>
#include <unordered_map>
#include <libKDL/lexer/lexeme.hpp>
#include <libKDL/lexer/lexeme_stream.hpp>
#include <libKDL/session/interpreter/expression.hpp>
#include <libKDL/session/interpreter/scope.hpp>

namespace kdl::session
{
    struct type_constructor
    {
    public:
        explicit type_constructor(const std::vector<lexer::lexeme>& arguments);

        [[nodiscard]] auto has_arguments() const -> bool;
        [[nodiscard]] auto argument_count() const -> std::size_t;

        [[nodiscard]] auto argument_at(std::int32_t idx) const -> lexer::lexeme;
        [[nodiscard]] auto arguments() const -> std::vector<lexer::lexeme>;

        [[nodiscard]] auto expressions() const -> const std::vector<interpreter::expression>&;
        auto add_expression(const interpreter::expression& expr) -> void;

        auto execute(interpreter::scope& scope, const std::vector<lexer::lexeme>& arguments) const -> void;

    private:
        std::vector<lexer::lexeme> m_arguments;
        std::vector<interpreter::expression> m_expressions;
    };
}