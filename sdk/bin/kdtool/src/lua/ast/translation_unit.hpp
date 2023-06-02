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
#include <vector>
#include <unordered_map>
#include <clang-c/Index.h>
#include "lua/ast/lua_namespace.hpp"
#include "lua/ast/lua_class.hpp"
#include "lua/ast/lua_enum.hpp"

namespace kdtool::lua_api::ast
{
    struct translation_unit
    {
        translation_unit(const std::string &path, bool parse_documentation, const std::vector<std::string> &includes);

        ~translation_unit();

        auto process() -> void;

        [[nodiscard]] auto reference() const -> CXTranslationUnit;

        [[nodiscard]] auto path() const -> std::string;

        auto add_namespaces(const std::unordered_map<ast::symbol::hash_value, std::shared_ptr<ast::lua_namespace>>& namespaces) -> void;
        auto add_classes(const std::unordered_map<ast::symbol::hash_value, std::shared_ptr<ast::lua_class>>& classes) -> void;
        auto add_enums(const std::unordered_map<ast::symbol::hash_value, std::shared_ptr<ast::lua_enum>>& enums) -> void;

        [[nodiscard]] auto namespaces() const -> std::vector<std::shared_ptr<ast::lua_namespace>>;
        [[nodiscard]] auto classes() const -> std::vector<std::shared_ptr<ast::lua_class>>;
        [[nodiscard]] auto enums() const -> std::vector<std::shared_ptr<ast::lua_enum>>;

    private:
        std::string m_path;
        CXTranslationUnit m_tu { nullptr };
        CXIndex m_index { nullptr };

        std::unordered_map<ast::symbol::hash_value, std::shared_ptr<ast::lua_namespace>> m_namespaces;
        std::unordered_map<ast::symbol::hash_value, std::shared_ptr<ast::lua_class>> m_classes;
        std::unordered_map<ast::symbol::hash_value, std::shared_ptr<ast::lua_enum>> m_enums;

    };
}