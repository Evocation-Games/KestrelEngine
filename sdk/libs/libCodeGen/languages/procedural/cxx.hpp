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
#include <libCodeGen/ast/procedural.hpp>

namespace codegen::language
{
    struct cxx
    {
        [[nodiscard]] static auto name() -> std::string { return "C++"; }
        [[nodiscard]] static auto lc_name() -> std::string { return "cxx"; };
        [[nodiscard]] static auto extension() -> std::string { return "cpp"; };

        // MARK: - Comment Support

        [[nodiscard]] static auto comment(const std::string& str) -> emit::segment
        {
            return emit::segment("// " + str, emit::line_break_mode::after);
        }

        // MARK: - Preprocessor Support

        [[nodiscard]] static auto include_library(const std::string& path) -> emit::segment
        {
            return emit::segment("#include <" + path + ">", emit::line_break_mode::after);
        }

        [[nodiscard]] static auto include_file(const std::string& path) -> emit::segment
        {
            return emit::segment("#include \"" + path + "\"", emit::line_break_mode::after);
        }

        // MARK: - Literal Support

        [[nodiscard]] static auto string(const std::string& str) -> emit::segment
        {
            return emit::segment("\"" + str + "\"");
        }

        // MARK: - Operator Support

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
            return "->";
        }

        [[nodiscard]] static auto scope_resolution_operator_string() -> std::string
        {
            return "::";
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
            return {
                emit::segment("static_cast<"),
                emit::segment(type),
                emit::segment(">("),
                value,
                emit::segment(")")
            };
        }

        // MARK: - Symbol Support

        [[nodiscard]] static auto symbol(const std::string& name) -> emit::segment
        {
            return emit::segment(name);
        }

        // MARK: - Type Support

        [[nodiscard]] static auto void_type_name() -> std::string
        {
            return scalar_type_name(procedural::type::scalar::void_type);
        }

        [[nodiscard]] static auto error_type_name() -> std::string
        {
            return "std::error";
        }

        [[nodiscard]] static auto auto_inference_type_name() -> std::string
        {
            return "auto";
        }

        [[nodiscard]] static auto string_type_name() -> std::string
        {
            return "std::string";
        }

        [[nodiscard]] static auto scalar_type_name(procedural::type::scalar type) -> std::string
        {
            switch (type) {
                case procedural::type::scalar::void_type: return "void";
                case procedural::type::scalar::boolean_type: return "bool";
                case procedural::type::scalar::char_type: return "std::int8_t";
                case procedural::type::scalar::short_type: return "std::int16_t";
                case procedural::type::scalar::long_type: return "std::int32_t";
                case procedural::type::scalar::long_long_type: return "std::int64_t";
                case procedural::type::scalar::uchar_type: return "std::uint8_t";
                case procedural::type::scalar::ushort_type: return "std::uint16_t";
                case procedural::type::scalar::ulong_type: return "std::uint32_t";
                case procedural::type::scalar::ulong_long_type: return "std::uint64_t";
                case procedural::type::scalar::int_type: return "int";
                case procedural::type::scalar::uint_type: return "unsigned int";
                case procedural::type::scalar::float_type: return "float";
                case procedural::type::scalar::double_type: return "double";
                case procedural::type::scalar::long_double_type: return "long double";
            }
        }

        [[nodiscard]] static auto const_qualified_type_name(const std::string& name) -> std::string
        {
            return "const " + name;
        }

        [[nodiscard]] static auto type_name_pointer(const std::string& name) -> std::string
        {
            return name + " " + pointer_operator_string();
        }

        [[nodiscard]] static auto type_name_reference(const std::string& name) -> std::string
        {
            return name + reference_operator_string();
        }

        [[nodiscard]] static auto template_specialized_type_name(const std::string& name, const std::vector<std::string>& specializations) -> std::string
        {
            std::string specialization_str;
            for (const auto& specialization : specializations) {
                if (!specialization_str.empty()) {
                    specialization_str += list_delimiter_string();
                }
                specialization_str  = specialization;
            }
            return name + "<" + specialization_str + ">";
        }

