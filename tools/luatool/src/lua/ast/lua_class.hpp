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

#pragma once

#include <unordered_map>
#include "lua/ast/symbol.hpp"
#include "lua/ast/namespace_path.hpp"
#include "lua/ast/type.hpp"
#include "lua/ast/lua_constructor.hpp"
#include "lua/ast/lua_reference.hpp"
#include "lua/ast/lua_enrollment.hpp"
#include "lua/ast/lua_property.hpp"
#include "lua/ast/lua_function.hpp"
#include "lua/ast/lua_variable.hpp"

namespace luatool::lua_api::ast
{
    struct lua_class
    {
        typedef std::size_t key;

        lua_class() = default;
        explicit lua_class(std::shared_ptr<ast::symbol> symbol);

        [[nodiscard]] auto object_reference() const -> ast::lua_reference;
        [[nodiscard]] auto object_symbol() const -> std::shared_ptr<ast::symbol>;

        [[nodiscard]] auto enrollment() const -> const ast::lua_enrollment&;
        [[nodiscard]] auto as_namespace() const -> bool;

        auto set_enrollment(const ast::lua_enrollment& enrollment) -> void;
        auto set_as_namespace() -> void;

        [[nodiscard]] auto properties() const -> std::vector<std::shared_ptr<lua_property>>;
        [[nodiscard]] auto has_property(std::shared_ptr<ast::symbol> symbol) const -> bool;
        auto get_property(std::shared_ptr<ast::symbol> symbol) -> std::shared_ptr<lua_property>;
        auto add_property_getter(std::shared_ptr<ast::symbol> symbol) -> std::shared_ptr<lua_property>;
        auto add_property_setter(std::shared_ptr<ast::symbol> symbol) -> std::shared_ptr<lua_property>;

        [[nodiscard]] auto functions() const -> std::vector<std::shared_ptr<lua_function>>;
        [[nodiscard]] auto has_function(std::shared_ptr<ast::symbol> symbol) const -> bool;
        auto get_function(std::shared_ptr<ast::symbol> symbol) -> std::shared_ptr<lua_function>;
        auto add_function(std::shared_ptr<ast::symbol> symbol) -> std::shared_ptr<lua_function>;

        [[nodiscard]] auto variables() const -> std::vector<std::shared_ptr<lua_variable>>;
        [[nodiscard]] auto has_variable(std::shared_ptr<ast::symbol> symbol) const -> bool;
        auto get_variable(std::shared_ptr<ast::symbol> symbol) -> std::shared_ptr<lua_variable>;
        auto add_variable(std::shared_ptr<ast::symbol> symbol, bool is_mutable) -> std::shared_ptr<lua_variable>;

        [[nodiscard]] auto has_constructor() const -> bool;
        [[nodiscard]] auto is_building_constructor() const -> bool;
        auto constructor() -> ast::lua_constructor&;
        auto begin_constructor() -> void;
        auto end_constructor() -> void;

        [[nodiscard]] auto is_templated() const -> bool;
        [[nodiscard]] auto templates() const -> const std::vector<std::shared_ptr<ast::symbol>>&;
        [[nodiscard]] auto variants() const -> const std::unordered_map<std::string, ast::type>&;
        auto add_template_typename(const std::string& name) -> void;
        auto add_template_typename(std::shared_ptr<ast::symbol> name) -> void;
        auto add_template_variant(const std::string& name, const ast::type& type) -> void;

        [[nodiscard]] auto hash_key() const -> key;

    private:
        bool m_editing_constructor { false };
        bool m_as_namespace { false };
        std::shared_ptr<ast::symbol> m_symbol;
        ast::lua_constructor m_constructor { false };
        ast::lua_reference m_reference;
        ast::lua_enrollment m_enrollment;
        std::vector<std::shared_ptr<ast::symbol>> m_template_typenames;
        std::unordered_map<std::string, ast::type> m_template_variants;
        std::unordered_map<ast::lua_property::key, std::shared_ptr<ast::lua_property>> m_properties;
        std::unordered_map<ast::lua_function::key, std::shared_ptr<ast::lua_function>> m_functions;
        std::unordered_map<ast::lua_variable::key, std::shared_ptr<ast::lua_variable>> m_variables;
    };
}