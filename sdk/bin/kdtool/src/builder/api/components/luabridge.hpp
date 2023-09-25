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

#include <functional>
#include <libCodeGen/languages/language.hpp>
#include <libCodeGen/builder/emit/segment.hpp>
#include <libCodeGen/ast/statement/statement.hpp>
#include <libCodeGen/ast/procedural.hpp>
#include "project/structure/namespace/namespace_definition.hpp"
#include "project/structure/enum/enum_definition.hpp"
#include "project/structure/class/class_definition.hpp"
#include "project/structure/class/property/property_definition.hpp"
#include "project/structure/class/constructor/constructor_definition.hpp"
#include "project/structure/function/function_definition.hpp"

namespace kdtool::builder
{
    template<codegen::language::general_procedural_support L>
    struct luabridge
    {
        luabridge()
        {
            add(codegen::emit::segment({
                codegen::ast::member_function_call<L>(m_symbols.global_namespace, m_symbols.lua).emit()
            }, codegen::emit::line_break_mode::none, codegen::emit::indentation_mode::indent_after));
        };

        static auto global_namespace() -> luabridge<L> { return {}; }

        [[nodiscard]] auto end() const -> std::shared_ptr<codegen::ast::segment_statement<L>>
        {
            auto segments = m_segment_sequence;
            for (auto i = 0; i < m_indent; ++i) {
                segments.emplace_back("", codegen::emit::line_break_mode::before, codegen::emit::indentation_mode::outdent_before);
            }
            return std::make_shared<codegen::ast::segment_statement<L>>(segments);
        }

        auto namespaces(const std::vector<std::string>& names, const std::function<auto(luabridge<L>&)->void>& fn) -> luabridge<L>&
        {
            for (const auto& name : names) {
                begin_namespace(name);
            }
            fn(*this);
            for (const auto& name : names) {
                end_namespace();
            }
            return *this;
        }

        auto define_class(const std::string& source_name, const std::string& name, const std::function<auto(luabridge<L>&)->void>& fn) -> luabridge<L>&
        {
            begin_class(source_name, name);
            fn(*this);
            end_class();
            return *this;
        }

        auto begin_namespace(const std::string& name) -> luabridge<L>&
        {
            m_indent++;
            add(codegen::emit::segment({
                codegen::emit::segment(L::reference_member_operator_string()),
                codegen::ast::function_call<L>(m_symbols.beginNamespace, {
                    std::make_shared<codegen::ast::string_statement<L>>(name)
                }).emit()
            }, codegen::emit::line_break_mode::before, codegen::emit::indentation_mode::indent_after));
            return *this;
        }

        auto end_namespace() -> luabridge<L>&
        {
            m_indent--;
            add(codegen::emit::segment({
                codegen::emit::segment(L::reference_member_operator_string()),
                codegen::ast::function_call<L>(m_symbols.endNamespace).emit()
            }, codegen::emit::line_break_mode::before, codegen::emit::indentation_mode::outdent_before));
            return *this;
        }

        auto begin_class(const std::string& source_name, const std::string& name) -> luabridge<L>
        {
            std::vector<std::shared_ptr<codegen::ast::statement<L>>> specializations;
            specializations.emplace_back(std::make_shared<codegen::ast::symbol_statement<L>>(source_name));

            std::shared_ptr<codegen::ast::statement<L>> name_arg = std::make_shared<codegen::ast::string_statement<L>>(name);
            if (name == "name.c_str()") {
                // TODO: Fix this
                name_arg = std::make_shared<codegen::ast::function_call<L>>(
                    std::make_shared<codegen::ast::symbol<L>>("name.c_str")
                );
            }

            m_indent++;
            add(codegen::emit::segment({
                codegen::emit::segment(L::reference_member_operator_string()),
                codegen::ast::template_specialized_function_call<L>(m_symbols.beginClass, specializations, {
                    name_arg
                }).emit()
            }, codegen::emit::line_break_mode::before, codegen::emit::indentation_mode::indent_after));
            return *this;
        }

        auto end_class() -> luabridge<L>&
        {
            m_indent--;
            add(codegen::emit::segment({
                codegen::emit::segment(L::reference_member_operator_string()),
                codegen::ast::function_call<L>(m_symbols.endClass).emit()
            }, codegen::emit::line_break_mode::before, codegen::emit::indentation_mode::outdent_before));
            return *this;
        }

        auto add_constructor(const std::vector<std::string>& parameters, const std::string& type) -> luabridge<L>&
        {
            std::vector<codegen::emit::segment> parameter_segments;
            parameter_segments.reserve(parameters.size());
            for (const auto& parameter : parameters) {
                parameter_segments.emplace_back(parameter);
            }

            std::vector<std::shared_ptr<codegen::ast::statement<L>>> specializations;
            specializations.emplace_back(std::make_shared<codegen::ast::function_type<L>>(L::void_type_name(), parameter_segments));
            specializations.emplace_back(std::make_shared<codegen::ast::symbol_statement<L>>(type));

            add(codegen::emit::segment({
                codegen::emit::segment(L::reference_member_operator_string()),
                codegen::ast::template_specialized_function_call<L>(m_symbols.addConstructor, specializations).emit()
            }, codegen::emit::line_break_mode::before));
            return *this;
        }