        [[nodiscard]] static auto template_specialization_decl(const std::vector<std::string>& params) -> emit::segment
        {
            if (params.empty()) {
                return emit::segment("");
            }

            std::vector<emit::segment> out;
            out.emplace_back("template<");
            for (const auto& param : params) {
                if (out.size() > 1) {
                    out.emplace_back(", ");
                }
                out.emplace_back("typename " + param);
            }
            out.emplace_back(">");
            return emit::segment(out);
        }

        [[nodiscard]] static auto shared_ptr(const std::initializer_list<procedural::type_system<cxx>::type_id>& specializations = {}) -> procedural::type_system<cxx>::type_id
        {
            return procedural::template_specailized<cxx>(
               procedural::named<cxx>("std::shared_ptr"),
               std::vector<procedural::type_system<cxx>::type_id>(specializations)
            );
        }

        [[nodiscard]] static auto string() -> procedural::type_system<cxx>::type_id
        {
            return procedural::string_type<cxx>();
        }

        // MARK: - Function Support

        [[nodiscard]] static auto function_decl(
            const std::shared_ptr<ast::node>& name,
            const std::string& return_type,
            const std::shared_ptr<ast::node>& parameter_list
        ) -> emit::segment {
            return {
                emit::segment("auto ", emit::line_break_mode::before),
                name->emit(),
                emit::segment("("),
                parameter_list->emit(),
                emit::segment(") -> "),
                emit::segment(return_type),
            };
        }

        [[nodiscard]] static auto function_name(const std::shared_ptr<ast::node>& name) -> emit::segment
        {
            return name->emit();
        }

        [[nodiscard]] static auto parameter_list(const std::vector<emit::segment>& list) -> emit::segment
        {
            if (list.empty()) {
                return emit::segment("");
            }

            std::vector<emit::segment> segments;
            segments.reserve((list.size() * 2) - 1);
            for (const auto& parameter : list) {
                if (!segments.empty()) {
                    segments.emplace_back(list_delimiter_string());
                }
                segments.emplace_back(parameter);
            }
            return emit::segment(segments);
        }

        [[nodiscard]] static auto parameter(const std::shared_ptr<ast::node>& name, const std::string& type) -> emit::segment
        {
            return { emit::segment(type), emit::segment(" "), name->emit() };
        }

        [[nodiscard]] static auto function_type(const std::vector<emit::segment>& parameters, const std::string& type) -> emit::segment
        {
            return {
                emit::segment("auto(*)("),
                emit::segment(parameter_list(parameters)),
                emit::segment(")->"),
                emit::segment(type)
            };
        }

        // MARK: - Statement Support

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
            return {
                name->emit(),
                emit::segment("<"),
                emit::segment(specializations),
                emit::segment(">"),
                emit::segment("("),
                emit::segment(args),
                emit::segment(")")
            };
        }

        [[nodiscard]] static auto address_of(const std::shared_ptr<ast::node>& node) -> emit::segment
        {
            return {
                emit::segment(address_of_operator_string()),
                node->emit()
            };
        }

        [[nodiscard]] static auto static_statement(const emit::segment& statement) -> emit::segment
        {
            return {
                emit::segment("static "),
                statement
            };
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
                emit::segment(type),
                emit::segment(" "),
                name->emit()
            };
        }

        // MARK: - Component Statement Support

        [[nodiscard]] static auto begin_compound_statement() -> emit::segment
        {
            return emit::segment("{", emit::line_break_mode::full, emit::indentation_mode::indent_after);
        }

        [[nodiscard]] static auto end_compound_statement() -> emit::segment
        {
            return emit::segment("}", emit::line_break_mode::full, emit::indentation_mode::outdent_before);
        }

    };
}