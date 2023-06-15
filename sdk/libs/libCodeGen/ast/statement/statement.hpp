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

#include <string>
#include <vector>
#include <libCodeGen/ast/core/node.hpp>

namespace codegen::ast
{
    template<language::statement_support L>
    struct statement : public node
    {

    };

    template<language::statement_support L>
    struct segment_statement : public statement<L>
    {
        explicit segment_statement(const emit::segment& segment) : m_segment(segment) {}
        explicit segment_statement(const std::vector<emit::segment>& segments) : m_segment(segments) {}

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            return m_segment;
        }

    private:
        emit::segment m_segment;
    };

    template<language::compound_statement_support L>
    struct compound_statement : public statement<L>
    {
        virtual auto add_statement(const std::shared_ptr<ast::statement<L>>& statement) -> void
        {
            m_statements.emplace_back(statement);
        }

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            std::vector<emit::segment> out;
            out.emplace_back(L::begin_compound_statement());
            for (const auto& stmt : m_statements) {
                out.emplace_back(stmt->emit());
                out.emplace_back(L::statement_end_operator_string());
            }
            out.emplace_back(L::end_compound_statement());
            return emit::segment(out);
        }

    private:
        std::vector<std::shared_ptr<statement<L>>> m_statements;
    };

    template<language::statement_support L>
    struct chained_statement : public statement<L>
    {
        explicit chained_statement(const std::string& chain_operator = ".")
            : statement<L>(), m_chain_operator(chain_operator)
        {}

        virtual auto add_statement(const std::shared_ptr<ast::statement<L>>& statement) -> void
        {
            m_statements.emplace_back(statement);
        }

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            std::vector<emit::segment> out;
            for (const auto& stmt : m_statements) {
                if (!out.empty()) {
                    out.emplace_back(m_chain_operator, emit::line_break_mode::before);
                }
                out.emplace_back(stmt->emit());
            }
            return emit::segment(out);
        }

    private:
        std::string m_chain_operator;
        std::vector<std::shared_ptr<statement<L>>> m_statements;
    };

    template<language::statement_support L>
    struct symbol_statement : public statement<L>
    {
        explicit symbol_statement(const std::shared_ptr<ast::symbol<L>>& symbol) : m_symbol(symbol) {}
        explicit symbol_statement(const std::string& symbol) : m_symbol(std::make_shared<ast::symbol<L>>(symbol)) {}

        [[nodiscard]] auto emit() const -> emit::segment override { return m_symbol->emit(); }

    private:
        std::shared_ptr<ast::symbol<L>> m_symbol;
    };
}