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

#include <string>
#include <optional>
#include <libFoundation/system/filesystem/path.hpp>
#include <libDocumentation/parser/type/documentation.hpp>
#include "project/structure/version/version.hpp"

namespace kdtool::project::structure
{
    struct construct_definition;

    struct symbol
    {
        symbol() = default;

        explicit symbol(
            const std::string& name,
            const std::shared_ptr<struct symbol>& parent = nullptr,
            const std::shared_ptr<struct documentation::object>& documentation = nullptr,
            const std::shared_ptr<struct construct_definition>& definition = nullptr
        );

        [[nodiscard]] auto filename() const -> foundation::filesystem::path;
        [[nodiscard]] auto resolved_name(const std::string& delimiter = ".") const -> std::string;

        [[nodiscard]] auto display_name() const -> std::string;
        [[nodiscard]] auto is_static() const -> bool;
        [[nodiscard]] auto name() const -> std::string;
        [[nodiscard]] auto is_root() const -> bool;
        [[nodiscard]] auto parent() const -> std::weak_ptr<struct symbol>;
        [[nodiscard]] auto documentation() const -> std::weak_ptr<struct documentation::object>;
        [[nodiscard]] auto definition() const -> std::weak_ptr<struct construct_definition>;
        [[nodiscard]] auto available() const -> std::optional<struct version>;
        [[nodiscard]] auto deprecated() const -> std::optional<struct version>;
        [[nodiscard]] auto source_identifier() const -> std::string;
        [[nodiscard]] auto source_resolved_identifier(const std::string& scope_resolution_operator = "", const std::string& join_delimiter = "") const -> std::string;
        [[nodiscard]] auto source_resolved_identifier(bool including_identifier, const std::string& scope_resolution_operator) const -> std::string;
        [[nodiscard]] auto all_source_resolved_identifiers() const -> std::vector<std::string>;
        [[nodiscard]] auto children() const -> std::vector<std::weak_ptr<struct symbol>>;

        auto make_static() -> void;

        auto set_documentation(const std::shared_ptr<struct documentation::object>& documentation) -> void;
        auto set_definition(const std::shared_ptr<struct construct_definition>& definition) -> void;
        auto set_source_identifier(const std::string& identifier, const std::string& resolved = "") -> void;
        auto add_source_identifier(const std::string& identifier, const std::string& resolved = "") -> void;
        auto set_display_name(const std::string& display_name) -> void;

        auto set_available(const struct version& version) -> void;
        auto set_deprecated(const struct version& version) -> void;

        auto add_child(const std::weak_ptr<struct symbol>& symbol) -> void;

    private:
        struct source_symbol {
            std::string identifier;
            std::string resolved;
        };
        bool m_static { false };
        std::string m_display_name;
        std::string m_name;
        std::weak_ptr<struct symbol> m_parent;
        std::vector<std::weak_ptr<struct symbol>> m_children;
        std::shared_ptr<struct documentation::object> m_documentation;
        std::weak_ptr<struct construct_definition> m_definition;
        std::optional<struct version> m_available;
        std::optional<struct version> m_deprecated;
        std::vector<struct source_symbol> m_source_symbols;
        bool m_added_source_symbols { false };
    };
}
