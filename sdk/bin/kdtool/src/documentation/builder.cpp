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

#include <chrono>
#include <thread>
#include <libCodeGen/spec/markup/anchor.hpp>
#include <libCodeGen/spec/markup/list.hpp>
#include "documentation/builder.hpp"
#include "documentation/components/availability_table.hpp"
#include "documentation/components/description.hpp"
#include "documentation/components/example.hpp"
#include "documentation/components/constructor.hpp"
#include "documentation/components/warning.hpp"
#include "documentation/components/property_index_list.hpp"
#include "documentation/components/function_index_list.hpp"
#include "documentation/components/text.hpp"
#include "documentation/components/function.hpp"
#include "documentation/components/parameters_table.hpp"
#include "documentation/components/return_description.hpp"
#include "documentation/components/case_index_list.hpp"

// MARK: - Construction

kdtool::documentation::builder::builder(const std::shared_ptr<kdtool::project>& project, const std::string& path)
    : m_project(project), m_path(path)
{}

// MARK: - Building

auto kdtool::documentation::builder::build() -> void
{
    for (const auto& lua_namespace : m_project->all_namespaces()) {
        build(lua_namespace);
    }

    for (const auto& lua_class : m_project->all_classes()) {
        build(lua_class);
    }

    for (const auto& lua_enum : m_project->all_enums()) {
        build(lua_enum);
    }
}

auto kdtool::documentation::builder::export_as(const std::shared_ptr<codegen::markup_language>& language) -> void
{
    // Setup the root documentation path
    auto docs_root = m_path.child(language->lc_name());
    docs_root.create_directory(false);

    // Index Path
    auto index_page = std::make_shared<codegen::spec::markup::page>("Kestrel Lua API");
    index_page->add<codegen::spec::markup::heading>("Kestrel Lua API", 1);

    auto index_list = index_page->add<codegen::spec::markup::list>();
    for (const auto& directory : m_directories) {
        build_directory(docs_root, language, directory.second, index_list);
    }

    index_page->save(language, docs_root.child("index." + language->extension()));
}

auto kdtool::documentation::builder::build_directory(
    const foundation::filesystem::path& docs_root,
    const std::shared_ptr<codegen::markup_language>& language,
    const std::shared_ptr<struct directory_entry>& entry,
    std::shared_ptr<codegen::spec::markup::list>& list
) -> void {
    auto item_body = std::make_shared<codegen::spec::markup::markup_node>();

    if (entry->page) {
        auto path = docs_root;
        std::string relative_path;
        for (const auto& component : entry->path) {
            path = path.child(component);
            relative_path += component + "/";
        }
        path = path.child(entry->name + "." + language->extension());
        relative_path += entry->name + "." + language->extension();

        path.create_directory(true);
        entry->page->save(language, path);

        if (entry->name == "index") {
            item_body->add<codegen::spec::markup::anchor>(text(entry->title), relative_path);
        }
        else {
            return;
        }
    }
    else {
        item_body->add<codegen::spec::markup::text>(entry->title, codegen::spec::markup::text::style::bold);
    }

    if (!entry->children.empty()) {
        auto sub_list = item_body->add<codegen::spec::markup::list>();
        for (const auto& child : entry->children) {
            build_directory(docs_root, language, child.second, sub_list);
        }
    }

    list->add_item(item_body);
}

auto kdtool::documentation::builder::directory_entry(const std::vector<std::string>& path, std::shared_ptr<struct directory_entry> entry) -> std::shared_ptr<struct directory_entry>
{
    if (path.empty()) {
        return entry;
    }

    auto& entries = entry ? entry->children : m_directories;
    auto it = entries.find(path.front());
    if (it != entries.end()) {
        // Found the item
        return directory_entry({ path.begin() + 1, path.end() }, it->second);
    }
    else {
        // Not found.
        auto new_entry = std::make_shared<struct directory_entry>();

        new_entry->name = path.front();
        new_entry->title = path.front();
        entries.emplace(new_entry->name, new_entry);

        if (path.size() == 2 && path.back() == "index") {
            return new_entry;
        }
        else {
            return directory_entry({path.begin() + 1, path.end()}, new_entry);
        }
    }
}

auto kdtool::documentation::builder::add_page(const std::string& title, const std::vector<std::string>& path, const std::string& name, bool no_title) -> std::shared_ptr<codegen::spec::markup::page>
{
    auto page = std::make_shared<codegen::spec::markup::page>(title, name);

    if (!no_title) {
        page->add<codegen::spec::markup::heading>(title, 1);
    }

    auto entry_path = path;
    entry_path.emplace_back(name);

    auto entry = directory_entry(entry_path);
    entry->name = name;
    entry->title = title.empty() ? name : title;
    entry->path = path;
    entry->page = page;
    return page;
}

// MARK: - Namespace

