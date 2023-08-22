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

#include <memory>
#include "builder/builder.hpp"
#include "builder/api/components/function.hpp"
#include "builder/api/components/luabridge.hpp"
#include "project/project.hpp"
#include "project/structure/construct_definition.hpp"
#include "project/structure/class/class_definition.hpp"
#include "project/structure/enrollment/enrollment.hpp"
#include <libCodeGen/builder/builder.hpp>
#include <libCodeGen/languages/procedural/scope.hpp>
#include <libCodeGen/ast/procedural.hpp>
#include <libScripting/annotations/macro.hpp>

namespace kdtool::builder
{
    template<codegen::language::general_procedural_support L>
    struct api : public std::enable_shared_from_this<api<L>>, public codegen::procedural_builder<L>, public base
    {
        api(const std::shared_ptr<project::index>& project, const std::string& root)
            : base(project), codegen::procedural_builder<L>(root)
        {}

        auto build() -> void override
        {
            codegen::procedural_builder<L>::build();

            // Start building the AST and generating syntax for it.
            generate_header_documentation();
            generate_includes();

            // Generate any required the forward declarations.
            for (const auto& definition : base::project()->all_definitions()) {
                generate_enrollment(definition, true);
            }

            generate_global_enrollment();

            for (const auto& definition : base::project()->all_definitions()) {
                generate_enrollment(definition);
            }

            // Save the contents of the generated source.
            codegen::procedural_builder<L>::save();
        }

    private:
        auto generate_header_documentation() -> void
        {
            codegen::procedural_builder<L>::template add<codegen::ast::comment<L>>("Lua API auto-generated by kdtool.");
            codegen::procedural_builder<L>::template add<codegen::ast::comment<L>>("Do not edit or commit this file into source control.");
            codegen::procedural_builder<L>::template add<codegen::ast::nl<L>>();
        }

        auto generate_includes() -> void
        {
            auto include_library = [&] (const std::string& path) {
                codegen::procedural_builder<L>::template add<codegen::ast::include_library<L>>(path);
            };

            include_library("memory");
            include_library("libKestrel/lua/runtime/runtime.hpp");
            for (const auto& path : base::project()->scanned_include_paths()) {
                include_library(path);
            }

            codegen::procedural_builder<L>::template add<codegen::ast::nl<L>>();
        }

        auto generate_global_enrollment() -> void
        {
            auto scope = codegen::procedural_builder<L>::scope();
            auto symbol = scope->symbol("kestrel::lua::enroll_lua_api");
            auto enroll_function = std::make_shared<codegen::ast::function<L>>(symbol->resolved_name_node());
            enroll_function->set_return_type(codegen::language::procedural::void_type<L>());

            enroll_function->add_parameter(
                std::make_shared<codegen::ast::symbol<L>>("lua"),
                codegen::language::procedural::const_qualified<L>(
                    codegen::language::procedural::reference<L>(
                        codegen::language::cxx::shared_ptr({ codegen::language::procedural::named<L>("kestrel::lua::runtime") })
                    )
                )
            );

            auto enroll_function_imp = std::make_shared<codegen::ast::compound_statement<L>>();
            enroll_function->set_statement(enroll_function_imp);

            for (const auto& definition : base::project()->all_definitions()) {
                std::vector<std::shared_ptr<codegen::ast::statement<L>>> args;

                if (definition->enrollment()->requires_runtime()) {
                    args.emplace_back(std::make_shared<codegen::ast::symbol_statement<L>>("lua"));
                }

                switch (definition->instance_type()) {
                    case project::structure::construct_definition::type::is_class: {
                        const auto& class_definition = std::reinterpret_pointer_cast<project::structure::class_definition>(definition);
                        if (!class_definition->all_template_variants().empty()) {
                            for (const auto& variant : class_definition->all_template_variants()) {
                                if (definition->enrollment()->requires_custom_name()) {
                                    args.insert(args.begin(), std::make_shared<codegen::ast::string_statement<L>>(variant.first));
                                }
                                auto enroll_function_symbol = generate_enrollment_symbol(definition, { variant.second });
                                auto call = std::make_shared<codegen::ast::function_call<L>>(
                                    enroll_function_symbol->resolved_name_node(), args
                                );
                                enroll_function_imp->add_statement(call);
                                enroll_function_imp->add_node(std::make_shared<codegen::ast::nl<L>>(), false);
                                args.erase(args.begin());
                            }
                            break;
                        }
                    }
                    default: {
                        auto enroll_function_symbol = generate_enrollment_symbol(definition);
                        auto call = std::make_shared<codegen::ast::function_call<L>>(
                            enroll_function_symbol->resolved_name_node(), args
                        );
                        enroll_function_imp->add_statement(call);
                        break;
                    }
                }
                enroll_function_imp->add_node(std::make_shared<codegen::ast::nl<L>>(), false);
            }

            codegen::procedural_builder<L>::add(enroll_function);
            codegen::procedural_builder<L>::template add<codegen::ast::nl<L>>();
        }