        auto add_property(const std::string& name, const std::string& getter_symbol, const std::string& setter_symbol = "") -> luabridge<L>&
        {
            add(codegen::emit::segment({
                codegen::emit::segment(L::reference_member_operator_string()),
                codegen::ast::function_call<L>(m_symbols.addProperty, {
                    std::make_shared<codegen::ast::string_statement<L>>(name),
                    std::make_shared<codegen::ast::address_of_statement<L>>(
                        std::make_shared<codegen::ast::symbol<L>>(getter_symbol)
                    ),
                    setter_symbol.empty() ? nullptr : std::make_shared<codegen::ast::address_of_statement<L>>(
                        std::make_shared<codegen::ast::symbol<L>>(setter_symbol)
                    )
                }).emit()
            }, codegen::emit::line_break_mode::before));
            return *this;
        }

        auto add_static_property(const std::string& name, const std::string& getter_symbol, const std::string& setter_symbol = "") -> luabridge<L>&
        {
            add(codegen::emit::segment({
                codegen::emit::segment(L::reference_member_operator_string()),
                codegen::ast::function_call<L>(m_symbols.addStaticProperty, {
                    std::make_shared<codegen::ast::string_statement<L>>(name),
                    std::make_shared<codegen::ast::address_of_statement<L>>(
                        std::make_shared<codegen::ast::symbol<L>>(getter_symbol)
                    ),
                    setter_symbol.empty() ? nullptr : std::make_shared<codegen::ast::address_of_statement<L>>(
                        std::make_shared<codegen::ast::symbol<L>>(setter_symbol)
                    )
                }).emit()
            }, codegen::emit::line_break_mode::before));
            return *this;
        }

        auto add_variable() -> luabridge<L>&
        {
            return *this;
        }

        auto add_function(const std::string& name, const std::string& symbol) -> luabridge<L>&
        {
            add(codegen::emit::segment({
                codegen::emit::segment(L::reference_member_operator_string()),
                codegen::ast::function_call<L>(m_symbols.addFunction, {
                    std::make_shared<codegen::ast::string_statement<L>>(name),
                    std::make_shared<codegen::ast::address_of_statement<L>>(
                        std::make_shared<codegen::ast::symbol<L>>(symbol)
                    ),
                }).emit()
            }, codegen::emit::line_break_mode::before));
            return *this;
        }

        auto add_static_function(const std::string& name, const std::string& symbol) -> luabridge<L>&
        {
            add(codegen::emit::segment({
                codegen::emit::segment(L::reference_member_operator_string()),
                codegen::ast::function_call<L>(m_symbols.addStaticFunction, {
                    std::make_shared<codegen::ast::string_statement<L>>(name),
                    std::make_shared<codegen::ast::address_of_statement<L>>(
                        std::make_shared<codegen::ast::symbol<L>>(symbol)
                    ),
                }).emit()
            }, codegen::emit::line_break_mode::before));
            return *this;
        }

    public:

        auto add_class(const std::shared_ptr<project::structure::class_definition>& definition) -> luabridge<L>&
        {
            auto class_symbol = definition->symbol()->resolved_source_identifier(
                L::scope_resolution_operator_string(),
                definition->symbol()->source_template_parameters()
            );

            auto name = definition->symbol()->lua_identifier();
            if (definition->enrollment()->requires_custom_name()) {
                name = "name.c_str()";
            }

            return define_class(class_symbol, name, [&] (auto& lua) {
                add_constructor(definition);

                for (const auto& property : definition->all_properties()) {
                    add_property(property);
                }

                for (const auto& function : definition->all_functions()) {
                    add_function(function);
                }
            });
        }

        auto add_enum(const std::shared_ptr<project::structure::enum_definition>& definition) -> luabridge<L>&
        {
            auto prefix = definition->symbol()->resolved_source_identifier("_");
            return namespaces({ definition->symbol()->lua_identifier() }, [&] (auto& lua) {
                for (const auto& case_definition : definition->all_cases()) {
                    lua.add_property(
                        case_definition->symbol()->lua_identifier(),
                        case_definition->static_symbol_suggestion(prefix)
                    );
                }
            });
        }

