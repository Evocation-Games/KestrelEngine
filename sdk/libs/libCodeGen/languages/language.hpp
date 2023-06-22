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
#include <vector>
#include <memory>
#include <concepts>
#include <type_traits>
#include <libCodeGen/ast/core/node.hpp>
#include <libCodeGen/builder/emit/segment.hpp>
#include <libCodeGen/languages/procedural/scalar.hpp>

#if !defined(NamedArgument)
#   define NamedArgument(_n, _arg)  _arg
#endif

namespace codegen::language
{
    struct statement;

    // MARK: - Common

    template<class L>
    concept metadata = requires(const L& lang, const std::shared_ptr<ast::node>& node) {
        // Metadata
        { L::name() } -> std::same_as<std::string>;
        { L::lc_name() } -> std::same_as<std::string>;
        { L::extension() } -> std::same_as<std::string>;
    };

    // MARK: - Procedural Languages

    template<class L>
    concept preprocessor_support = requires(const L& lang, const std::string& str) {
        requires metadata<L>;
        { L::include_library(str) } -> std::same_as<emit::segment>;
        { L::include_file(str) } -> std::same_as<emit::segment>;
    };

    template<class L>
    concept comment_support = requires(const L& lang, const std::string& str) {
        requires metadata<L>;
        { L::comment(str) } -> std::same_as<emit::segment>;
    };

    template<class L>
    concept literal_support = requires(const L& lang, const std::string& str) {
        requires metadata<L>;
        { L::string(str) } -> std::same_as<emit::segment>;
    };

    template<class L>
    concept operator_support = requires(const L& lang, const std::string& str, const emit::segment& segment) {
        requires metadata<L>;
        { L::statement_end_operator_string() } -> std::same_as<std::string>;
        { L::list_delimiter_string() } -> std::same_as<std::string>;
        { L::addition_operator_string() } -> std::same_as<std::string>;
        { L::subtraction_operator_string() } -> std::same_as<std::string>;
        { L::multiplication_operator_string() } -> std::same_as<std::string>;
        { L::division_operator_string() } -> std::same_as<std::string>;
        { L::modulo_operator_string() } -> std::same_as<std::string>;
        { L::reference_member_operator_string() } -> std::same_as<std::string>;
        { L::pointer_member_operator_string() } -> std::same_as<std::string>;
        { L::dereference_operator_string() } -> std::same_as<std::string>;
        { L::address_of_operator_string() } -> std::same_as<std::string>;
        { L::pointer_operator_string() } -> std::same_as<std::string>;
        { L::reference_operator_string() } -> std::same_as<std::string>;
        { L::scope_resolution_operator_string() } -> std::same_as<std::string>;
        { L::assignment_operator_string() } -> std::same_as<std::string>;
        { L::static_cast_operator(NamedArgument(Type, str),
                                  NamedArgument(Value, segment)) } -> std::same_as<emit::segment>;
    };

    template<class L>
    concept symbol_support = requires(const L& lang, const std::string& str) {
        requires metadata<L>;
        { L::symbol(str) } -> std::same_as<emit::segment>;
    };

    template<class L>
    concept type_support = requires(
        const L& lang,
        const std::string& str,
        procedural::type::scalar scalar,
        const std::vector<std::string>& str_list
    ) {
        requires metadata<L>;
        requires symbol_support<L>;
        { L::void_type_name() } -> std::same_as<std::string>;
        { L::error_type_name() } -> std::same_as<std::string>;
        { L::auto_inference_type_name() } -> std::same_as<std::string>;
        { L::string_type_name() } -> std::same_as<std::string>;
        { L::scalar_type_name(scalar) } -> std::same_as<std::string>;
        { L::const_qualified_type_name(str) } -> std::same_as<std::string>;
        { L::type_name_pointer(str) } -> std::same_as<std::string>;
        { L::type_name_reference(str) } -> std::same_as<std::string>;
        { L::template_specialized_type_name(str, str_list) } -> std::same_as<std::string>;
    };