        auto generate_enrollment(
            const std::shared_ptr<project::structure::construct_definition>& definition,
            bool declaration_only = false
        ) -> void {
            if (!definition->enrollment()) {
                return;
            }
            auto enroll_function_symbol = generate_enrollment_symbol(definition);
            auto enroll_function = std::make_shared<codegen::ast::function<L>>(enroll_function_symbol->resolved_name_node());
            enroll_function->set_return_type(codegen::language::procedural::void_type<L>());

            if (definition->instance_type() == project::structure::construct_definition::type::is_class) {
                const auto class_definition = std::reinterpret_pointer_cast<project::structure::class_definition>(definition);
                if (!class_definition->all_template_variants().empty()) {
                    enroll_function->add_template_parameter(
                        codegen::language::procedural::named<L>("T")
                    );
                }
            }

            std::string definition_name = definition->symbol()->name();

            if (definition->enrollment()->requires_custom_name()) {
                definition_name = "name";
                enroll_function->add_parameter(
                    std::make_shared<codegen::ast::symbol<L>>(definition_name),
                    codegen::language::procedural::const_qualified<L>(
                        codegen::language::procedural::reference<L>(
                            codegen::language::procedural::string_type<L>()
                        )
                    )
                );
            }

            if (definition->enrollment()->requires_runtime()) {
                enroll_function->add_parameter(
                    std::make_shared<codegen::ast::symbol<L>>("lua"),
                    codegen::language::procedural::const_qualified<L>(
                        codegen::language::procedural::reference<L>(
                            codegen::language::cxx::shared_ptr({ codegen::language::procedural::named<L>("kestrel::lua::runtime") })
                        )
                    )
                );
            }

            if (!declaration_only) {
                auto enroll_function_imp = std::make_shared<codegen::ast::compound_statement<L>>();
                enroll_function->set_statement(enroll_function_imp);

                auto namespaces = foundation::string::split(definition->symbol()->display_name(), ".");
                namespaces.pop_back();

                enroll_function_imp->add_statement(
                    luabridge<L>::global_namespace()
                        .namespaces(namespaces, [&] (luabridge<L>& lua) {

                            switch (definition->instance_type()) {
                                case project::structure::construct_definition::type::is_class: {
                                    lua.add_class(std::reinterpret_pointer_cast<project::structure::class_definition>(definition));
                                    break;
                                }

                                case project::structure::construct_definition::type::is_enum: {
                                    const auto &enum_definition = std::reinterpret_pointer_cast<project::structure::enum_definition>(definition);
                                    codegen::procedural_builder<L>::add(lua.prepare_enum_cases(enum_definition));
                                    lua.add_enum(enum_definition);
                                    break;
                                }

                                case project::structure::construct_definition::type::is_namespace: {
                                    lua.add_namespace(std::reinterpret_pointer_cast<project::structure::namespace_definition>(definition));
                                    break;
                                }
                                default: break;
                            }

                        })
                        .end()
                );
            }
            else {
                if (definition->instance_type() != project::structure::construct_definition::type::is_enum) {
                    return;
                }
            }

            codegen::procedural_builder<L>::add(enroll_function);
        }

    private:
        auto generate_enrollment_symbol(
            const std::shared_ptr<project::structure::construct_definition>& definition,
            const std::vector<std::string>& template_parameters = {}
        ) -> std::shared_ptr<codegen::language::procedural::symbol<L>> {
            auto scope = codegen::procedural_builder<L>::scope();
            switch (definition->instance_type()) {
                case project::structure::construct_definition::type::is_namespace: {
                    return scope->symbol({
                        definition->symbol()->source_resolved_identifier(),
                        definition->enrollment()->symbol()->name()
                    });
                }
                case project::structure::construct_definition::type::is_class: {
                    const auto& class_definition = std::reinterpret_pointer_cast<project::structure::class_definition>(definition);
                    return scope->symbol({
                        definition->symbol()->source_resolved_identifier(L::scope_resolution_operator_string(), "", template_parameters),
                        definition->enrollment()->symbol()->name()
                    });
                }
                case project::structure::construct_definition::type::is_enum: {
                    return scope->symbol(
                        definition->symbol()->source_resolved_identifier(L::scope_resolution_operator_string(), "_") +
                        "_" + definition->enrollment()->symbol()->name()
                    );
                    break;
                }
                default:
                    // Construct type is not elligable for enrollment.
                    return nullptr;
            }
        }

    private:

    };
}