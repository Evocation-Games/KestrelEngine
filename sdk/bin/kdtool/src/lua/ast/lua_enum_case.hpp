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

namespace kdtool::lua_api::ast
{
    struct lua_enum_case
    {
        typedef std::size_t key;

        lua_enum_case() = default;
        explicit lua_enum_case(std::shared_ptr<ast::symbol> symbol, const std::string& value);

        [[nodiscard]] auto object_symbol() const -> std::shared_ptr<ast::symbol>;
        [[nodiscard]] auto internal_value_symbol() const -> std::shared_ptr<ast::symbol>;
        [[nodiscard]] auto value() const -> std::string;

        [[nodiscard]] auto hash_key() const -> key;

    private:
        std::shared_ptr<ast::symbol> m_symbol;
        std::shared_ptr<ast::symbol> m_static_symbol;
        std::string m_value;
    };
}