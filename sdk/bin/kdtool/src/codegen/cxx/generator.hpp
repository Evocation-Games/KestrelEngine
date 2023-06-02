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

#include "lua/ast/translation_unit.hpp"
#include "codegen/common/lua_api_aggregator.hpp"
#include "codegen/cxx/cxx_emit_buffer.hpp"

namespace kdtool::codegen::cxx
{
    class generator
    {
    public:
        generator() = default;

        auto require_inclusion_of(const std::string& path) -> void;
        auto add_translation_unit(lua_api::ast::translation_unit *tu) -> void;

        auto compile(const std::string& path) -> void;

    private:
        auto compile_inclusions() -> void;
        auto compile_global_enrollment() -> void;
        auto compile_namespace_enrollment(std::shared_ptr<lua_api::ast::lua_namespace> ns) -> void;
        auto compile_class_enrollment(std::shared_ptr<lua_api::ast::lua_class> klass) -> void;
        auto compile_enum_enrollment(std::shared_ptr<lua_api::ast::lua_enum> lua_enum) -> void;

        auto begin_namespaces(const lua_api::ast::namespace_path& path, const std::function<auto()->void>& block) -> void;

        auto begin_namespace(std::shared_ptr<lua_api::ast::symbol> ns_name) -> void;
        auto begin_namespace(const std::string& ns_name) -> void;
        auto end_namespace() -> void;

        auto begin_class(std::shared_ptr<lua_api::ast::lua_class> klass) -> void;
        auto end_class() -> void;

    private:
        std::vector<std::string> m_includes;
        common::api::aggregator m_aggregator;
        emit_buffer m_buffer;
        std::size_t m_namespace_depth { 0 };
    };
}