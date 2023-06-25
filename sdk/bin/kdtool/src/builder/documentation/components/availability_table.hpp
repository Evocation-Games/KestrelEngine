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

#include <libCodeGen/builder/component.hpp>
#include <libCodeGen/languages/language.hpp>
#include <libCodeGen/ast/markup.hpp>
#include "builder/buffer.hpp"

namespace kdtool::builder::component
{
    template<codegen::language::metadata L>
    struct availability_table : public codegen::component
    {
        explicit availability_table(const std::shared_ptr<project::structure::symbol>& symbol)
            : m_symbol(symbol)
        {}

        [[nodiscard]] auto emit() const -> codegen::emit::segment override
        {
            buffer<L> buffer;

            auto table = std::make_shared<codegen::ast::table<L>>();
            table->add_column("Aspect");
            table->add_column("Value");

            std::string source_symbol_name = "C++ Symbol";
            if (auto definition = m_symbol->definition().lock()) {
                table->add_row({
                    std::make_shared<codegen::ast::text<L>>("File"),
                    std::make_shared<codegen::ast::inline_code<L>>(definition->location())
                });

                switch (definition->instance_type()) {
                    case project::structure::construct_definition::type::is_resource_type:
                        source_symbol_name = "Resource Type Code";
                        break;
                    default: break;
                }
            }

            bool is_first_symbol = true;
            for (const auto& symbol : m_symbol->all_source_resolved_identifiers()) {
                table->add_row({
                    std::make_shared<codegen::ast::text<L>>(is_first_symbol ? source_symbol_name : ""),
                    std::make_shared<codegen::ast::inline_code<L>>(symbol)
                });
                is_first_symbol = false;
            }

            if (m_symbol->available().has_value()) {
                table->add_row({ "Available", m_symbol->available()->string() });
            }
            if (m_symbol->deprecated().has_value()) {
                table->add_row({ "Deprecated", m_symbol->deprecated()->string() });
            }

            buffer.add(table);
            return buffer.segments();
        }

    private:
        std::shared_ptr<project::structure::symbol> m_symbol;
    };
}