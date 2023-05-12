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

#include "analyzer/clang.hpp"
#include "analyzer/translation_unit_context.hpp"
#include "lua/ast/translation_unit.hpp"
#include "lua/ast/lua_namespace.hpp"
#include "lua/ast/lua_class.hpp"
#include "lua/ast/lua_enum.hpp"
#include "lua/ast/lua_enum_case.hpp"
#include "lua/ast/lua_constructor.hpp"
#include "lua/annotations/annotation.hpp"
#include "lua/annotations/tag.hpp"

// MARK: - Construction

luatool::analyzer::translation_unit_context::translation_unit_context(const lua_api::ast::translation_unit* tu)
    : m_tu(tu)
{}

// MARK: -

auto luatool::analyzer::translation_unit_context::start_traversing_ast() -> void
{
    auto cursor = clang_getTranslationUnitCursor(m_tu->reference());
    clang_visitChildren(cursor, translation_unit_context::visit_node, this);
}

auto luatool::analyzer::translation_unit_context::namespaces() const -> const std::unordered_map<lua_api::ast::symbol::hash_value, std::shared_ptr<lua_api::ast::lua_namespace>>&
{
    return m_namespaces.all;
}

auto luatool::analyzer::translation_unit_context::classes() const -> const std::unordered_map<lua_api::ast::symbol::hash_value, std::shared_ptr<lua_api::ast::lua_class>>&
{
    return m_classes.all;
}

auto luatool::analyzer::translation_unit_context::enums() const -> const std::unordered_map<lua_api::ast::symbol::hash_value, std::shared_ptr<lua_api::ast::lua_enum>>&
{
    return m_enum.all;
}


// MARK: - Node Visitor

auto luatool::analyzer::translation_unit_context::should_visit_node(CXCursor cursor) const -> bool
{
    auto location = clang_getCursorLocation(cursor);
    auto kind = clang_getCursorKind(cursor);
    return !(
        (!clang_isAttribute(kind)) &&
        (kind != CXCursor_CXXMethod && kind != CXCursor_FunctionDecl) &&
        (clang_Location_isFromMainFile(location) == 0)
    );
}

auto luatool::analyzer::translation_unit_context::visit_node(CXCursor cursor, CXCursor parent, CXClientData data) -> CXChildVisitResult
{
    auto context = reinterpret_cast<translation_unit_context *>(data);
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
        case CXCursor_ParmDecl: {
            context->visit_parameter(cursor);
            break;
        }
        case CXCursor_TemplateTypeParameter: {
            context->visit_template_parameter(cursor, parent);
            break;
        }
        default: {
            clang_visitChildren(cursor, translation_unit_context::visit_node, data);
            break;
        }
    }

    return CXChildVisit_Continue;
}

auto luatool::analyzer::translation_unit_context::visit_annotation(CXCursor cursor, CXCursor parent) -> void
{
    lua_api::annotation annotation(cursor);
    switch (clang_getCursorKind(parent)) {
        case CXCursor_Namespace: {
            construct_namespace(cursor, parent, annotation);
            break;
        }

        case CXCursor_StructDecl:
        case CXCursor_ClassDecl:
        case CXCursor_ClassTemplate: {
            construct_class(cursor, parent, annotation);
            break;
        }

        case CXCursor_EnumDecl: {
            construct_enum(cursor, parent, annotation);
            break;
        }

        case CXCursor_EnumConstantDecl: {
            construct_enum_case(cursor, parent, annotation);
            break;
        }

        case CXCursor_Constructor: {
            construct_constructor(cursor, parent, annotation);
            break;
        }

        case CXCursor_FunctionDecl:
        case CXCursor_CXXMethod: {
            construct_function(cursor, parent, annotation);
            break;
        }

        case CXCursor_FieldDecl: {
            construct_variable(cursor, parent, annotation);
            break;
        }

        default: {
            break;
        }
    }
}

auto luatool::analyzer::translation_unit_context::visit_namespace(CXCursor cursor) -> void
{
    m_namespace_path.emplace_back(clang::spelling(cursor));
    clang_visitChildren(cursor, translation_unit_context::visit_node, this);
    m_namespace_path.pop_back();
    m_namespaces.current = nullptr;
}

auto luatool::analyzer::translation_unit_context::namespace_path() const -> lua_api::ast::namespace_path
{
    return lua_api::ast::namespace_path(m_namespace_path);
}

auto luatool::analyzer::translation_unit_context::visit_class(CXCursor cursor) -> void
{
    m_classes.templates.clear();
    clang_visitChildren(cursor, translation_unit_context::visit_node, this);
    if (m_classes.current && (clang::spelling(cursor) == m_classes.current->object_symbol()->cxx_identifier())) {
        m_classes.current = nullptr;
    }
}

