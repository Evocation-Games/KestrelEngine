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
#include <libCodeGen/ast/core/node.hpp>
#include <libCodeGen/ast/symbol/symbol.hpp>

namespace codegen::ast
{
    template<language::resource_reference L>
    struct resource_reference : public node
    {
        explicit resource_reference(std::int64_t id)
            : m_id(id)
        {}

        explicit resource_reference(const std::string& type, std::int64_t id)
            : m_id(id), m_type(type)
        {}

        explicit resource_reference(const std::string& container, const std::string& type, std::int64_t id)
            : m_id(id), m_type(type), m_container(container)
        {}

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            if (!m_container.empty() && !m_type.empty()) {
                return L::typed_contained_reference(
                    std::make_shared<ast::symbol<L>>(m_container),
                    std::make_shared<ast::symbol<L>>(m_type),
                    m_id
                );
            }
            else if (!m_type.empty()) {
                return L::typed_reference(
                    std::make_shared<ast::symbol<L>>(m_type),
                    m_id
                );
            }
            else {
                return L::global_reference(m_id);
            }
        }

    private:
        std::int64_t m_id;
        std::string m_type;
        std::string m_container;
    };
}