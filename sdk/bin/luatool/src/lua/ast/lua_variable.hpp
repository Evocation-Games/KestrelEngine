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
#include "lua/ast/type.hpp"

namespace luatool::lua_api::ast
{
    struct lua_variable
    {
        typedef std::size_t key;

        lua_variable() = default;
        explicit lua_variable(std::shared_ptr<ast::symbol> symbol, bool is_mutable = false);

        [[nodiscard]] auto is_defined() const -> bool;
        [[nodiscard]] auto is_static() const -> bool;
        [[nodiscard]] auto is_mutable() const -> bool;
        [[nodiscard]] auto symbol() const -> std::shared_ptr<ast::symbol>;

        [[nodiscard]] auto hash_key() const -> key;

    private:
        std::shared_ptr<ast::symbol> m_symbol;
        bool m_mutable { false };
    };
}