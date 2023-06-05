// Copyright (c) 2023 Tom Hancocks
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
#include <string>
#include <unordered_map>
#include "lua/ast/translation_unit.hpp"
#include "lua/ast/lua_namespace.hpp"
#include "lua/ast/lua_class.hpp"
#include "lua/ast/lua_enum.hpp"

namespace kdtool
{
    struct project
    {
        project() = default;

        auto add_include_path(const std::string& path) -> void;
        auto add_translation_unit(const std::string& path) -> void;

        auto add(const std::shared_ptr<lua_api::ast::lua_namespace>& lua_namespace) -> void;
        auto add(const std::shared_ptr<lua_api::ast::lua_class>& lua_class) -> void;
        auto add(const std::shared_ptr<lua_api::ast::lua_enum>& lua_enum) -> void;

        [[nodiscard]] auto include_paths() const -> std::vector<std::string>;
        [[nodiscard]] auto all_namespaces() const -> std::vector<std::shared_ptr<lua_api::ast::lua_namespace>>;
        [[nodiscard]] auto all_classes() const -> std::vector<std::shared_ptr<lua_api::ast::lua_class>>;
        [[nodiscard]] auto all_enums() const -> std::vector<std::shared_ptr<lua_api::ast::lua_enum>>;

    private:
        std::vector<std::string> m_include_dirs;
        std::unordered_map<lua_api::ast::lua_namespace::key, std::shared_ptr<lua_api::ast::lua_namespace>> m_namespaces;
        std::unordered_map<lua_api::ast::lua_class::key, std::shared_ptr<lua_api::ast::lua_class>> m_classes;
        std::unordered_map<lua_api::ast::lua_enum::key, std::shared_ptr<lua_api::ast::lua_enum>> m_enums;
    };
}