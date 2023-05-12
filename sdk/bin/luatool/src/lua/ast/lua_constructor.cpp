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

#include "lua/ast/lua_constructor.hpp"

// MARK: - Construction

luatool::lua_api::ast::lua_constructor::lua_constructor(bool defined, const std::vector<ast::lua_parameter>& fixed_parameters)
    : m_defined(defined), m_fixed_parameters(fixed_parameters)
{}

// MARK: - Accessors

auto luatool::lua_api::ast::lua_constructor::is_defined() const -> bool
{
    return m_defined;
}

// MARK: - Parameters

auto luatool::lua_api::ast::lua_constructor::parameters() const -> const std::vector<ast::lua_parameter> &
{
    return m_parameters;
}

auto luatool::lua_api::ast::lua_constructor::fixed_parameters() const -> const std::vector<ast::lua_parameter> &
{
    return m_fixed_parameters;
}

auto luatool::lua_api::ast::lua_constructor::add_parameter(std::shared_ptr<ast::symbol> symbol, const ast::type &type) -> void
{
    add_parameter(ast::lua_parameter(symbol, type));
}

auto luatool::lua_api::ast::lua_constructor::add_parameter(const ast::lua_parameter &parameter) -> void
{
    for (auto fixed : m_fixed_parameters) {
        if (fixed.symbol()->cxx_identifier() == parameter.symbol()->cxx_identifier()) {
            m_parameters.emplace_back(fixed);
            return;
        }
    }
    m_parameters.emplace_back(parameter);
}

auto luatool::lua_api::ast::lua_constructor::add_fixed_parameter(const ast::lua_parameter &parameter) -> void
{
    m_fixed_parameters.emplace_back(parameter);
}