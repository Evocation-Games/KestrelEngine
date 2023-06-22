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
#include "project/structure/namespace/namespace_definition.hpp"
#include "project/structure/symbol.hpp"
#include "builder/documentation/pages/basic_page.hpp"
#include "builder/documentation/components/availability_table.hpp"
#include "builder/documentation/components/description.hpp"
#include "builder/documentation/pages/layout_decider.hpp"

namespace kdtool::builder::page
{
    template<codegen::language::markup_support L>
    struct namespace_page : public basic<L>
    {
        namespace_page(const std::shared_ptr<project::structure::construct_definition>& definition, const std::string& root_dir)
            : basic<L>(definition, root_dir)
        {}

        [[nodiscard]] auto filename() const -> foundation::filesystem::path override
        {
            return basic<L>::filename().child("index");
        }

        auto build_content() -> void override
        {
            const auto& ns = basic<L>::template definition<project::structure::namespace_definition>();

            // Properties
            codegen::builder<L>::template add<codegen::ast::heading<L>>("Properties", 2);
            codegen::builder<L>::template add<codegen::ast::begin_list<L>>();
            for (const auto& property : ns->all_properties()) {
                codegen::builder<L>::template add<codegen::ast::begin_list_item<L>>();
                codegen::builder<L>::template add<codegen::ast::anchor<L>>(
                    property->name(),
                    property->symbol()->filename().name() + "." + L::extension()
                );
                codegen::builder<L>::template add<codegen::ast::end_list_item<L>>();
                basic<L>::layout_decision(property);
            }
            codegen::builder<L>::template add<codegen::ast::end_list<L>>();

            // Functions
            codegen::builder<L>::template add<codegen::ast::heading<L>>("Functions", 2);
            codegen::builder<L>::template add<codegen::ast::begin_list<L>>();
            for (const auto& function : ns->all_functions()) {
                codegen::builder<L>::template add<codegen::ast::begin_list_item<L>>();
                codegen::builder<L>::template add<codegen::ast::anchor<L>>(
                    function->symbol()->display_name(),
                    function->symbol()->filename().name() + "." + L::extension()
                );
                codegen::builder<L>::template add<codegen::ast::end_list_item<L>>();
                basic<L>::layout_decision(function);
            }
            codegen::builder<L>::template add<codegen::ast::end_list<L>>();
        }
    };
}