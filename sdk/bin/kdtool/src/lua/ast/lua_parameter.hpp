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

#include <string>
#include <clang-c/Index.h>
#include "lua/ast/type.hpp"
#include "lua/ast/symbol.hpp"

namespace kdtool::lua_api::ast
{
    struct lua_parameter
    {
        lua_parameter() = default;
        explicit lua_parameter(CXCursor cursor);
        explicit lua_parameter(const std::string& name, ast::type type = {});
        explicit lua_parameter(std::shared_ptr<ast::symbol> symbol, ast::type type = {});

        [[nodiscard]] auto symbol() const -> std::shared_ptr<ast::symbol>;
        [[nodiscard]] auto type() const -> ast::type;

    private:
        std::shared_ptr<ast::symbol> m_symbol;
        ast::type m_type;
    };
}