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
#include <memory>
#include <libFoundation/system/filesystem/path.hpp>
#include <libCodeGen/spec/markup/page.hpp>
#include <libCodeGen/spec/markup/table.hpp>
#include <libCodeGen/spec/markup/heading.hpp>
#include <libCodeGen/languages/markup/markup.hpp>
#include "project/project.hpp"

namespace kdtool::documentation
{
    class builder: public std::enable_shared_from_this<builder>
    {
    public:
        builder(const std::shared_ptr<kdtool::project>& project, const std::string& path);

        auto build() -> void;

        auto export_as(const std::shared_ptr<codegen::markup_language>& language) -> void;

        auto add_function_page(const std::shared_ptr<lua_api::ast::lua_function>& lua_function, const std::shared_ptr<lua_api::ast::symbol>& owner) -> std::shared_ptr<codegen::spec::markup::page>;
        auto add_variable_page(const std::shared_ptr<lua_api::ast::lua_variable>& lua_function, const std::shared_ptr<lua_api::ast::symbol>& owner) -> std::shared_ptr<codegen::spec::markup::page>;
        auto add_property_page(const std::shared_ptr<lua_api::ast::lua_property>& lua_function, const std::shared_ptr<lua_api::ast::symbol>& owner) -> std::shared_ptr<codegen::spec::markup::page>;
        auto add_case_page(const std::shared_ptr<lua_api::ast::lua_enum_case>& lua_function, const std::shared_ptr<lua_api::ast::symbol>& owner) -> std::shared_ptr<codegen::spec::markup::page>;

    private:
        auto build(const std::shared_ptr<lua_api::ast::lua_namespace>& lua_namespace) -> std::shared_ptr<codegen::spec::markup::page>;
        auto build(const std::shared_ptr<lua_api::ast::lua_class>& lua_class) -> std::shared_ptr<codegen::spec::markup::page>;
        auto build(const std::shared_ptr<lua_api::ast::lua_enum>& lua_enum) -> std::shared_ptr<codegen::spec::markup::page>;

        auto index_for(const std::shared_ptr<lua_api::ast::symbol>& symbol, const std::shared_ptr<codegen::markup_language>& language) -> std::string;
        auto build_index(const std::shared_ptr<codegen::markup_language>& language) -> std::shared_ptr<codegen::spec::markup::page>;

    private:
        auto add_page(const std::string& title, const std::vector<std::string>& path, const std::string& name = "index", bool no_title = false) -> std::shared_ptr<codegen::spec::markup::page>;

    private:
        struct page_reference
        {
            std::shared_ptr<codegen::spec::markup::page> page;
            std::vector<std::string> path;
            std::string name;

            page_reference(const std::shared_ptr<codegen::spec::markup::page>& page, const std::vector<std::string>& path, const std::string& name)
                : page(page), path(path), name(name)
            {}
        };

        foundation::filesystem::path m_path;
        std::shared_ptr<kdtool::project> m_project;
        std::vector<page_reference> m_pages;
    };
}