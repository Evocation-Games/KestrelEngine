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

#include <memory>
#include "analyzer/cxx/cxx_analyzer.hpp"
#include "analyzer/cxx/clang.hpp"
#include "analyzer/cxx/types/parser.hpp"
#include "project/structure/namespace/namespace_definition.hpp"
#include "project/structure/class/class_definition.hpp"
#include "project/structure/enum/enum_definition.hpp"
#include "project/structure/function/function_definition.hpp"
#include "project/structure/class/variable/variable_definition.hpp"
#include "project/structure/class/constructor/constructor_definition.hpp"
#include "project/structure/enrollment/enrollment.hpp"
#include "project/structure/type/type.hpp"
#include <libScripting/annotations/parser.hpp>
#include <libDocumentation/parser/parser.hpp>

// MARK: - Construction

kdtool::cxx::analyzer::analyzer(const std::shared_ptr<project::index> &project, const foundation::filesystem::path &path)
    : m_index(project)
{
    const auto& includes = project->include_paths();
    const auto argument_count = (includes.size() * 2) + 2;
    auto arguments = new const char*[argument_count];

    arguments[0] = "-Wdocumentation";
    arguments[1] = "-fparse-all-comments";

    auto i = 2;
    for (const auto& include : includes) {
        arguments[i++] = "-I";
        arguments[i++] = include.c_str();
    }

    // Check if the path, starts with one of the include paths. If it does, then remove the include path
    // prefix.
    m_state.filepath = path.string();
    for (const auto& include_path : includes) {
        if ( m_state.filepath.starts_with(include_path)) {
            m_state.filepath.erase(0, include_path.size());

            // Make sure there isn't a leading '/' remaining.
            if ( m_state.filepath.starts_with('/')) {
                m_state.filepath.erase(0, 1);
            }

            // Add an include path for the this.
            project->add_include_path(m_state.filepath, true);

            break;
        }
    }

    m_clang.index = clang_createIndex(0, 0);
    m_clang.tu = clang_parseTranslationUnit(m_clang.index, path.string().c_str(), arguments, argument_count, nullptr, 0, CXTranslationUnit_None);
}

// MARK: - Destruction

kdtool::cxx::analyzer::~analyzer()
{
    clang_disposeTranslationUnit(m_clang.tu);
    clang_disposeIndex(m_clang.index);
}

// MARK: - Name Management

auto kdtool::cxx::analyzer::resolved_name(const std::string& name) const -> std::string
{
    std::string out;
    for (const auto& parent_name : m_state.name_stack) {
        if (!out.empty()) {
            out += "::";
        }
        out += parent_name;
    }

    if (!name.empty()) {
        if (!out.empty()) {
            out += "::";
        }
        out += name;
    }
    return std::move(out);
}

auto kdtool::cxx::analyzer::push(const std::string &name) -> void
{
    m_state.name_stack.emplace_back(name);
    m_state.definition_stack.emplace_back(nullptr);
    m_state.parameter_type_fixes.clear();
}

auto kdtool::cxx::analyzer::pop() -> void
{
    m_state.name_stack.pop_back();
    m_state.definition_stack.pop_back();
    m_state.parameter_type_fixes.clear();
}

auto kdtool::cxx::analyzer::set_current_definition(const std::shared_ptr<struct project::structure::construct_definition> &definition) -> void
{
    m_state.definition_stack.pop_back();
    m_state.definition_stack.emplace_back(definition);
}

// MARK: - Traversal

auto kdtool::cxx::analyzer::run() -> void
{
    auto cursor = clang_getTranslationUnitCursor(m_clang.tu);
    clang_visitChildren(cursor, cxx::analyzer::visit_node, this);
}

auto kdtool::cxx::analyzer::should_visit_node(CXCursor cursor) const -> bool
{
    auto location = clang_getCursorLocation(cursor);
    auto kind = clang_getCursorKind(cursor);
    return !(
        (!clang_isAttribute(kind)) &&
        (kind != CXCursor_CXXMethod && kind != CXCursor_FunctionDecl) &&
        (clang_Location_isFromMainFile(location) == 0)
    );
}

