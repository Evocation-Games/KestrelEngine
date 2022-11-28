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
#include "codegen/documentation/structure/heading.hpp"
#include "codegen/documentation/structure/table.hpp"
#include "codegen/documentation/structure/horizontal_rule.hpp"

// MARK: - Translation Unit

auto luatool::codegen::documentation::generator::add_translation_unit(lua_api::ast::translation_unit *tu) -> void
{
    for (auto ns : tu->namespaces()) {
        m_aggregator.merge_namespace(ns);
    }

    for (auto klass : tu->classes()) {
        m_aggregator.merge_class(klass);
    }
}

// MARK: - Compilation

auto luatool::codegen::documentation::generator::compile(const std::string& path) -> void
{
    for (auto ns : m_aggregator.all_namespaces()) {
        m_pages.emplace_back(compile(ns));
    }

    for (auto klass : m_aggregator.all_classes()) {
        m_pages.emplace_back(compile(klass));
    }

    for (auto page : m_pages) {
        std::string page_out;
        for (auto& line : page->emit()) {
            page_out += line + "\n";
        }

//        std::cout << page_out << std::endl;
    }
}

// MARK: - Singular Compilers

auto luatool::codegen::documentation::generator::construct_availability_table(std::shared_ptr<lua_api::ast::symbol> symbol, std::shared_ptr<table> cxx_table) -> void
{
    cxx_table->add_row({ "Introduced", symbol->introduced_version() });
    if (symbol->is_deprecated()) {
        cxx_table->add_row({ "Deprecated", symbol->deprecated_version() });
    }
}

auto luatool::codegen::documentation::generator::compile(std::shared_ptr<page> page, std::shared_ptr<lua_api::ast::symbol> symbol) -> void
{
    auto cxx_table = page->add_child<table>(std::initializer_list<std::string>({ "C++ Information", "Value" }));
    cxx_table->add_row({ "Source File", "`" + symbol->include_path() + "`" });
    cxx_table->add_row({ "Symbol", "`" + symbol->cxx_resolved_identifier() + "`" });
    construct_availability_table(symbol, cxx_table);

    if (!symbol->raw_documentation().empty()) {
        page->add_child<heading>("Description", 3);
        page->add_child<text>(symbol->documentation());
    }
}

auto luatool::codegen::documentation::generator::compile(std::shared_ptr<lua_api::ast::lua_namespace> ns) -> std::shared_ptr<page>
{
    auto page = std::make_shared<documentation::page>(ns->object_symbol()->lua_identifier());
    compile(page, ns->object_symbol());

    auto variables = ns->variables();
    if (!variables.empty()) {
        page->add_child<horizontal_rule>();
        page->add_child<heading>("Variables", 2);
        for (auto variable : variables) {
            compile(page, variable, ns->object_symbol());
        }
    }

    auto properties = ns->properties();
    if (!properties.empty()) {
        page->add_child<horizontal_rule>();
        page->add_child<heading>("Properties", 2);
        for (auto property : properties) {
            compile(page, property, ns->object_symbol());
        }
    }

    auto functions = ns->functions();
    if (!functions.empty()) {
        page->add_child<horizontal_rule>();
        page->add_child<heading>("Functions", 2);
        for (auto function : functions) {
            compile(page, function, ns->object_symbol());
        }
    }

    return page;
}

auto luatool::codegen::documentation::generator::compile(std::shared_ptr<lua_api::ast::lua_class> klass) -> std::shared_ptr<page>
{
    auto page = std::make_shared<documentation::page>(klass->object_symbol()->lua_identifier());
    compile(page, klass->object_symbol());

    if (klass->has_constructor()) {
        page->add_child<heading>("Constructor", 2);
        page->add_child<heading>("`" + klass->object_symbol()->lua_resolved_identifier() + "()`");
    }

    auto variables = klass->variables();
    if (!variables.empty()) {
        page->add_child<horizontal_rule>();
        page->add_child<heading>("Variables", 2);
        for (auto variable : variables) {
            compile(page, variable, klass->object_symbol());
        }
    }

    auto properties = klass->properties();
    if (!properties.empty()) {
        page->add_child<horizontal_rule>();
        page->add_child<heading>("Properties", 2);
        for (auto property : properties) {
            compile(page, property, klass->object_symbol());
        }
    }

    auto functions = klass->functions();
    if (!functions.empty()) {
        page->add_child<horizontal_rule>();
        page->add_child<heading>("Functions", 2);
        for (auto function : functions) {
            compile(page, function, klass->object_symbol());
        }
    }

    return page;
}

auto luatool::codegen::documentation::generator::compile(std::shared_ptr<page> page, std::shared_ptr<lua_api::ast::lua_function> function, std::shared_ptr<lua_api::ast::symbol> owner) -> void
{
    // Build a representation of the function name.
    std::string function_label = owner->lua_identifier();
    if (function->is_static()) {
        function_label += ".";
    }
    else {
        function_label += ":";
    }
    function_label += function->symbol()->lua_identifier() + "()";
    // TODO: Add parameters into the documentation
    page->add_child<heading>("`" + function_label + "`", 3);

    // Add some basic meta data
    auto cxx_table = page->add_child<table>(std::initializer_list<std::string>({ "Aspect", "Value" }));
    cxx_table->add_row({ "C++ Symbol", "`" + function->symbol()->cxx_resolved_identifier() + "()`" });
    construct_availability_table(function->symbol(), cxx_table);

    // Add some documentation for the function.
    if (function->symbol()->documentation().empty()) {
        page->add_child<text>("No description available.");
    }
    else {
        page->add_child<text>(function->symbol()->documentation());
    }
}

auto luatool::codegen::documentation::generator::compile(std::shared_ptr<page> page, std::shared_ptr<lua_api::ast::lua_property> property, std::shared_ptr<lua_api::ast::symbol> owner) -> void
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
        page->add_child<text>(property->getter()->documentation());
    }

    if (property->has_setter() && !property->setter()->documentation().empty()) {
        page->add_child<heading>("Setter Description", 4);
        page->add_child<text>(property->setter()->documentation());
    }
}


auto luatool::codegen::documentation::generator::compile(std::shared_ptr<page> page, std::shared_ptr<lua_api::ast::lua_variable> variable, std::shared_ptr<lua_api::ast::symbol> owner) -> void
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
        page->add_child<text>(variable->symbol()->documentation());
    }

}