    template<class L>
    concept function_support = requires(
        const L& lang,
        const std::string& str,
        const std::shared_ptr<ast::node>& node,
        const std::vector<emit::segment>& segments
    ) {
        requires metadata<L>;
        requires symbol_support<L>;
        { L::function_decl(NamedArgument(Name, node),
                           NamedArgument(ReturnType, str),
                           NamedArgument(Parameters, node)) } -> std::same_as<emit::segment>;
        { L::function_name(NamedArgument(Name, node)) } -> std::same_as<emit::segment>;
        { L::parameter_list(NamedArgument(Parameters, segments)) } -> std::same_as<emit::segment>;
        { L::parameter(NamedArgument(Name, node), NamedArgument(Type, str)) } -> std::same_as<emit::segment>;
        { L::function_type(NamedArgument(Parameters, segments),
                           NamedArgument(ReturnType, str)) } -> std::same_as<emit::segment>;
    };

    template<class L>
    concept statement_support = requires(
        const L& lang,
        const std::string& str,
        const std::shared_ptr<ast::node>& node,
        const emit::segment& segment,
        const std::vector<emit::segment>& segments
    ) {
        requires metadata<L>;
        requires operator_support<L>;
        requires function_support<L>;
        { L::function_call(NamedArgument(Function, node),
                           NamedArgument(Arguments, segments)) } -> std::same_as<emit::segment>;
        { L::specialized_function_call(NamedArgument(Function, node),
                                       NamedArgument(Specializations, segments),
                                       NamedArgument(Arguments, segments)) } -> std::same_as<emit::segment>;
        { L::address_of(NamedArgument(Symbol, node)) } -> std::same_as<emit::segment>;
        { L::static_statement(NamedArgument(Statement, segment)) } -> std::same_as<emit::segment>;
        { L::assignment_statement(NamedArgument(lhs, segment),
                                  NamedArgument(rhs, segment)) } -> std::same_as<emit::segment>;
        { L::variable_decl_statement(NamedArgument(Name, node),
                                     NamedArgument(Type, str)) } -> std::same_as<emit::segment>;
    };

    template<class L>
    concept compound_statement_support = requires(
        const L& lang
    ) {
        requires metadata<L>;
        requires statement_support<L>;
        { L::begin_compound_statement() } -> std::same_as<emit::segment>;
        { L::end_compound_statement() } -> std::same_as<emit::segment>;
    };

    template<class L>
    concept general_procedural_support = requires(const L& lang, const std::string& str) {
        requires metadata<L>;
        requires preprocessor_support<L>;
        requires comment_support<L>;
        requires literal_support<L>;
        requires operator_support<L>;
        requires symbol_support<L>;
        requires type_support<L>;
        requires function_support<L>;
        requires statement_support<L>;
        requires compound_statement_support<L>;
    };

    // MARK: - Markup Languages

    template<class L>
    concept markup_support = requires(const L& lang, const std::string& str, std::int32_t i32) {
        requires metadata<L>;

        { L::text(str) } -> std::same_as<emit::segment>;
        { L::bold(str) } -> std::same_as<emit::segment>;
        { L::italic(str) } -> std::same_as<emit::segment>;
        { L::strikethrough(str) } -> std::same_as<emit::segment>;
        { L::inline_code(str) } -> std::same_as<emit::segment>;

        { L::anchor(str, str) } -> std::same_as<emit::segment>;

        { L::heading(str, i32) } -> std::same_as<emit::segment>;
        { L::preformatted(str) } -> std::same_as<emit::segment>;
        { L::blockquote(str) } -> std::same_as<emit::segment>;
        { L::divider() } -> std::same_as<emit::segment>;

        { L::begin_list() } -> std::same_as<emit::segment>;
        { L::begin_sublist() } -> std::same_as<emit::segment>;
        { L::begin_list_item() } -> std::same_as<emit::segment>;
        { L::end_list_item() } -> std::same_as<emit::segment>;
        { L::end_sublist() } -> std::same_as<emit::segment>;
        { L::end_list() } -> std::same_as<emit::segment>;

        { L::begin_table() } -> std::same_as<emit::segment>;
        { L::begin_table_header() } -> std::same_as<emit::segment>;
        { L::begin_table_header_cell() } -> std::same_as<emit::segment>;
        { L::end_table_header_cell() } -> std::same_as<emit::segment>;
        { L::end_table_header() } -> std::same_as<emit::segment>;
        { L::begin_table_row() } -> std::same_as<emit::segment>;
        { L::begin_table_cell() } -> std::same_as<emit::segment>;
        { L::end_table_cell() } -> std::same_as<emit::segment>;
        { L::end_table_row() } -> std::same_as<emit::segment>;
        { L::end_table() } -> std::same_as<emit::segment>;
    };

}