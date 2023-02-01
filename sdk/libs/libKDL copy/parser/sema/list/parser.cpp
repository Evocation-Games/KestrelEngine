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

#include <libKDL/parser/sema/list/parser.hpp>
#include <libKDL/exception/unexpected_lexeme_exception.hpp>
#include <libKDL/parser/sema/interpreter/function/parser.hpp>
#include <libKDL/parser/sema/interpreter/variable/parser.hpp>
#include <libKDL/parser/sema/interpreter/expression/parser.hpp>
#include <libKDL/session/interpreter/expression.hpp>

// MARK: - Construction

kdl::sema::list::parser::parser(lexer::lexeme_stream &stream)
    : m_stream(stream)
{}

// MARK: - Configuration

auto kdl::sema::list::parser::set_list_start(enum lexer::lexeme::type lx) -> void
{
    m_list_start = lx;
}

auto kdl::sema::list::parser::set_list_end(enum lexer::lexeme::type lx) -> void
{
    m_list_end = lx;
}

auto kdl::sema::list::parser::set_delimiter(enum lexer::lexeme::type lx) -> void
{
    m_delimit = lx;
}

auto kdl::sema::list::parser::add_valid_list_item(enum lexer::lexeme::type lx) -> void
{
    m_valid_lexemes.emplace_back(lx, std::make_optional<std::string>());
}

auto kdl::sema::list::parser::add_valid_list_item(enum lexer::lexeme::type lx, const std::string &text) -> void
{
    m_valid_lexemes.emplace_back(lx, text);
}

// MARK: - Parser

auto kdl::sema::list::parser::parse(session::interpreter::scope& scope, bool resolve_vars) -> std::vector<lexer::lexeme>
{
    // Build a list of valid expectations
    std::vector<lexer::expectation::function> expectations;
    for (const auto& [type, text] : m_valid_lexemes) {
        if (text.has_value()) {
            expectations.emplace_back(lexer::expectation(type, text.value()).be_true());
        }
        else {
            expectations.emplace_back(lexer::expectation(type).be_true());
        }
    }

    // Introduce a basic expression expectation in the list.
    std::vector<lexer::lexeme> out;
    m_stream.ensure({ lexer::expectation(m_list_start).be_true() });
    while (m_stream.expect({ lexer::expectation(m_list_end).be_false() })) {
        if (m_stream.expect({
            lexer::expectation(lexer::lexeme::identifier).be_true(),
            lexer::expectation(lexer::lexeme::l_paren).be_true()
        })) {
            m_stream.push(interpreter::function::call(m_stream, scope));
        }
        else if (m_stream.expect({ lexer::expectation(lexer::lexeme::var).be_true() })) {
            if (resolve_vars) {
                m_stream.push(interpreter::variable::resolve(m_stream, scope));
            }
            else {
                m_stream.push(m_stream.read());
            }
        }

        if (!m_stream.expect_any(expectations)) {
            throw unexpected_lexeme_exception("Unexpected lexeme '" + m_stream.peek().text() + "' encountered in list.");
        }

        if (m_stream.peek().is(lexer::lexeme::l_expr)) {
            auto expr = interpreter::expression::compile(m_stream);
            out.emplace_back(expr.evaluate(scope));
        }
        else {
            out.emplace_back(m_stream.read());
        }

        if (m_stream.expect({ lexer::expectation(m_list_end).be_false() })) {
            m_stream.ensure({ lexer::expectation(m_delimit).be_true() });
        }
    }

    m_stream.ensure({ lexer::expectation(m_list_end).be_true() });
    return out;
}