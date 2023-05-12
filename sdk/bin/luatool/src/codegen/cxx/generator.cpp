// Copyright (c) 2022 Tom Hancocks
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

#include <fstream>
#include <functional>
#include "codegen/cxx/generator.hpp"

// MARK: - Translation Unit Management

auto luatool::codegen::cxx::generator::add_translation_unit(lua_api::ast::translation_unit *tu) -> void
{
    require_inclusion_of(tu->path());
    for (auto ns : tu->namespaces()) {
        m_aggregator.merge_namespace(ns);
    }

    for (auto klass : tu->classes()) {
        m_aggregator.merge_class(klass);
    }

    for (auto lua_enum : tu->enums()) {
        m_aggregator.merge_enum(lua_enum);
    }
}

// MARK: - Inclusions

auto luatool::codegen::cxx::generator::require_inclusion_of(const std::string &path) -> void
{
    if (std::find(m_includes.begin(), m_includes.end(), path) == m_includes.end()) {
        m_includes.emplace_back(path);
    }
}

// MARK: - Compiler

auto luatool::codegen::cxx::generator::compile(const std::string& path) -> void
{
    m_buffer.emit_comment("Kestrel Lua API auto-generated by luatool.");
    m_buffer.emit_comment("Do not edit or commit this file into source control.");
    m_buffer.newline();

    compile_inclusions();
    m_buffer.newline();

    for (auto lua_enum : m_aggregator.all_enums()) {
        compile_enum_enrollment(lua_enum);
    }

    compile_global_enrollment();

    for (auto ns : m_aggregator.all_namespaces()) {
        compile_namespace_enrollment(ns);
    }

    for (auto klass : m_aggregator.all_classes()) {
        compile_class_enrollment(klass);
    }

    std::ofstream file(path);
    file << m_buffer.string();
}

auto luatool::codegen::cxx::generator::compile_inclusions() -> void
{
    // Default Inclusions
    m_buffer.emit_include_system("memory");
    m_buffer.emit_include_system("libKestrel/lua/runtime/runtime.hpp");

    // Inferred Inclusions
    for (const auto& inclusion : m_includes) {
        m_buffer.emit_include_system(inclusion);
    }
}


// MARK: - Global Enrollment

