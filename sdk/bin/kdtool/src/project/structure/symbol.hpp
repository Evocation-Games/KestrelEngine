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
        symbol() = default;
        explicit symbol(const std::string& name, const std::shared_ptr<struct symbol>& parent = nullptr);

        // Identity
        [[nodiscard]] auto hash() const -> std::string;

        // Access
        [[nodiscard]] auto is_static() const -> bool;
        [[nodiscard]] auto is_builtin() const -> bool;
        [[nodiscard]] auto is_constant() const -> bool;
        [[nodiscard]] auto is_mutable() const -> bool;
        [[nodiscard]] auto is_reference_stub() const -> bool;
        auto make_static() -> void;
        auto make_builtin() -> void;
        auto make_constant() -> void;
        auto make_mutable() -> void;
        auto make_reference() -> void;

        // Definition
        [[nodiscard]] auto definition() const -> std::weak_ptr<struct construct_definition>;
        auto set_definition(const std::weak_ptr<struct construct_definition>& definition) -> void;

        // File
        [[nodiscard]] auto filename() const -> foundation::filesystem::path;
        auto set_filename(const foundation::filesystem::path& filename) -> void;

        // Parent/Children
        [[nodiscard]] auto is_root() const -> bool;
        [[nodiscard]] auto parent() const -> std::weak_ptr<struct symbol>;
        [[nodiscard]] auto children() const -> std::vector<std::weak_ptr<struct symbol>>;
        auto add_child(const std::shared_ptr<struct symbol>& symbol) -> void;

        // Version
        [[nodiscard]] auto deprecation() const -> std::optional<struct version>;
        [[nodiscard]] auto available() const -> std::optional<struct version>;
        auto set_deprecation_version(const struct version& version) -> void;
        auto set_available_version(const struct version& version) -> void;

        // Documentation
        [[nodiscard]] auto documentation() const -> std::weak_ptr<struct documentation::object>;
        auto set_documentation(const std::shared_ptr<struct documentation::object>& documentation) -> void;
        [[nodiscard]] auto display_name() const -> std::string;
        auto set_display_name(const std::string& name) -> void;

        // Source
        [[nodiscard]] auto source_identifier(const std::vector<std::string>& template_parameters = {}) const -> std::string;
        [[nodiscard]] auto resolved_source_identifier(const std::string& scope_operator = "::",
                                                      const std::vector<std::string>& template_parameters = {}) const -> std::string;
        [[nodiscard]] auto source_template_parameters() const -> std::vector<std::string>;
        auto set_source_identifier(const std::string& identifier) -> void;
        auto add_source_template_parameter(const std::string& parameter) -> void;

        // Lua
        [[nodiscard]] auto has_lua_identifier() const -> bool;
        [[nodiscard]] auto lua_identifier() const -> std::string;
        [[nodiscard]] auto resolved_lua_identifier() const -> std::string;
        auto set_lua_identifier(const std::string& identifier) -> void;

    private:
        std::weak_ptr<struct symbol> m_parent;
        std::vector<std::weak_ptr<struct symbol>> m_children;
        struct {
            bool is_static { false };
            bool is_builtin { false };
            bool is_mutable { false };
            bool is_constant { false };
            bool is_reference { false };
        } m_access;
        struct {
            foundation::filesystem::path filename;
            std::string identifier;
            std::vector<std::string> templates;
        } m_source;
        struct {
            std::vector<std::string> identifiers;
            std::string display_name;
        } m_lua;
        struct {
            std::optional<struct version> deprecated;
            std::optional<struct version> available;
        } m_version;
        std::shared_ptr<struct documentation::object> m_documentation;
        std::weak_ptr<struct construct_definition> m_definition;
    };

}