auto luatool::analyzer::translation_unit_context::visit_enum(CXCursor cursor) -> void
{
    clang_visitChildren(cursor, translation_unit_context::visit_node, this);
    m_enum.current = nullptr;
}

auto luatool::analyzer::translation_unit_context::visit_enum_case(CXCursor cursor) -> void
{
    if (!m_enum.current) {
        return;
    }
    clang_visitChildren(cursor, translation_unit_context::visit_node, this);
}

auto luatool::analyzer::translation_unit_context::visit_parameter(CXCursor cursor) -> void
{
    if (m_classes.current && m_classes.current->is_building_constructor()) {
        lua_api::ast::type type(cursor);
        auto symbol = lua_api::ast::symbol::make_cxx(cursor);

        // TODO: Parameter Fixes

        m_classes.current->constructor().add_parameter(symbol, type);
    }
}

auto luatool::analyzer::translation_unit_context::visit_constructor(CXCursor cursor) -> void
{
    if (!m_classes.current || m_classes.current->has_constructor()) {
        return;
    }

    clang_visitChildren(cursor, translation_unit_context::visit_node, this);
    m_classes.current->end_constructor();
}

auto luatool::analyzer::translation_unit_context::visit_template_parameter(CXCursor cursor, CXCursor parent) -> void
{
    if (!m_classes.current && (clang_getCursorKind(parent) == CXCursor_ClassTemplate)) {
        m_classes.templates.emplace_back(clang::spelling(cursor));
        return;
    }
    else if (m_classes.current) {
        for (const auto& template_typename : m_classes.templates) {
            m_classes.current->add_template_typename(template_typename);
        }
    }
}

// MARK: - Node Construction

auto luatool::analyzer::translation_unit_context::construct_symbol(CXCursor cursor, const lua_api::annotation &annotation) -> std::shared_ptr<lua_api::ast::symbol>
{
    auto path = namespace_path();
    if (clang_getCursorKind(cursor) == CXCursor_Namespace && path.is_namespace(cursor)) {
        path = path.parent();
    }

    auto symbol = lua_api::ast::symbol::make_cxx(cursor, path);
    symbol->set_include_path(m_tu->path());

    if (annotation.has(lua_api::annotation_tag::name::symbol)) {
        symbol->apply_lua_identifier(annotation.tag(lua_api::annotation_tag::name::symbol));
    }
    symbol->apply_raw_documentation(clang::documentation(cursor));

    if (annotation.has(lua_api::annotation_tag::name::available)) {
        symbol->set_version_introduced(annotation.tag(lua_api::annotation_tag::name::available));
    }
    if (annotation.has(lua_api::annotation_tag::name::deprecated)) {
        symbol->set_version_deprecated(annotation.tag(lua_api::annotation_tag::name::deprecated));
    }

    return symbol;
}

auto luatool::analyzer::translation_unit_context::construct_namespace(CXCursor cursor, CXCursor parent, const lua_api::annotation &annotation) -> void
{
    auto symbol = construct_symbol(parent, annotation);
    auto hash = symbol->lua_identifier_hash();

    m_namespaces.current = std::make_shared<lua_api::ast::lua_namespace>(symbol);
    m_namespaces.all.emplace(std::pair(hash, m_namespaces.current));

    clang_visitChildren(cursor, translation_unit_context::visit_node, this);
}

auto luatool::analyzer::translation_unit_context::construct_class(CXCursor cursor, CXCursor parent, const lua_api::annotation &annotation) -> void
{
    if (!m_classes.current && annotation.has(lua_api::annotation_tag::name::symbol)) {
        auto symbol = construct_symbol(parent, annotation);
        auto hash = symbol->lua_identifier_hash();

        m_classes.current = std::make_shared<lua_api::ast::lua_class>(symbol);
        m_classes.all.emplace(std::pair(hash, m_classes.current));

        for (auto type : m_classes.templates) {
            m_classes.current->add_template_typename(type);
        }

        m_classes.templates.clear();
    }
    else if (m_classes.current && annotation.has(lua_api::annotation_tag::name::as_namespace)) {
        m_classes.current->set_as_namespace();
    }


    clang_visitChildren(cursor, translation_unit_context::visit_node, this);
}

auto luatool::analyzer::translation_unit_context::construct_enum(CXCursor cursor, CXCursor parent, const lua_api::annotation &annotation) -> void
{
    auto symbol = construct_symbol(parent, annotation);
    auto hash = symbol->lua_identifier_hash();

    m_enum.current = std::make_shared<lua_api::ast::lua_enum>(symbol);
    m_enum.all.emplace(std::pair(hash, m_enum.current));

    clang_visitChildren(cursor, translation_unit_context::visit_node, this);
}

