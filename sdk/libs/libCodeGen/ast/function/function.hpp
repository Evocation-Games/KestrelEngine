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
#include <libCodeGen/ast/core/node.hpp>
#include <libCodeGen/ast/symbol/symbol.hpp>
#include <libCodeGen/ast/function/parameter.hpp>
#include <libCodeGen/ast/statement/statement.hpp>

namespace codegen::ast
{
    template<language::function_support L>
    struct function : public node
    {
        explicit function(const std::shared_ptr<ast::symbol<L>>& name)
            : m_name(name)
        {}

        explicit function(const std::shared_ptr<ast::symbol<L>>& name, const typename language::procedural::type_system<L>::type_id& return_type)
            : m_name(name), m_return(return_type)
        {}

        [[nodiscard]] auto name() const -> std::shared_ptr<ast::symbol<L>> { return m_name; }
        [[nodiscard]] auto return_type() const -> typename language::procedural::type_system<L>::type_id { return m_return; }
        [[nodiscard]] auto statement() const -> std::shared_ptr<ast::statement<L>> { return m_statement; }

        auto set_return_type(const typename language::procedural::type_system<L>::type_id& type) -> void
        {
            m_return = type;
        }

        auto add_parameter(const std::shared_ptr<ast::symbol<L>>& name, const typename language::procedural::type_system<L>::type_id& type = nullptr) -> void
        {
            m_parameters->add_parameter(
                name,
                type ?: language::procedural::error_type<L>()
            );
        }

        auto set_statement(const std::shared_ptr<ast::statement<L>>& statement) -> void
        {
            m_statement = statement;
        }

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            if (m_statement) {
                return {
                    L::function_decl(m_name, m_return->name(), m_parameters),
                    m_statement->emit()
                };
            }
            else {
                return {
                    L::function_decl(m_name, m_return->name(), m_parameters)
                };
            }
        }

    private:
        std::shared_ptr<ast::symbol<L>> m_name;
        typename language::procedural::type_system<L>::type_id m_return { language::procedural::void_type<L>() };
        std::shared_ptr<ast::parameter_list<L>> m_parameters { std::make_shared<ast::parameter_list<L>>() };
        std::shared_ptr<ast::statement<L>> m_statement;
    };

    template<language::function_support L>
    struct function_type : public statement<L>
    {
        function_type(
            const typename language::procedural::type_system<L>::type_id& return_type,
            const std::vector<emit::segment>& parameters
        ) : statement<L>(), m_return(return_type->name()), m_parameters(parameters)
        {}

        function_type(
            const std::string& return_type,
            const std::vector<emit::segment>& parameters
        ) : statement<L>(), m_return(return_type), m_parameters(parameters)
        {}

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            return L::function_type(m_parameters, m_return);
        }

    private:
        std::string m_return;
        std::vector<emit::segment> m_parameters { std::make_shared<ast::parameter_list<L>>() };
    };
}