auto kdtool::documentation::builder::build(const std::shared_ptr<lua_api::ast::lua_namespace>& lua_namespace) -> std::shared_ptr<codegen::spec::markup::page>
{
    auto symbol = lua_namespace->object_symbol();
    auto page = add_page(symbol->lua_resolved_identifier(), symbol->lua_namespace_path().nested(symbol->lua_identifier()).path_vector());

    page->add(availability_table(symbol).build(shared_from_this()));
    page->add(description(symbol).build(shared_from_this()));

    if (!symbol->documentation().example().empty()) {
        page->add(example(symbol).build(shared_from_this()));
    }

    if (!lua_namespace->properties().empty()) {
        page->add(property_index_list(lua_namespace->properties(), symbol).build(shared_from_this()));
    }

    if (!lua_namespace->functions().empty()) {
        page->add(function_index_list(lua_namespace->functions(), symbol).build(shared_from_this()));
    }

    return page;
}

// MARK: - Class

auto kdtool::documentation::builder::build(const std::shared_ptr<lua_api::ast::lua_class>& lua_class) -> std::shared_ptr<codegen::spec::markup::page>
{
    auto symbol = lua_class->object_symbol();
    auto page = add_page(symbol->lua_resolved_identifier(), symbol->lua_namespace_path().nested(symbol->lua_identifier()).path_vector());

    page->add(availability_table(symbol).build(shared_from_this()));

    if (!symbol->documentation().warning().empty()) {
        page->add(warning(symbol).build(shared_from_this()));
    }

    page->add(description(symbol).build(shared_from_this()));

    if (!symbol->documentation().example().empty()) {
        page->add(example(symbol).build(shared_from_this()));
    }

    if (lua_class->has_constructor()) {
        page->add(constructor(symbol, lua_class->constructor()).build(shared_from_this()));
    }

    if (!lua_class->properties().empty()) {
        page->add(property_index_list(lua_class->properties(), symbol).build(shared_from_this()));
    }

    if (!lua_class->functions().empty()) {
        page->add(function_index_list(lua_class->functions(), symbol).build(shared_from_this()));
    }

    return page;
}

// MARK: - Enum

auto kdtool::documentation::builder::build(const std::shared_ptr<lua_api::ast::lua_enum>& lua_enum) -> std::shared_ptr<codegen::spec::markup::page>
{
    auto symbol = lua_enum->object_symbol();
    auto page = add_page(symbol->lua_resolved_identifier(), symbol->lua_namespace_path().nested(symbol->lua_identifier()).path_vector());

    page->add(availability_table(symbol).build(shared_from_this()));
    page->add(description(symbol).build(shared_from_this()));

    if (!symbol->documentation().example().empty()) {
        page->add(example(symbol).build(shared_from_this()));
    }

    page->add(case_index_list(lua_enum).build(shared_from_this()));

    return page;
}

// MARK: - Function Page

auto kdtool::documentation::builder::add_function_page(const std::shared_ptr<lua_api::ast::lua_function>& lua_function, const std::shared_ptr<lua_api::ast::symbol>& owner) -> std::shared_ptr<codegen::spec::markup::page>
{
    auto symbol = lua_function->symbol();
    auto path = owner->lua_namespace_path().nested(owner->lua_identifier());
    auto page = add_page(symbol->lua_resolved_identifier(), path.path_vector(), symbol->lua_resolved_identifier(), true);

    page->add<codegen::spec::markup::anchor>(text(path.path_string(".")), "index");
    page->add<codegen::spec::markup::heading>(function(symbol).build(shared_from_this()), 1);

    page->add(availability_table(symbol).build(shared_from_this()));

    if (!symbol->documentation().warning().empty()) {
        page->add(warning(symbol).build(shared_from_this()));
    }

    page->add(description(symbol).build(shared_from_this()));

    if (!symbol->documentation().parameters().empty()) {
        page->add(parameters_table(symbol).build(shared_from_this()));
    }

    if (!symbol->documentation().return_description().empty()) {
        page->add(return_description(symbol).build(shared_from_this()));
    }

    if (!symbol->documentation().example().empty()) {
        page->add(example(symbol).build(shared_from_this()));
    }

    return page;
}

// MARK: - Variable Page

auto kdtool::documentation::builder::add_variable_page(const std::shared_ptr<lua_api::ast::lua_variable>& lua_variable, const std::shared_ptr<lua_api::ast::symbol>& owner) -> std::shared_ptr<codegen::spec::markup::page>
{
    auto symbol = lua_variable->symbol();
    auto path = owner->lua_namespace_path().nested(owner->lua_identifier());
    auto page = add_page(symbol->lua_resolved_identifier(), path.path_vector(), symbol->lua_resolved_identifier(), true);

    page->add<codegen::spec::markup::anchor>(text(path.path_string(".")), "index");
    page->add<codegen::spec::markup::heading>(function(symbol).build(shared_from_this()), 1);

    page->add(availability_table(symbol).build(shared_from_this()));

    if (!symbol->documentation().warning().empty()) {
        page->add(warning(symbol).build(shared_from_this()));
    }

    page->add(description(symbol).build(shared_from_this()));

    if (!symbol->documentation().example().empty()) {
        page->add(example(symbol).build(shared_from_this()));
    }

    return page;
}

