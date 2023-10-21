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
#include <libCodeGen/ast/component/component_files.hpp>

namespace codegen::ast
{
    template<language::component_definition_support L>
    struct component : public node
    {
    public:
        component(const std::shared_ptr<ast::symbol<L>>& name, const std::shared_ptr<ast::symbol<L>>& type, std::int64_t id)
            : m_name(name),
              m_type(type),
              m_id(id)
        {}

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            std::vector<emit::segment> out;
            out.reserve(m_nodes.size());
            for (const auto& node : m_nodes) {
                out.emplace_back(node->emit());
            }

            return {
                L::component_decl(m_name, m_id, m_type),
                L::begin_compound_statement(),
                emit::segment(out),
                L::end_compound_statement()
            };
        }

        auto comment(const std::string& str) -> void
        {
            m_nodes.emplace_back(std::make_shared<ast::comment<L>>(str));
        }

        auto files(const std::string& path, const std::function<auto(const std::shared_ptr<component_files<L>>&)->void>& fn) -> void
        {
            auto files = std::make_shared<ast::component_files<L>>(path);
            fn(files);

            m_nodes.emplace_back(files);
        }

    private:
        std::shared_ptr<ast::symbol<L>> m_name;
        std::shared_ptr<ast::symbol<L>> m_type;
        std::int64_t m_id;
        std::vector<std::shared_ptr<ast::node>> m_nodes;
    };
}