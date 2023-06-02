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

#include "lua/ast/lua_enum.hpp"

// MARK: - Construction

kdtool::lua_api::ast::lua_enum::lua_enum(std::shared_ptr<ast::symbol> symbol)
    : m_symbol(symbol)
{}

// MARK: - Accessors

auto kdtool::lua_api::ast::lua_enum::object_symbol() const -> std::shared_ptr<ast::symbol>
{
    return m_symbol;
}

auto kdtool::lua_api::ast::lua_enum::enrollment() const -> const ast::lua_enrollment &
{
    return m_enrollment;
}

auto kdtool::lua_api::ast::lua_enum::set_enrollment(const ast::lua_enrollment& enrollment) -> void
{
    m_enrollment = enrollment;
}

// MARK: - Cases

auto kdtool::lua_api::ast::lua_enum::all_cases() const -> const std::unordered_map<ast::lua_enum_case::key, std::shared_ptr<ast::lua_enum_case>>&
{
    return m_all_cases;
}


auto kdtool::lua_api::ast::lua_enum::add_case(std::shared_ptr<ast::lua_enum_case> enum_case) -> void
{
    m_all_cases.emplace(std::pair(enum_case->hash_key(), enum_case));
}

// MARK: - Hash Key

auto kdtool::lua_api::ast::lua_enum::hash_key() const -> key
{
    return m_symbol->lua_identifier_hash();
}
