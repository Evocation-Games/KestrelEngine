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

#include <vector>
#include "lua/ast/lua_parameter.hpp"

namespace luatool::lua_api::ast
{
    struct lua_constructor
    {
        lua_constructor() = default;
        explicit lua_constructor(bool defined, const std::vector<ast::lua_parameter>& fixed_parameters = {});

        [[nodiscard]] auto is_defined() const -> bool;
        [[nodiscard]] auto parameters() const -> const std::vector<ast::lua_parameter>&;
        [[nodiscard]] auto fixed_parameters() const -> const std::vector<ast::lua_parameter>&;

        auto add_parameter(std::shared_ptr<ast::symbol> symbol, const ast::type& type) -> void;
        auto add_parameter(const ast::lua_parameter& parameter) -> void;
        auto add_fixed_parameter(const ast::lua_parameter& parameter) -> void;

    private:
        bool m_defined { true };
        std::vector<ast::lua_parameter> m_parameters;
        std::vector<ast::lua_parameter> m_fixed_parameters;
    };
}