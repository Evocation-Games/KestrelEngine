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
    struct resource_field_page : public basic<L>
    {
        resource_field_page(const std::shared_ptr<project::structure::construct_definition>& definition, const std::string& root_dir, const std::string& reference_root)
            : basic<L>(definition, root_dir, reference_root)
        {}

        [[nodiscard]] auto filename() const -> foundation::filesystem::path override
        {
            return basic<L>::filename().child("index");
        }

        [[nodiscard]] auto build_title_heading() const -> std::shared_ptr<codegen::ast::heading<L>> override
        {
            auto header = std::make_shared<codegen::ast::heading<L>>(basic<L>::symbol()->resolved_name(), 1);
            header->add_style_class("resource-field");
            header->add_style_class("symbol");
            return header;
        }

        auto build_content() -> void override
        {
            const auto& field = basic<L>::template definition<project::structure::resource_field_definition>();
            auto& instance = const_cast<::resource::definition::type::field&>(field->instance());

            // Repeat Information
            if (instance.repeatable().enabled()) {
                codegen::builder<L>::template add<codegen::ast::heading<L>>("Repeatable", 2);
                auto repeat_table = std::make_shared<codegen::ast::table<L>>();
                repeat_table->header_row()->add_cell()->add_content("Aspect");
                repeat_table->header_row()->add_cell()->add_content("Value");

                auto count = instance.repeatable().upper_bound() - instance.repeatable().lower_bound() + 1;
                if (count > 1) {
                    repeat_table->add_row({ "Lower Bound", std::to_string(instance.repeatable().lower_bound()) });
                    repeat_table->add_row({ "Upper Bound", std::to_string(instance.repeatable().upper_bound()) });
                    repeat_table->add_row({ "Count", std::to_string(count) });
                }

                if (instance.repeatable().has_count_field()) {
                    repeat_table->add_row({ "Count Field", instance.repeatable().count_field()->label() });
                }

                codegen::builder<L>::add(repeat_table);
            }

            // Values
            codegen::builder<L>::template add<codegen::ast::heading<L>>("Values", 2);
            auto value_table = std::make_shared<codegen::ast::table<L>>();
            value_table->header_row()->add_cell()->add_content("Value Name");
            value_table->header_row()->add_cell()->add_content("Type");
            value_table->header_row()->add_cell()->add_content("Required");

            for (const auto& value : field->all_values()) {
                const auto& row = value_table->add_row();
                row->add_cell()->add_content(
                    std::make_shared<codegen::ast::anchor<L>>(
                        value->basename(),
                        value->basename() + "/index." + L::extension()
                    )
                );
                row->add_cell()->add_content(value->type_name());
                row->add_cell()->add_content(value->has_default_value() ? "Optional" : "Required");
                basic<L>::layout_decision(value);
            }

            codegen::builder<L>::add(value_table);
        }

    };
}