auto kdtool::cxx::analyzer::visit_node(CXCursor cursor, CXCursor parent, CXClientData data) -> CXChildVisitResult
{
    auto context = reinterpret_cast<cxx::analyzer *>(data);
    if (!(context && context->should_visit_node(cursor))) {
        return CXChildVisit_Continue;
    }

    switch (clang_getCursorKind(cursor)) {
        case CXCursor_AnnotateAttr: {
            context->visit_annotation(cursor, parent);
            break;
        }
        case CXCursor_Namespace: {
            context->visit_namespace(cursor);
            break;
        }
        case CXCursor_StructDecl:
        case CXCursor_ClassDecl:
        case CXCursor_ClassTemplate: {
            context->visit_class(cursor);
            break;
        }
        case CXCursor_EnumDecl: {
            context->visit_enum(cursor);
            break;
        }
        case CXCursor_EnumConstantDecl: {
            context->visit_enum_case(cursor);
            break;
        }
        case CXCursor_Constructor: {
            context->visit_constructor(cursor);
            break;
        }
        case CXCursor_CXXMethod:
        case CXCursor_FunctionDecl: {
            context->visit_function(cursor);
            break;
        }
        case CXCursor_ParmDecl: {
            context->visit_parameter(cursor);
            break;
        }
        case CXCursor_TemplateTypeParameter: {
            context->visit_template_parameter(cursor, parent);
            break;
        }
        default: {
            clang_visitChildren(cursor, cxx::analyzer::visit_node, data);
            break;
        }
    }

    return CXChildVisit_Continue;
}

auto kdtool::cxx::analyzer::visit_annotation(CXCursor cursor, CXCursor parent) -> void
{
    const auto raw_annotation = clang::spelling(cursor);
    const auto annotations = scripting::annotation::parse(raw_annotation);

    if (annotations.has(scripting::annotation::tag::parameter_type)) {
        register_parameter_type_fix(annotations);
        return;
    }
    else if (annotations.has(scripting::annotation::tag::template_variant)) {
        if (const auto& class_definition = parent_definition<project::structure::class_definition>()) {
            class_definition->add_template_variant(
                annotations.attachment(scripting::annotation::tag::symbol).value(),
                annotations.attachment(scripting::annotation::tag::template_variant).value()
            );
            return;
        }
    }

    switch (clang_getCursorKind(parent)) {
        case CXCursor_Namespace: {
            construct_namespace(cursor, parent, annotations);
            break;
        }
        case CXCursor_StructDecl:
        case CXCursor_ClassDecl:
        case CXCursor_ClassTemplate: {
            construct_class(cursor, parent, annotations);
            break;
        }
        case CXCursor_EnumDecl: {
            construct_enum(cursor, parent, annotations);
            break;
        }
        case CXCursor_EnumConstantDecl: {
            construct_enum_case(cursor, parent, annotations);
            break;
        }
        case CXCursor_Constructor: {
            construct_constructor(cursor, parent, annotations);
            break;
        }
        case CXCursor_FunctionDecl:
        case CXCursor_CXXMethod: {
            construct_function(cursor, parent, annotations);
            break;
        }
        case CXCursor_FieldDecl: {
            construct_variable(cursor, parent, annotations);
            break;
        }
        default: {
            break;
        }
    }
}

auto kdtool::cxx::analyzer::visit_namespace(CXCursor cursor) -> void
{
    push(clang::spelling(cursor));
    clang_visitChildren(cursor, cxx::analyzer::visit_node, this);
    pop();
}

auto kdtool::cxx::analyzer::visit_class(CXCursor cursor) -> void
{
    push(clang::spelling(cursor));
    clang_visitChildren(cursor, cxx::analyzer::visit_node, this);
    pop();
}

auto kdtool::cxx::analyzer::visit_enum(CXCursor cursor) -> void
{
    push(clang::spelling(cursor));
    clang_visitChildren(cursor, cxx::analyzer::visit_node, this);
    pop();
}

auto kdtool::cxx::analyzer::visit_enum_case(CXCursor cursor) -> void
{
    if (auto definition = current_definition<struct project::structure::enum_definition>()) {
        clang_visitChildren(cursor, cxx::analyzer::visit_node, this);
    }
}