        auto prepare_enum_cases(const std::shared_ptr<project::structure::enum_definition>& definition) -> codegen::emit::segment
        {
            auto prefix = definition->symbol()->resolved_source_identifier("_");
            std::vector<codegen::emit::segment> out;
            for (const auto& case_definition : definition->all_cases()) {
                auto stmt = std::make_shared<codegen::ast::static_statement<L>>(
                    std::make_shared<codegen::ast::assignment_statement<L>>(
                        std::make_shared<codegen::ast::variable_decl_statement<L>>(
                            std::make_shared<codegen::ast::symbol<L>>(case_definition->static_symbol_suggestion(prefix)),
                            codegen::language::procedural::auto_type<L>()
                        ),
                        std::make_shared<codegen::ast::cast_statement<L>>(
                            std::make_shared<codegen::ast::symbol_statement<L>>(
                                case_definition->symbol()->resolved_source_identifier(L::scope_resolution_operator_string())
                            ),
                            codegen::language::procedural::long_type<L>()
                        )
                    )
                );

                out.emplace_back(stmt->emit());
                out.emplace_back(L::statement_end_operator_string(), codegen::emit::line_break_mode::after);
            }
            return codegen::emit::segment(out);
        }

        auto add_namespace(const std::shared_ptr<project::structure::namespace_definition>& definition) -> luabridge<L>&
        {
            return namespaces({ definition->symbol()->lua_identifier() }, [&] (auto& lua) {
                for (const auto& property : definition->all_properties()) {
                    add_property(property);
                }

                for (const auto& function : definition->all_functions()) {
                    add_function(function);
                }
            });
        }

        auto add_constructor(const std::shared_ptr<project::structure::class_definition>& definition) -> luabridge<L>&
        {
            if (definition && definition->constructor() && definition->enrollment()->is_defined()) {
                add_constructor(
                    definition->constructor()->parameter_type_names(),
                    definition->enrollment()->reference()->name()
                );
            }
            return *this;
        }

        auto add_property(const std::shared_ptr<project::structure::property_definition>& definition) -> luabridge<L>&
        {
            if (definition->is_static()) {
                add_static_property(
                    definition->symbol()->lua_identifier(),
                    definition->getter() ? definition->getter()->symbol()->resolved_source_identifier(L::scope_resolution_operator_string()) : "",
                    definition->setter() ? definition->setter()->symbol()->resolved_source_identifier(L::scope_resolution_operator_string()) : ""
                );
            }
            else {
                add_property(
                    definition->symbol()->lua_identifier(),
                    definition->getter() ? definition->getter()->symbol()->resolved_source_identifier(L::scope_resolution_operator_string()) : "",
                    definition->setter() ? definition->setter()->symbol()->resolved_source_identifier(L::scope_resolution_operator_string()) : ""
                );
            }
            return *this;
        }

        auto add_function(const std::shared_ptr<project::structure::function_definition>& definition) -> luabridge<L>&
        {
            if (definition->is_static()) {
                add_static_function(
                    definition->symbol()->lua_identifier(),
                    definition->symbol()->resolved_source_identifier(L::scope_resolution_operator_string())
                );
            }
            else {
                add_function(
                    definition->symbol()->lua_identifier(),
                    definition->symbol()->resolved_source_identifier(L::scope_resolution_operator_string())
                );
            }
            return *this;
        }

    private:
        auto add(const codegen::emit::segment& segment) -> void
        {
            m_segment_sequence.emplace_back(segment);
        }

    private:
        std::int32_t m_indent { 1 };
        struct {
            std::shared_ptr<codegen::ast::symbol<L>> lua { std::make_shared<codegen::ast::symbol<L>>("lua") };
            std::shared_ptr<codegen::ast::symbol<L>> global_namespace { std::make_shared<codegen::ast::symbol<L>>("global_namespace") };
            std::shared_ptr<codegen::ast::symbol<L>> beginNamespace { std::make_shared<codegen::ast::symbol<L>>("beginNamespace") };
            std::shared_ptr<codegen::ast::symbol<L>> endNamespace { std::make_shared<codegen::ast::symbol<L>>("endNamespace") };
            std::shared_ptr<codegen::ast::symbol<L>> beginClass { std::make_shared<codegen::ast::symbol<L>>("beginClass") };
            std::shared_ptr<codegen::ast::symbol<L>> endClass { std::make_shared<codegen::ast::symbol<L>>("endClass") };
            std::shared_ptr<codegen::ast::symbol<L>> addConstructor { std::make_shared<codegen::ast::symbol<L>>("addConstructor") };
            std::shared_ptr<codegen::ast::symbol<L>> addProperty { std::make_shared<codegen::ast::symbol<L>>("addProperty") };
            std::shared_ptr<codegen::ast::symbol<L>> addVariable { std::make_shared<codegen::ast::symbol<L>>("addVariable") };
            std::shared_ptr<codegen::ast::symbol<L>> addFunction { std::make_shared<codegen::ast::symbol<L>>("addFunction") };
            std::shared_ptr<codegen::ast::symbol<L>> addStaticFunction { std::make_shared<codegen::ast::symbol<L>>("addStaticFunction") };
            std::shared_ptr<codegen::ast::symbol<L>> addStaticProperty { std::make_shared<codegen::ast::symbol<L>>("addStaticProperty") };
        } m_symbols;
        std::vector<codegen::emit::segment> m_segment_sequence;
    };
}