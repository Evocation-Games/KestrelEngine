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

namespace kdtool::codegen::documentation
{
    class page;
    class table;

    class generator
    {
    public:
        generator() = default;

        auto add_translation_unit(lua_api::ast::translation_unit *tu) -> void;

        auto compile(const std::string& path) -> void;

    private:
        auto compile(std::shared_ptr<page> page, std::shared_ptr<lua_api::ast::symbol> symbol) -> void;

        auto compile(std::shared_ptr<lua_api::ast::lua_namespace> ns) -> std::shared_ptr<page> ;
        auto compile(std::shared_ptr<lua_api::ast::lua_class> klass) -> std::shared_ptr<page> ;

        auto compile(std::shared_ptr<page> page, std::shared_ptr<lua_api::ast::lua_function> function, std::shared_ptr<lua_api::ast::symbol> owner) -> void;
        auto compile(std::shared_ptr<page> page, std::shared_ptr<lua_api::ast::lua_property> property, std::shared_ptr<lua_api::ast::symbol> owner) -> void;
        auto compile(std::shared_ptr<page> page, std::shared_ptr<lua_api::ast::lua_variable> variable, std::shared_ptr<lua_api::ast::symbol> owner) -> void;

        auto construct_availability_table(std::shared_ptr<lua_api::ast::symbol> symbol, std::shared_ptr<table> cxx_table) -> void;

    private:
        common::api::aggregator m_aggregator;
        std::vector<std::shared_ptr<page>> m_pages;
    };
}