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

#include <memory>
#include <libCodeGen/builder/builder.hpp>
#include <libCodeGen/ast/markup.hpp>
#include "project/structure/construct_definition.hpp"
#include "project/structure/symbol.hpp"
#include "builder/documentation/components/availability_table.hpp"
#include "builder/documentation/components/description.hpp"

namespace kdtool::builder::page
{
    template<codegen::language::markup_support L>
    struct basic : public codegen::builder<L>
    {
        basic(const std::shared_ptr<project::structure::construct_definition>& definition, const std::string& root_dir, const std::string& reference_root)
            : codegen::builder<L>(root_dir, reference_root), m_definition(definition)
        {}

        template<typename T, typename std::enable_if<std::is_base_of<project::structure::construct_definition, T>::value>::type* = nullptr>
        [[nodiscard]] auto definition() const -> std::shared_ptr<T>
        {
            return std::reinterpret_pointer_cast<T>(m_definition);
        }

        [[nodiscard]] auto symbol() const -> std::shared_ptr<project::structure::symbol>
        {
            return m_definition->symbol();
        }

        virtual auto handle_layout_decision(const std::function<auto(const std::shared_ptr<struct project::structure::construct_definition>&)->void>& handler) -> void
        {
            m_layout_decision = handler;
        }

        virtual auto layout_decision(const std::shared_ptr<struct project::structure::construct_definition>& property) -> void
        {
            m_layout_decision(property);
        }

        [[nodiscard]] virtual auto filename() const -> foundation::filesystem::path
        {
            return symbol()->filename();
        }

        virtual auto build_header() -> void
        {
            codegen::builder<L>::template add<codegen::ast::prologue<L>>(
                symbol()->resolved_name(),
                codegen::builder<L>::reference_root().child("css").child("style.css").string()
            );
            codegen::builder<L>::template add<codegen::ast::heading<L>>(symbol()->resolved_name(), 1);
            codegen::builder<L>::template add_component<component::availability_table<L>>(symbol());

            if (auto documentation = symbol()->documentation().lock()) {
                for (const auto& section : documentation->sections()) {
                    codegen::builder<L>::template add_component<component::documentation_section<L>>(section);
                }
            }
        }

        virtual auto build_footer() -> void
        {
            codegen::builder<L>::template add<codegen::ast::epilogue<L>>();
        }

        virtual auto build_content() -> void
        {

        }

        auto build() -> void override
        {
            codegen::builder<L>::build();
            build_header();

            codegen::builder<L>::template add<codegen::ast::nl<L>>();
            codegen::builder<L>::template add<codegen::ast::divider<L>>();
            build_content();
            codegen::builder<L>::template add<codegen::ast::nl<L>>();
            codegen::builder<L>::template add<codegen::ast::divider<L>>();

            build_footer();
            codegen::builder<L>::save(filename().string());
        }

    private:
        std::shared_ptr<project::structure::construct_definition> m_definition;
        std::function<auto(const std::shared_ptr<struct project::structure::construct_definition>&)->void> m_layout_decision { [&] (const auto& p) {} };
    };
}