auto luatool::codegen::cxx::generator::compile_global_enrollment() -> void
{
    lua_api::ast::namespace_path kestrel_path("kestrel");
    auto kestrel_lua_path = kestrel_path.nested("lua");

    auto runtime_symbol = lua_api::ast::symbol::make_cxx("runtime", false, kestrel_lua_path);
    auto enroll_lua_api_symbol = lua_api::ast::symbol::make_cxx("enroll_lua_api", false, kestrel_lua_path);

    auto enroll_lua_api = std::make_shared<lua_api::ast::lua_function>(enroll_lua_api_symbol);
    enroll_lua_api->set_return_type(lua_api::ast::type::void_type());
    enroll_lua_api->add_parameter(lua_api::ast::lua_parameter("runtime", lua_api::ast::type::shared_ptr(runtime_symbol->cxx_resolved_identifier())));

    m_buffer.emit_section_comment("Global Enrollment");
    m_buffer.emit_function(enroll_lua_api);
    m_buffer.emit_block_start();

    m_buffer.emit_comment("Namespaces");
    for (auto ns : m_aggregator.all_namespaces()) {
        const auto& enrollment = ns->enrollment();
        if (!enrollment.is_defined()) {
            continue;
        }
        m_buffer.start_line();
        m_buffer.emit_resolved_symbol(ns->object_symbol());
        m_buffer.emit("::", -1, false);
        m_buffer.emit_symbol(enrollment.symbol());

        std::vector<std::shared_ptr<lua_api::ast::symbol>> arguments;
        if (enrollment.requires_runtime()) {
            arguments.emplace_back(runtime_symbol);
        }
        m_buffer.emit_arguments(arguments);
        m_buffer.semicolon();
    }

    m_buffer.newline();
    m_buffer.emit_comment("Classes");
    for (auto klass : m_aggregator.all_classes()) {
        const auto& enrollment = klass->enrollment();
        if (!enrollment.is_defined()) {
            continue;
        }

        if (klass->variants().empty()) {
            m_buffer.start_line();
            m_buffer.emit_resolved_symbol(klass->object_symbol());
            m_buffer.emit("::", -1, false);
            m_buffer.emit_symbol(enrollment.symbol());

            std::vector<std::shared_ptr<lua_api::ast::symbol>> arguments;
            if (enrollment.requires_runtime()) {
                arguments.emplace_back(runtime_symbol);
            }
            m_buffer.emit_arguments(arguments);
            m_buffer.semicolon();
        }
        else {
            for (auto variant : klass->variants()) {
                m_buffer.start_line();
                m_buffer.emit_resolved_symbol(klass->object_symbol());
                m_buffer.emit("<", -1, false);
                m_buffer.emit_type(variant.second);
                m_buffer.emit(">", -1, false);
                m_buffer.emit("::", -1, false);
                m_buffer.emit_symbol(enrollment.symbol());

                std::vector<std::shared_ptr<lua_api::ast::symbol>> arguments;
                if (enrollment.requires_custom_name()) {
                    arguments.emplace_back(lua_api::ast::symbol::make_cxx("\"" + variant.first + "\""));
                }
                if (enrollment.requires_runtime()) {
                    arguments.emplace_back(runtime_symbol);
                }
                m_buffer.emit_arguments(arguments);
                m_buffer.semicolon();
            }
        }

    }

    m_buffer.newline();
    m_buffer.emit_comment("Enums");
    for (auto lua_enum : m_aggregator.all_enums()) {
        const auto& enrollment = lua_enum->enrollment();
        if (!enrollment.is_defined()) {
            continue;
        }

        auto enroll_name_path = lua_enum->object_symbol()->cxx_namespace_path().path_string("_");
        auto enroll_symbol = lua_api::ast::symbol::make_cxx("enroll_lua_api_enum_" + enroll_name_path + "_" + lua_enum->object_symbol()->cxx_identifier());

        m_buffer.start_line();
        m_buffer.emit_symbol(enroll_symbol);

        std::vector<std::shared_ptr<lua_api::ast::symbol>> arguments;
        if (enrollment.requires_runtime()) {
            arguments.emplace_back(runtime_symbol);
        }
        m_buffer.emit_arguments(arguments);
        m_buffer.semicolon();
    }

    m_buffer.emit_block_end();
    m_buffer.newline(2);
}

// MARK: - Namespace Enrollment

