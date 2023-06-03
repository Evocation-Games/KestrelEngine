// Copyright (c) 2022 Tom Hancocks
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

#include <iostream>
#include <fstream>
#include <initializer_list>
#include "codegen/documentation/documentation.hpp"
#include "codegen/documentation/structure/page.hpp"
#include "codegen/documentation/structure/text.hpp"
#include "codegen/documentation/structure/code.hpp"
#include "codegen/documentation/structure/heading.hpp"
#include "codegen/documentation/structure/table.hpp"
#include "codegen/documentation/structure/quote.hpp"
#include "codegen/documentation/structure/list.hpp"
#include "codegen/documentation/structure/link.hpp"
#include "codegen/documentation/structure/horizontal_rule.hpp"

// MARK: - Translation Unit

auto kdtool::codegen::documentation::generator::add_translation_unit(lua_api::ast::translation_unit *tu) -> void
{
    for (auto ns : tu->namespaces()) {
        m_aggregator.merge_namespace(ns);
    }

    for (auto klass : tu->classes()) {
        m_aggregator.merge_class(klass);
    }
}

// MARK: - Compilation

auto kdtool::codegen::documentation::generator::compile(const std::string& path) -> void
{
    for (auto ns : m_aggregator.all_namespaces()) {
        m_pages.emplace_back(compile(ns));
    }

    for (auto klass : m_aggregator.all_classes()) {
        m_pages.emplace_back(compile(klass));
    }

    for (auto page : m_pages) {
        page->render(path);
    }
}

// MARK: - Singular Compilers

auto kdtool::codegen::documentation::generator::construct_availability_table(std::shared_ptr<lua_api::ast::symbol> symbol, std::shared_ptr<table> cxx_table) -> void
{
    cxx_table->add_row({ "Introduced", symbol->introduced_version() });
    if (symbol->is_deprecated()) {
        cxx_table->add_row({ "Deprecated", symbol->deprecated_version() });
    }
}

auto kdtool::codegen::documentation::generator::compile(std::shared_ptr<page> page, std::shared_ptr<lua_api::ast::symbol> symbol) -> void
{
    auto cxx_table = page->add_child<table>(std::initializer_list<std::string>({ "C++ Information", "Value" }));
    cxx_table->add_row({ "Source File", "`" + symbol->include_path() + "`" });
    cxx_table->add_row({ "Symbol", "`" + symbol->cxx_resolved_identifier() + "`" });
    construct_availability_table(symbol, cxx_table);

    if (!symbol->raw_documentation().empty()) {
        page->add_child<heading>("Description", 2);
        page->add_child<text>(symbol->documentation().description());
    }
}

auto kdtool::codegen::documentation::generator::compile(std::shared_ptr<lua_api::ast::lua_namespace> ns) -> std::shared_ptr<page>
{
    auto name = ns->object_symbol()->lua_resolved_identifier();
    auto page = std::make_shared<documentation::page>(name, build_path(ns->object_symbol()));
    compile(page, ns->object_symbol());

    page->add_child<text>("");
    page->add_child<horizontal_rule>();
    page->add_child<text>("");

    auto variables = ns->variables();
    if (!variables.empty()) {
        page->add_child<heading>("Variables", 2);

        auto variable_list = page->add_child<list>();
        for (const auto& variable : variables) {
            auto variable_page = create_variable_page(variable, ns->object_symbol());
            m_pages.emplace_back(variable_page);

            variable_list->add_item(new link(variable->symbol()->lua_identifier(), variable_page->basename()));
        }
    }

    auto properties = ns->properties();
    if (!properties.empty()) {
        page->add_child<heading>("Properties", 2);

        auto property_list = page->add_child<list>();
        for (const auto& property : properties) {
            auto property_page = create_property_page(property, ns->object_symbol());
            m_pages.emplace_back(property_page);

            property_list->add_item(new link(property->getter()->lua_identifier(), property_page->basename()));
        }
    }

    auto functions = ns->functions();
    if (!functions.empty()) {
        page->add_child<heading>("Functions", 2);

        auto function_list = page->add_child<list>();
        for (const auto& function : functions) {
            auto function_page = create_function_page(function, ns->object_symbol());
            m_pages.emplace_back(function_page);

            function_list->add_item(new link(function->symbol()->lua_identifier(), function_page->basename()));
        }
    }

    return page;
}

