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

#include <unordered_map>
#include <libKDL/codegen/lua/ast.hpp>
#include <libKDL/codegen/lua/kestrel_api.hpp>
#include <libResource/definition/type/instance.hpp>
#include <libResource/definition/template/field.hpp>
#include <libInterpreter/scope/scope.hpp>

namespace kdl::codegen::lua
{
    struct exporter
    {
        explicit exporter(const resource::definition::type::instance *type, interpreter::scope *scope);

        [[nodiscard]] auto generate() -> std::string;

    private:
        interpreter::scope *m_scope { nullptr };
        const resource::definition::type::instance *m_type_definition { nullptr };
        ast::generator m_generator;
        kestrel_api m_api;

        struct {
            ast::symbol *klass_name { nullptr };
            ast::class_definition *klass { nullptr };
            ast::function_definition *type_code_func { nullptr };
            std::unordered_map<std::string, ast::node *> binary_fields;
            std::unordered_map<std::string, std::string> export_names;
        } m_type;

        auto produce_header() -> void;
        auto produce_class_boilerplate() -> void;
        auto produce_type_constants() -> void;
        auto produce_model_loader() -> void;
        auto produce_type_properties() -> void;
        auto determine_export_names() -> void;
        auto produce_property_getter(ast::property_definition *property) -> void;
        auto produce_property_setter(ast::property_definition *property) -> void;

        auto prepare_template_read_calls(ast::symbol *resource, ast::symbol *data) -> void;
        auto prepare_template_read_calls(ast::symbol *resource) -> void;
        auto produce_template_property_mapping(ast::symbol *resource) -> void;

        auto produce_read_call(const resource::definition::binary_template::field *binary_field,
                               const resource::definition::type::field_value *field_value,
                               ast::symbol *data) -> ast::node *;
        auto produce_read_call(const resource::definition::binary_template::field *binary_field,
                               ast::symbol *data) -> ast::node *;

        auto build_field_name(const resource::definition::type::field_value *field_value, std::int64_t field_number = -1) -> ast::symbol *;
        auto field_for_binary_field(const resource::definition::binary_template::field *binary_field) -> resource::definition::type::field;
        auto field_value_for_binary_field(const resource::definition::binary_template::field *binary_field) -> resource::definition::type::field_value;
        auto associated_explicit_type_binary_field(const resource::definition::binary_template::field *binary_field) -> const resource::definition::type::descriptor *;

        auto produce_resource_reader_for_loop(ast::node *var, ast::symbol *data,
                                              ast::node *lower, ast::node *upper,
                                              const std::function<auto(ast::node *)->void>& block) -> void;
    };
}