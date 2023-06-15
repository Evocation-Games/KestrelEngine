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
#include "project/project.hpp"
#include "project/structure/construct_definition.hpp"
#include "project/structure/enrollment/enrollment.hpp"
#include "project/structure/type/type.hpp"
#include <libFoundation/system/filesystem/path.hpp>
#include <libScripting/annotations/set.hpp>
#include <clang-c/Index.h>

namespace kdtool::cxx
{
    struct analyzer
    {
        analyzer(const std::shared_ptr<project::index>& project, const foundation::filesystem::path& path);
        ~analyzer();

        auto run() -> void;

    private:
        [[nodiscard]] auto resolved_name(const std::string& name = "") const -> std::string;

        auto push(const std::string& name) -> void;
        auto pop() -> void;
        auto set_current_definition(const std::shared_ptr<struct project::structure::construct_definition>& definition) -> void;

        template<project::structure::is_construct_definition T>
        [[nodiscard]] auto current_definition() -> std::shared_ptr<T>
        {
            if (m_state.definition_stack.empty() || m_state.definition_stack.back() == nullptr) {
                return nullptr;
            }

            if (m_state.definition_stack.back()->instance_type() == T::type()) {
                return std::reinterpret_pointer_cast<T>(m_state.definition_stack.back());
            }
            else {
                return nullptr;
            }
        }

        template<project::structure::is_construct_definition T>
        [[nodiscard]] auto parent_definition() -> std::shared_ptr<T>
        {
            if (m_state.definition_stack.size() < 2) {
                return nullptr;
            }

            auto& parent = m_state.definition_stack.at(m_state.definition_stack.size() - 2);
            if (parent && parent->instance_type() == T::type()) {
                return std::reinterpret_pointer_cast<T>(parent);
            }
            else {
                return nullptr;
            }
        }

    private:
        static auto visit_node(CXCursor cursor, CXCursor parent, CXClientData data) -> CXChildVisitResult;
        [[nodiscard]] auto should_visit_node(CXCursor cursor) const -> bool;

        auto visit_annotation(CXCursor cursor, CXCursor parent) -> void;
        auto visit_namespace(CXCursor cursor) -> void;
        auto visit_class(CXCursor cursor) -> void;
        auto visit_enum(CXCursor cursor) -> void;
        auto visit_enum_case(CXCursor cursor) -> void;
        auto visit_function(CXCursor cursor) -> void;
        auto visit_parameter(CXCursor cursor) -> void;
        auto visit_constructor(CXCursor cursor) -> void;
        auto visit_template_parameter(CXCursor cursor, CXCursor parent) -> void;

    private:
        auto construct_symbol(CXCursor cursor, const scripting::annotation::set& annotations) -> std::shared_ptr<project::structure::symbol>;
        auto construct_namespace(CXCursor cursor, CXCursor parent, const scripting::annotation::set& annotations) -> std::shared_ptr<project::structure::construct_definition>;
        auto construct_class(CXCursor cursor, CXCursor parent, const scripting::annotation::set& annotations) -> std::shared_ptr<project::structure::construct_definition>;
        auto construct_enum(CXCursor cursor, CXCursor parent, const scripting::annotation::set& annotations) -> std::shared_ptr<project::structure::construct_definition>;
        auto construct_enum_case(CXCursor cursor, CXCursor parent, const scripting::annotation::set& annotations) -> std::shared_ptr<project::structure::construct_definition>;
        auto construct_parameter(CXCursor cursor) -> std::shared_ptr<project::structure::construct_definition>;
        auto construct_constructor(CXCursor cursor, CXCursor parent, const scripting::annotation::set& annotations) -> std::shared_ptr<project::structure::construct_definition>;
        auto construct_template_parameter(CXCursor cursor, CXCursor parent, const scripting::annotation::set& annotations) -> std::shared_ptr<project::structure::construct_definition>;
        auto construct_function(CXCursor cursor, CXCursor parent, const scripting::annotation::set& annotations) -> std::shared_ptr<project::structure::construct_definition>;
        auto construct_variable(CXCursor cursor, CXCursor parent, const scripting::annotation::set& annotations) -> std::shared_ptr<project::structure::construct_definition>;
        auto construct_enrollment(CXCursor cursor, const scripting::annotation::set& annotations, const std::shared_ptr<struct project::structure::symbol>& symbol) -> std::shared_ptr<project::structure::enrollment>;

        auto register_parameter_type_fix(const scripting::annotation::set& annotations) -> void;

    private:
        struct {
            CXTranslationUnit tu { nullptr };
            CXIndex index { nullptr };
        } m_clang;

        struct {
            std::string filepath;
            std::vector<std::string> name_stack;
            std::vector<std::shared_ptr<struct project::structure::construct_definition>> definition_stack;
            std::unordered_map<std::string, std::shared_ptr<struct project::structure::type::base>> parameter_type_fixes;
        } m_state;

        std::shared_ptr<project::index> m_index;
    };
}