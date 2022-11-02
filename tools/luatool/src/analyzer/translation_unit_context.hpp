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
#include <clang-c/Index.h>
#include "lua/annotations/annotation.hpp"

namespace luatool::lua_api::ast
{
    struct translation_unit;
    struct lua_namespace;
    struct lua_class;
    struct lua_enum;
    struct lua_enum_case;
}

namespace luatool::analyzer
{
    struct translation_unit_context
    {
        explicit translation_unit_context(const lua_api::ast::translation_unit* tu);

        auto start_traversing_ast() -> void;

        [[nodiscard]] auto namespaces() const -> const std::unordered_map<lua_api::ast::symbol::hash_value, std::shared_ptr<lua_api::ast::lua_namespace>>&;
        [[nodiscard]] auto classes() const -> const std::unordered_map<lua_api::ast::symbol::hash_value, std::shared_ptr<lua_api::ast::lua_class>>&;
        [[nodiscard]] auto enums() const -> const std::unordered_map<lua_api::ast::symbol::hash_value, std::shared_ptr<lua_api::ast::lua_enum>>&;

    private:
        static auto visit_node(CXCursor cursor, CXCursor parent, CXClientData data) -> CXChildVisitResult;
        [[nodiscard]] auto should_visit_node(CXCursor cursor) const -> bool;

        auto visit_annotation(CXCursor cursor, CXCursor parent) -> void;
        auto visit_namespace(CXCursor cursor) -> void;
        auto visit_class(CXCursor cursor) -> void;
        auto visit_enum(CXCursor cursor) -> void;
        auto visit_enum_case(CXCursor cursor) -> void;
        auto visit_parameter(CXCursor cursor) -> void;
        auto visit_constructor(CXCursor cursor) -> void;
        auto visit_template_parameter(CXCursor cursor, CXCursor parent) -> void;

        auto construct_symbol(CXCursor cursor, const lua_api::annotation& annotation) -> std::shared_ptr<lua_api::ast::symbol>;
        auto construct_namespace(CXCursor cursor, CXCursor parent, const lua_api::annotation& annotation) -> void;
        auto construct_class(CXCursor cursor, CXCursor parent, const lua_api::annotation& annotation) -> void;
        auto construct_enum(CXCursor cursor, CXCursor parent, const lua_api::annotation& annotation) -> void;
        auto construct_enum_case(CXCursor cursor, CXCursor parent, const lua_api::annotation& annotation) -> void;
        auto construct_constructor(CXCursor cursor, CXCursor parent, const lua_api::annotation& annotation) -> void;
        auto construct_function(CXCursor cursor, CXCursor parent, const lua_api::annotation& annotation) -> void;
        auto construct_getter(CXCursor cursor, CXCursor parent, const lua_api::annotation& annotation) -> void;
        auto construct_setter(CXCursor cursor, CXCursor parent, const lua_api::annotation& annotation) -> void;
        auto construct_variable(CXCursor cursor, CXCursor parent, const lua_api::annotation& annotation) -> void;
        auto construct_enrollment(CXCursor cursor, CXCursor parent, const lua_api::annotation& annotation) -> void;

    private:
        [[nodiscard]] auto namespace_path() const -> lua_api::ast::namespace_path;

    private:
        const lua_api::ast::translation_unit* m_tu { nullptr };
        std::vector<std::string> m_namespace_path;
        struct {
            std::shared_ptr<lua_api::ast::lua_namespace> current;
            std::unordered_map<lua_api::ast::symbol::hash_value, std::shared_ptr<lua_api::ast::lua_namespace>> all;
        } m_namespaces;
        struct {
            std::vector<std::string> templates;
            std::shared_ptr<lua_api::ast::lua_class> current;
            std::unordered_map<lua_api::ast::symbol::hash_value, std::shared_ptr<lua_api::ast::lua_class>> all;
        } m_classes;
        struct {
            std::shared_ptr<lua_api::ast::lua_enum> current;
            std::unordered_map<lua_api::ast::symbol::hash_value, std::shared_ptr<lua_api::ast::lua_enum>> all;
        } m_enum;
    };
}