auto kdtool::codegen::documentation::generator::compile(std::shared_ptr<lua_api::ast::lua_class> klass) -> std::shared_ptr<page>
{
    auto name = klass->object_symbol()->lua_resolved_identifier();
    auto page = std::make_shared<documentation::page>(name, build_path(klass->object_symbol()));
    compile(page, klass->object_symbol());

    if (klass->has_constructor()) {
        page->add_child<heading>("Constructor", 2);
//        page->add_child<heading>("`" + klass->object_symbol()->lua_resolved_identifier() + "()`");
    }

    page->add_child<text>("");
    page->add_child<horizontal_rule>();
    page->add_child<text>("");

    auto variables = klass->variables();
    if (!variables.empty()) {
        page->add_child<heading>("Variables", 2);

        auto variable_list = page->add_child<list>();
        for (const auto& variable : variables) {
            auto variable_page = create_variable_page(variable, klass->object_symbol());
            m_pages.emplace_back(variable_page);

            variable_list->add_item(new link(variable->symbol()->lua_identifier(), variable_page->basename()));
        }
    }

    auto properties = klass->properties();
    if (!properties.empty()) {
        page->add_child<heading>("Properties", 2);

        auto property_list = page->add_child<list>();
        for (const auto& property : properties) {
            auto property_page = create_property_page(property, klass->object_symbol());
            m_pages.emplace_back(property_page);

            property_list->add_item(new link(property->getter()->lua_identifier(), property_page->basename()));
        }
    }

    auto functions = klass->functions();
    if (!functions.empty()) {
        page->add_child<heading>("Functions", 2);

        auto function_list = page->add_child<list>();
        for (const auto& function : functions) {
            auto function_page = create_function_page(function, klass->object_symbol());
            m_pages.emplace_back(function_page);

            function_list->add_item(new link(function->symbol()->lua_identifier(), function_page->basename()));
        }
    }

    return page;
}

auto kdtool::codegen::documentation::generator::compile(std::shared_ptr<page> page, std::shared_ptr<lua_api::ast::lua_function> function, std::shared_ptr<lua_api::ast::symbol> owner) -> void
{
    // Build a representation of the function name.
    std::string function_label = owner->lua_identifier();
    if (function->is_static()) {
        function_label += ".";
    }
    else {
        function_label += ":";
    }

    std::string parameter_list;
    auto parameter_table = std::make_shared<table>(std::initializer_list<std::string>({ "Parameter", "Description" }));
    for (const auto& parameter : function->symbol()->documentation().parameters()) {
        if (!parameter_list.empty()) {
            parameter_list += ", ";
        }
        parameter_list += parameter.name();
        parameter_table->add_row({ parameter.name(), parameter.description() });
    }

    function_label += function->symbol()->lua_identifier() + "(" + parameter_list + ")";
    page->add_child<heading>("`" + function_label + "`", 3);

    // Add some basic meta data
    auto cxx_table = page->add_child<table>(std::initializer_list<std::string>({ "Aspect", "Value" }));
    cxx_table->add_row({ "C++ Symbol", "`" + function->symbol()->cxx_resolved_identifier() + "()`" });
    construct_availability_table(function->symbol(), cxx_table);

    // Add a warning if it exists
    if (!function->symbol()->documentation().warning().empty()) {
        page->add_child<text>("");
        page->add_child<quote>("WARNING", function->symbol()->documentation().warning());
        page->add_child<text>("");
    }

    // Add some documentation for the function.
    if (function->symbol()->documentation().empty()) {
        page->add_child<text>("No description available.");
    }
    else {
        auto heading = page->add_child<text>("");
        heading->append_text("Description", text::style::bold);
        page->add_child<text>("");
        page->add_child<text>(function->symbol()->documentation().description());
    }

    // If there are parameters add a table for them.
    if (!parameter_list.empty()) {
        page->add_child<text>("");
        auto heading = page->add_child<text>("");
        heading->append_text("Parameters", text::style::bold);
        page->add_child<text>("");
        page->add_child(parameter_table);
        page->add_child<text>("");
    }

    // Add an example if it exists
    if (!function->symbol()->documentation().example().empty()) {
        auto heading = page->add_child<text>("");
        heading->append_text("Example", text::style::bold);
        page->add_child<text>("");
        page->add_child<code>(function->symbol()->documentation().example());
    }
}

