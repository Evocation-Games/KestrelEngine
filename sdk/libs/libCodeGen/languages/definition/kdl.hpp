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
#include <libCodeGen/languages/language.hpp>

namespace codegen::language
{
    struct kdl
    {

#pragma region Basic Metadata
        [[nodiscard]] static auto name() -> std::string { return "KDL"; }
        [[nodiscard]] static auto lc_name() -> std::string { return "kdl"; };
        [[nodiscard]] static auto extension() -> std::string { return "kdl"; };
#pragma endregion

#pragma region Comment Support
        [[nodiscard]] static auto comment(const std::string& str) -> emit::segment
        {
            return emit::segment("// " + str, emit::line_break_mode::after);
        }
#pragma endregion

#pragma region Preprocessor Support
        [[nodiscard]] static auto preprocessor_keyword(const std::string& name) -> emit::segment
        {
            return emit::segment("@" + name + " ", emit::line_break_mode::none);
        }

        [[nodiscard]] static auto include_library(const std::string& name) -> emit::segment
        {
            return {
                preprocessor_keyword("import"),
                symbol(name),
                emit::segment(statement_end_operator_string(), emit::line_break_mode::after)
            };
        }

        [[nodiscard]] static auto include_file(const std::string& path) -> emit::segment
        {
            return {
                preprocessor_keyword("import"),
                string(path),
                emit::segment(statement_end_operator_string(), emit::line_break_mode::after)
            };
        }
#pragma endregion

#pragma region Symbol Support
        [[nodiscard]] static auto symbol(const std::string& name) -> emit::segment
        {
            return emit::segment(name, emit::line_break_mode::none);
        }
#pragma endregion

#pragma region Literal Support
        [[nodiscard]] static auto string(const std::string& value) -> emit::segment
        {
            return emit::segment("\"" + value + "\"", emit::line_break_mode::none);
        }
#pragma endregion

#pragma region Operator Support
        [[nodiscard]] static auto statement_end_operator_string() -> std::string
        {
            return ";";
        }

        [[nodiscard]] static auto addition_operator_string() -> std::string
        {
            return "+";
        }

        [[nodiscard]] static auto subtraction_operator_string() -> std::string
        {
            return "-";
        }

        [[nodiscard]] static auto multiplication_operator_string() -> std::string
        {
            return "*";
        }

        [[nodiscard]] static auto division_operator_string() -> std::string
        {
            return "/";
        }

        [[nodiscard]] static auto modulo_operator_string() -> std::string
        {
            return "%";
        }

        [[nodiscard]] static auto reference_member_operator_string() -> std::string
        {
            return ".";
        }

        [[nodiscard]] static auto pointer_member_operator_string() -> std::string
        {
            return ".";
        }

        [[nodiscard]] static auto scope_resolution_operator_string() -> std::string
        {
            return ".";
        }

        [[nodiscard]] static auto list_delimiter_string() -> std::string
        {
            return ", ";
        }

        [[nodiscard]] static auto dereference_operator_string() -> std::string
        {
            return "*";
        }

        [[nodiscard]] static auto address_of_operator_string() -> std::string
        {
            return "&";
        }

        [[nodiscard]] static auto pointer_operator_string() -> std::string
        {
            return "*";
        }

        [[nodiscard]] static auto reference_operator_string() -> std::string
        {
            return "&";
        }

        [[nodiscard]] static auto assignment_operator_string() -> std::string
        {
            return "=";
        }

        [[nodiscard]] static auto static_cast_operator(const std::string& type, const emit::segment& value) -> emit::segment
        {
            throw std::runtime_error("Static casting is not supported in KDL");
        }
#pragma endregion

#pragma region Function Support
        [[nodiscard]] static auto function_decl(
            const std::shared_ptr<ast::node>& name,
            const std::string& return_type,
            const std::shared_ptr<ast::node>& parameters
        ) -> emit::segment {
            return {
                preprocessor_keyword("function"),
                function_name(name),
                emit::segment(" " + assignment_operator_string() + " ")
            };
        }

