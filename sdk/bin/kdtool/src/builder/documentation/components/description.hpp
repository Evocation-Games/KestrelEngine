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

#include <libCodeGen/builder/component.hpp>
#include <libCodeGen/languages/language.hpp>
#include <libCodeGen/ast/markup.hpp>
#include <libDocumentation/parser/type/documentation.hpp>
#include "builder/buffer.hpp"

namespace kdtool::builder::component
{
    template<codegen::language::metadata L>
    struct documentation_section : public codegen::component
    {
        explicit documentation_section(const documentation::section& section)
            : m_section(section)
        {}

        [[nodiscard]] auto emit() const -> codegen::emit::segment override
        {
            buffer<L> buffer;

            buffer.template add<codegen::ast::heading<L>>(m_section.title(), 2);

            if (m_section.name() == documentation::section::name::example) {
                buffer.template add<codegen::ast::preformatted<L>>(m_section.body());
            }
            else {
                buffer.template add<codegen::ast::text<L>>(m_section.body());
            }

            return buffer.segments();
        }

    private:
        documentation::section m_section;
    };
}