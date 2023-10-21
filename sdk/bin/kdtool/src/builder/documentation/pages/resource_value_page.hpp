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
#include "project/structure/resource_type/resource_field_definition.hpp"
#include "project/structure/symbol.hpp"
#include <libResource/definition/template/instance.hpp>
#include <libResource/definition/template/field.hpp>

namespace kdtool::builder::page
{
    template<codegen::language::markup_support L>
    struct resource_value_page : public basic<L>
    {
        resource_value_page(const std::shared_ptr<project::structure::construct_definition>& definition, const std::string& root_dir, const std::string& reference_root)
            : basic<L>(definition, root_dir, reference_root)
        {}

        [[nodiscard]] auto filename() const -> foundation::filesystem::path override
        {
            return basic<L>::filename().child("index");
        }

        [[nodiscard]] auto build_title_heading() const -> std::shared_ptr<codegen::ast::heading<L>> override
        {
            auto header = std::make_shared<codegen::ast::heading<L>>(basic<L>::symbol()->lua_identifier(), 1);
            header->add_style_class("resource-field-value");
            header->add_style_class("symbol");
            return header;
        }

        auto build_content() -> void override
        {
            const auto& value = basic<L>::template definition<project::structure::resource_value_definition>();
            const auto& instance = value->instance();

            // Type
            codegen::builder<L>::template add<codegen::ast::heading<L>>("Type Information", 2);
            codegen::builder<L>::template add<codegen::ast::preformatted<L>>(value->type_name());

            // Symbols
            if (instance.has_symbols()) {
                codegen::builder<L>::template add<codegen::ast::heading<L>>("Symbols / Constants", 2);
                auto table = std::make_shared<codegen::ast::table<L>>();
                table->header_row()->add_cell()->add_content("Symbol Identifier");
                table->header_row()->add_cell()->add_content("Value");

                for (const auto& symbol : value->all_symbols()) {
                    const auto& row = table->add_row();
                    row->add_cell()->add_content(
                        std::make_shared<codegen::ast::anchor<L>>(
                            symbol->name(),
                            symbol->name() + "." + L::extension()
                        )
                    );
                    row->add_cell()->add_content(std::make_shared<codegen::ast::inline_code<L>>(symbol->value()));
                    basic<L>::layout_decision(symbol);
                }

                codegen::builder<L>::add(table);
            }
        }

    };
}