        [[nodiscard]] static auto function_name(const std::shared_ptr<ast::node>& name) -> emit::segment
        {
            return name->emit();
        }

        [[nodiscard]] static auto parameter_list(const std::vector<emit::segment>& list) -> emit::segment
        {
            throw std::runtime_error("Explicit parameter lists are not implemented in KDL");
        }

        [[nodiscard]] static auto parameter(const std::shared_ptr<ast::node>& name, const std::string& type) -> emit::segment
        {
            throw std::runtime_error("Explicit parameters are not implemented in KDL");
        }

        [[nodiscard]] static auto function_type(const std::vector<emit::segment>& parameters, const std::string& type) -> emit::segment
        {
            throw std::runtime_error("Function types are not implemented in KDL");
        }

#pragma endregion

#pragma region Statement Support
        [[nodiscard]] static auto function_call(const std::shared_ptr<ast::node>& name, const std::vector<emit::segment>& args) -> emit::segment
        {
            return {
                name->emit(),
                emit::segment("("),
                emit::segment(args),
                emit::segment(")")
            };
        }

        [[nodiscard]] static auto specialized_function_call(
            const std::shared_ptr<ast::node>& name,
            const std::vector<emit::segment>& specializations,
            const std::vector<emit::segment>& args
        ) -> emit::segment {
            throw std::runtime_error("Specialised functions are not support in KDL");
        }

        [[nodiscard]] static auto address_of(const std::shared_ptr<ast::node>& node) -> emit::segment
        {
            throw std::runtime_error("KDL does not support fetching the address of something.");
        }

        [[nodiscard]] static auto static_statement(const emit::segment& statement) -> emit::segment
        {
            throw std::runtime_error("KDL does not support static statements.");
        }

        [[nodiscard]] static auto assignment_statement(const emit::segment& lhs, const emit::segment& rhs) -> emit::segment
        {
            return {
                lhs,
                emit::segment(" "),
                emit::segment(assignment_operator_string()),
                emit::segment(" "),
                rhs
            };
        }

        [[nodiscard]] static auto variable_decl_statement(const std::shared_ptr<ast::node>& name, const std::string& type) -> emit::segment
        {
            return {
                preprocessor_keyword("var "),
                name->emit(),
                emit::segment(" " + assignment_operator_string() + " ")
            };
        }
#pragma endregion

#pragma region Compound Statement Support
        [[nodiscard]] static auto begin_compound_statement() -> emit::segment
        {
            return emit::segment(" {", emit::line_break_mode::after, emit::indentation_mode::indent_after);
        }

        [[nodiscard]] static auto end_compound_statement() -> emit::segment
        {
            return emit::segment("}" + statement_end_operator_string(), emit::line_break_mode::full, emit::indentation_mode::outdent_before);
        }
#pragma endregion

#pragma region Resource Metadata Support
        [[nodiscard]] static auto metadata(const std::string& name, const std::string& value) -> emit::segment
        {
            return {
                preprocessor_keyword(name),
                string(value),
                emit::segment(statement_end_operator_string(), emit::line_break_mode::after)
            };
        }
#pragma endregion

#pragma region Resource Reference Support
        [[nodiscard]] static auto global_reference(std::int64_t id) -> emit::segment
        {
            return {
                emit::segment("#", emit::line_break_mode::none),
                emit::segment(std::to_string(id), emit::line_break_mode::none)
            };
        }

        [[nodiscard]] static auto typed_reference(const std::shared_ptr<ast::node>& type, std::int64_t id) -> emit::segment
        {
            return {
                emit::segment("#", emit::line_break_mode::none),
                type->emit(),
                emit::segment(".", emit::line_break_mode::none),
                emit::segment(std::to_string(id), emit::line_break_mode::none)
            };
        }

