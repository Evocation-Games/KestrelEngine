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
#include <libCodeGen/languages/language.hpp>
#include <libCodeGen/languages/procedural/type/system.hpp>
#include <libFoundation/string/joined.hpp>
#include <libCodeGen/ast/symbol/symbol.hpp>

namespace codegen::language::procedural
{
    template<symbol_support L>
    struct symbol
    {
        enum class kind { is_variable, is_namespace, is_class, is_enum, is_enum_case };

        symbol() = default;
        explicit symbol(const std::string& name, enum kind kind = kind::is_variable)
            : m_name(name), m_resolved_name(name), m_kind(kind)
        {}

        explicit symbol(const std::initializer_list<std::string>& name, enum kind kind = kind::is_variable)
            : m_name(std::vector<std::string>(name).back()),
              m_resolved_name(foundation::string::joined(name, L::scope_resolution_operator_string())),
              m_kind(kind)
        {}

        explicit symbol(const std::vector<std::string>& name, enum kind kind = kind::is_variable)
            : m_name(name.back()),
              m_resolved_name(foundation::string::joined(name, L::scope_resolution_operator_string())),
              m_kind(kind)
        {}

        [[nodiscard]] virtual auto node() const -> std::shared_ptr<ast::symbol<L>>
        {
            return std::make_shared<ast::symbol<L>>(m_name);
        }

        [[nodiscard]] virtual auto resolved_name_node() const -> std::shared_ptr<ast::symbol<L>>
        {
            return std::make_shared<ast::symbol<L>>(m_resolved_name);
        }

    private:
        std::string m_name;
        std::string m_resolved_name;
        enum kind m_kind;
    };

    template<type_support L>
    struct typed_symbol : public symbol<L>
    {
        typed_symbol() = default;

        explicit typed_symbol(
            const std::string& name,
            const typename type_system<L>::type_id& type = void_type<L>(),
            enum symbol<L>::kind kind = symbol<L>::kind::is_variable
        )
            : symbol<L>(name, kind)
        {}

        explicit typed_symbol(
            const std::initializer_list<std::string>& name,
            const typename type_system<L>::type_id& type = void_type<L>(),
            enum symbol<L>::kind kind = symbol<L>::kind::is_variable
        )
            : symbol<L>(name, kind)
        {}

    private:
        typename type_system<L>::type_id m_type;
    };
}