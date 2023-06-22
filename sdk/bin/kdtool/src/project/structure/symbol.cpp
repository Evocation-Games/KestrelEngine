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

// MARK: - Accessors

auto kdtool::project::structure::symbol::display_name() const -> std::string
{
    return m_display_name.empty() ? name() : m_display_name;
}

auto kdtool::project::structure::symbol::set_display_name(const std::string& display_name) -> void
{
    m_display_name = display_name;
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
    add_source_identifier(identifier, resolved);
}

auto kdtool::project::structure::symbol::add_source_identifier(const std::string &identifier, const std::string &resolved) -> void
{
    struct source_symbol symbol;
    symbol.identifier = identifier;
    symbol.resolved = resolved.empty() ? identifier : resolved;

    if (!m_source_symbols.empty() && !m_added_source_symbols) {
        m_added_source_symbols = true;
        m_source_symbols.clear();
    }

    m_source_symbols.emplace_back(std::move(symbol));
}

auto kdtool::project::structure::symbol::source_identifier() const -> std::string
{
    return m_source_symbols.front().identifier;
}

auto kdtool::project::structure::symbol::source_resolved_identifier(const std::string& scope_resolution_operator, const std::string& joined_delimiter) const -> std::string
{
    if (!scope_resolution_operator.empty() && !joined_delimiter.empty()) {
        return foundation::string::joined(foundation::string::split(m_source_symbols.front().resolved, scope_resolution_operator), joined_delimiter);
    }
    return m_source_symbols.front().resolved;
}

auto kdtool::project::structure::symbol::source_resolved_identifier(bool including_identifier, const std::string& scope_resolution_operator) const -> std::string
{
    auto out = m_source_symbols.front().resolved;
    if (!out.empty()) {
        out += scope_resolution_operator;
    }
    return out + m_source_symbols.front().identifier;
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

auto kdtool::project::structure::symbol::make_static() -> void
{
    m_static = true;
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
        return { name() };
    }
    else {
        return parent().lock()->filename().child(name());
    }
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
    m_children.emplace_back(symbol);
}

auto kdtool::project::structure::symbol::children() const -> std::vector<std::weak_ptr<struct symbol>>
{
    return m_children;
}