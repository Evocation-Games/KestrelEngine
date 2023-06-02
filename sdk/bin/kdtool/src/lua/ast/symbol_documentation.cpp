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

#include "lua/ast/symbol_documentation.hpp"

// MARK: - Construction

kdtool::lua_api::ast::symbol_documentation::parameter::parameter(const std::string &name, const std::string &description)
    : m_name(name), m_description(description)
{}

// MARK: - Accessors

auto kdtool::lua_api::ast::symbol_documentation::parameter::name() const -> std::string
{
    return m_name;
}

auto kdtool::lua_api::ast::symbol_documentation::parameter::description() const -> std::string
{
    return m_description;
}

auto kdtool::lua_api::ast::symbol_documentation::description() const -> std::string
{
    return m_description;
}

auto kdtool::lua_api::ast::symbol_documentation::example() const -> std::string
{
    return m_example;
}

auto kdtool::lua_api::ast::symbol_documentation::warning() const -> std::string
{
    return m_warning;
}

auto kdtool::lua_api::ast::symbol_documentation::return_description() const -> std::string
{
    return m_return;
}

auto kdtool::lua_api::ast::symbol_documentation::parameters() const -> std::vector<parameter>
{
    return m_parameters;
}

auto kdtool::lua_api::ast::symbol_documentation::set_description(const std::string &description) -> void
{
    m_description = description;
}

auto kdtool::lua_api::ast::symbol_documentation::set_example(const std::string &example) -> void
{
    m_example = example;
}

auto kdtool::lua_api::ast::symbol_documentation::set_warning(const std::string &warning) -> void
{
    m_warning = warning;
}

auto kdtool::lua_api::ast::symbol_documentation::set_return_description(const std::string &return_description) -> void
{
    m_return = return_description;
}

auto kdtool::lua_api::ast::symbol_documentation::add_parameter(const kdtool::lua_api::ast::symbol_documentation::parameter &parameter) -> void
{
    m_parameters.emplace_back(parameter);
}
