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

#include "lua/ast/namespace_path.hpp"
#include "analyzer/clang.hpp"

// MARK: - Construction

kdtool::lua_api::ast::namespace_path::namespace_path(const namespace_path &path)
    : m_ns_names(path.m_ns_names)
{}

kdtool::lua_api::ast::namespace_path::namespace_path(namespace_path &&path) noexcept
    : m_ns_names(std::move(path.m_ns_names))
{}

kdtool::lua_api::ast::namespace_path::namespace_path(const std::string& ns_name)
    : m_ns_names(1, ns_name)
{}

kdtool::lua_api::ast::namespace_path::namespace_path(const std::vector<std::string>& ns_names)
    : m_ns_names(ns_names)
{}

kdtool::lua_api::ast::namespace_path::namespace_path(CXCursor ns_name)
    : m_ns_names(1, clang::spelling(ns_name))
{}

// MARK: - Nesting

auto kdtool::lua_api::ast::namespace_path::nested(const std::string& ns_name) const -> namespace_path
{
    namespace_path new_path(*this);
    new_path.push(ns_name);
    return new_path;
}

auto kdtool::lua_api::ast::namespace_path::nested(CXCursor ns_name) const -> namespace_path
{
    return nested(clang::spelling(ns_name));
}

auto kdtool::lua_api::ast::namespace_path::parent() const -> namespace_path
{
    namespace_path new_path(*this);
    new_path.pop();
    return new_path;
}

auto kdtool::lua_api::ast::namespace_path::push(const std::string& ns_name) -> void
{
    m_ns_names.insert(m_ns_names.end(), ns_name);
}

auto kdtool::lua_api::ast::namespace_path::push(CXCursor ns_name) -> void
{
    push(clang::spelling(ns_name));
}

auto kdtool::lua_api::ast::namespace_path::pop() -> void
{
    if (!m_ns_names.empty()) {
        m_ns_names.pop_back();
    }
}

// MARK: - Querying

auto kdtool::lua_api::ast::namespace_path::has_namespace(const std::string& ns_name) const -> bool
{
    return (!m_ns_names.empty()) && (std::find(m_ns_names.begin(), m_ns_names.end(), ns_name) != m_ns_names.end());
}

auto kdtool::lua_api::ast::namespace_path::is_namespace(const std::string& ns_name) const -> bool
{
    return (!m_ns_names.empty()) && (m_ns_names.back() == ns_name);
}

auto kdtool::lua_api::ast::namespace_path::is_namespace(CXCursor ns_name) const -> bool
{
    return is_namespace(clang::spelling(ns_name));
}

auto kdtool::lua_api::ast::namespace_path::path_string(const char *delimiter) const -> std::string
{
    return std::move(path_string("", delimiter));
}

auto kdtool::lua_api::ast::namespace_path::path_string(const std::string &name, const char *delimiter) const -> std::string
{
    std::string out;
    for (const std::string& ns_name : m_ns_names) {
        out += ns_name + delimiter;
    }

    if (name.empty() && out.ends_with(delimiter)) {
        // Remove the trailing delimiter.
        out.erase(out.find_last_of(delimiter));
    }
    else {
        out += name;
    }

    return std::move(out);
}

// MARK: - Visiting / Iteration

auto kdtool::lua_api::ast::namespace_path::path_vector() const -> const std::vector<std::string>&
{
    return m_ns_names;
}

// MARK: - Assignment Operators

auto kdtool::lua_api::ast::namespace_path::operator=(const namespace_path &path) -> namespace_path &
{
    if (this != &path) {
        m_ns_names = path.m_ns_names;
    }
    return *this;
}

auto kdtool::lua_api::ast::namespace_path::operator=(namespace_path&& path) noexcept -> namespace_path&
{
    if (this != &path) {
        m_ns_names = std::move(path.m_ns_names);
    }
    return *this;
}