auto kdtool::cxx::analyzer::visit_parameter(CXCursor cursor) -> void
{
    if (current_definition<struct project::structure::function_definition>()
     || current_definition<struct project::structure::constructor_definition>())
    {
        clang_visitChildren(cursor, cxx::analyzer::visit_node, this);
        construct_parameter(cursor);
    }
}

auto kdtool::cxx::analyzer::visit_constructor(CXCursor cursor) -> void
{
    if (auto definition = current_definition<struct project::structure::class_definition>()) {
        push(clang::spelling(cursor));
        clang_visitChildren(cursor, cxx::analyzer::visit_node, this);
        pop();
    }
}

auto kdtool::cxx::analyzer::visit_function(CXCursor cursor) -> void
{
    if (current_definition<struct project::structure::namespace_definition>()
        || current_definition<struct project::structure::class_definition>())
    {
        push(clang::spelling(cursor));
        clang_visitChildren(cursor, cxx::analyzer::visit_node, this);
        pop();
    }
}

auto kdtool::cxx::analyzer::visit_template_parameter(CXCursor cursor, CXCursor parent) -> void
{
    m_state.template_type_parameters.emplace_back(clang::spelling(cursor));
}

// MARK: - Construct Definitions

auto kdtool::cxx::analyzer::construct_symbol(CXCursor cursor, const scripting::annotation::set& annotations) -> std::shared_ptr<project::structure::symbol>
{
    std::shared_ptr<project::structure::symbol> symbol;
    if (!annotations.has(scripting::annotation::tag::symbol)) {
        return nullptr;
    }

    // Is the symbol static?
    bool is_static = false;
    if (clang::is_static(cursor)) {
        is_static = true;
    }

    std::string symbol_name;
    for (const auto& node : m_state.definition_stack) {
        if (!node) {
            continue;
        }
        symbol_name += node->symbol()->resolved_name() + ".";
    }

    // Source CXX Symbol
    std::string source_symbol_parameters;
    auto source_symbol_resolved = foundation::string::joined(m_state.name_stack, "::");
    auto source_symbol = clang::spelling(cursor);

    if (!m_state.template_type_parameters.empty()) {
        std::vector<std::string> template_parameters;
        template_parameters.reserve(m_state.template_type_parameters.size());
        for (const auto& param : m_state.template_type_parameters) {
            template_parameters.emplace_back(param);
        }
        m_state.template_type_parameters.clear();
        source_symbol_parameters = "<" + foundation::string::joined(template_parameters, ", ") + ">";
    }
    auto source_symbol_name = source_symbol_resolved + source_symbol_parameters;

    // Create the symbol
    symbol_name += annotations.attachment(scripting::annotation::tag::symbol).value();
    symbol = m_index->symbol_named(source_symbol_resolved, "::");
    symbol->set_display_name(symbol_name);

    if (is_static) {
        symbol->make_static();
    }

    // Configure the symbol with any appropriate information
    const auto raw_documentation = clang::documentation(cursor);
    symbol->set_documentation(documentation::parser(raw_documentation).build());

    if (annotations.has(scripting::annotation::tag::available)) {
        symbol->set_available(project::structure::version(annotations.attachment(scripting::annotation::tag::available).value()));
    }

    if (annotations.has(scripting::annotation::tag::deprecated)) {
        symbol->set_deprecated(project::structure::version(annotations.attachment(scripting::annotation::tag::deprecated).value()));
    }

    return symbol;
}

auto kdtool::cxx::analyzer::construct_namespace(CXCursor cursor, CXCursor parent, const scripting::annotation::set& annotations) -> std::shared_ptr<project::structure::construct_definition>
{
    const auto& symbol = construct_symbol(parent, annotations);
    if (!symbol) {
        return nullptr;
    }

    auto namespace_definition = std::make_shared<struct project::structure::namespace_definition>(symbol);
    namespace_definition->set_location(m_state.filepath);
    symbol->set_definition(namespace_definition);
    m_index->add_definition(namespace_definition);
    set_current_definition(namespace_definition);
    return namespace_definition;
}

