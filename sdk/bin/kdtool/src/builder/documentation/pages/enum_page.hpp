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
#include "project/structure/enum/enum_definition.hpp"
#include "project/structure/enum/enum_case_definition.hpp"
#include "project/structure/symbol.hpp"
#include "builder/documentation/pages/basic_page.hpp"
#include "builder/documentation/components/availability_table.hpp"
#include "builder/documentation/components/description.hpp"
#include "builder/documentation/pages/layout_decider.hpp"

namespace kdtool::builder::page
{
    template<codegen::language::markup_support L>
    struct enum_page : public basic<L>
    {
        enum_page(const std::shared_ptr<project::structure::construct_definition>& definition, const std::string& root_dir, const std::string& reference_root)
            : basic<L>(definition, root_dir, reference_root)
        {}

        [[nodiscard]] auto filename() const -> foundation::filesystem::path override
        {
            return basic<L>::filename().child("index");
        }

        auto build_content() -> void override
        {
            const auto& enum_definition = basic<L>::template definition<project::structure::enum_definition>();

            // Properties
            codegen::builder<L>::template add<codegen::ast::heading<L>>("All Cases", 2);
            auto case_list = std::make_shared<codegen::ast::list<L>>();
            for (const auto& enum_case : enum_definition->all_cases()) {
                case_list->template add_item<codegen::ast::list_item<L>>(
                    std::make_shared<codegen::ast::anchor<L>>(
                        enum_case->symbol()->display_name(),
                        enum_case->symbol()->filename().name() + "." + L::extension()
                    )
                );
                basic<L>::layout_decision(enum_case);
            }
            codegen::builder<L>::add(case_list);
        }
    };
}