        [[nodiscard]] static auto typed_contained_reference(const std::shared_ptr<ast::node>& container, const std::shared_ptr<ast::node>& type, std::int64_t id) -> emit::segment
        {
            return {
                emit::segment("#", emit::line_break_mode::none),
                container->emit(),
                emit::segment(".", emit::line_break_mode::none),
                type->emit(),
                emit::segment(".", emit::line_break_mode::none),
                emit::segment(std::to_string(id), emit::line_break_mode::none)
            };
        }

        [[nodiscard]] static auto contained_reference(const std::shared_ptr<ast::node>& container, std::int64_t id) -> emit::segment
        {
            return {
                emit::segment("#", emit::line_break_mode::none),
                container->emit(),
                emit::segment(".", emit::line_break_mode::none),
                emit::segment(std::to_string(id), emit::line_break_mode::none)
            };
        }
#pragma endregion

#pragma region Resource Component Support
        [[nodiscard]] static auto component_decl(const std::shared_ptr<ast::node>& ns, std::int64_t id, const std::shared_ptr<ast::node>& type) -> emit::segment
        {
            if (ns->emit().string().starts_with("_")) {
                return {
                    emit::segment("component<", emit::line_break_mode::none),
                    type->emit(),
                    emit::segment(", ", emit::line_break_mode::none),
                    kdl::global_reference(id),
                    emit::segment("> ", emit::line_break_mode::none),
                    ns->emit()
                };
            }
            else {
                return {
                    emit::segment("component<", emit::line_break_mode::none),
                    ns->emit(), emit::segment(".", emit::line_break_mode::none), type->emit(),
                    emit::segment(", ", emit::line_break_mode::none),
                    kdl::global_reference(id),
                    emit::segment("> ", emit::line_break_mode::none),
                    ns->emit()
                };
            }
        }

        [[nodiscard]] static auto component_files_decl(const std::string& directory_path) -> emit::segment
        {
            return emit::segment("files(\"" + directory_path + "\")", emit::line_break_mode::none);
        }

        [[nodiscard]] static auto component_types_decl() -> emit::segment
        {
            return emit::segment("api_export", emit::line_break_mode::none);
        }

        [[nodiscard]] static auto component_basic_file(const std::string& file) -> emit::segment
        {
            return emit::segment("\"" + file + "\"", emit::line_break_mode::none);
        }

        [[nodiscard]] static auto component_named_file(const std::string& file, const std::string& name) -> emit::segment
        {
            return emit::segment("\"" + file + "\" -> \"" + name + "\"", emit::line_break_mode::none);
        }
#pragma endregion

#pragma region Resource Declaration Support
        [[nodiscard]] static auto resource_declaration_decl(const std::shared_ptr<ast::node>& ns, const std::shared_ptr<ast::node>& type) -> emit::segment
        {
            return {
                emit::segment("declare "),
                ns->emit(), emit::segment("."), type->emit()
            };
        }

        [[nodiscard]] static auto scene_declaration_decl(const std::shared_ptr<ast::node>& ns, std::int64_t id) -> emit::segment
        {
            return {
                emit::segment("scene<"),
                global_reference(id),
                emit::segment("> "),
                ns->emit()
            };
        }

        [[nodiscard]] static auto dialog_declaration_decl(const std::shared_ptr<ast::node>& ns, std::int64_t id) -> emit::segment
        {
            return {
                emit::segment("dialog<"),
                global_reference(id),
                emit::segment("> "),
                ns->emit()
            };
        }

        [[nodiscard]] static auto control_declaration_decl(const std::shared_ptr<ast::node>& type, const std::string& name) -> emit::segment
        {
            return {
                type->emit(),
                emit::segment(" ("),
                string(name),
                emit::segment(")")
            };
        }

        [[nodiscard]] static auto new_resource() -> emit::segment
        {
            return emit::segment("new");
        }
#pragma endregion

    };
}