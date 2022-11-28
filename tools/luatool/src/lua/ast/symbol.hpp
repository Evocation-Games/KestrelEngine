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
#include <memory>
#include <clang-c/Index.h>
#include "lua/ast/namespace_path.hpp"

namespace luatool::lua_api::ast
{
    /**
     * Represents either a C++ or Lua symbol.
     */
    struct symbol
    {
        typedef std::size_t hash_value;

        symbol() = default;
        symbol(const symbol& existing);
        symbol(symbol&& old) noexcept;

        auto operator=(const symbol& path) -> symbol&;
        auto operator=(symbol&& path) noexcept -> symbol&;

        static auto make_cxx(const std::string& identifier) -> std::shared_ptr<ast::symbol>;
        static auto make_cxx(CXCursor identifier) -> std::shared_ptr<ast::symbol>;
        static auto make_cxx(const std::string& identifier, bool is_static, const ast::namespace_path& path) -> std::shared_ptr<ast::symbol>;
        static auto make_cxx(CXCursor identifier, const ast::namespace_path& path) -> std::shared_ptr<ast::symbol>;
        static auto make_lua(const std::string& identifier, const ast::namespace_path& path) -> std::shared_ptr<ast::symbol>;

        [[nodiscard]] auto is_defined() const -> bool;
        [[nodiscard]] auto is_cxx_defined() const -> bool;
        [[nodiscard]] auto is_lua_defined() const -> bool;
        [[nodiscard]] auto is_static() const -> bool;

        [[nodiscard]] auto include_path() const -> std::string;

        [[nodiscard]] auto cxx_identifier() const -> std::string;
        [[nodiscard]] auto cxx_namespace_path() const -> ast::namespace_path;
        [[nodiscard]] auto cxx_namespace_path_string() const -> std::string;
        [[nodiscard]] auto cxx_resolved_identifier() const -> std::string;

        [[nodiscard]] auto lua_identifier() const -> std::string;
        [[nodiscard]] auto lua_namespace_path() const -> ast::namespace_path;
        [[nodiscard]] auto lua_namespace_path_string() const -> std::string;
        [[nodiscard]] auto lua_resolved_identifier() const -> std::string;

        auto set_include_path(const std::string& path) -> void;

        auto make_static() -> void;
        auto apply_cxx_identifier(const std::string& identifier) -> void;
        auto apply_cxx_identifier(CXCursor identifier) -> void;
        auto apply_cxx_namespace(const ast::namespace_path& path) -> void;

        auto apply_lua_identifier(const std::string& identifier) -> void;
        auto apply_lua_namespace(const ast::namespace_path& path) -> void;

        [[nodiscard]] auto cxx_member(const std::string& member_name) const -> std::shared_ptr<ast::symbol>;
        [[nodiscard]] auto member(CXCursor member_name) const -> std::shared_ptr<ast::symbol>;
        [[nodiscard]] auto member(std::shared_ptr<ast::symbol> member_symbol) const -> std::shared_ptr<ast::symbol>;

        [[nodiscard]] auto cxx_identifier_hash() const -> hash_value;
        [[nodiscard]] auto lua_identifier_hash() const -> hash_value;

        [[nodiscard]] auto raw_documentation() const -> std::string;
        [[nodiscard]] auto documentation() const -> std::string;
        auto apply_raw_documentation(const std::string& documentation) -> void;

        [[nodiscard]] auto introduced_version() const -> std::string;
        auto set_version_introduced(const std::string& str) -> void;

        [[nodiscard]] auto is_deprecated() const -> bool;
        [[nodiscard]] auto deprecated_version() const -> std::string;
        auto set_version_deprecated(const std::string& str) -> void;

        auto attach_template_symbol(const std::string& str) -> std::shared_ptr<symbol>;

    private:
        std::string m_raw_documentation;
        std::string m_include_path;
        std::string m_introduced_version { "Unknown" };
        std::string m_deprecated_version;
        struct {
            bool is_static { false };
            std::string identifier;
            ast::namespace_path path;
        } cxx;
        struct {
            std::string identifier;
            ast::namespace_path path;
        } lua;
    };
}