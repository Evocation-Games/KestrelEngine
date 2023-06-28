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
#include "project/structure/class/class_definition.hpp"
#include "project/structure/symbol.hpp"
#include "builder/documentation/pages/basic_page.hpp"
#include "builder/documentation/components/availability_table.hpp"
#include "builder/documentation/components/description.hpp"
#include "builder/documentation/pages/layout_decider.hpp"

namespace kdtool::builder::page
{
    template<codegen::language::markup_support L>
    struct class_page : public basic<L>
    {
        class_page(const std::shared_ptr<project::structure::construct_definition>& definition, const std::string& root_dir, const std::string& reference_root)
            : basic<L>(definition, root_dir, reference_root)
        {}

        [[nodiscard]] auto filename() const -> foundation::filesystem::path override
        {
            return basic<L>::filename().child("index");
        }

        [[nodiscard]] auto build_title_heading() const -> std::shared_ptr<codegen::ast::heading<L>> override
        {
            auto header = std::make_shared<codegen::ast::heading<L>>(basic<L>::symbol()->resolved_name(), 1);
            header->add_style_class("class");
            header->add_style_class("symbol");
            return header;
        }

        auto build_content() -> void override
        {
            const auto& klass = basic<L>::template definition<project::structure::class_definition>();

            // Instance Variables
            codegen::builder<L>::template add<codegen::ast::heading<L>>("Variables", 2);

            // Properties
            codegen::builder<L>::template add<codegen::ast::heading<L>>("Properties", 2);
            auto properties_list = std::make_shared<codegen::ast::list<L>>();
            for (const auto& property : klass->all_properties()) {
                properties_list->template add_item<codegen::ast::list_item<L>>(
                    std::make_shared<codegen::ast::anchor<L>>(
                        property->name(),
                        property->symbol()->filename().name() + "." + L::extension()
                    )
                )->add_style_class("property");
                basic<L>::layout_decision(property);
            }
            codegen::builder<L>::add(properties_list);

            // Functions
            codegen::builder<L>::template add<codegen::ast::heading<L>>("Functions", 2);
            auto functions_list = std::make_shared<codegen::ast::list<L>>();
            for (const auto& function : klass->all_functions()) {
                functions_list->template add_item<codegen::ast::list_item<L>>(
                    std::make_shared<codegen::ast::anchor<L>>(
                        function->symbol()->display_name(),
                        function->symbol()->filename().name() + "." + L::extension()
                    )
                )->add_style_class("function");
                basic<L>::layout_decision(function);
            }
            codegen::builder<L>::add(functions_list);

        }
    };
}