auto kdtool::codegen::documentation::generator::compile(std::shared_ptr<page> page, std::shared_ptr<lua_api::ast::lua_property> property, std::shared_ptr<lua_api::ast::symbol> owner) -> void
{
    // Build a representation of the function name.
    std::string property_label = owner->lua_identifier() + "." + property->getter()->lua_identifier();
    page->add_child<heading>("`" + property_label + "`", 3);

    // Add some basic meta data
    auto cxx_table = page->add_child<table>(std::initializer_list<std::string>({ "Aspect", "Value" }));

    cxx_table->add_row({ "Type", "`int`" });

    if (property->has_getter()) {
        cxx_table->add_row({ "C++ Getter Function Symbol", "`" + property->getter()->cxx_resolved_identifier() + "()`" });
        construct_availability_table(property->getter(), cxx_table);
    }

    if (property->has_setter()) {
        cxx_table->add_row({ "C++ Setter Function Symbol", "`" + property->setter()->cxx_resolved_identifier() + "()`" });
        construct_availability_table(property->setter(), cxx_table);
    }

    // Add some documentation for the function.
    if (property->has_getter() && !property->getter()->documentation().empty()) {
        page->add_child<heading>("Getter Description", 4);
        page->add_child<text>(property->getter()->documentation().description());
    }

    if (property->has_setter() && !property->setter()->documentation().empty()) {
        page->add_child<heading>("Setter Description", 4);
        page->add_child<text>(property->setter()->documentation().description());
    }
}


auto kdtool::codegen::documentation::generator::compile(std::shared_ptr<page> page, std::shared_ptr<lua_api::ast::lua_variable> variable, std::shared_ptr<lua_api::ast::symbol> owner) -> void
{
    // Build a representation of the variable name.
    std::string variable_label = owner->lua_identifier() + "." + variable->symbol()->lua_identifier();
    page->add_child<heading>("`" + variable_label + "`", 3);

    // Add some basic meta data
    auto cxx_table = page->add_child<table>(std::initializer_list<std::string>({ "Aspect", "Value" }));

    cxx_table->add_row({ "Type", "`int`" });
    cxx_table->add_row({ "Mutability", variable->is_mutable() ? "Mutable" : "Immutable" });
    construct_availability_table(variable->symbol(), cxx_table);

    // Add some documentation for the function.
    if (!variable->symbol()->documentation().empty()) {
        page->add_child<text>(variable->symbol()->documentation().description());
    }
}

// MARK: - Child Pages

auto kdtool::codegen::documentation::generator::create_variable_page(std::shared_ptr<lua_api::ast::lua_variable> variable, std::shared_ptr<lua_api::ast::symbol> owner) -> std::shared_ptr<page>
{
    auto resolved = variable->symbol()->lua_resolved_identifier();
    auto variable_page = std::make_shared<page>("Variable: " + resolved, build_path(variable->symbol(), owner));

    compile(variable_page, variable, owner);

    return variable_page;
}

auto kdtool::codegen::documentation::generator::create_property_page(std::shared_ptr<lua_api::ast::lua_property> property, std::shared_ptr<lua_api::ast::symbol> owner) -> std::shared_ptr<page>
{
    auto resolved = property->getter()->lua_resolved_identifier();
    auto property_page = std::make_shared<page>("Property: " + resolved, build_path(property->getter(), owner));

    compile(property_page, property, owner);

    return property_page;
}

auto kdtool::codegen::documentation::generator::create_function_page(std::shared_ptr<lua_api::ast::lua_function> function, std::shared_ptr<lua_api::ast::symbol> owner) -> std::shared_ptr<page>
{
    auto resolved = function->symbol()->lua_resolved_identifier();
    auto function_page = std::make_shared<page>("Function: " + resolved, build_path(function->symbol(), owner));

    compile(function_page, function, owner);

    return function_page;
}

// MARK: - Paths

auto kdtool::codegen::documentation::generator::build_path(std::shared_ptr<lua_api::ast::symbol> symbol, std::shared_ptr<lua_api::ast::symbol> owner) -> std::string
{
    std::string path;

    if (owner) {
        path = owner->lua_resolved_identifier("/") + "/";
    }

    if (symbol) {
        path += symbol->lua_resolved_identifier("/");
        if (!owner) {
            path += "/index";
        }
    }

    return path;
}