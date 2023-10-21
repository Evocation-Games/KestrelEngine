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
#include <functional>
#include <libFoundation/system/filesystem/path.hpp>
#include <libCodeGen/builder/emit/buffer.hpp>
#include <libCodeGen/ast/definition.hpp>
#include <libCodeGen/languages/definition/kdl.hpp>

#define KDL ::codegen::language::kdl

namespace kdl::codegen::kdl
{
    struct file
    {
    public:
        explicit file(const foundation::filesystem::path& path);

        auto nl() -> void;
        auto comment(const std::string& str) -> void;

        auto metadata_project(const std::string& name) -> void;
        auto metadata_copyright(const std::string& name) -> void;
        auto metadata_author(const std::string& name) -> void;
        auto metadata_version(const std::string& name) -> void;
        auto metadata_license(const std::string& name) -> void;

        auto import_file(const std::string& path) -> void;
        auto import_module(const std::string& name) -> void;

        auto component(const std::string& name,
                       const std::string& type,
                       std::int64_t base_id,
                       const std::function<auto(const std::shared_ptr<::codegen::ast::component<KDL>>&)->void>& fn) -> void;

        auto declaration(const std::string& name,
                         const std::string& type,
                         const std::function<auto(const std::shared_ptr<::codegen::ast::resource_declaration<KDL>>&)->void>& fn) -> void;

        auto scene(const std::string& name,
                   bool is_dialog,
                   std::int64_t id,
                   const std::function<auto(const std::shared_ptr<::codegen::ast::scene_declaration<KDL>>&)->void>& fn) -> void;

        auto save() -> void;

    private:
        foundation::filesystem::path m_path;
        ::codegen::emit::buffer m_buffer;
    };
}