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

#include <stdexcept>
#include "lua/ast/symbol.hpp"

namespace luatool::lua_api::ast
{
    struct lua_property
    {
        typedef std::size_t key;

        lua_property() = default;

        [[nodiscard]] auto is_defined() const -> bool;
        [[nodiscard]] auto is_static() const -> bool;
        [[nodiscard]] auto has_getter() const -> bool;
        [[nodiscard]] auto has_setter() const -> bool;

        [[nodiscard]] auto getter() const -> std::shared_ptr<ast::symbol>;
        [[nodiscard]] auto setter() const -> std::shared_ptr<ast::symbol>;

        auto add_getter(std::shared_ptr<ast::symbol> symbol) -> void;
        auto add_setter(std::shared_ptr<ast::symbol> symbol) -> void;

        [[nodiscard]] auto hash_key() const -> key;

    private:
        key m_hash_key;
        std::shared_ptr<ast::symbol> m_getter;
        std::shared_ptr<ast::symbol> m_setter;

        auto recompute_hash_key() -> void;
    };
}

namespace std
{
    template<>
    struct hash<luatool::lua_api::ast::lua_property>
    {
        auto operator()(const luatool::lua_api::ast::lua_property& property) const -> luatool::lua_api::ast::lua_property::key
        {
            return property.hash_key();
        }
    };
}