auto luatool::analyzer::translation_unit_context::construct_enum_case(CXCursor cursor, CXCursor parent, const lua_api::annotation &annotation) -> void
{
    auto symbol = construct_symbol(parent, annotation);
    auto hash = symbol->lua_identifier_hash();

    auto enum_case = std::make_shared<lua_api::ast::lua_enum_case>(symbol, std::to_string(clang_getEnumConstantDeclValue(parent)));
    m_enum.current->add_case(enum_case);

    clang_visitChildren(cursor, translation_unit_context::visit_node, this);
}

auto luatool::analyzer::translation_unit_context::construct_constructor(CXCursor cursor, CXCursor parent, const luatool::lua_api::annotation &annotation) -> void
{
    if (!m_classes.current) {
        return;
    }

    if (annotation.has(lua_api::annotation_tag::name::constructor)) {
        m_classes.current->begin_constructor();
    }
    else if (annotation.has(lua_api::annotation_tag::name::type_fix)) {
        auto param_name = annotation.tag(lua_api::annotation_tag::name::symbol);
        auto custom_type = annotation.tag(lua_api::annotation_tag::name::type_fix);

        lua_api::ast::type type(custom_type);
        m_classes.current->constructor().add_fixed_parameter(lua_api::ast::lua_parameter(param_name, type));
    }
    clang_visitChildren(cursor, translation_unit_context::visit_node, this);
}

auto luatool::analyzer::translation_unit_context::construct_function(CXCursor cursor, CXCursor parent, const lua_api::annotation &annotation) -> void
{
    if (!m_classes.current && !m_namespaces.current) {
        return;
    }

    if (annotation.has(lua_api::annotation_tag::name::getter)) {
        construct_getter(cursor, parent, annotation);
    }
    else if (annotation.has(lua_api::annotation_tag::name::setter)) {
        construct_setter(cursor, parent, annotation);
    }
    else if (annotation.has(lua_api::annotation_tag::name::enrollment)) {
        construct_enrollment(cursor, parent, annotation);
    }
    else if (annotation.has(lua_api::annotation_tag::name::template_variant) && m_classes.current) {
        lua_api::ast::type type(annotation.tag(lua_api::annotation_tag::name::template_variant));
        m_classes.current->add_template_variant(annotation.tag(lua_api::annotation_tag::name::symbol), type);
    }
    else {
        // Constructing a simple function.
        auto symbol = construct_symbol(parent, annotation);
        auto hash = symbol->lua_identifier_hash();

        if (m_classes.current) {
            m_classes.current->add_function(symbol);
        }
        else {
            symbol->make_static();
            m_namespaces.current->add_function(symbol);
        }
    }
}

auto luatool::analyzer::translation_unit_context::construct_getter(CXCursor cursor, CXCursor parent, const lua_api::annotation &annotation) -> void
{
    auto symbol = construct_symbol(parent, annotation);

    if (m_classes.current) {
        m_classes.current->add_property_getter(symbol);
    }
    else {
        symbol->make_static();
        m_namespaces.current->add_property_getter(symbol);
    }
}

auto luatool::analyzer::translation_unit_context::construct_setter(CXCursor cursor, CXCursor parent, const lua_api::annotation &annotation) -> void
{
    auto symbol = construct_symbol(parent, annotation);

    if (m_classes.current) {
        m_classes.current->add_property_setter(symbol);
    }
    else {
        symbol->make_static();
        m_namespaces.current->add_property_setter(symbol);
    }
}

auto luatool::analyzer::translation_unit_context::construct_enrollment(CXCursor cursor, CXCursor parent, const lua_api::annotation &annotation) -> void
{
    auto symbol = construct_symbol(parent, annotation);
    lua_api::ast::lua_enrollment enrollment(symbol);
    enrollment.set_requires_runtime(true);

    if (annotation.has(lua_api::annotation_tag::name::reference)) {
        lua_api::ast::lua_reference reference(annotation.tag(lua_api::annotation_tag::name::reference));
        enrollment.set_reference(reference);
    }

    if (annotation.has(lua_api::annotation_tag::name::named)) {
        enrollment.set_requires_custom_name(true);
    }

    if (m_enum.current) {
        m_enum.current->set_enrollment(enrollment);
    }
    else if (m_classes.current) {
        m_classes.current->set_enrollment(enrollment);
    }
    else if (m_namespaces.current) {
        m_namespaces.current->set_enrollment(enrollment);
    }
}

auto luatool::analyzer::translation_unit_context::construct_variable(CXCursor cursor, CXCursor parent, const lua_api::annotation &annotation) -> void
{
    if (!m_classes.current && !m_namespaces.current) {
        return;
    }

    auto symbol = construct_symbol(parent, annotation);
    auto is_mutable = annotation.has(lua_api::annotation_tag::name::is_mutable);

    if (m_classes.current) {
        m_classes.current->add_variable(symbol, is_mutable);
    }
    else {
        symbol->make_static();
        m_namespaces.current->add_variable(symbol, is_mutable);
    }
}