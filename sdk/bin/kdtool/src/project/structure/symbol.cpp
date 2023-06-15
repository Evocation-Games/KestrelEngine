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
    m_source_symbol.identifier = foundation::string::trim(name);
    m_source_symbol.resolved = name;
}

// MARK: - Accessors

auto kdtool::project::structure::symbol::name() const -> std::string
{
    return m_name;
}

auto kdtool::project::structure::symbol::is_root() const -> bool
{
    return m_parent.lock() == nullptr;
}

auto kdtool::project::structure::symbol::parent() const -> std::weak_ptr<struct symbol>
{
    return m_parent;
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
    m_source_symbol.identifier = identifier;
    m_source_symbol.resolved = resolved.empty() ? identifier : resolved;
}

auto kdtool::project::structure::symbol::source_identifier() const -> std::string
{
    return m_source_symbol.identifier;
}

auto kdtool::project::structure::symbol::source_resolved_identifier(const std::string& scope_resolution_operator, const std::string& joined_delimiter) const -> std::string
{
    if (!scope_resolution_operator.empty() && !joined_delimiter.empty()) {
        return foundation::string::joined(foundation::string::split(m_source_symbol.resolved, scope_resolution_operator), joined_delimiter);
    }
    return m_source_symbol.resolved;
}

auto kdtool::project::structure::symbol::source_resolved_identifier(bool including_identifier, const std::string& scope_resolution_operator) const -> std::string
{
    auto out = m_source_symbol.resolved;
    if (!out.empty()) {
        out += scope_resolution_operator;
    }
    return out + m_source_symbol.identifier;
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
        return name();
    }
    else {
        return parent().lock()->resolved_name(delimiter) + delimiter + name();
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