auto kdtool::cxx::analyzer::construct_class(CXCursor cursor, CXCursor parent, const scripting::annotation::set &annotations) -> std::shared_ptr<project::structure::construct_definition>
{
    const auto& symbol = construct_symbol(parent, annotations);
    if (!symbol) {
        return nullptr;
    }

    auto class_definition = std::make_shared<struct project::structure::class_definition>(symbol);
    class_definition->set_location(m_state.filepath);
    symbol->set_definition(class_definition);
    m_index->add_definition(class_definition);
    set_current_definition(class_definition);
    return class_definition;
}

auto kdtool::cxx::analyzer::construct_enum(CXCursor cursor, CXCursor parent, const scripting::annotation::set &annotations) -> std::shared_ptr<project::structure::construct_definition>
{
    const auto& symbol = construct_symbol(parent, annotations);
    if (!symbol) {
        return nullptr;
    }

    auto enum_definition = std::make_shared<struct project::structure::enum_definition>(symbol);
    enum_definition->set_location(m_state.filepath);
    enum_definition->set(std::make_shared<struct project::structure::enrollment>(
        std::make_shared<struct project::structure::symbol>("enroll_enum_api")
    ));
    symbol->set_definition(enum_definition);
    m_index->add_definition(enum_definition);
    set_current_definition(enum_definition);
    return enum_definition;
}

auto kdtool::cxx::analyzer::construct_enum_case(CXCursor cursor, CXCursor parent, const scripting::annotation::set &annotations) -> std::shared_ptr<project::structure::construct_definition>
{
    if (auto enum_definition = current_definition<struct project::structure::enum_definition>()) {
        const auto& symbol = construct_symbol(parent, annotations);
        if (!symbol) {
            return nullptr;
        }

        auto enum_case_definition = std::make_shared<struct project::structure::enum_case_definition>(symbol);
        enum_case_definition->set_location(m_state.filepath);
        enum_case_definition->set_value(clang_getEnumConstantDeclValue(cursor));
        symbol->set_definition(enum_case_definition);
        enum_definition->add(enum_case_definition);
        return enum_case_definition;
    }
    return nullptr;
}

auto kdtool::cxx::analyzer::construct_constructor(CXCursor cursor, CXCursor parent, const scripting::annotation::set &annotations) -> std::shared_ptr<project::structure::construct_definition>
{
    if (!annotations.has(scripting::annotation::tag::is_constructor)) {
        return nullptr;
    }

    if (auto class_definition = parent_definition<struct project::structure::class_definition>()) {
        auto constructor = std::make_shared<struct project::structure::constructor_definition>();
        constructor->set_location(m_state.filepath);
        class_definition->set(constructor);
        set_current_definition(constructor);
        return constructor;
    }
    return nullptr;
}

auto kdtool::cxx::analyzer::construct_function(CXCursor cursor, CXCursor parent, const scripting::annotation::set &annotations) -> std::shared_ptr<project::structure::construct_definition>
{
    const auto& symbol = construct_symbol(parent, annotations);
    if (auto class_definition = parent_definition<struct project::structure::class_definition>()) {
        if (annotations.has(scripting::annotation::tag::is_enrollment)) {
            auto enrollment = construct_enrollment(parent, annotations, symbol);
            class_definition->set(enrollment);
            return nullptr;
        }

        if (!symbol) {
            return nullptr;
        }

        auto function = std::make_shared<struct project::structure::function_definition>(symbol);
        function->set_location(m_state.filepath);

        if (annotations.has(scripting::annotation::tag::is_getter)) {
            auto property = class_definition->property(symbol);
            property->set_location(m_state.filepath);
            property->add_getter(function);
        }
        else if (annotations.has(scripting::annotation::tag::is_setter)) {
            auto property = class_definition->property(symbol);
            property->set_location(m_state.filepath);
            property->add_setter(function);
        }
        else {
            // Basic function
            class_definition->add(function);
            symbol->set_definition(function);
        }

        set_current_definition(function);

        return function;
    }
    else if (auto namespace_definition = parent_definition<struct project::structure::namespace_definition>()) {
        if (annotations.has(scripting::annotation::tag::is_enrollment)) {
            auto enrollment = construct_enrollment(parent, annotations, symbol);
            namespace_definition->set(enrollment);
            return nullptr;
        }

        if (!symbol) {
            return nullptr;
        }

        auto function = std::make_shared<struct project::structure::function_definition>(symbol);
        function->set_location(m_state.filepath);

        if (annotations.has(scripting::annotation::tag::is_getter)) {
            auto property = namespace_definition->property(symbol);
            property->set_location(m_state.filepath);
            property->add_getter(function);
        }
        else if (annotations.has(scripting::annotation::tag::is_setter)) {
            auto property = namespace_definition->property(symbol);
            property->set_location(m_state.filepath);
            property->add_setter(function);
        }
        else {
            // Basic function
            namespace_definition->add(function);
            symbol->set_definition(function);
        }

        set_current_definition(function);

        return function;
    }
    return nullptr;
}

