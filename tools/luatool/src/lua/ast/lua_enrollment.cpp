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

#include "lua/ast/lua_enrollment.hpp"

// MARK: - Construction

template<typename T>
    requires luatool::lua_api::ast::is_lua_object<T>
luatool::lua_api::ast::lua_enrollment::lua_enrollment(const T *object, std::shared_ptr<ast::symbol> symbol)
{
    m_symbol = object->object_symbol()->member(symbol);
    m_reference = object->object_reference();
}

luatool::lua_api::ast::lua_enrollment::lua_enrollment(std::shared_ptr<ast::symbol> symbol)
{
    m_symbol = symbol;
}

// MARK: - Querying

auto luatool::lua_api::ast::lua_enrollment::is_defined() const -> bool
{
    return m_symbol->is_defined();
}

auto luatool::lua_api::ast::lua_enrollment::requires_runtime() const -> bool
{
    return m_requires_runtime;
}

auto luatool::lua_api::ast::lua_enrollment::requires_custom_name() const -> bool
{
    return m_custom_name;
}

auto luatool::lua_api::ast::lua_enrollment::symbol() const -> std::shared_ptr<ast::symbol>
{
    return m_symbol;
}

// MARK: - Configuration

auto luatool::lua_api::ast::lua_enrollment::set_reference(const ast::lua_reference &reference) -> void
{
    m_reference = reference;
}

auto luatool::lua_api::ast::lua_enrollment::set_requires_runtime(bool f) -> void
{
    m_requires_runtime = f;
}

auto luatool::lua_api::ast::lua_enrollment::set_requires_custom_name(bool f) -> void
{
    m_custom_name = f;
}

auto luatool::lua_api::ast::lua_enrollment::set_uses_custom_implementation(bool f) -> void
{
    m_custom_implementation = f;
}