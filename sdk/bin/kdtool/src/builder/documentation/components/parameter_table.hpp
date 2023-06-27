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
#include <libCodeGen/builder/component.hpp>
#include <libCodeGen/languages/language.hpp>
#include <libCodeGen/ast/markup.hpp>
#include "builder/buffer.hpp"
#include "project/structure/function/parameter/parameter_definition.hpp"

namespace kdtool::builder::component
{
    template<codegen::language::metadata L>
    struct parameter_table : public codegen::component
    {
        explicit parameter_table(const std::vector<std::shared_ptr<struct project::structure::parameter_definition>>& parameters)
            : m_parameters(parameters)
        {}

        [[nodiscard]] auto emit() const -> codegen::emit::segment override
        {
            auto table = std::make_shared<codegen::ast::table<L>>();
            table->header_row()->add_cell()->add_content("Name");
            table->header_row()->add_cell()->add_content("Type");
            table->header_row()->add_cell()->add_content("Description");

            for (const auto& param : m_parameters) {
                const auto& row = table->add_row();
                row->add_cell()->add_content(param->symbol()->display_name());
                row->add_cell()->add_content(param->type()->name());
                row->add_cell()->add_content(param->description());
            }

            return table->emit();
        }

    private:
        std::vector<std::shared_ptr<struct project::structure::parameter_definition>> m_parameters;
    };
}