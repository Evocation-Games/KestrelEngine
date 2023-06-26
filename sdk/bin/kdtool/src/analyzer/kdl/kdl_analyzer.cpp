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
#include "project/structure/resource_type/resource_field_definition.hpp"
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

auto kdtool::kdl::analyzer::construct_symbol(const std::string& type_name, const std::string& code) -> std::shared_ptr<project::structure::symbol>
{
    std::shared_ptr<project::structure::symbol> symbol;
    auto symbol_name = "rsrc-type:" + type_name + "<" + code + ">";
    symbol = m_index->symbol_named(symbol_name);
    symbol->set_basename(type_name);
    symbol->set_display_name(type_name);
    symbol->set_source_identifier(code);
    return symbol;
}

auto kdtool::kdl::analyzer::construct_symbol(const std::string& type_name, const std::string& code, const std::string& field) -> std::shared_ptr<project::structure::symbol>
{
    std::shared_ptr<project::structure::symbol> symbol;
    auto symbol_name = "rsrc-type-member:" + type_name + "<" + code + ">:" + field;
    symbol = m_index->symbol_named(symbol_name);
    symbol->set_basename(field);
    symbol->set_display_name(field);
    symbol->set_source_identifier(code);
    return symbol;
}

auto kdtool::kdl::analyzer::construct_symbol(const std::string& type_name, const std::string& code, const std::string& field, const std::string& value) -> std::shared_ptr<project::structure::symbol>
{
    std::shared_ptr<project::structure::symbol> symbol;
    auto symbol_name = "rsrc-type-member:" + type_name + "<" + code + ">:" + field + ":" + value;
    symbol = m_index->symbol_named(symbol_name);
    symbol->set_basename(value);
    symbol->set_display_name(value);
    symbol->set_source_identifier(code);
    return symbol;
}

auto kdtool::kdl::analyzer::construct_symbol(
    const std::string& type_name,
    const std::string& code,
    const std::string& field,
    const std::string& value,
    const std::string& sym
) -> std::shared_ptr<project::structure::symbol> {
    std::shared_ptr<project::structure::symbol> symbol;
    auto symbol_name = "rsrc-type-member:" + type_name + "<" + code + ">:" + field + ":" + value + "." + sym;
    symbol = m_index->symbol_named(symbol_name);
    symbol->set_basename(sym);
    symbol->set_display_name(sym);
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

auto kdtool::kdl::analyzer::construct_resource_type(const resource::definition::type::instance& type) -> std::shared_ptr<project::structure::resource_type_definition>
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

    for (const auto& field : type.all_fields()) {
        auto field_definition = construct_resource_field(type, field);
        symbol->add_child(field_definition->symbol());
        resource_type_definition->add_field(field_definition);
    }

    return resource_type_definition;
}

auto kdtool::kdl::analyzer::construct_resource_field(
    const resource::definition::type::instance& type,
    const resource::definition::type::field& field
) -> std::shared_ptr<project::structure::resource_field_definition> {
    auto symbol = construct_symbol(type.name(), type.code(), field.name());

    if (field.has_decorator("documentation")) {
        construct_documentation(symbol, field.decorators_named("documentation"));
    }

    auto resource_field_definition = std::make_shared<struct project::structure::resource_field_definition>(symbol);
    resource_field_definition->set_location(m_path.name());
    resource_field_definition->set_instance(field);
    symbol->set_definition(resource_field_definition);
    m_index->add_definition(resource_field_definition);

    for (const auto& value : field.values()) {
        auto value_definition = construct_resource_value(type, field, value);
        symbol->add_child(value_definition->symbol());
        resource_field_definition->add_value(value_definition);
    }

    return resource_field_definition;
}

auto kdtool::kdl::analyzer::construct_resource_value(
    const resource::definition::type::instance& type,
    const resource::definition::type::field &field,
    const resource::definition::type::field_value &field_value
) -> std::shared_ptr<project::structure::resource_value_definition> {
    auto symbol = construct_symbol(type.name(), type.code(), field.name(), field_value.export_name());
    if (field_value.has_decorator("documentation")) {
        construct_documentation(symbol, field_value.decorators_named("documentation"));
    }

    auto resource_value_definition = std::make_shared<struct project::structure::resource_value_definition>(symbol);
    resource_value_definition->set_location(m_path.name());
    resource_value_definition->set_instance(field_value);
    symbol->set_definition(resource_value_definition);
    m_index->add_definition(resource_value_definition);

    for (const auto& sym : field_value.symbols()) {
        auto symbol_definition = construct_resource_value_symbol(type, field, field_value, sym.second);
        symbol->add_child(symbol_definition->symbol());
        resource_value_definition->add_symbol(symbol_definition);
    }

    return resource_value_definition;
}

auto kdtool::kdl::analyzer::construct_resource_value_symbol(
    const resource::definition::type::instance &type,
    const resource::definition::type::field &field,
    const resource::definition::type::field_value &field_value,
    const resource::definition::type::symbol &value_symbol
) -> std::shared_ptr<project::structure::resource_value_symbol_definition> {
    auto symbol = construct_symbol(type.name(), type.code(), field.name(), field_value.export_name(), value_symbol.name());
    if (field_value.has_decorator("documentation")) {
        construct_documentation(symbol, field_value.decorators_named("documentation"));
    }

    auto resource_value_symbol_definition = std::make_shared<struct project::structure::resource_value_symbol_definition>(symbol);
    resource_value_symbol_definition->set_location(m_path.name());
    resource_value_symbol_definition->set_instance(value_symbol);
    symbol->set_definition(resource_value_symbol_definition);
    m_index->add_definition(resource_value_symbol_definition);

    return resource_value_symbol_definition;
}