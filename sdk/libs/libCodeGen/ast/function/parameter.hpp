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
#include <libCodeGen/languages/procedural/type/system.hpp>

namespace codegen::ast
{
    template<language::function_support L>
    struct parameter : public node
    {
        parameter(const std::shared_ptr<ast::symbol<L>>& name, const typename language::procedural::type_system<L>::type_id& type)
            : node(), m_name(name), m_type(type)
        {}

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            return L::parameter(m_name, m_type->name());
        }

        [[nodiscard]] auto name() const -> std::shared_ptr<ast::symbol<L>> { return m_name; }
        [[nodiscard]] auto type() const -> typename language::procedural::type_system<L>::type_id { return m_type; }

    private:
        std::shared_ptr<ast::symbol<L>> m_name;
        typename language::procedural::type_system<L>::type_id m_type;
    };

    template<language::function_support L>
    struct parameter_list : public node
    {
        parameter_list() = default;

        auto add_parameter(const std::shared_ptr<parameter<L>>& parameter) -> void
        {
            m_parameters.emplace_back(parameter);
        }

        auto add_parameter(const std::string& name, const typename language::procedural::type_system<L>::type_id& type) -> void
        {
            add_parameter(
                std::shared_ptr<ast::symbol<L>>(name),
                type
            );
        }

        auto add_parameter(const std::shared_ptr<ast::symbol<L>>& name, const typename language::procedural::type_system<L>::type_id& type) -> void
        {
            add_parameter(std::make_shared<parameter<L>>(name, type));
        }

        [[nodiscard]] auto parameters() const -> std::vector<std::shared_ptr<ast::node>>
        {
            return m_parameters;
        }

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            std::vector<emit::segment> out;
            out.reserve(m_parameters.size());
            for (const auto& ptr : m_parameters) {
                out.emplace_back(ptr->emit());
            }
            return L::parameter_list(out);
        }

    private:
        std::vector<std::shared_ptr<parameter<L>>> m_parameters;
    };
}