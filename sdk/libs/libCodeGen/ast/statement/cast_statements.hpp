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
#include <libCodeGen/ast/statement/statement.hpp>
#include <libCodeGen/languages/procedural/type/system.hpp>

namespace codegen::ast
{
    template<language::statement_support L>
    struct cast_statement : public statement<L>
    {
        explicit cast_statement(const std::shared_ptr<ast::statement<L>>& value, const typename language::procedural::type_system<L>::type_id& type)
            : m_value(value), m_type(type)
        {}

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            return L::static_cast_operator(m_type->name(), m_value->emit());
        }

    private:
        std::shared_ptr<ast::statement<L>> m_value;
        typename language::procedural::type_system<L>::type_id m_type;
    };
}