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

#include <iostream>
#include <libFoundation/string/split.hpp>
#include "project/project.hpp"
#include "analyzer/cxx/cxx_analyzer.hpp"
#include "analyzer/kdl/kdl_analyzer.hpp"

// MARK: - Includes

auto kdtool::project::index::add_include_path(const std::string& path, bool scanned) -> void
{
    if (scanned) {
        auto it = std::find(m_scanned_include_paths.begin(), m_scanned_include_paths.end(), path);
        if (it == m_scanned_include_paths.end()) {
            m_scanned_include_paths.emplace_back(path);
        }
    }
    else {
        auto it = std::find(m_include_paths.begin(), m_include_paths.end(), path);
        if (it == m_include_paths.end()) {
            m_include_paths.emplace_back(path);
        }
    }
}

auto kdtool::project::index::include_paths() const -> const std::vector<std::string>&
{
    return m_include_paths;
}

auto kdtool::project::index::scanned_include_paths() const -> const std::vector<std::string>&
{
    return m_scanned_include_paths;
}

// MARK: - Paths

auto kdtool::project::index::path_is_cxx(const std::string &path) -> bool
{
    return path.ends_with(".cpp")
           || path.ends_with(".hpp")
           || path.ends_with(".cc")
           || path.ends_with(".mm")
           || path.ends_with(".hh");
}

auto kdtool::project::index::path_is_lua(const std::string &path) -> bool
{
    return path.ends_with(".lua");
}

auto kdtool::project::index::path_is_kdl(const std::string &path) -> bool
{
    return path.ends_with(".kdl")
           || path.ends_with(".kdlproj")
           || path.ends_with(".kdlmodule");
}

// MARK: - Translation Unit

auto kdtool::project::index::add_translation_unit(const std::string &path) -> void
{
    if (path_is_cxx(path)) {
        cxx::analyzer unit(shared_from_this(), foundation::filesystem::path(path));
        unit.run();
    }
    else if (path_is_kdl(path)) {
        kdl::analyzer unit(shared_from_this(), foundation::filesystem::path(path));
        unit.run();
    }
    else if (path_is_lua(path)) {

    }
}

// MARK: - Symbol Management

auto kdtool::project::index::symbol_named(const std::string& resolved_name) -> std::shared_ptr<structure::symbol>
{
    const auto name_stack = foundation::string::split(resolved_name, ".");
    std::shared_ptr<structure::symbol> symbol;
    for (const auto& name : name_stack) {
        auto new_symbol = std::make_shared<structure::symbol>(name, symbol);
        symbol = add_symbol(new_symbol);
    }
    return symbol;
}

auto kdtool::project::index::add_symbol(const std::shared_ptr<structure::symbol>& symbol, int indent) -> std::shared_ptr<structure::symbol>
{
    std::string indent_str;
    for (int i = 0; i < indent; ++i) {
        indent_str += "    ";
    }

    // Check for the existance of the symbol.
    const auto name = symbol->resolved_name();
    auto it = m_symbols.find(name);
    if (it == m_symbols.end()) {
        m_symbols.emplace(name, symbol);
        std::cout << indent_str << "  + create symbol: " << name << std::endl;

        if (auto parent = symbol->parent().lock()) {
            parent->add_child(symbol);
        }

        // If we have added the symbol, then try and add the parents if required.
        if (auto parent = symbol->parent().lock()) {
            add_symbol(parent, indent + 1);
        }
        return symbol;
    }
    else {
        std::cout << indent_str << "  + existing symbol: " << name << std::endl;
        return it->second;
    }
}

auto kdtool::project::index::all_root_symbols() const -> std::vector<std::shared_ptr<structure::symbol>>
{
    std::vector<std::shared_ptr<structure::symbol>> out;
    for (const auto& sym : m_symbols) {
        if (sym.second->is_root()) {
            out.emplace_back(sym.second);
        }
    }
    return out;
}

// MARK: - Definition Management

auto kdtool::project::index::all_definitions() const -> std::vector<std::shared_ptr<structure::construct_definition>>
{
    std::vector<std::shared_ptr<structure::construct_definition>> out;
    out.reserve(m_definitions.size());
    for (const auto& it : m_definitions) {
        out.emplace_back(it.second);
    }
    return std::move(out);
}

auto kdtool::project::index::add_definition(const std::shared_ptr<structure::construct_definition>& definition) -> void
{
    add_symbol(definition->symbol());
    m_definitions.emplace(definition->symbol()->resolved_name(), definition);
}