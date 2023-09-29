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

#include <memory>
#include "project/project.hpp"
#include "project/structure/construct_definition.hpp"
#include "project/structure/resource_type/resource_type_definition.hpp"
#include <libFoundation/system/filesystem/path.hpp>
#include <libKDL/unit/file.hpp>
#include <libResource/definition/type/instance.hpp>

namespace kdtool::kdl
{
    struct analyzer
    {
        analyzer(const std::shared_ptr<project::index>& project, const foundation::filesystem::path& path);

        auto run() -> void;

    private:
        auto construct_symbol(const std::string& name, const std::string& code) -> std::shared_ptr<project::structure::symbol>;
        auto construct_symbol(const std::string& type_name, const std::string& code, const std::string& field) -> std::shared_ptr<project::structure::symbol>;
        auto construct_symbol(const std::string& type_name, const std::string& code, const std::string& field, const std::string& value) -> std::shared_ptr<project::structure::symbol>;
        auto construct_symbol(const std::string& type_name, const std::string& code, const std::string& field, const std::string& value, const std::string& symbol) -> std::shared_ptr<project::structure::symbol>;
        auto construct_documentation(const std::shared_ptr<project::structure::symbol>& symbol, const std::vector<resource::decorator>& decorators) -> void;
        auto construct_available(const std::shared_ptr<project::structure::symbol>& symbol, const std::vector<resource::decorator>& decorators) -> void;
        auto construct_deprecated(const std::shared_ptr<project::structure::symbol>& symbol, const std::vector<resource::decorator>& decorators) -> void;
        auto construct_resource_type(const resource::definition::type::instance& type) -> std::shared_ptr<project::structure::resource_type_definition>;
        auto construct_resource_field(const resource::definition::type::instance& type,
                                      const resource::definition::type::field& field) -> std::shared_ptr<project::structure::resource_field_definition>;
        auto construct_resource_value(const resource::definition::type::instance& type,
                                      const resource::definition::type::field& field,
                                      const resource::definition::type::field_value& field_value) -> std::shared_ptr<project::structure::resource_value_definition>;
        auto construct_resource_value_symbol(const resource::definition::type::instance& type,
                                             const resource::definition::type::field& field,
                                             const resource::definition::type::field_value& field_value,
                                             const resource::definition::type::symbol& symbol) -> std::shared_ptr<project::structure::resource_value_symbol_definition>;


    private:
        foundation::filesystem::path m_path;
        ::kdl::sema::context m_context;
        std::shared_ptr<project::index> m_index;
    };
}