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

#include <fstream>
#include <libKDL/codegen/kdl/file.hpp>
#include <libCodeGen/builder/emit/buffer.hpp>
#include <libCodeGen/languages/definition/kdl.hpp>
#include <libCodeGen/ast/definition.hpp>

#define KDL ::codegen::language::kdl

// MARK: - Construction

kdl::codegen::kdl::file::file(const foundation::filesystem::path& path)
    : m_path(path)
{}

// MARK: - Saving

auto kdl::codegen::kdl::file::save() -> void
{
    m_buffer.save(m_path);
}

// MARK: - Composing

auto kdl::codegen::kdl::file::nl() -> void
{
    m_buffer.write(std::make_shared<::codegen::ast::nl<KDL>>());
}

auto kdl::codegen::kdl::file::comment(const std::string &str) -> void
{
    m_buffer.write(std::make_shared<::codegen::ast::comment<KDL>>(str));
}

auto kdl::codegen::kdl::file::metadata_project(const std::string& name) -> void
{
    m_buffer.write(std::make_shared<::codegen::ast::project_name<KDL>>(name));
}

auto kdl::codegen::kdl::file::metadata_copyright(const std::string& name) -> void
{
    m_buffer.write(std::make_shared<::codegen::ast::copyright_name<KDL>>(name));
}

auto kdl::codegen::kdl::file::metadata_author(const std::string& name) -> void
{
    m_buffer.write(std::make_shared<::codegen::ast::author_name<KDL>>(name));
}

auto kdl::codegen::kdl::file::metadata_version(const std::string& name) -> void
{
    m_buffer.write(std::make_shared<::codegen::ast::version_name<KDL>>(name));
}

auto kdl::codegen::kdl::file::metadata_license(const std::string& name) -> void
{
    m_buffer.write(std::make_shared<::codegen::ast::license_name<KDL>>(name));
}

auto kdl::codegen::kdl::file::import_file(const std::string &path) -> void
{
    m_buffer.write(std::make_shared<::codegen::ast::include_file<KDL>>(path));
}

auto kdl::codegen::kdl::file::import_module(const std::string &name) -> void
{
    m_buffer.write(std::make_shared<::codegen::ast::include_library<KDL>>(name));
}

// MARK: - Components

auto kdl::codegen::kdl::file::component(const std::string &name,
                                        const std::string &type,
                                        std::int64_t base_id,
                                        const std::function<auto(const std::shared_ptr<::codegen::ast::component<KDL>>&)->void>& fn) -> void
{
    auto name_symbol = std::make_shared<::codegen::ast::symbol<KDL>>(name);
    auto type_symbol = std::make_shared<::codegen::ast::symbol<KDL>>(type);

    auto component = std::make_shared<::codegen::ast::component<KDL>>(name_symbol, type_symbol, base_id);
    fn(component);

    m_buffer.write(component);
}

// MARK: - Declaration

auto kdl::codegen::kdl::file::declaration(const std::string &name,
                                          const std::string &type,
                                          const std::function<auto(const std::shared_ptr<::codegen::ast::resource_declaration<KDL>>&)->void>& fn) -> void
{
    auto name_symbol = std::make_shared<::codegen::ast::symbol<KDL>>(name);
    auto type_symbol = std::make_shared<::codegen::ast::symbol<KDL>>(type);

    auto resource_decl = std::make_shared<::codegen::ast::resource_declaration<KDL>>(name_symbol, type_symbol);
    fn(resource_decl);

    m_buffer.write(resource_decl);
}

// MARK: - Scenes

auto kdl::codegen::kdl::file::scene(const std::string &name,
                                    bool is_dialog,
                                    std::int64_t id,
                                    const std::function<auto(const std::shared_ptr<::codegen::ast::scene_declaration<KDL>>&)->void>& fn) -> void
{
    auto name_symbol = std::make_shared<::codegen::ast::symbol<KDL>>(name);
    auto scene_decl = std::make_shared<::codegen::ast::scene_declaration<KDL>>(name_symbol, id, is_dialog);
    fn(scene_decl);
    m_buffer.write(scene_decl);
}