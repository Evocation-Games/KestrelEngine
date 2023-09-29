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

#include <libCodeGen/ast/statement/statement.hpp>
#include <libCodeGen/ast/symbol/symbol.hpp>

namespace codegen::ast
{
    template<language::statement_support L>
    struct function_call : public statement<L>
    {
        explicit function_call(
            const std::shared_ptr<ast::symbol<L>>& function_name
        ) : m_function_name(function_name), m_arguments({})
        {}

        explicit function_call(
            const std::shared_ptr<ast::symbol<L>>& function_name,
            const std::vector<std::shared_ptr<ast::statement<L>>>& arguments
        ) : m_function_name(function_name), m_arguments(arguments)
        {}

        virtual auto push_argument(const std::shared_ptr<ast::statement<L>>& arg) -> void
        {
            m_arguments.emplace_back(arg);
        }

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            std::vector<emit::segment> arguments;
            arguments.reserve(m_arguments.size());
            for (const auto& arg : m_arguments) {
                if (!arg) {
                    continue;
                }
                if (!arguments.empty()) {
                    arguments.emplace_back(L::list_delimiter_string());
                }
                arguments.emplace_back(arg->emit());
            }
            return L::function_call(m_function_name, arguments);
        }

        [[nodiscard]] virtual auto name() const -> std::shared_ptr<ast::symbol<L>>
        {
            return m_function_name;
        }

        [[nodiscard]] virtual auto arguments() const -> std::vector<std::shared_ptr<ast::statement<L>>>
        {
            return m_arguments;
        }

    private:
        std::shared_ptr<ast::symbol<L>> m_function_name;
        std::vector<std::shared_ptr<ast::statement<L>>> m_arguments;
    };

    template<language::statement_support L>
    struct member_function_call : public function_call<L>
    {
        member_function_call(
            const std::shared_ptr<ast::symbol<L>>& function_name,
            const std::shared_ptr<ast::symbol<L>>& receiver,
            const std::initializer_list<std::shared_ptr<ast::statement<L>>>& arguments = {}
        ) : function_call<L>(function_name, arguments), m_receiver(receiver)
        {}

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            return {
                m_receiver->emit(),
                emit::segment(L::pointer_member_operator_string()),
                function_call<L>::emit()
            };
        }

    private:
        std::shared_ptr<ast::symbol<L>> m_receiver;
    };

    template<language::statement_support L>
    struct template_specialized_function_call : public function_call<L>
    {
        template_specialized_function_call(
            const std::shared_ptr<ast::symbol<L>>& function_name,
            const std::vector<std::shared_ptr<ast::statement<L>>>& specializations,
            const std::initializer_list<std::shared_ptr<ast::statement<L>>>& arguments = {}
        ) : function_call<L>(function_name, arguments), m_specializations(specializations)
        {}

        [[nodiscard]] auto emit() const -> emit::segment
        {
            const auto& args = function_call<L>::arguments();
            std::vector<emit::segment> arguments;
            arguments.reserve(args.size());
            for (const auto& arg : args) {
                if (!arguments.empty()) {
                    arguments.emplace_back(", ");
                }
                arguments.emplace_back(arg->emit());
            }

            std::vector<emit::segment> specializations;
            specializations.reserve(args.size());
            for (const auto& specialization : m_specializations) {
                if (!specializations.empty()) {
                    specializations.emplace_back(", ");
                }
                specializations.emplace_back(specialization->emit());
            }

            return L::specialized_function_call(function_call<L>::name(), specializations, arguments);
        }

    private:
        std::vector<std::shared_ptr<ast::statement<L>>> m_specializations;
    };
}