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

#include "lua/ast/symbol.hpp"
#include "lua/ast/lua_parameter.hpp"
#include "lua/ast/type.hpp"

namespace luatool::lua_api::ast
{
    struct lua_function
    {
        typedef std::size_t key;

        lua_function() = default;
        explicit lua_function(std::shared_ptr<ast::symbol> symbol);

        [[nodiscard]] auto is_defined() const -> bool;
        [[nodiscard]] auto is_static() const -> bool;
        [[nodiscard]] auto symbol() const -> std::shared_ptr<ast::symbol>;

        [[nodiscard]] auto parameters() const -> const std::vector<lua_parameter>&;
        auto add_parameter(const lua_parameter& parameter) -> void;

        [[nodiscard]] auto return_type() const -> const ast::type&;
        auto set_return_type(const ast::type& type) -> void;

        [[nodiscard]] auto hash_key() const -> key;

    private:
        std::shared_ptr<ast::symbol> m_symbol;
        ast::type m_return_type;
        std::vector<lua_parameter> m_parameters;
    };
}


namespace std
{
    template<>
    struct hash<luatool::lua_api::ast::lua_function>
    {
        auto operator()(const luatool::lua_api::ast::lua_function& function) const -> luatool::lua_api::ast::lua_function::key
        {
            return std::hash<std::string>()(function.symbol()->cxx_identifier());
        }
    };
}