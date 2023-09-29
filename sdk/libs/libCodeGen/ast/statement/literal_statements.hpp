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

#include <libCodeGen/ast/literal/literals.hpp>
#include <libCodeGen/ast/statement/statement.hpp>

namespace codegen::ast
{
    template<language::statement_support L>
    struct address_of_statement : public statement<L>
    {
        explicit address_of_statement(const std::shared_ptr<node>& nested)
            : m_nested(nested)
        {}

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            return L::address_of(m_nested);
        }

    private:
        std::shared_ptr<node> m_nested;
    };

    template<language::statement_support L>
    struct string_statement : public statement<L>
    {
        explicit string_statement(const std::string& str) : m_value(str) {}

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            return m_value.emit();
        }

    private:
        ast::string_literal<L> m_value;
    };

    template<language::statement_support L>
    struct integer_statement : public statement<L>
    {
        explicit integer_statement(std::int64_t value) : m_value(value) {}

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            return emit::segment(std::to_string(m_value));
        }

    private:
        std::int64_t m_value;
    };
}