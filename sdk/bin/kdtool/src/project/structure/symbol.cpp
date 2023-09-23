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

#include <libFoundation/string/trim.hpp>
#include <libFoundation/string/split.hpp>
#include <libFoundation/string/joined.hpp>
#include "project/structure/symbol.hpp"

// MARK: - Construction

kdtool::project::structure::symbol::symbol(const std::string &name, const std::shared_ptr<struct symbol>& parent)
{
    m_source.identifier = name;
    m_parent = parent;
}

// MARK: - Identity

auto kdtool::project::structure::symbol::hash() const -> std::string
{
    return resolved_source_identifier();
}

// MARK: - Access

auto kdtool::project::structure::symbol::is_static() const -> bool
{
    return m_access.is_static;
}

auto kdtool::project::structure::symbol::is_builtin() const -> bool
{
    return m_access.is_builtin;
}

auto kdtool::project::structure::symbol::is_constant() const -> bool
{
    return m_access.is_constant;
}

auto kdtool::project::structure::symbol::is_reference_stub() const -> bool
{
    return m_access.is_reference;
}

auto kdtool::project::structure::symbol::make_static() -> void
{
    m_access.is_static = true;
}

auto kdtool::project::structure::symbol::make_builtin() -> void
{
    m_access.is_builtin = true;
}

auto kdtool::project::structure::symbol::make_constant() -> void
{
    m_access.is_constant = true;
}

auto kdtool::project::structure::symbol::make_reference() -> void
{
    m_access.is_reference = true;
}

// MARK: - File

auto kdtool::project::structure::symbol::filename() const -> foundation::filesystem::path
{
    auto it = shared_from_this();
    std::vector<std::string> components;
    while (it != nullptr) {
        components.insert(components.begin(), it->m_lua.identifiers.begin(), it->m_lua.identifiers.end());
        it = it->parent().lock();
    }

    foundation::filesystem::path path(components);
    return path;
}

// MARK: - Definition

auto kdtool::project::structure::symbol::definition() const -> std::weak_ptr<struct construct_definition>
{
    return m_definition;
}

auto kdtool::project::structure::symbol::set_definition(const std::weak_ptr<struct construct_definition> &definition) -> void
{
    m_definition = definition;
}

// MARK: - Parent/Children

auto kdtool::project::structure::symbol::is_root() const -> bool
{
    return parent().lock() == nullptr;
}

auto kdtool::project::structure::symbol::parent() const -> std::weak_ptr<struct symbol>
{
    return m_parent.lock();
}

auto kdtool::project::structure::symbol::children() const -> std::vector<std::weak_ptr<struct symbol>>
{
    return m_children;
}

auto kdtool::project::structure::symbol::add_child(const std::shared_ptr<struct symbol> &symbol) -> void
{
    symbol->m_parent = shared_from_this();
    m_children.emplace_back(symbol);
}

// MARK: - Version

auto kdtool::project::structure::symbol::deprecation() const -> std::optional<struct version>
{
    return m_version.deprecated;
}

auto kdtool::project::structure::symbol::available() const -> std::optional<struct version>
{
    return m_version.available;
}

auto kdtool::project::structure::symbol::set_deprecation_version(const struct kdtool::project::structure::version &version) -> void
{
    m_version.deprecated = version;
}

auto kdtool::project::structure::symbol::set_available_version(const struct kdtool::project::structure::version &version) -> void
{
    m_version.available = version;
}

// MARK: - Documentation

auto kdtool::project::structure::symbol::documentation() const -> std::weak_ptr<struct documentation::object>
{
    return m_documentation;
}

auto kdtool::project::structure::symbol::set_documentation(const std::shared_ptr<struct documentation::object> &documentation) -> void
{
    m_documentation = documentation;
}

auto kdtool::project::structure::symbol::display_name() const -> std::string
{
    return m_lua.display_name.empty() ? lua_identifier() : m_lua.display_name;
}

auto kdtool::project::structure::symbol::set_display_name(const std::string &name) -> void
{
    m_lua.display_name = name;
}

// MARK: - Source

auto kdtool::project::structure::symbol::source_file() const -> foundation::filesystem::path
{
    return m_source.filename;
}

auto kdtool::project::structure::symbol::source_identifier(const std::vector<std::string>& template_parameters) const -> std::string
{
    std::string out = m_source.identifier;
    if (!template_parameters.empty()) {
        out += "<" + foundation::string::joined(template_parameters, ",") + ">";
    }
    return out;
}

auto kdtool::project::structure::symbol::resolved_source_identifier(const std::string& scope_operator,
                                                                    const std::vector<std::string>& template_parameters) const -> std::string
{
    auto templates = template_parameters;
    if (templates.empty()) {
        templates = m_source.templates;
    }

    if (auto parent = m_parent.lock()) {
        return foundation::string::joined({ parent->resolved_source_identifier(scope_operator), source_identifier(templates) }, scope_operator);
    }
    else {
        return source_identifier();
    }
}

auto kdtool::project::structure::symbol::source_template_parameters() const -> std::vector<std::string>
{
    return m_source.templates;
}

auto kdtool::project::structure::symbol::set_source_file(const foundation::filesystem::path &path) -> void
{
    m_source.filename = path;
}

auto kdtool::project::structure::symbol::set_source_identifier(const std::string &identifier) -> void
{
    m_source.identifier = identifier;
}

auto kdtool::project::structure::symbol::add_source_template_parameter(const std::string &parameter) -> void
{
    m_source.templates.emplace_back(parameter);
}

// MARK: - Lua

auto kdtool::project::structure::symbol::has_lua_identifier() const -> bool
{
    return !m_lua.identifiers.empty();
}

auto kdtool::project::structure::symbol::lua_identifier() const -> std::string
{
    if (has_lua_identifier()) {
        return m_lua.identifiers.back();
    }
    else {
        return "";
    }
}

auto kdtool::project::structure::symbol::resolved_lua_identifier() const -> std::string
{
    if (has_lua_identifier()) {
        auto self = foundation::string::joined(m_lua.identifiers, ".");
        if (auto parent = m_parent.lock()) {
            if (parent->has_lua_identifier()) {
                self = foundation::string::joined({ parent->resolved_lua_identifier(), self }, ".");
            }
        }
        return self;
    }
    else {
        return "";
    }
}

auto kdtool::project::structure::symbol::set_lua_identifier(const std::string &identifier) -> void
{
    m_lua.identifiers = foundation::string::split(identifier, ".");
}