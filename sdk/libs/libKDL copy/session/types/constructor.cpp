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
#include <libKDL/session/types/constructor.hpp>

// MARK: - Construction

kdl::session::type_constructor::type_constructor(const std::vector<lexer::lexeme> &arguments)
    : m_arguments(arguments)
{}

// MARK: - Argument Management

auto kdl::session::type_constructor::has_arguments() const -> bool
{
    return !m_arguments.empty();
}

auto kdl::session::type_constructor::arguments() const -> std::vector<lexer::lexeme>
{
    return m_arguments;
}

auto kdl::session::type_constructor::argument_count() const -> std::size_t
{
    return m_arguments.size();
}

auto kdl::session::type_constructor::argument_at(std::int32_t idx) const -> lexer::lexeme
{
    return m_arguments.at(idx);
}

// MARK: - Expression Management

auto kdl::session::type_constructor::expressions() const -> const std::vector<interpreter::expression>&
{
    return m_expressions;
}

auto kdl::session::type_constructor::add_expression(const interpreter::expression &expr) -> void
{
    m_expressions.emplace_back(expr);
}

auto kdl::session::type_constructor::execute(interpreter::scope &scope, const std::vector<lexer::lexeme>& arguments) const -> void
{
    if (arguments.size() != argument_count()) {
        throw std::runtime_error("");
    }

    auto shadow_scope = scope.shadow();
    for (std::int32_t i = 0; i < argument_count(); ++i) {
        shadow_scope.add_variable(argument_at(i), arguments.at(i));
    }

    for (auto& expr : m_expressions) {
        expr.evaluate(shadow_scope);
    }
}