auto luatool::codegen::cxx::generator::compile_namespace_enrollment(std::shared_ptr<lua_api::ast::lua_namespace> ns) -> void
{
    if (!ns->enrollment().is_defined()) {
        return;
    }

    const auto& enrollment = ns->enrollment();

    lua_api::ast::namespace_path kestrel_path("kestrel");
    auto kestrel_lua_path = kestrel_path.nested("lua");

    auto runtime_symbol = lua_api::ast::symbol::make_cxx("runtime", false, kestrel_lua_path);
    auto enroll_symbol = ns->object_symbol()->cxx_member(enrollment.symbol()->cxx_identifier());

    auto enroll_lua_api = std::make_shared<lua_api::ast::lua_function>(enroll_symbol);
    enroll_lua_api->set_return_type(lua_api::ast::type::void_type());
    enroll_lua_api->add_parameter(lua_api::ast::lua_parameter("runtime", lua_api::ast::type::shared_ptr(runtime_symbol->cxx_resolved_identifier())));

    m_buffer.emit_function(enroll_lua_api);
    m_buffer.emit_block_start();

    m_buffer.start_line();
    m_buffer.emit_symbol(runtime_symbol);
    m_buffer.emit("->global_namespace()", -1);
    m_buffer.increase_indent();

    begin_namespaces(ns->object_symbol()->lua_namespace_path(), [&] {
        begin_namespace(ns->object_symbol());

        // Variables
        for (auto variable : ns->variables()) {
            m_buffer.emit(".addData(\"", 0, false);
            m_buffer.emit_lua_symbol(variable->symbol());
            m_buffer.emit("\", &", -1, false);
            m_buffer.emit_symbol(ns->object_symbol());
            m_buffer.emit("::", -1, false);
            m_buffer.emit_symbol(variable->symbol());
            m_buffer.emit(", ", -1, false);
            m_buffer.emit(variable->is_mutable() ? "true" : "false", -1, false);
            m_buffer.emit(")", -1);
        }

        // Properties
        for (auto property : ns->properties()) {
            m_buffer.emit(".addProperty(\"", 0, false);
            m_buffer.emit_lua_symbol(property->getter());
            m_buffer.emit("\", &", -1, false);
            m_buffer.emit_symbol(ns->object_symbol());
            m_buffer.emit("::", -1, false);
            m_buffer.emit_symbol(property->getter());

            if (property->has_setter()) {
                m_buffer.emit(", &", -1, false);
                m_buffer.emit_symbol(ns->object_symbol());
                m_buffer.emit("::", -1, false);
                m_buffer.emit_symbol(property->setter());
            }

            m_buffer.emit(")", -1);
        }

        // Functions
        for (auto function : ns->functions()) {
            m_buffer.emit(".addFunction(\"", 0, false);
            m_buffer.emit_lua_symbol(function->symbol());
            m_buffer.emit("\", &", -1, false);
            m_buffer.emit_symbol(ns->object_symbol());
            m_buffer.emit("::", -1, false);
            m_buffer.emit_symbol(function->symbol());
            m_buffer.emit(")", -1);
        }

        end_namespace();
    });

    m_buffer.decrease_indent();
    m_buffer.emit_block_end();
    m_buffer.newline();
}

// MARK: - Class Enrollment

