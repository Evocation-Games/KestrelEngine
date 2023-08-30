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
#include <memory>
#include <libFoundation/system/filesystem/path.hpp>
#include <libDocumentation/parser/type/documentation.hpp>
#include "project/structure/version/version.hpp"

namespace kdtool::project::structure
{
    struct construct_definition;

    struct symbol: public std::enable_shared_from_this<symbol>
    {
        enum class type { is_namespace, is_class, is_enum, is_type, is_function, is_constant, is_field, is_property };

        symbol() = default;

        explicit symbol(
            const std::string& name,
            const std::shared_ptr<struct symbol>& parent = nullptr,
            const std::shared_ptr<struct documentation::object>& documentation = nullptr,
            const std::shared_ptr<struct construct_definition>& definition = nullptr
        );

        static auto from_resolved_name(const std::string& resolved) -> std::shared_ptr<symbol>;

        [[nodiscard]] auto filename() const -> foundation::filesystem::path;
        [[nodiscard]] auto resolved_name(const std::string& delimiter = ".") const -> std::string;

        [[nodiscard]] auto hash() const -> std::string;
        [[nodiscard]] auto display_name() const -> std::string;
        [[nodiscard]] auto is_static() const -> bool;
        [[nodiscard]] auto is_constant() const -> bool;
        [[nodiscard]] auto is_mutable() const -> bool;
        [[nodiscard]] auto is_reference_stub() const -> bool;
        [[nodiscard]] auto name() const -> std::string;
        [[nodiscard]] auto basename() const -> std::string;
        [[nodiscard]] auto is_built_in() const -> bool;
        [[nodiscard]] auto is_root() const -> bool;
        [[nodiscard]] auto parent() const -> std::weak_ptr<struct symbol>;
        [[nodiscard]] auto documentation() const -> std::weak_ptr<struct documentation::object>;
        [[nodiscard]] auto definition() const -> std::weak_ptr<struct construct_definition>;
        [[nodiscard]] auto available() const -> std::optional<struct version>;
        [[nodiscard]] auto deprecated() const -> std::optional<struct version>;
        [[nodiscard]] auto source_identifier() const -> std::string;
        [[nodiscard]] auto source_resolved_identifier(const std::string& scope_resolution_operator = "",
                                                      const std::string& join_delimiter = "",
                                                      const std::vector<std::string>& template_parameter_subs = {}) const -> std::string;
        [[nodiscard]] auto source_resolved_identifier(bool including_identifier, const std::string& scope_resolution_operator) const -> std::string;
        [[nodiscard]] auto all_source_resolved_identifiers() const -> std::vector<std::string>;
        [[nodiscard]] auto children() const -> std::vector<std::weak_ptr<struct symbol>>;
        [[nodiscard]] auto template_parameters() const -> std::vector<std::string>;
        [[nodiscard]] auto source_template_parameters() const -> std::vector<std::string>;

        auto make_static() -> void;
        auto make_built_in() -> void;
        auto make_constant() -> void;
        auto make_mutable() -> void;
        auto make_reference_stub() -> void;

        auto set_documentation(const std::shared_ptr<struct documentation::object>& documentation) -> void;
        auto set_definition(const std::shared_ptr<struct construct_definition>& definition) -> void;
        auto set_source_identifier(const std::string& identifier, const std::string& resolved = "") -> void;
        auto add_source_identifier(const std::string& identifier, const std::string& resolved = "", bool propogate = true) -> void;
        auto set_display_name(const std::string& display_name) -> void;
        auto set_basename(const std::string& basename) -> void;
        auto add_template_parameter(const std::string& parameter) -> void;
        auto add_source_template_parameter(const std::string& parameter) -> void;

        auto set_available(const struct version& version) -> void;
        auto set_deprecated(const struct version& version) -> void;

        auto add_child(const std::weak_ptr<struct symbol>& symbol) -> void;

    private:
        [[nodiscard]] auto source_template_parameters_attachment(const std::vector<std::string>& subs = {}) const -> std::string;

    private:
        struct source_symbol {
            std::string identifier;
            std::string resolved;
        };
        bool m_static { false };
        bool m_built_in { false };
        bool m_constant { false };
        bool m_mutable { false };
        bool m_reference_stub { false };
        std::string m_display_name;
        std::string m_name;
        std::string m_basename;
        std::weak_ptr<struct symbol> m_parent;
        std::vector<std::weak_ptr<struct symbol>> m_children;
        std::shared_ptr<struct documentation::object> m_documentation;
        std::weak_ptr<struct construct_definition> m_definition;
        std::optional<struct version> m_available;
        std::optional<struct version> m_deprecated;
        std::vector<struct source_symbol> m_source_symbols;
        std::vector<std::string> m_template_parameters;
        std::vector<std::string> m_source_template_parameters;
        bool m_added_source_symbols { false };
    };
}
