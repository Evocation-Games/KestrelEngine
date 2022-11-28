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

#include "lua/ast/lua_variable.hpp"

// MARK: - Constructor

luatool::lua_api::ast::lua_variable::lua_variable(std::shared_ptr<ast::symbol> symbol, bool is_mutable)
    : m_symbol(symbol), m_mutable(is_mutable)
{}

// MARK: - Accessors

auto luatool::lua_api::ast::lua_variable::is_defined() const -> bool
{
    return m_symbol->is_defined();
}

auto luatool::lua_api::ast::lua_variable::is_static() const -> bool
{
    return m_symbol->is_static();
}

auto luatool::lua_api::ast::lua_variable::is_mutable() const -> bool
{
    return m_mutable;
}

auto luatool::lua_api::ast::lua_variable::symbol() const -> std::shared_ptr<ast::symbol>
{
    return m_symbol;
}

// MARK: - Hashing

auto luatool::lua_api::ast::lua_variable::hash_key() const -> key
{
    return m_symbol->lua_identifier_hash();
}
