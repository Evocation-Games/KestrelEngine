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
#include "lua/ast/lua_function.hpp"

namespace kdtool::codegen::cxx
{
    class emit_buffer
    {
    public:
        emit_buffer() = default;

        auto increase_indent() -> void;
        auto decrease_indent() -> void;
        auto reset_indent() -> void;

        auto newline(std::size_t count = 1) -> void;
        auto space(std::size_t count = 1) -> void;
        auto start_line() -> void;
        auto semicolon() -> void;

        auto emit(const std::string& line = "", std::int32_t indent = 0, bool line_break = true) -> void;
        auto emit_comment(const std::string& comment) -> void;
        auto emit_section_comment(const std::string& section_name) -> void;
        auto emit_block_start() -> void;
        auto emit_block_end() -> void;
        auto emit_include(const std::string& path) -> void;
        auto emit_include_system(const std::string& path) -> void;

        auto emit_namespace_path(const lua_api::ast::namespace_path& path) -> void;
        auto emit_namespace_path(const std::vector<std::string>& list) -> void;
        auto emit_parameter(const std::string& parameter) -> void;
        auto emit_parameter(const lua_api::ast::lua_parameter& parameter) -> void;
        auto emit_arguments(const std::vector<std::shared_ptr<lua_api::ast::symbol>>& arguments) -> void;
        auto emit_type(const lua_api::ast::type& type) -> void;
        auto emit_symbol(std::shared_ptr<lua_api::ast::symbol> symbol) -> void;
        auto emit_resolved_symbol(std::shared_ptr<lua_api::ast::symbol> symbol) -> void;
        auto emit_lua_symbol(std::shared_ptr<lua_api::ast::symbol> symbol) -> void;
        auto emit_function(std::shared_ptr<lua_api::ast::lua_function> function) -> void;
        auto emit_function(const std::string& name, const std::vector<std::string>& ns, const std::vector<std::string>& params, const std::string& return_type) -> void;

        auto emit_template_declaration(const std::vector<std::string>& typenames) -> void;
        auto emit_template(const std::vector<std::string>& typenames) -> void;

        [[nodiscard]] auto string() const -> std::string;

    private:
        std::int32_t m_indent { 0 };
        std::int32_t m_indent_adjust { 0 };
        std::vector<std::string> m_output;
    };
}