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

#include <string>
#include <vector>
#include "analyzer/kdl/kdl_analyzer.hpp"
#include "project/structure/resource_type/resource_type_definition.hpp"
#include <libDocumentation/parser/parser.hpp>

// MARK: - Construction

kdtool::kdl::analyzer::analyzer(const std::shared_ptr<project::index> &project, const foundation::filesystem::path &path)
    : m_index(project), m_path(path)
{
    ::kdl::unit::file session(m_context);
    std::vector<std::string> definitions {
        "extended"
    };
    session.import_file(path.string(), definitions);
}

// MARK: - Traversal

auto kdtool::kdl::analyzer::run() -> void
{
    for (const auto& type : m_context.registered_types) {
        construct_resource_type(type.second);
    }
}

// MARK: - Constructs

auto kdtool::kdl::analyzer::construct_symbol(const std::string& name, const std::string& code) -> std::shared_ptr<project::structure::symbol>
{
    std::shared_ptr<project::structure::symbol> symbol;
    auto symbol_name = "rsrc-type:" + name + "<" + code + ">";
    symbol = m_index->symbol_named(symbol_name);
    symbol->set_basename(name);
    symbol->set_display_name(name);
    symbol->set_source_identifier(code);
    return symbol;
}

auto kdtool::kdl::analyzer::construct_documentation(const std::shared_ptr<project::structure::symbol>& symbol, const std::vector<resource::decorator>& decorators) -> void
{
    if (decorators.empty()) {
        return;
    }

    const auto& documentation = decorators.at(0);
    if (!documentation.has_associated_values()) {
        return;
    }

    const auto& raw_documentation = documentation.associated_value_at(0);
    symbol->set_documentation(documentation::parser(raw_documentation).build());
}

auto kdtool::kdl::analyzer::construct_resource_type(const resource::definition::type::instance& type) -> std::shared_ptr<project::structure::construct_definition>
{
    auto symbol = construct_symbol(type.name(), type.code());

    if (type.has_decorator("documentation")) {
        construct_documentation(symbol, type.decorators_named("documentation"));
    }

    auto resource_type_definition = std::make_shared<struct project::structure::resource_type_definition>(symbol);
    resource_type_definition->set_location(m_path.name());
    resource_type_definition->set_instance(type);
    symbol->set_definition(resource_type_definition);
    m_index->add_definition(resource_type_definition);

    return resource_type_definition;
}

