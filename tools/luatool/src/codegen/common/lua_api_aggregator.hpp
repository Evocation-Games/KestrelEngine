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

#include "lua/ast/lua_namespace.hpp"
#include "lua/ast/lua_class.hpp"
#include "lua/ast/lua_enum.hpp"
#include "lua/ast/lua_property.hpp"
#include "lua/ast/lua_variable.hpp"
#include "lua/ast/lua_function.hpp"

namespace luatool::codegen::common::api
{
    class aggregator
    {
    public:
        aggregator() = default;

        auto merge_namespace(std::shared_ptr<lua_api::ast::lua_namespace> ns) -> void;
        auto merge_class(std::shared_ptr<lua_api::ast::lua_class> klass) -> void;
        auto merge_enum(std::shared_ptr<lua_api::ast::lua_enum> lua_enum) -> void;

        [[nodiscard]] auto all_namespaces() const -> std::vector<std::shared_ptr<lua_api::ast::lua_namespace>>;
        [[nodiscard]] auto all_classes() const -> std::vector<std::shared_ptr<lua_api::ast::lua_class>>;
        [[nodiscard]] auto all_enums() const -> std::vector<std::shared_ptr<lua_api::ast::lua_enum>>;

    private:
        std::unordered_map<lua_api::ast::lua_namespace::key, std::shared_ptr<lua_api::ast::lua_namespace>> m_namespaces;
        std::unordered_map<lua_api::ast::lua_class::key, std::shared_ptr<lua_api::ast::lua_class>> m_classes;
        std::unordered_map<lua_api::ast::lua_enum::key, std::shared_ptr<lua_api::ast::lua_enum>> m_enums;
    };
}