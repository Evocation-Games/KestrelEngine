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
        resource_type_page(const std::shared_ptr<project::structure::construct_definition>& definition, const std::string& root_dir, const std::string& reference_root)
            : basic<L>(definition, root_dir, reference_root)
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
            auto field_list = std::make_shared<codegen::ast::list<L>>();
            field_list->add_style_class("resource-fields");
            for (const auto& field : type->all_fields()) {
                field_list->template add_item<codegen::ast::list_item<L>>(
                    std::make_shared<codegen::ast::anchor<L>>(
                        field->basename(),
                        field->basename() + "/index." + L::extension()
                    )
                );
                basic<L>::layout_decision(field);
            }
            codegen::builder<L>::add(field_list);


            // Template
            codegen::builder<L>::template add<codegen::ast::heading<L>>("Resource Template", 2);
            const auto& table = std::make_shared<codegen::ast::table<L>>();
            const auto& header = table->header_row();
            header->add_cell()->add_content("Field Name");
            header->add_cell()->add_content("Binary Type");
            header->add_cell()->add_content("Size");

            const auto *tmpl = instance.binary_template();
            for (const auto& field : tmpl->all_fields()) {
                const auto& row = table->add_row();
                row->add_cell()->add_content(field.label());
                row->add_cell()->add_content(std::make_shared<codegen::ast::inline_code<L>>(field.type().name()));
                row->add_cell()->add_content("-");
            }

            codegen::builder<L>::add(table);
        }
    };
}