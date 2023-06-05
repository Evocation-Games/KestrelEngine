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

#include <libCodeGen/spec/markup/heading.hpp>
#include "documentation/components/parameters_table.hpp"
#include "documentation/components/text.hpp"

// MARK: - Construction

kdtool::documentation::parameters_table::parameters_table(const std::shared_ptr<lua_api::ast::symbol> &symbol)
    : m_symbol(symbol)
{}

// MARK: - Building

auto kdtool::documentation::parameters_table::build(const std::shared_ptr<builder>& builder) const -> std::shared_ptr<codegen::spec::markup::markup_node>
{
    auto body = std::make_shared<codegen::spec::markup::markup_node>();
    body->add<codegen::spec::markup::heading>("Parameters", 2);

    auto table = body->add<codegen::spec::markup::table>(std::initializer_list<std::string>({
        "Parameter", "Type", "Description"
    }));

    for (const auto& parameter : m_symbol->documentation().parameters()) {
        table->add_row({
            inline_code(parameter.name()),
            inline_code("?"),
            text(parameter.description()),
        });
    }

    return body;
}
