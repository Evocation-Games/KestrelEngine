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

#include <stdexcept>
#include "lua/ast/lua_class.hpp"

// MARK: - Construction

kdtool::lua_api::ast::lua_class::lua_class(std::shared_ptr<ast::symbol> symbol)
    : m_symbol(symbol)
{}

// MARK: - Lua Object Helpers

auto kdtool::lua_api::ast::lua_class::object_symbol() const -> std::shared_ptr<ast::symbol>
{
    return m_symbol;
}

auto kdtool::lua_api::ast::lua_class::object_reference() const -> ast::lua_reference
{
    return m_reference;
}

// MARK: - Enrollment

auto kdtool::lua_api::ast::lua_class::enrollment() const -> const ast::lua_enrollment &
{
    return m_enrollment;
}

auto kdtool::lua_api::ast::lua_class::set_enrollment(const ast::lua_enrollment& enrollment) -> void
{
    m_enrollment = enrollment;
}

// MARK: - Representation

auto kdtool::lua_api::ast::lua_class::as_namespace() const -> bool
{
    return m_as_namespace;
}

auto kdtool::lua_api::ast::lua_class::set_as_namespace() -> void
{
    m_as_namespace = true;
}

// MARK: - Properties

auto kdtool::lua_api::ast::lua_class::properties() const -> std::vector<std::shared_ptr<ast::lua_property>>
{
    std::vector<std::shared_ptr<ast::lua_property>> out;
    for (auto it : m_properties) {
        out.emplace_back(it.second);
    }
    return std::move(out);
}

auto kdtool::lua_api::ast::lua_class::has_property(std::shared_ptr<ast::symbol> symbol) const -> bool
{
    return m_properties.find(symbol->lua_identifier_hash()) != m_properties.end();
}

auto kdtool::lua_api::ast::lua_class::get_property(std::shared_ptr<ast::symbol> symbol) -> std::shared_ptr<lua_property>
{
    return m_properties.at(symbol->lua_identifier_hash());
}

auto kdtool::lua_api::ast::lua_class::add_property_getter(std::shared_ptr<ast::symbol> symbol) -> std::shared_ptr<lua_property>
{
    if (has_property(symbol)) {
        // We need to adjust an existing property.
        auto property = get_property(symbol);
        if (property->has_getter()) {
            throw std::logic_error("Attempt to overwrite an already existing property getter. (" + property->getter()->lua_identifier() + ")");
        }
        property->add_getter(symbol);
        return property;
    }
    else {
        auto property = std::make_shared<ast::lua_property>();
        property->add_getter(symbol);
        m_properties.emplace(std::pair(property->hash_key(), property));
        return get_property(symbol);
    }
}

auto kdtool::lua_api::ast::lua_class::add_property_setter(std::shared_ptr<ast::symbol> symbol) -> std::shared_ptr<lua_property>
{
    if (has_property(symbol)) {
        // We need to adjust an existing property.
        auto property = get_property(symbol);
        if (property->has_setter()) {
            throw std::logic_error("Attempt to overwrite an already existing property setter.");
        }
        property->add_setter(symbol);
        return property;
    }
    else {
        auto property = std::make_shared<ast::lua_property>();
        property->add_setter(symbol);
        m_properties.emplace(std::pair(property->hash_key(), property));
        return get_property(symbol);
    }
}

// MARK: - Functions

auto kdtool::lua_api::ast::lua_class::functions() const -> std::vector<std::shared_ptr<ast::lua_function>>
{
    std::vector<std::shared_ptr<ast::lua_function>> out;
    for (auto it : m_functions) {
        out.emplace_back(it.second);
    }
    return std::move(out);
}

auto kdtool::lua_api::ast::lua_class::has_function(std::shared_ptr<ast::symbol> symbol) const -> bool
{
    return m_functions.find(symbol->cxx_identifier_hash()) != m_functions.end();
}

auto kdtool::lua_api::ast::lua_class::get_function(std::shared_ptr<ast::symbol> symbol) -> std::shared_ptr<lua_function>
{
    auto it = m_functions.find(symbol->cxx_identifier_hash());
    return it->second;
}

auto kdtool::lua_api::ast::lua_class::add_function(std::shared_ptr<ast::symbol> symbol) -> std::shared_ptr<lua_function>
{
    auto function = std::make_shared<ast::lua_function>(symbol);
    m_functions.emplace(std::pair(function->hash_key(), function));
    return get_function(symbol);
}

// MARK: - Variables

auto kdtool::lua_api::ast::lua_class::variables() const -> std::vector<std::shared_ptr<lua_variable>>
{
    std::vector<std::shared_ptr<ast::lua_variable>> out;
    for (auto it : m_variables) {
        out.emplace_back(it.second);
    }
    return std::move(out);
}

auto kdtool::lua_api::ast::lua_class::has_variable(std::shared_ptr<ast::symbol> symbol) const -> bool
{
    return m_variables.find(symbol->cxx_identifier_hash()) != m_variables.end();
}

auto kdtool::lua_api::ast::lua_class::get_variable(std::shared_ptr<ast::symbol> symbol) -> std::shared_ptr<lua_variable>
{
    auto it = m_variables.find(symbol->cxx_identifier_hash());
    return it->second;
}

auto kdtool::lua_api::ast::lua_class::add_variable(std::shared_ptr<ast::symbol> symbol, bool is_mutable) -> std::shared_ptr<lua_variable>
{
    auto variable = std::make_shared<ast::lua_variable>(symbol, is_mutable);
    m_variables.emplace(std::pair(variable->hash_key(), variable));
    return variable;
}

// MARK: - Constructor

auto kdtool::lua_api::ast::lua_class::has_constructor() const -> bool
{
    return m_constructor.is_defined();
}

auto kdtool::lua_api::ast::lua_class::is_building_constructor() const -> bool
{
    return has_constructor() && m_editing_constructor;
}

auto kdtool::lua_api::ast::lua_class::constructor() -> ast::lua_constructor &
{
    return m_constructor;
}

auto kdtool::lua_api::ast::lua_class::begin_constructor() -> void
{
    m_constructor = lua_constructor(true, m_constructor.fixed_parameters());
    m_editing_constructor = true;
}

auto kdtool::lua_api::ast::lua_class::end_constructor() -> void
{
    m_editing_constructor = false;
}

// MARK: - Templates

auto kdtool::lua_api::ast::lua_class::is_templated() const -> bool
{
    return !m_template_typenames.empty();
}

auto kdtool::lua_api::ast::lua_class::templates() const -> const std::vector<std::shared_ptr<ast::symbol>> &
{
    return m_template_typenames;
}

auto kdtool::lua_api::ast::lua_class::variants() const -> const std::unordered_map<std::string, ast::type> &
{
    return m_template_variants;
}

auto kdtool::lua_api::ast::lua_class::add_template_typename(const std::string &name) -> void
{
    add_template_typename(ast::symbol::make_cxx(name));
}

auto kdtool::lua_api::ast::lua_class::add_template_typename(std::shared_ptr<ast::symbol> name) -> void
{
    m_template_typenames.emplace_back(name);
}

auto kdtool::lua_api::ast::lua_class::add_template_variant(const std::string& name, const ast::type& type) -> void
{
    m_template_variants.emplace(std::pair(name, type));
}

// MARK: - Hash Key

auto kdtool::lua_api::ast::lua_class::hash_key() const -> key
{
    return m_symbol->lua_identifier_hash();
}