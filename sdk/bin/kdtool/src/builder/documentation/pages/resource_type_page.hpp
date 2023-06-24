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
#include "project/structure/resource_type/resource_type_definition.hpp"
#include "project/structure/symbol.hpp"
#include "builder/documentation/pages/basic_page.hpp"
#include "builder/documentation/components/availability_table.hpp"
#include "builder/documentation/components/description.hpp"
#include "builder/documentation/pages/layout_decider.hpp"
#include <libResource/definition/template/instance.hpp>
#include <libResource/definition/template/field.hpp>


namespace kdtool::builder::page
{
    template<codegen::language::markup_support L>
    struct resource_type_page : public basic<L>
    {
        resource_type_page(const std::shared_ptr<project::structure::construct_definition>& definition, const std::string& root_dir)
            : basic<L>(definition, root_dir)
        {}

        [[nodiscard]] auto filename() const -> foundation::filesystem::path override
        {
            return basic<L>::filename().child("index");
        }

        auto build_content() -> void override
        {
            const auto& type = basic<L>::template definition<project::structure::resource_type_definition>();
            const auto& instance = type->instance();

            // Constructors
//            codegen::builder<L>::template add<codegen::ast::heading<L>>("Constructors", 2);

            // Fields
            codegen::builder<L>::template add<codegen::ast::heading<L>>("Fields", 2);
            codegen::builder<L>::template add<codegen::ast::begin_list<L>>();
            for (const auto& field : instance.all_fields()) {
                codegen::builder<L>::template add<codegen::ast::begin_list_item<L>>();
                codegen::builder<L>::template add<codegen::ast::text<L>>(field.name());
                codegen::builder<L>::template add<codegen::ast::end_list_item<L>>();
            }
            codegen::builder<L>::template add<codegen::ast::end_list<L>>();

            // Template
            codegen::builder<L>::template add<codegen::ast::heading<L>>("Resource Template", 2);
            auto table = std::make_shared<codegen::ast::table<L>>();
            table->add_column("Field Name");
            table->add_column("Binary Type");
            table->add_column("Size");

            const auto *tmpl = instance.binary_template();
            for (const auto& field : tmpl->all_fields()) {
                table->add_row({
                    field.label(),
                    field.type().name(),
                    "-"
                });
            }

            codegen::builder<L>::add(table);

        }
    };
}