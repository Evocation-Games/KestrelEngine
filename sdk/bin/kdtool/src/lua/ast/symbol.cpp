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

#include <sstream>
#include "lua/ast/symbol.hpp"
#include "analyzer/clang.hpp"

// MARK: - Construction

kdtool::lua_api::ast::symbol::symbol(const symbol &existing)
{
    m_raw_documentation = existing.raw_documentation();
    m_include_path = existing.m_include_path;
    cxx.is_static = existing.cxx.is_static;
    cxx.identifier = existing.cxx.identifier;
    cxx.path = existing.cxx.path;
    lua.identifier = existing.lua.identifier;
    lua.path = existing.lua.path;
}

kdtool::lua_api::ast::symbol::symbol(symbol &&old) noexcept
    : m_raw_documentation(std::move(old.m_raw_documentation)),
      m_include_path(std::move(old.m_include_path))
{
    cxx.is_static = old.cxx.is_static;
    cxx.identifier = std::move(old.cxx.identifier);
    cxx.path = std::move(old.cxx.path);
    lua.identifier = std::move(old.lua.identifier);
    lua.path = std::move(old.lua.path);
}

auto kdtool::lua_api::ast::symbol::make_cxx(const std::string &identifier) -> std::shared_ptr<ast::symbol>
{
    auto symbol = std::make_shared<ast::symbol>();
    symbol->cxx.identifier = identifier;
    return symbol;
}

auto kdtool::lua_api::ast::symbol::make_cxx(CXCursor identifier) -> std::shared_ptr<ast::symbol>
{
    return make_cxx(clang::spelling(identifier));
}

auto kdtool::lua_api::ast::symbol::make_cxx(const std::string& identifier, bool is_static, const ast::namespace_path& path) -> std::shared_ptr<ast::symbol>
{
    auto symbol = std::make_shared<ast::symbol>();
    symbol->cxx.identifier = identifier;
    symbol->cxx.is_static = is_static;
    symbol->cxx.path = path;
    return symbol;
}

auto kdtool::lua_api::ast::symbol::make_cxx(CXCursor identifier, const ast::namespace_path& path) -> std::shared_ptr<ast::symbol>
{
    return std::move(make_cxx(clang::spelling(identifier), clang::is_static(identifier), path));
}

auto kdtool::lua_api::ast::symbol::make_lua(const std::string& identifier, const ast::namespace_path& path) -> std::shared_ptr<ast::symbol>
{
    auto symbol = std::make_shared<ast::symbol>();
    symbol->lua.identifier = identifier;
    symbol->lua.path = path;
    return symbol;
}


// MARK: - Assignment Operators

auto kdtool::lua_api::ast::symbol::operator=(const symbol &existing) -> struct symbol&
{
    if (this != &existing) {
        m_include_path = existing.m_include_path;
        m_raw_documentation = existing.m_raw_documentation;
        cxx.is_static = existing.cxx.is_static;
        cxx.identifier = existing.cxx.identifier;
        cxx.path = existing.cxx.path;
        lua.identifier = existing.lua.identifier;
        lua.path = existing.lua.path;
    }
    return *this;
}

auto kdtool::lua_api::ast::symbol::operator=(symbol&& existing) noexcept -> struct symbol&
{
    if (this != &existing) {
        m_include_path = std::move(existing.m_include_path);
        m_raw_documentation = std::move(existing.m_raw_documentation);
        cxx.is_static = existing.cxx.is_static;
        cxx.identifier = std::move(existing.cxx.identifier);
        cxx.path = std::move(existing.cxx.path);
        lua.identifier = std::move(existing.lua.identifier);
        lua.path = std::move(existing.lua.path);
    }
    return *this;
}

// MARK: - Querying

auto kdtool::lua_api::ast::symbol::include_path() const -> std::string
{
    return m_include_path;
}

auto kdtool::lua_api::ast::symbol::set_include_path(const std::string &path) -> void
{
    m_include_path = path;
    m_include_path.erase(0, path.find("libKestrel"));
}

auto kdtool::lua_api::ast::symbol::is_defined() const -> bool
{
    return is_cxx_defined() || is_lua_defined();
}

auto kdtool::lua_api::ast::symbol::is_cxx_defined() const -> bool
{
    return !cxx.identifier.empty();
}

auto kdtool::lua_api::ast::symbol::is_lua_defined() const -> bool
{
    return !lua.identifier.empty();
}

auto kdtool::lua_api::ast::symbol::is_static() const -> bool
{
    return cxx.is_static;
}

auto kdtool::lua_api::ast::symbol::cxx_identifier() const -> std::string
{
    return cxx.identifier;
}

auto kdtool::lua_api::ast::symbol::cxx_namespace_path() const -> ast::namespace_path
{
    return cxx.path;
}

auto kdtool::lua_api::ast::symbol::cxx_namespace_path_string() const -> std::string
{
    return cxx.path.path_string(ast::namespace_path::cxx_delimiter);
}

auto kdtool::lua_api::ast::symbol::cxx_resolved_identifier() const -> std::string
{
    return cxx.path.path_string(cxx.identifier, ast::namespace_path::cxx_delimiter);
}

auto kdtool::lua_api::ast::symbol::lua_identifier() const -> std::string
{
    return lua.identifier;
}

auto kdtool::lua_api::ast::symbol::lua_namespace_path() const -> ast::namespace_path
{
    return lua.path;
}