auto kdtool::cxx::analyzer::construct_variable(CXCursor cursor, CXCursor parent, const scripting::annotation::set &annotations) -> std::shared_ptr<project::structure::construct_definition>
{
    const auto& symbol = construct_symbol(parent, annotations);
    if (!symbol) {
        return nullptr;
    }

    auto var = std::make_shared<struct project::structure::variable_definition>(symbol);
    var->set_location(m_state.filepath);
    symbol->set_definition(var);

    if (annotations.has(scripting::annotation::tag::mutability)) {
    }

    if (auto class_definition = current_definition<struct project::structure::class_definition>()) {
        class_definition->add(var);
    }
    else if (auto namespace_definition = current_definition<struct project::structure::namespace_definition>()) {
        namespace_definition->add(var);
    }

    return var;
}

auto kdtool::cxx::analyzer::construct_parameter(CXCursor cursor) -> std::shared_ptr<project::structure::construct_definition>
{
    const auto name = clang::spelling(cursor);
    const auto symbol = std::make_shared<struct project::structure::symbol>(name);
    auto parameter = std::make_shared<struct project::structure::parameter_definition>(symbol);
    parameter->set_location(m_state.filepath);

    auto it = m_state.parameter_type_fixes.find(name);
    if (it == m_state.parameter_type_fixes.end()) {
        auto type = project::structure::type::parse_string<cxx::type_parser>(clang::spelling(clang_getCursorType(cursor)));
        parameter->set_type(type);
    }
    else {
        parameter->set_type(it->second);
    }

    if (auto function = current_definition<struct project::structure::function_definition>()) {
        function->add(parameter);
    }
    else if (auto constructor = current_definition<struct project::structure::constructor_definition>()) {
        constructor->add(parameter);
    }

    return parameter;
}

// MARK: - Type Fixes

auto kdtool::cxx::analyzer::register_parameter_type_fix(const scripting::annotation::set &annotations) -> void
{
    const auto parameter_name = annotations.attachment(scripting::annotation::tag::symbol).value();
    const auto type_name = annotations.attachment(scripting::annotation::tag::parameter_type).value();
    auto type = project::structure::type::parse_string<cxx::type_parser>(type_name);

    auto it = m_state.parameter_type_fixes.find(parameter_name);
    if (it != m_state.parameter_type_fixes.end()) {
        it->second = type;
    }
    else {
        m_state.parameter_type_fixes.emplace(parameter_name, type);
    }
}

// MARK: - Enrollments

auto kdtool::cxx::analyzer::construct_enrollment(CXCursor cursor, const scripting::annotation::set& annotations, const std::shared_ptr<struct project::structure::symbol>& symbol) -> std::shared_ptr<project::structure::enrollment>
{
    auto enrollment = std::make_shared<struct project::structure::enrollment>(symbol);
    enrollment->set_requires_runtime(true);

    if (annotations.has(scripting::annotation::tag::reference)) {
        const auto reference_name = annotations.attachment(scripting::annotation::tag::reference).value();
        auto reference = std::make_shared<struct project::structure::construct_definition::reference>(reference_name);
        enrollment->set_reference(reference);
    }

    if (annotations.has(scripting::annotation::tag::requires_enrollment_name)) {
        enrollment->set_requires_custom_name(true);
    }

    return enrollment;
}