auto luatool::codegen::cxx::generator::compile_class_enrollment(std::shared_ptr<lua_api::ast::lua_class> klass) -> void
{
    if (!klass->enrollment().is_defined()) {
        return;
    }

    const auto& enrollment = klass->enrollment();

    lua_api::ast::namespace_path kestrel_path("kestrel");
    auto kestrel_lua_path = kestrel_path.nested("lua");

    std::string template_symbol;
    if (klass->is_templated()) {
        m_buffer.emit("template<", 0, false);
        template_symbol += "<";
        bool first = true;
        for (auto type : klass->templates()) {
            if (!first) {
                m_buffer.emit(", ", -1, false);
                template_symbol += ", ";
            }
            first = false;
            m_buffer.emit("typename ", -1, false);
            m_buffer.emit_symbol(type);
            template_symbol += type->cxx_identifier();
        }
        m_buffer.emit(">", -1);
        template_symbol += ">";
    }

    auto runtime_symbol = lua_api::ast::symbol::make_cxx("runtime", false, kestrel_lua_path);
    auto enroll_symbol = klass->object_symbol()->attach_template_symbol(template_symbol)->cxx_member(enrollment.symbol()->cxx_identifier());

    auto enroll_lua_api = std::make_shared<lua_api::ast::lua_function>(enroll_symbol);
    enroll_lua_api->set_return_type(lua_api::ast::type::void_type());

    if (klass->enrollment().requires_custom_name()) {
        enroll_lua_api->add_parameter(lua_api::ast::lua_parameter("name", lua_api::ast::type("const std::string&")));
    }
    enroll_lua_api->add_parameter(lua_api::ast::lua_parameter("runtime", lua_api::ast::type::shared_ptr(runtime_symbol->cxx_resolved_identifier())));


    m_buffer.emit_function(enroll_lua_api);
    m_buffer.emit_block_start();

    m_buffer.start_line();
    m_buffer.emit_symbol(runtime_symbol);
    m_buffer.emit("->global_namespace()", -1);
    m_buffer.increase_indent();

    begin_namespaces(klass->object_symbol()->lua_namespace_path(), [&] {
        // Begin the class...
        if (klass->as_namespace()) {
            begin_namespace(klass->object_symbol());
        }
        else {
            begin_class(klass);
        }

        // Constructor
        if (klass->has_constructor() && !klass->enrollment().requires_custom_name()) {
            m_buffer.emit(".addConstructor<auto(*)(", 0, false);
            bool first_param = true;
            for (const auto& param : klass->constructor().parameters()) {
                if (!first_param) {
                    m_buffer.emit(", ", -1, false);
                }
                m_buffer.emit_type(param.type());
                first_param = false;
            }
            m_buffer.emit(")->void, lua_reference>()", -1);
        }

        // Variables
        for (auto variable : klass->variables()) {
            m_buffer.emit(".addData(\"", 0, false);
            m_buffer.emit_lua_symbol(variable->symbol());
            m_buffer.emit("\", &", -1, false);
            m_buffer.emit_symbol(klass->object_symbol());
            m_buffer.emit("::", -1, false);
            m_buffer.emit_symbol(variable->symbol());
            m_buffer.emit(", ", -1, false);
            m_buffer.emit(variable->is_mutable() ? "true" : "false", -1, false);
            m_buffer.emit(")", -1);
        }

        // Properties
        for (auto property : klass->properties()) {
            if (property->is_static() && !klass->as_namespace()) {
                m_buffer.emit(".addStaticProperty(\"", 0, false);
            }
            else {
                m_buffer.emit(".addProperty(\"", 0, false);
            }
            m_buffer.emit_lua_symbol(property->getter());
            m_buffer.emit("\", &", -1, false);
            m_buffer.emit_symbol(klass->object_symbol());
            m_buffer.emit("::", -1, false);
            m_buffer.emit_symbol(property->getter());
            if (property->has_setter()) {
                m_buffer.emit(", &", -1, false);
                m_buffer.emit_symbol(klass->object_symbol());
                m_buffer.emit("::", -1, false);
                m_buffer.emit_symbol(property->setter());
            }
            m_buffer.emit(")", -1);
        }

        // Functions
        for (auto function : klass->functions()) {
            if (function->is_static() && !klass->as_namespace()) {
                m_buffer.emit(".addStaticFunction(\"", 0, false);
            }
            else {
                m_buffer.emit(".addFunction(\"", 0, false);
            }
            m_buffer.emit_lua_symbol(function->symbol());
            m_buffer.emit("\", &", -1, false);
            m_buffer.emit_symbol(klass->object_symbol());
            m_buffer.emit("::", -1, false);
            m_buffer.emit_symbol(function->symbol());
            m_buffer.emit(")", -1);
        }

        if (klass->as_namespace()) {
            end_namespace();
        }
        else {
            end_class();

            if (klass->object_symbol()->lua_namespace_path().path_vector().empty()) {
                m_buffer.semicolon();
            }
        }
    });

    m_buffer.decrease_indent();
    m_buffer.emit_block_end();
    m_buffer.newline();
}


// MARK: - Enum Enrollment

