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
#include "project/structure/function/function_definition.hpp"
#include "project/structure/function/parameter/parameter_definition.hpp"
#include "project/structure/symbol.hpp"
#include "builder/documentation/pages/basic_page.hpp"
#include "builder/documentation/components/availability_table.hpp"
#include "builder/documentation/components/parameter_table.hpp"
#include "builder/documentation/components/description.hpp"
#include "builder/documentation/pages/layout_decider.hpp"

namespace kdtool::builder::page
{
    template<codegen::language::markup_support L>
    struct function_page : public basic<L>
    {
        function_page(const std::shared_ptr<project::structure::construct_definition>& definition, const std::string& root_dir, const std::string& reference_root)
            : basic<L>(definition, root_dir, reference_root)
        {}

        [[nodiscard]] auto build_title_heading() const -> std::shared_ptr<codegen::ast::heading<L>> override
        {
            auto header = std::make_shared<codegen::ast::heading<L>>(basic<L>::symbol()->resolved_name(), 1);
            header->add_style_class("function");
            header->add_style_class("symbol");
            return header;
        }

        auto build_content() -> void override
        {
            const auto& fn = basic<L>::template definition<project::structure::function_definition>();

            // Parameters
            const auto& parameters = fn->all_parameters();
            if (!parameters.empty()) {
                codegen::builder<L>::template add<codegen::ast::heading<L>>("Parameters", 2);
                codegen::builder<L>::template add_component<component::parameter_table<L>>(parameters);
            }
        }

    };
}