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

#include "analyzer/clang.hpp"
#include "lua/ast/translation_unit.hpp"
#include "analyzer/translation_unit_context.hpp"

// MARK: - Construction

kdtool::lua_api::ast::translation_unit::translation_unit(const std::string& path, bool parse_documentation, const std::vector<std::string>& includes)
    : m_path(path)
{
    auto argument_count = (includes.size() * 2) + (parse_documentation ? 2 : 0);
    auto arguments = new const char *[argument_count];
    auto i = 0;

    if (parse_documentation) {
        arguments[i++] = "-Wdocumentation";
        arguments[i++] = "-fparse-all-comments";
    }

    for (const auto& include : includes) {
        arguments[i++] = "-I";
        arguments[i++] = include.c_str();
    }

    m_index = clang_createIndex(0, 0);
    m_tu = clang_parseTranslationUnit(m_index, path.c_str(), arguments, argument_count, nullptr, 0, CXTranslationUnit_None);

    for (const auto& dir : includes) {
        if (m_path.starts_with(dir)) {
            m_path.erase(0, dir.size());
            break;
        }
    }
    if (m_path.starts_with("/")) {
        m_path.erase(0, 1);
    }
}

// MARK: - Destruction

kdtool::lua_api::ast::translation_unit::~translation_unit()
{
//    clang_disposeTranslationUnit(m_tu);
//    clang_disposeIndex(m_index);
}

// MARK: - Accessors

auto kdtool::lua_api::ast::translation_unit::reference() const -> CXTranslationUnit
{
    return m_tu;
}

auto kdtool::lua_api::ast::translation_unit::path() const -> std::string
{
    return m_path;
}

// MARK: - Processing

auto kdtool::lua_api::ast::translation_unit::process() -> void
{
    auto context = analyzer::translation_unit_context(this);
    context.start_traversing_ast();

    add_namespaces(context.namespaces());
    add_classes(context.classes());
    add_enums(context.enums());
}

// MARK: - Management

auto kdtool::lua_api::ast::translation_unit::add_namespaces(const std::unordered_map<ast::symbol::hash_value, std::shared_ptr<ast::lua_namespace>> &namespaces) -> void
{
    for (const auto& it : namespaces) {
        if (m_namespaces.contains(it.first)) {
            throw std::runtime_error("Namespace collision in translation unit.");
        }
        m_namespaces.emplace(std::pair(it.first, it.second));
    }
}

auto kdtool::lua_api::ast::translation_unit::add_classes(const std::unordered_map<ast::symbol::hash_value, std::shared_ptr<ast::lua_class>> &classes) -> void
{
    for (const auto& it : classes) {
        if (m_classes.contains(it.first)) {
            throw std::runtime_error("Class collision in translation unit.");
        }
        m_classes.emplace(std::pair(it.first, it.second));
    }
}

auto kdtool::lua_api::ast::translation_unit::add_enums(const std::unordered_map<ast::symbol::hash_value, std::shared_ptr<ast::lua_enum>> &enums) -> void
{
    for (const auto& it : enums) {
        if (m_enums.contains(it.first)) {
            throw std::runtime_error("Enums collision in translation unit.");
        }
        m_enums.emplace(std::pair(it.first, it.second));
    }
}

auto kdtool::lua_api::ast::translation_unit::namespaces() const -> std::vector<std::shared_ptr<ast::lua_namespace>>
{
    std::vector<std::shared_ptr<ast::lua_namespace>> namespaces;
    for (const auto& it : m_namespaces) {
        namespaces.emplace_back(it.second);
    }
    return namespaces;
}

auto kdtool::lua_api::ast::translation_unit::classes() const -> std::vector<std::shared_ptr<ast::lua_class>>
{
    std::vector<std::shared_ptr<ast::lua_class>> classes;
    for (const auto& it : m_classes) {
        classes.emplace_back(it.second);
    }
    return classes;
}

auto kdtool::lua_api::ast::translation_unit::enums() const -> std::vector<std::shared_ptr<ast::lua_enum>>
{
    std::vector<std::shared_ptr<ast::lua_enum>> enums;
    for (const auto& it : m_enums) {
        enums.emplace_back(it.second);
    }
    return enums;
}