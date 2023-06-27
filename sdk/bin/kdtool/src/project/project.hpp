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
#include <string>
#include <memory>
#include <unordered_map>
#include "project/structure/symbol.hpp"
#include "project/structure/construct_definition.hpp"

namespace kdtool::project
{
    struct index : public std::enable_shared_from_this<index>
    {
        index() = default;

        auto add_include_path(const std::string& path, bool scanned = false) -> void;
        auto add_translation_unit(const std::string& path) -> void;

        auto add_symbol(const std::shared_ptr<structure::symbol>& symbol, int indent = 0) -> std::shared_ptr<structure::symbol>;
        auto add_definition(const std::shared_ptr<structure::construct_definition>& definition) -> void;

        auto set_title(const std::string& title) -> void;

        [[nodiscard]] auto include_paths() const -> const std::vector<std::string>&;
        [[nodiscard]] auto scanned_include_paths() const -> const std::vector<std::string>&;
        [[nodiscard]] auto all_definitions() const -> std::vector<std::shared_ptr<structure::construct_definition>>;
        [[nodiscard]] auto all_root_symbols() const -> std::vector<std::shared_ptr<structure::symbol>>;
        [[nodiscard]] auto symbol_named(const std::string& name) -> std::shared_ptr<structure::symbol>;
        [[nodiscard]] auto title() const -> std::string;

    private:
        static auto path_is_cxx(const std::string& path) -> bool;
        static auto path_is_lua(const std::string& path) -> bool;
        static auto path_is_kdl(const std::string& path) -> bool;

    private:
        std::string m_title { "Untitled Documentation" };
        std::vector<std::string> m_include_paths;
        std::vector<std::string> m_scanned_include_paths;
        std::unordered_map<std::string, std::shared_ptr<structure::symbol>> m_symbols;
        std::unordered_map<std::string, std::shared_ptr<structure::construct_definition>> m_definitions;

    };
}