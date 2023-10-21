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
#include <sstream>
#include <vector>
#include <libCodeGen/ast/core/node.hpp>
#include <libCodeGen/ast/symbol/symbol.hpp>
#include <libCodeGen/ast/comment/comment.hpp>
#include <libCodeGen/ast/statement/assignment_statement.hpp>
#include <libCodeGen/ast/literal/resource_reference.hpp>

namespace codegen::ast
{
    template<language::resource_declaration_support L>
    struct resource_instance : public node
    {
    public:
        resource_instance(std::int64_t id, const std::string& name)
            : m_name(name),
              m_id(id)
        {}

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            std::vector<emit::segment> out;
            out.reserve(m_nodes.size());
            for (const auto& node : m_nodes) {
                out.emplace_back(node->emit());
                out.emplace_back(emit::segment(L::statement_end_operator_string()));
                out.emplace_back(ast::nl<L>().emit());
            }

            return {
                L::new_resource(),
                emit::segment("("),
                L::global_reference(m_id),
                emit::segment(", "),
                emit::segment("\"" + m_name + "\""),
                emit::segment(")"),
                L::begin_compound_statement(),
                emit::segment(out),
                L::end_compound_statement()
            };
        }

        auto comment(const std::string& str) -> void
        {
            m_nodes.emplace_back(std::make_shared<ast::comment<L>>(str));
        }

        auto set(const std::string& field, const std::shared_ptr<ast::node>& value) -> void
        {
            auto assign = std::make_shared<ast::assignment_statement<L>>(
                std::make_shared<segment_statement<L>>(std::make_shared<ast::symbol<L>>(field)->emit(), false),
                std::make_shared<segment_statement<L>>(value->emit(), false)
            );
            m_nodes.emplace_back(assign);
        }

        [[nodiscard]] auto ref(const std::string& container, const std::string& type, std::int64_t id) -> std::shared_ptr<ast::node>
        {
            return std::make_shared<resource_reference<L>>(container, type, id);
        }

        [[nodiscard]] auto ref(const std::string& type, std::int64_t id) -> std::shared_ptr<ast::node>
        {
            return std::make_shared<resource_reference<L>>(type, id);
        }

        [[nodiscard]] auto ref(std::int64_t id) -> std::shared_ptr<ast::node>
        {
            return std::make_shared<resource_reference<L>>(id);
        }

    private:
        std::int64_t m_id;
        std::string m_name;
        std::vector<std::shared_ptr<ast::node>> m_nodes;
    };
}