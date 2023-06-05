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

#include "project/project.hpp"
#include "lua/ast/translation_unit.hpp"

// MARK: - Merging

auto kdtool::project::add_include_path(const std::string& path) -> void
{
    m_include_dirs.emplace_back(path);
}

auto kdtool::project::add_translation_unit(const std::string& path) -> void
{
    auto tu = std::make_shared<lua_api::ast::translation_unit>(path, true, include_paths());
    tu->process();

    for (const auto& lua_namespace : tu->namespaces()) {
        add(lua_namespace);
    }

    for (const auto& lua_class : tu->classes()) {
        add(lua_class);
    }

    for (const auto& lua_enum : tu->enums()) {
        add(lua_enum);
    }
}

auto kdtool::project::add(const std::shared_ptr<lua_api::ast::lua_namespace>& lua_namespace) -> void
{
    m_namespaces.emplace(lua_namespace->hash_key(), lua_namespace);
}

auto kdtool::project::add(const std::shared_ptr<lua_api::ast::lua_class>& lua_class) -> void
{
    m_classes.emplace(lua_class->hash_key(), lua_class);
}

auto kdtool::project::add(const std::shared_ptr<lua_api::ast::lua_enum>& lua_enum) -> void
{
    m_enums.emplace(lua_enum->hash_key(), lua_enum);
}

// MARK: - Access

auto kdtool::project::include_paths() const -> std::vector<std::string>
{
    return m_include_dirs;
}

auto kdtool::project::all_namespaces() const -> std::vector<std::shared_ptr<lua_api::ast::lua_namespace>>
{
    std::vector<std::shared_ptr<lua_api::ast::lua_namespace>> namespaces;
    namespaces.reserve(m_namespaces.size());
    for (const auto& it : m_namespaces) {
        namespaces.emplace_back(it.second);
    }
    return namespaces;
}

auto kdtool::project::all_classes() const -> std::vector<std::shared_ptr<lua_api::ast::lua_class>>
{
    std::vector<std::shared_ptr<lua_api::ast::lua_class>> classes;
    classes.reserve(m_classes.size());
    for (const auto& it : m_classes) {
        classes.emplace_back(it.second);
    }
    return classes;
}

auto kdtool::project::all_enums() const -> std::vector<std::shared_ptr<lua_api::ast::lua_enum>>
{
    std::vector<std::shared_ptr<lua_api::ast::lua_enum>> enums;
    enums.reserve(m_enums.size());
    for (const auto& it : m_enums) {
        enums.emplace_back(it.second);
    }
    return enums;
}