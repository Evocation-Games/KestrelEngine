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
#include <libCodeGen/ast/markup/text.hpp>

namespace codegen::ast
{
    template<language::markup_support L>
    struct table : public node
    {

        auto add_column(const std::string& header) -> void
        {
            add_column(std::make_shared<text<L>>(header));
        }

        auto add_column(const std::shared_ptr<node>& header) -> void
        {
            m_headings.emplace_back(header);
        }

        auto add_row(const std::initializer_list<std::string>& columns) -> void
        {
            std::vector<std::shared_ptr<node>> column_nodes;
            for (const auto& cell : columns) {
                column_nodes.emplace_back(std::make_shared<text<L>>(cell));
            }
            add_row(column_nodes);
        }

        auto add_row(const std::initializer_list<std::shared_ptr<node>>& columns) -> void
        {
            std::vector<std::shared_ptr<node>> column_nodes(columns);
            add_row(column_nodes);
        }

        auto add_row(const std::vector<std::shared_ptr<node>>& columns) -> void
        {
            m_rows.emplace_back(columns);
        }

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            std::vector<emit::segment> out;

            out.emplace_back(L::begin_table());
            out.emplace_back(L::begin_table_header());
            for (const auto& cell : m_headings) {
                out.emplace_back(L::begin_table_header_cell());
                out.emplace_back(cell->emit());
                out.emplace_back(L::end_table_header_cell());
            }
            out.emplace_back(L::end_table_header());

            for (const auto& row : m_rows) {
                out.emplace_back(L::begin_table_row());
                for (const auto& cell : row) {
                    out.emplace_back(L::begin_table_cell());
                    out.emplace_back(cell->emit());
                    out.emplace_back(L::end_table_cell());
                }
                out.emplace_back(L::end_table_row());
            }

            out.emplace_back(L::end_table());

            return emit::segment(out);
        }

    private:
        std::vector<std::shared_ptr<node>> m_headings;
        std::vector<std::vector<std::shared_ptr<node>>> m_rows;
    };
}