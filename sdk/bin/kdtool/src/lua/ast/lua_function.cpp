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

#include "lua/ast/lua_function.hpp"

// MARK: - Construction

kdtool::lua_api::ast::lua_function::lua_function(std::shared_ptr<ast::symbol> symbol)
    : m_symbol(symbol)
{}

// MARK: - Querying

auto kdtool::lua_api::ast::lua_function::is_defined() const -> bool
{
    return m_symbol->is_defined();
}

auto kdtool::lua_api::ast::lua_function::is_static() const -> bool
{
    return m_symbol->is_static();
}

auto kdtool::lua_api::ast::lua_function::symbol() const -> std::shared_ptr<ast::symbol>
{
    return m_symbol;
}

// MARK: - Parameters

auto kdtool::lua_api::ast::lua_function::parameters() const -> const std::vector<lua_parameter> &
{
    return m_parameters;
}

auto kdtool::lua_api::ast::lua_function::add_parameter(const lua_parameter &parameter) -> void
{
    m_parameters.emplace_back(parameter);
}

// MARK: - Return Types

auto kdtool::lua_api::ast::lua_function::return_type() const -> const ast::type &
{
    return m_return_type;
}

auto kdtool::lua_api::ast::lua_function::set_return_type(const ast::type &type) -> void
{
    m_return_type = type;
}

// MARK: - Hash Keys

auto kdtool::lua_api::ast::lua_function::hash_key() const -> key
{
    return m_symbol->cxx_identifier_hash();
}