auto kdtool::lua_api::ast::symbol::lua_namespace_path_string() const -> std::string
{
    return lua.path.path_string(ast::namespace_path::lua_delimiter);
}

auto kdtool::lua_api::ast::symbol::lua_resolved_identifier(const char *delimiter) const -> std::string
{
    return lua.path.path_string(lua.identifier, delimiter);
}

// MARK: - Modifiers

auto kdtool::lua_api::ast::symbol::make_static() -> void
{
    cxx.is_static = true;
}

auto kdtool::lua_api::ast::symbol::apply_cxx_identifier(const std::string& identifier) -> void
{
    if (identifier.find_first_of(ast::namespace_path::cxx_delimiter) != std::string::npos) {
        // The symbol is a namespace path...
        cxx.path = {};
        std::string s(identifier);
        std::string delimiter = ast::namespace_path::cxx_delimiter;
        while (!s.empty()) {
            auto it = s.find_first_of(delimiter);
            if (it == std::string::npos) {
                lua.identifier = s;
                break;
            }
            else {
                lua.path.push(s.substr(0, it));
                s.erase(it + delimiter.size());
            }
        }
    }
    else {
        cxx.identifier = identifier;
    }
}

auto kdtool::lua_api::ast::symbol::apply_cxx_identifier(CXCursor identifier) -> void
{
    apply_cxx_identifier(clang::spelling(identifier));
}

auto kdtool::lua_api::ast::symbol::apply_cxx_namespace(const ast::namespace_path& path) -> void
{
    cxx.path = path;
}


auto kdtool::lua_api::ast::symbol::apply_lua_identifier(const std::string& identifier) -> void
{
    if (identifier.find_first_of(ast::namespace_path::lua_delimiter) != std::string::npos) {
        // The symbol is a namespace path...
        lua.path = {};
        std::string s(identifier);
        std::string delimiter = ast::namespace_path::lua_delimiter;
        while (!s.empty()) {
            auto it = s.find_first_of(delimiter);
            if (it == std::string::npos) {
                lua.identifier = s;
                break;
            }
            else {
                lua.path.push(s.substr(0, it));
                s.erase(0, it + delimiter.size());
            }
        }
    }
    else {
        lua.identifier = identifier;
    }
}

auto kdtool::lua_api::ast::symbol::apply_lua_namespace(const ast::namespace_path& path) -> void
{
    lua.path = path;
}

// MARK: - Members

auto kdtool::lua_api::ast::symbol::cxx_member(const std::string& member_name) const -> std::shared_ptr<ast::symbol>
{
    auto member = std::make_shared<ast::symbol>(*this);
    member->cxx.identifier = member_name;
    member->cxx.path.push(cxx.identifier);
    return member;
}

auto kdtool::lua_api::ast::symbol::member(CXCursor member_name) const -> std::shared_ptr<ast::symbol>
{
    return cxx_member(clang::spelling(member_name));
}

auto kdtool::lua_api::ast::symbol::member(std::shared_ptr<ast::symbol> member_symbol) const -> std::shared_ptr<ast::symbol>
{
    auto member = std::make_shared<ast::symbol>(*this);
    member->cxx.identifier = member_symbol->cxx.identifier;
    member->cxx.path.push(cxx.identifier);
    member->lua = member_symbol->lua;
    return member;
}

// MARK: - Hashing

auto kdtool::lua_api::ast::symbol::cxx_identifier_hash() const -> hash_value
{
    return std::hash<std::string>()(cxx_resolved_identifier());
}

auto kdtool::lua_api::ast::symbol::lua_identifier_hash() const -> hash_value
{
    return std::hash<std::string>()(lua_resolved_identifier());
}

// MARK: - Documentation

auto kdtool::lua_api::ast::symbol::raw_documentation() const -> std::string
{
    return m_raw_documentation;
}

auto kdtool::lua_api::ast::symbol::apply_raw_documentation(const std::string &documentation) -> void
{
    m_raw_documentation = documentation.empty() ? "" : documentation;
    m_documentation = symbol_documentation(m_raw_documentation);
}

auto kdtool::lua_api::ast::symbol::documentation() const -> ast::symbol_documentation
{
    return m_documentation;
}

// MARK: - Availablility

auto kdtool::lua_api::ast::symbol::introduced_version() const -> std::string
{
    return m_introduced_version;
}

auto kdtool::lua_api::ast::symbol::set_version_introduced(const std::string& str) -> void
{
    std::string key;
    std::string buffer;
    for (const auto& c : str) {
        if (c == ' ' || c == '"') {
            continue;
        }

        if (c == '=') {
            key = buffer;
            buffer.clear();
        }
        else {
            buffer += c;
        }
    }

    if (key == "available") {
        m_introduced_version = buffer;
    }
    else {
        m_introduced_version = "Unknown";
    }
}


auto kdtool::lua_api::ast::symbol::is_deprecated() const -> bool
{
    return !m_deprecated_version.empty();
}

auto kdtool::lua_api::ast::symbol::deprecated_version() const -> std::string
{
    return m_deprecated_version;
}

auto kdtool::lua_api::ast::symbol::set_version_deprecated(const std::string& str) -> void
{
    m_deprecated_version = str;
}

auto kdtool::lua_api::ast::symbol::attach_template_symbol(const std::string &str) -> std::shared_ptr<symbol>
{
    return symbol::make_cxx(cxx.identifier + str, cxx.is_static, cxx.path);
}