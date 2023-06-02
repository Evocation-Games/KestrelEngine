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

#include "lua/ast/lua_parameter.hpp"
#include "analyzer/clang.hpp"

// MARK: - Construction

kdtool::lua_api::ast::lua_parameter::lua_parameter(CXCursor cursor)
    : m_symbol(symbol::make_cxx(cursor))
{

}

kdtool::lua_api::ast::lua_parameter::lua_parameter(const std::string &name, ast::type type)
    : m_symbol(symbol::make_cxx(name)), m_type(type)
{

}

kdtool::lua_api::ast::lua_parameter::lua_parameter(std::shared_ptr<ast::symbol> symbol, ast::type type)
    : m_symbol(symbol), m_type(type)
{

}

// MARK: - Accessors

auto kdtool::lua_api::ast::lua_parameter::symbol() const -> std::shared_ptr<ast::symbol>
{
    return m_symbol;
}

auto kdtool::lua_api::ast::lua_parameter::type() const -> ast::type
{
    return m_type;
}