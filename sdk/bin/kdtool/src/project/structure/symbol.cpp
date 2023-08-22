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

kdtool::project::structure::symbol::symbol(
    const std::string &name,
    const std::shared_ptr<struct symbol> &parent,
    const std::shared_ptr<documentation::object>& documentation,
    const std::shared_ptr<struct construct_definition> &definition
)
    : m_name(name), m_parent(parent), m_definition(definition), m_documentation(documentation)
{
    add_source_identifier(foundation::string::trim(name), name);
}

auto kdtool::project::structure::symbol::from_resolved_name(const std::string& resolved) -> std::shared_ptr<symbol>
{
    const auto components = foundation::string::split(resolved, ".");
    std::shared_ptr<symbol> symbol;
    for (const auto& name : components) {
        auto new_symbol = std::make_shared<struct symbol>(name, symbol);
        new_symbol->set_basename(name);
        symbol = new_symbol;
    }
    return symbol;
}

// MARK: - Accessors

auto kdtool::project::structure::symbol::hash() const -> std::string
{
    return resolved_name();
}

auto kdtool::project::structure::symbol::display_name() const -> std::string
{
    return m_display_name.empty() ? name() : m_display_name;
}

auto kdtool::project::structure::symbol::set_display_name(const std::string& display_name) -> void
{
    m_display_name = display_name;
}

auto kdtool::project::structure::symbol::set_basename(const std::string& basename) -> void
{
    m_basename = basename;
}

auto kdtool::project::structure::symbol::basename() const -> std::string
{
    return m_basename.empty() ? m_name : m_basename;
}

auto kdtool::project::structure::symbol::name() const -> std::string
{
    return m_name;
}

auto kdtool::project::structure::symbol::is_root() const -> bool
{
    return m_parent.lock() == nullptr;
}

auto kdtool::project::structure::symbol::definition() const -> std::weak_ptr<struct construct_definition>
{
    return m_definition;
}

auto kdtool::project::structure::symbol::documentation() const -> std::weak_ptr<struct documentation::object>
{
    return m_documentation;
}

auto kdtool::project::structure::symbol::set_definition(const std::shared_ptr<struct construct_definition> &definition) -> void
{
    m_definition = definition;
}

auto kdtool::project::structure::symbol::set_documentation(const std::shared_ptr<struct documentation::object> &documentation) -> void
{
    m_documentation = documentation;
}

auto kdtool::project::structure::symbol::set_source_identifier(const std::string &identifier, const std::string &resolved) -> void
{
    m_source_symbols.clear();
    add_source_identifier(identifier, resolved);
}

auto kdtool::project::structure::symbol::add_source_identifier(const std::string &identifier, const std::string &resolved, bool propogate) -> void
{
    struct source_symbol symbol;
    symbol.identifier = identifier;
    symbol.resolved = resolved.empty() ? identifier : resolved;

    if (auto parent = m_parent.lock()) {
        // TODO: Pass the scope resolution operator here...
        auto components = foundation::string::split(symbol.resolved, "::");
        components.pop_back();
        if ((!components.empty()) && propogate) {
            auto parent_id = components.back();
            auto parent_resolved = foundation::string::joined(components, "::");
            parent->set_source_identifier(parent_id, parent_resolved);
        }
    }

    if (!m_source_symbols.empty() && !m_added_source_symbols) {
        m_added_source_symbols = true;
        m_source_symbols.clear();
    }

    m_source_symbols.emplace_back(std::move(symbol));
}

auto kdtool::project::structure::symbol::source_template_parameters_attachment(const std::vector<std::string>& subs) const -> std::string
{
    if (m_source_template_parameters.empty()) {
        return "";
    }
    if (subs.size() == m_source_template_parameters.size()) {
        return "<" + foundation::string::joined(subs, ", ") + ">";
    }
    else {
        return "<" + foundation::string::joined(m_source_template_parameters, ", ") + ">";
    }
}

auto kdtool::project::structure::symbol::source_identifier() const -> std::string
{
    return m_source_symbols.front().identifier + source_template_parameters_attachment();
}

auto kdtool::project::structure::symbol::source_resolved_identifier(
    const std::string& scope_resolution_operator,
    const std::string& joined_delimiter,
    const std::vector<std::string>& template_parameter_subs
) const -> std::string {
    if (!scope_resolution_operator.empty() && !joined_delimiter.empty()) {
        return foundation::string::joined(foundation::string::split(m_source_symbols.front().resolved, scope_resolution_operator), joined_delimiter) + source_template_parameters_attachment();
    }

    // If we have a parent, then try and resolve with it instead.
    if (const auto& parent = m_parent.lock()) {
        return parent->source_resolved_identifier(scope_resolution_operator, joined_delimiter, template_parameter_subs)
             + scope_resolution_operator
             + m_source_symbols.front().identifier + source_template_parameters_attachment(template_parameter_subs);
    }

    // We have no parent, so just use the resolved string
    return m_source_symbols.front().resolved + source_template_parameters_attachment(template_parameter_subs);
}