auto luatool::codegen::cxx::generator::compile_enum_enrollment(std::shared_ptr<lua_api::ast::lua_enum> lua_enum) -> void
{
    if (!lua_enum->enrollment().is_defined()) {
        return;
    }

    const auto& enrollment = lua_enum->enrollment();

    lua_api::ast::namespace_path kestrel_path("kestrel");
    auto kestrel_lua_path = kestrel_path.nested("lua");

    auto enroll_name_path = lua_enum->object_symbol()->cxx_namespace_path().path_string("_");

    auto runtime_symbol = lua_api::ast::symbol::make_cxx("runtime", false, kestrel_lua_path);
    auto enroll_symbol = lua_api::ast::symbol::make_cxx("enroll_lua_api_enum_" + enroll_name_path + "_" + lua_enum->object_symbol()->cxx_identifier());

    auto enroll_lua_api = std::make_shared<lua_api::ast::lua_function>(enroll_symbol);
    enroll_lua_api->set_return_type(lua_api::ast::type::void_type());
    enroll_lua_api->add_parameter(lua_api::ast::lua_parameter("runtime", lua_api::ast::type::shared_ptr(runtime_symbol->cxx_resolved_identifier())));

    m_buffer.emit_function(enroll_lua_api);
    m_buffer.emit_block_start();

    for (auto enum_case : lua_enum->all_cases()) {
        m_buffer.emit("static auto ", 0, false);
        m_buffer.emit_symbol(enum_case.second->internal_value_symbol());
        m_buffer.emit(" = static_cast<std::int32_t>(", -1, false);
        m_buffer.emit_resolved_symbol(lua_enum->object_symbol()->member(enum_case.second->object_symbol()));
        m_buffer.emit(");", -1);
    }

    m_buffer.newline();
    m_buffer.start_line();
    m_buffer.emit_symbol(runtime_symbol);
    m_buffer.emit("->global_namespace()", -1);
    m_buffer.increase_indent();

    begin_namespaces(lua_enum->object_symbol()->lua_namespace_path(), [&] {
        begin_namespace(lua_enum->object_symbol());

        for (auto enum_case : lua_enum->all_cases()) {
            m_buffer.emit(".addProperty(\"", 0, false);
            m_buffer.emit_lua_symbol(enum_case.second->object_symbol());
            m_buffer.emit("\", &", -1, false);
            m_buffer.emit_symbol(enum_case.second->internal_value_symbol());
            m_buffer.emit(", false)", -1);
        }

        end_namespace();
    });

    m_buffer.decrease_indent();
    m_buffer.emit_block_end();
    m_buffer.newline();
}

// MARK: - Helpers


auto luatool::codegen::cxx::generator::begin_namespaces(const lua_api::ast::namespace_path &path, const std::function<auto() -> void> &block) -> void
{
    for (auto ns_name : path.path_vector()) {
        begin_namespace(ns_name);
    }

    block();

    for (auto _ : path.path_vector()) {
        end_namespace();
    }
}

auto luatool::codegen::cxx::generator::begin_namespace(std::shared_ptr<lua_api::ast::symbol> ns_name) -> void
{
    m_buffer.emit(".beginNamespace(\"", 0, false);
    m_buffer.emit_lua_symbol(ns_name);
    m_buffer.emit("\")", -1);
    m_buffer.increase_indent();
    m_namespace_depth++;
}

auto luatool::codegen::cxx::generator::begin_namespace(const std::string& ns_name) -> void
{
    m_buffer.emit(".beginNamespace(\"" + ns_name + "\")", 0);
    m_buffer.increase_indent();
    m_namespace_depth++;
}

auto luatool::codegen::cxx::generator::end_namespace() -> void
{
    m_namespace_depth--;
    m_buffer.decrease_indent();
    m_buffer.emit(".endNamespace()", 0, false);
    if (m_namespace_depth == 0) {
        m_buffer.semicolon();
    }
    else {
        m_buffer.newline();
    }
}

auto luatool::codegen::cxx::generator::begin_class(std::shared_ptr<lua_api::ast::lua_class> klass) -> void
{
    m_buffer.emit(".beginClass<", 0, false);
    m_buffer.emit_symbol(klass->object_symbol());

    if (klass->is_templated()) {
        m_buffer.emit("<", -1, false);
        bool first = true;
        for (auto type : klass->templates()) {
            if (!first) {
                m_buffer.emit(", ", -1, false);
            }
            m_buffer.emit_symbol(type);
            first = false;
        }
        m_buffer.emit(">", -1, false);
    }

    if (klass->enrollment().requires_custom_name()) {
        m_buffer.emit(">(name.c_str())", -1);
    }
    else {
        m_buffer.emit(">(\"" + klass->object_symbol()->lua_identifier() + "\")", -1);
    }

    m_buffer.increase_indent();
}

auto luatool::codegen::cxx::generator::end_class() -> void
{
    m_buffer.decrease_indent();
    m_buffer.emit(".endClass()", 0);
}