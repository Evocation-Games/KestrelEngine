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

#include <iostream>
#include "codegen/cxx/cxx_emit_buffer.hpp"

// MARK: - Indentation

auto luatool::codegen::cxx::emit_buffer::increase_indent() -> void
{
    m_indent_adjust++;
}

auto luatool::codegen::cxx::emit_buffer::decrease_indent() -> void
{
    m_indent_adjust--;
}

auto luatool::codegen::cxx::emit_buffer::reset_indent() -> void
{
    m_indent_adjust = 0;
}


// MARK: - Emit Functions

auto luatool::codegen::cxx::emit_buffer::newline(std::size_t count) -> void
{
    for (auto n = 0; n < count; ++n) {
        m_output.emplace_back("\n");
    }
}

auto luatool::codegen::cxx::emit_buffer::space(std::size_t count) -> void
{
    for (auto n = 0; n < count; ++n) {
        m_output.emplace_back(" ");
    }
}

auto luatool::codegen::cxx::emit_buffer::start_line() -> void
{
    emit("", 0, false);
}

auto luatool::codegen::cxx::emit_buffer::semicolon() -> void
{
    emit(";", -1);
}

auto luatool::codegen::cxx::emit_buffer::emit(const std::string& line, std::int32_t indent, bool line_break) -> void
{
    std::string result = line;

    if (indent >= 0) {
        for (int i = 0; i < std::max(indent, m_indent + m_indent_adjust); ++i) {
            result = "    " + result;
        }
    }

    if (line_break) {
        result += "\n";
    }
    m_output.emplace_back(result);
}

auto luatool::codegen::cxx::emit_buffer::emit_comment(const std::string& comment) -> void
{
    emit("// " + comment);
}

auto luatool::codegen::cxx::emit_buffer::emit_section_comment(const std::string& section_name) -> void
{
    emit("// MARK: - " + section_name);
}

auto luatool::codegen::cxx::emit_buffer::emit_block_start() -> void
{
    emit("{");
    m_indent++;
    m_indent_adjust = 0;
}

auto luatool::codegen::cxx::emit_buffer::emit_block_end() -> void
{
    m_indent_adjust = 0;
    m_indent--;
    emit("}");
}

auto luatool::codegen::cxx::emit_buffer::emit_include_system(const std::string& path) -> void
{
    emit("#include <" + path + ">");
}

auto luatool::codegen::cxx::emit_buffer::emit_include(const std::string& path) -> void
{
    emit("#include \"" + path + "\"");
}

auto luatool::codegen::cxx::emit_buffer::emit_namespace_path(const lua_api::ast::namespace_path &path) -> void
{
    emit(path.path_string(), -1, false);
}

auto luatool::codegen::cxx::emit_buffer::emit_namespace_path(const std::vector<std::string>& list) -> void
{
    for (const auto& name : list) {
        emit(name + "::", -1, false);
    }
}

auto luatool::codegen::cxx::emit_buffer::emit_parameter(const std::string& parameter) -> void
{
    emit(parameter, -1, false);
}

auto luatool::codegen::cxx::emit_buffer::emit_parameter(const lua_api::ast::lua_parameter& parameter) -> void
{
    emit_type(parameter.type());
    space();
    emit_symbol(parameter.symbol());
}

auto luatool::codegen::cxx::emit_buffer::emit_arguments(const std::vector<std::shared_ptr<lua_api::ast::symbol>> &arguments) -> void
{
    emit("(" , -1, false);
    bool first_arg = true;
    for (const auto& arg : arguments) {
        if (!first_arg) {
            emit(", ", -1, false);
        }
        emit_symbol(arg);
        first_arg = false;
    }
    emit(")", -1, false);
}

auto luatool::codegen::cxx::emit_buffer::emit_type(const lua_api::ast::type& type) -> void
{
    emit(type.string(), -1, false);
}

auto luatool::codegen::cxx::emit_buffer::emit_symbol(std::shared_ptr<lua_api::ast::symbol> symbol) -> void
{
    emit(symbol->cxx_identifier(), -1, false);
}

auto luatool::codegen::cxx::emit_buffer::emit_lua_symbol(std::shared_ptr<lua_api::ast::symbol> symbol) -> void
{
    emit(symbol->lua_identifier(), -1, false);
}

auto luatool::codegen::cxx::emit_buffer::emit_resolved_symbol(std::shared_ptr<lua_api::ast::symbol> symbol) -> void
{
    emit(symbol->cxx_resolved_identifier(), -1, false);
}


auto luatool::codegen::cxx::emit_buffer::emit_function(std::shared_ptr<lua_api::ast::lua_function> function) -> void
{
    emit("auto ", -1, false);
    emit_resolved_symbol(function->symbol());
    emit("(" , -1, false);

    bool first_param = true;
    for (const auto& param : function->parameters()) {
        if (!first_param) {
            emit(", ", -1, false);
        }
        emit_parameter(param);
        first_param = false;
    }

    emit(") -> ", -1, false);
    emit_type(function->return_type());
    newline();
}

auto luatool::codegen::cxx::emit_buffer::emit_function(const std::string &name, const std::vector<std::string> &ns, const std::vector<std::string> &params, const std::string &return_type) -> void
{
    emit("auto ", -1, false);
    emit_namespace_path(ns);
    emit(name, -1, false);
    emit("(", -1, false);

    bool first_param = true;
    for (const auto& param : params) {
        if (!first_param) {
            emit(", ", -1, false);
        }
        emit_parameter(param);
        first_param = false;
    }

    emit(") -> " + return_type);
}

auto luatool::codegen::cxx::emit_buffer::emit_template_declaration(const std::vector<std::string>& typenames) -> void
{
    emit("template", 0, false);
    emit_template(typenames);
    newline();
}

auto luatool::codegen::cxx::emit_buffer::emit_template(const std::vector<std::string>& typenames) -> void
{
    emit("<", -1, false);
    bool first = true;
    for (const auto& type : typenames) {
        if (!first) {
            emit(", ", -1, false);
        }
        emit("typename " + type, -1, false);
        first = false;
    }
    emit(">", -1, false);
}


// MARK: - Output

auto luatool::codegen::cxx::emit_buffer::string() const -> std::string
{
    std::string out;
    for (const auto& chunk : m_output) {
        out += chunk;
    }
    return std::move(out);
}