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
#include "builder/buffer.hpp"
#include "builder/documentation/directory/entry.hpp"

namespace kdtool::builder::component
{
    template<codegen::language::metadata L>
    struct directory : public codegen::component
    {
        explicit directory(const std::shared_ptr<struct builder::directory::entry>& entry)
            : m_entry(entry)
        {}

        [[nodiscard]] auto emit() const -> codegen::emit::segment override
        {
            buffer<L> buffer;
            buffer.template add<codegen::ast::begin_list_item<L>>();
            buffer.template add<codegen::ast::text<L>>(m_entry->name());

            if (!m_entry->is_leaf()) {
                buffer.template add<codegen::ast::begin_list<L>>();
                m_entry->each([&] (const auto& child) {
                    auto dir = std::make_shared<directory<L>>(child);
                    buffer.add(dir);
                });
                buffer.template add<codegen::ast::end_list<L>>();
            }

            buffer.template add<codegen::ast::end_list_item<L>>();
            return buffer.segments();
        }

    private:
        std::shared_ptr<struct builder::directory::entry> m_entry;
    };
}