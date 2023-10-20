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
#include <functional>
#include <libCodeGen/ast/core/node.hpp>
#include <libCodeGen/ast/symbol/symbol.hpp>
#include <libCodeGen/ast/comment/comment.hpp>
#include <libCodeGen/ast/declaration/resource_instance.hpp>
#include <libCodeGen/ast/literal/literals.hpp>
#include <libCodeGen/ast/statement/call_statements.hpp>
#include <libCodeGen/ast/statement/literal_statements.hpp>
#include <libCodeGen/ast/declaration/control_declaration.hpp>

namespace codegen::ast
{
    template<language::resource_declaration_support L>
    struct scene_declaration : public node
    {
    public:
        scene_declaration(const std::shared_ptr<ast::symbol<L>>& name, std::int64_t id, bool is_dialog = false)
            : m_name(name), m_id(id), m_dialog(is_dialog)
        {}

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            std::vector<emit::segment> out;
            out.reserve(m_nodes.size());
            for (const auto& node : m_nodes) {
                if (node) {
                    out.emplace_back(node->emit());
                    out.emplace_back(emit::segment(L::statement_end_operator_string()));
                }
                out.emplace_back(ast::nl<L>().emit());
            }

            return {
                m_dialog ? L::dialog_declaration_decl(m_name, m_id) : L::scene_declaration_decl(m_name, m_id),
                L::begin_compound_statement(),
                emit::segment(out),
                L::end_compound_statement()
            };
        }

        auto comment(const std::string& str) -> void
        {
            m_nodes.emplace_back(std::make_shared<ast::comment<L>>(str));
        }

        auto nl() -> void
        {
            m_nodes.emplace_back(nullptr);
        }

        auto set(const std::string& field, const std::shared_ptr<ast::node>& value) -> void
        {
            auto assign = std::make_shared<ast::assignment_statement<L>>(
                std::make_shared<segment_statement<L>>(std::make_shared<ast::symbol<L>>(field)->emit(), false),
                std::make_shared<segment_statement<L>>(value->emit(), false)
            );
            m_nodes.emplace_back(assign);
        }

        [[nodiscard]] auto string(const std::string& value) -> std::shared_ptr<ast::node>
        {
            return std::make_shared<string_literal<L>>(value);
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

        [[nodiscard]] auto call(const std::string& name, const std::vector<std::shared_ptr<ast::statement<L>>>& args) -> std::shared_ptr<ast::node>
        {
            return std::make_shared<function_call<L>>(
                std::make_shared<symbol<L>>(name),
                args
            );
        }

        [[nodiscard]] auto frame_size(std::int64_t width, std::int64_t height) -> std::shared_ptr<ast::node>
        {
            return call("FrameSize", {
                std::make_shared<integer_statement<L>>(width),
                std::make_shared<integer_statement<L>>(height)
            });
        }

        [[nodiscard]] auto frame(std::int64_t x, std::int64_t y, std::int64_t width, std::int64_t height) -> std::shared_ptr<ast::node>
        {
            return call("Frame", {
                std::make_shared<integer_statement<L>>(x),
                std::make_shared<integer_statement<L>>(y),
                std::make_shared<integer_statement<L>>(width),
                std::make_shared<integer_statement<L>>(height)
            });
        }

        [[nodiscard]] auto function(const std::string& name) -> std::shared_ptr<ast::node>
        {
            return call("Function", {
                std::make_shared<symbol_statement<L>>(name)
            });
        }

        [[nodiscard]] auto options(const std::vector<std::string>& options) -> std::shared_ptr<ast::node>
        {
            std::vector<emit::segment> out;
            for (const auto& opt : options) {
                if (!out.empty()) {
                    out.emplace_back(" | ");
                }
                out.emplace_back(opt);
            }
            return std::make_shared<segment_statement<L>>(emit::segment(out));
        }

        auto control(const std::string& type,
                     const std::string& name,
                     const std::function<auto(const std::shared_ptr<control_declaration<L>>&)->void>& fn) -> void
        {
            auto control = std::make_shared<control_declaration<L>>(type, name);
            fn(control);
            m_nodes.emplace_back(control);
        }

    private:
        bool m_dialog;
        std::int64_t m_id;
        std::shared_ptr<ast::symbol<L>> m_name;
        std::vector<std::shared_ptr<ast::node>> m_nodes;
    };
}