auto kdtool::project::structure::symbol::source_resolved_identifier(bool including_identifier, const std::string& scope_resolution_operator) const -> std::string
{
    // If we have a parent, then try and resolve with it instead.
    if (const auto& parent = m_parent.lock()) {
        return parent->source_resolved_identifier(scope_resolution_operator)
               + scope_resolution_operator
               + m_source_symbols.front().identifier + source_template_parameters_attachment();
    }

    auto out = m_source_symbols.front().resolved;
    if (!out.empty()) {
        out += scope_resolution_operator;
    }
    return out + m_source_symbols.front().identifier + source_template_parameters_attachment();
}

auto kdtool::project::structure::symbol::all_source_resolved_identifiers() const -> std::vector<std::string>
{
    std::vector<std::string> out;
    for (const auto& symbol : m_source_symbols) {
        out.emplace_back(symbol.resolved);
    }
    return std::move(out);
}

auto kdtool::project::structure::symbol::is_static() const -> bool
{
    return m_static;
}

auto kdtool::project::structure::symbol::is_constant() const -> bool
{
    return m_constant;
}

auto kdtool::project::structure::symbol::is_mutable() const -> bool
{
    return m_mutable;
}

auto kdtool::project::structure::symbol::is_reference_stub() const -> bool
{
    return m_reference_stub;
}

auto kdtool::project::structure::symbol::make_static() -> void
{
    m_static = true;
}

auto kdtool::project::structure::symbol::make_constant() -> void
{
    m_constant = true;
}

auto kdtool::project::structure::symbol::make_mutable() -> void
{
    m_mutable = true;
}

auto kdtool::project::structure::symbol::make_reference_stub() -> void
{
    m_reference_stub = true;
}

// MARK: - Resolution

auto kdtool::project::structure::symbol::resolved_name(const std::string &delimiter) const -> std::string
{
    if (is_root()) {
        return display_name();
    }
    else {
        return parent().lock()->resolved_name(delimiter) + delimiter + display_name();
    }
}

auto kdtool::project::structure::symbol::filename() const -> foundation::filesystem::path
{
    if (is_root()) {
        return { basename() };
    }
    else {
        return parent().lock()->filename().child(basename());
    }
}

// MARK: - Built in

auto kdtool::project::structure::symbol::is_built_in() const -> bool
{
    return m_built_in;
}

auto kdtool::project::structure::symbol::make_built_in() -> void
{
    m_built_in = true;
}

// MARK: - Versions

auto kdtool::project::structure::symbol::available() const -> std::optional<struct version>
{
    return m_available;
}

auto kdtool::project::structure::symbol::set_available(const struct kdtool::project::structure::version &version) -> void
{
    m_available = version;
}

auto kdtool::project::structure::symbol::deprecated() const -> std::optional<struct version>
{
    return m_deprecated;
}

auto kdtool::project::structure::symbol::set_deprecated(const struct kdtool::project::structure::version &version) -> void
{
    m_deprecated = version;
}

// MARK: - Children

auto kdtool::project::structure::symbol::parent() const -> std::weak_ptr<struct symbol>
{
    return m_parent;
}

auto kdtool::project::structure::symbol::add_child(const std::weak_ptr<struct symbol> &symbol) -> void
{
    if (auto sym = symbol.lock()) {
        sym->m_parent = shared_from_this();
        m_children.emplace_back(sym);
    }
}

auto kdtool::project::structure::symbol::children() const -> std::vector<std::weak_ptr<struct symbol>>
{
    return m_children;
}

// MARK: - Template Parameters

auto kdtool::project::structure::symbol::add_template_parameter(const std::string& parameter) -> void
{
    m_template_parameters.emplace_back(parameter);
}

auto kdtool::project::structure::symbol::template_parameters() const -> std::vector<std::string>
{
    return m_template_parameters;
}

auto kdtool::project::structure::symbol::add_source_template_parameter(const std::string& parameter) -> void
{
    m_source_template_parameters.emplace_back(parameter);
}

auto kdtool::project::structure::symbol::source_template_parameters() const -> std::vector<std::string>
{
    return m_source_template_parameters;
}