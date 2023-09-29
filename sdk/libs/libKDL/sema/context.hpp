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
#include <unordered_map>
#include <unordered_set>
#include <libLexer/lexeme.hpp>
#include <libKDL/sema/decorator/decorator.hpp>
#include <libResource/definition/type/instance.hpp>
#include <libResource/declaration/instance.hpp>
#include <libInterpreter/scope/scope.hpp>
#include <libResourceCore/file.hpp>
#include <libFoundation/system/filesystem/file.hpp>
#include <libKDL/modules/module/module_definition.hpp>

namespace kdl::sema
{
    struct context
    {
        struct {
            bool surpress_resource_creation { false };
        } flags;
        decorator::collection current_decorators;
        auto evaluate_decorators() -> bool;

        std::unordered_set<std::string> definitions;
        std::unordered_map<std::string, resource::definition::type::instance> registered_types;
        std::unordered_map<std::string, kdl::module_definition> registered_modules;
        std::unordered_map<std::string, std::int64_t> field_repeat_counts;
        std::vector<resource::instance> resources;
        std::vector<std::shared_ptr<foundation::filesystem::file>> files;

        std::vector<interpreter::scope *> scope_stack;
        const resource::definition::type::descriptor *current_type_descriptor { nullptr };
        const resource::definition::type::instance *current_type { nullptr };
        const resource::definition::type::field *current_field { nullptr };
        const resource::definition::binary_template::field *current_binary_field { nullptr };

        auto register_type(const resource::definition::type::instance& type) -> resource::definition::type::instance *;
        [[nodiscard]] auto type_named(const std::string& name) const -> const resource::definition::type::instance *;
        [[nodiscard]] auto type_named(const lexer::lexeme& lx) const -> const resource::definition::type::instance *;
        [[nodiscard]] auto type(const resource::definition::type::descriptor& descriptor) const -> const resource::definition::type::instance *;

        auto register_module(const kdl::module_definition& module) -> kdl::module_definition *;
        [[nodiscard]] auto module_named(const std::string& name) const -> const kdl::module_definition *;
        [[nodiscard]] auto module_named(const lexer::lexeme& lx) const -> const kdl::module_definition *;

        auto create_scope() -> interpreter::scope *;
        auto pop_scope() -> void;
        auto active_scope() -> interpreter::scope *;
        auto root_scope() -> interpreter::scope *;

        foundation::filesystem::path root_path;
        foundation::filesystem::path output_path;
        [[nodiscard]] auto resolve_path(const std::string& path, const foundation::filesystem::path& source_path) const -> foundation::filesystem::path;
        [[nodiscard]] auto resolve_path(const foundation::filesystem::path& path, const foundation::filesystem::path& source_path) const -> foundation::filesystem::path;
    };
}