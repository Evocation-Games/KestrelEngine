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

#include "lua/ast/lua_property.hpp"

// MARK: - Construction

// MARK: - Querying

auto kdtool::lua_api::ast::lua_property::is_defined() const -> bool
{
    return m_getter->is_defined() || m_setter->is_defined();
}

auto kdtool::lua_api::ast::lua_property::is_static() const -> bool
{
    if ((m_setter && m_setter->is_defined()) && (m_getter && m_getter->is_defined()) && (m_getter->is_static() != m_setter->is_static())) {
        // The getter and the setter are not equivalent! This is an illegal state.
        throw std::logic_error("The getter and setter of the Lua Property do not agree on their 'static' state.");
    }

    return (m_getter && m_getter->is_defined() && m_getter->is_static()) || (m_setter && m_setter->is_defined() && m_setter->is_static());
}

auto kdtool::lua_api::ast::lua_property::getter() const -> std::shared_ptr<ast::symbol>
{
    return m_getter;
}

auto kdtool::lua_api::ast::lua_property::setter() const -> std::shared_ptr<ast::symbol>
{
    return m_setter;
}

auto kdtool::lua_api::ast::lua_property::has_getter() const -> bool
{
    return m_getter && m_getter->is_defined();
}

auto kdtool::lua_api::ast::lua_property::has_setter() const -> bool
{
    return m_setter && m_setter->is_defined();
}

auto kdtool::lua_api::ast::lua_property::add_getter(std::shared_ptr<ast::symbol> symbol) -> void
{
    m_getter = symbol;
    recompute_hash_key();
}

auto kdtool::lua_api::ast::lua_property::add_setter(std::shared_ptr<ast::symbol> symbol) -> void
{
    m_setter = symbol;
    recompute_hash_key();
}

// MARK: - Hash Key

auto kdtool::lua_api::ast::lua_property::recompute_hash_key() -> void
{
    // The key needs to be the Lua Identifier for the property. If we are missing the getter, then use the setter.
    // They _must_ be the same, so this shouldn't be an issue.
    if (m_getter) {
        if (m_setter && (m_getter->lua_resolved_identifier() != m_setter->lua_resolved_identifier())) {
            // The getter and the setter are not equivalent! This is an illegal state.
            throw std::logic_error("Lua property has differently identified getter and setters.");
        }
        m_hash_key = m_getter->lua_identifier_hash();
    }
    else if (m_setter) {
        m_hash_key = m_setter->lua_identifier_hash();
    }
    else {
        // We don't have a getter or a setter, this is _technically_ an illegal state to be in.
        throw std::logic_error("Lua property has neither a getter or a setter. This is an illegal state to be in.");
    }
}

auto kdtool::lua_api::ast::lua_property::hash_key() const -> key
{
    return m_hash_key;
}