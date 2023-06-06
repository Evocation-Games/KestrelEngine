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
#include <libCodeGen/spec/markup/list.hpp>
#include "documentation/components/property_index_list.hpp"
#include "documentation/components/text.hpp"
#include "documentation/builder.hpp"

// MARK: - Construction

kdtool::documentation::property_index_list::property_index_list(const std::vector<std::shared_ptr<lua_api::ast::lua_property>>& properties, const std::shared_ptr<lua_api::ast::symbol>& owner)
    : m_properties(properties), m_owner(owner)
{}

// MARK: - Building

auto kdtool::documentation::property_index_list::build(const std::shared_ptr<builder>& builder) const -> std::shared_ptr<codegen::spec::markup::markup_node>
{
    auto body = std::make_shared<codegen::spec::markup::markup_node>();

    body->add<codegen::spec::markup::heading>("Properties", 2);

    auto list = body->add<codegen::spec::markup::list>();
    for (const auto& property : m_properties) {
        auto page = builder->add_property_page(property, m_owner);
        list->add_item(anchor(property->getter()->lua_identifier(), page, true));
    }

    return body;
}