// MARK: - Property Page

auto kdtool::documentation::builder::add_property_page(const std::shared_ptr<lua_api::ast::lua_property>& lua_property, const std::shared_ptr<lua_api::ast::symbol>& owner) -> std::shared_ptr<codegen::spec::markup::page>
{
    auto symbol = lua_property->getter();
    auto path = owner->lua_namespace_path().nested(owner->lua_identifier());
    auto page = add_page(symbol->lua_resolved_identifier(), path.path_vector(), symbol->lua_resolved_identifier(), true);

    page->add<codegen::spec::markup::anchor>(text(path.path_string(".")), "index");
    page->add<codegen::spec::markup::heading>(symbol->lua_resolved_identifier(), 1);

    page->add(availability_table(symbol).build(shared_from_this()));

    if (!symbol->documentation().warning().empty()) {
        page->add(warning(symbol).build(shared_from_this()));
    }

    page->add(description(symbol).build(shared_from_this()));

    if (!symbol->documentation().return_description().empty()) {
        page->add(return_description(symbol).build(shared_from_this()));
    }

    if (!symbol->documentation().example().empty()) {
        page->add(example(symbol).build(shared_from_this()));
    }

    return page;
}

// MARK: - Enum Case Page

auto kdtool::documentation::builder::add_case_page(const std::shared_ptr<lua_api::ast::lua_enum_case>& lua_enum_case, const std::shared_ptr<lua_api::ast::symbol>& owner) -> std::shared_ptr<codegen::spec::markup::page>
{
    auto symbol = lua_enum_case->object_symbol();
    auto path = owner->lua_namespace_path().nested(owner->lua_identifier());
    auto page = add_page(symbol->lua_resolved_identifier(), path.path_vector(), symbol->lua_resolved_identifier(), true);

    page->add<codegen::spec::markup::anchor>(text(path.path_string(".")), "index");
    page->add<codegen::spec::markup::heading>(path.nested(symbol->lua_resolved_identifier()).path_string("."), 1);

    page->add(availability_table(symbol).build(shared_from_this()));

    if (!symbol->documentation().warning().empty()) {
        page->add(warning(symbol).build(shared_from_this()));
    }

    page->add(description(symbol).build(shared_from_this()));

    if (!symbol->documentation().example().empty()) {
        page->add(example(symbol).build(shared_from_this()));
    }

    return page;
}

// MARK: - Index Path

auto kdtool::documentation::builder::index_for(const std::shared_ptr<lua_api::ast::symbol>& symbol, const std::shared_ptr<codegen::markup_language>& language) -> std::string
{
    return symbol->lua_namespace_path().nested(symbol->lua_identifier()).path_string("/") + "/index";
}

auto kdtool::documentation::builder::build_index(const std::shared_ptr<codegen::markup_language>& language) -> std::shared_ptr<codegen::spec::markup::page>
{
    auto page = add_page("Kestrel Lua API", {});

    if (!m_project->all_namespaces().empty()) {
        page->add<codegen::spec::markup::heading>("Namespaces", 2);

        auto list = page->add<codegen::spec::markup::list>();
        for (const auto& lua_namespace : m_project->all_namespaces()) {
            list->add_item(std::make_shared<codegen::spec::markup::anchor>(
                text(lua_namespace->object_symbol()->lua_resolved_identifier()),
                index_for(lua_namespace->object_symbol(), language)
            ));
        }
    }

    if (!m_project->all_classes().empty()) {
        page->add<codegen::spec::markup::heading>("Classes", 2);

        auto list = page->add<codegen::spec::markup::list>();
        for (const auto& lua_class : m_project->all_classes()) {
            list->add_item(std::make_shared<codegen::spec::markup::anchor>(
                text(lua_class->object_symbol()->lua_resolved_identifier()),
                index_for(lua_class->object_symbol(), language)
            ));
        }
    }

    if (!m_project->all_enums().empty()) {
        page->add<codegen::spec::markup::heading>("Enums & Constants", 2);

        auto list = page->add<codegen::spec::markup::list>();
        for (const auto& lua_enum : m_project->all_enums()) {
            list->add_item(std::make_shared<codegen::spec::markup::anchor>(
                text(lua_enum->object_symbol()->lua_resolved_identifier()),
                index_for(lua_enum->object_symbol(), language)
            ));
        }
    }

    return page;
}