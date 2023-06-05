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

#include <initializer_list>
#include <libCodeGen/spec/markup/node.hpp>
#include <libCodeGen/spec/markup/text.hpp>
#include <libCodeGen/spec/markup/table_row.hpp>
#include <libCodeGen/spec/markup/table_header_row.hpp>

namespace codegen::spec::markup
{
    struct table : public markup_node
    {
        explicit table(std::size_t cols)
            : m_column_count(cols)
        {}

        table(const std::initializer_list<std::string>& cols)
            : m_column_count(cols.size())
        {
            for (const auto& heading : cols) {
                m_headings.emplace_back(std::make_shared<text>(heading));
            }
        }

        table(const std::initializer_list<std::shared_ptr<markup_node>>& cols)
            : m_column_count(cols.size()), m_headings(cols)
        {}

        auto add_row(const std::initializer_list<std::string>& cols) -> void
        {
            std::vector<std::shared_ptr<markup_node>> row;
            for (const auto& column : cols) {
                row.emplace_back(std::make_shared<text>(column));
            }
            m_rows.emplace_back(row);
        }

        auto add_row(const std::initializer_list<std::shared_ptr<markup_node>>& cols) -> void
        {
            std::vector<std::shared_ptr<markup_node>> row;
            for (const auto& column : cols) {
                row.emplace_back(column);
            }
            m_rows.emplace_back(row);
        }

        [[nodiscard]] auto emit(const std::shared_ptr<markup_language>& language) const -> std::vector<std::string> override
        {
            auto table_content = std::make_shared<markup_node>();

            auto table_header = table_content->add<table_header_row>();
            for (const auto& heading : m_headings) {
                table_header->add_cell(heading);
            }

            for (const auto& row : m_rows) {
                auto row_content = table_content->add<table_row>();
                for (const auto& cell : row) {
                    row_content->add_cell(cell);
                }
            }

            return language->table(table_content);
        }

    private:
        std::size_t m_column_count;
        std::vector<std::shared_ptr<markup_node>> m_headings;
        std::vector<std::vector<std::shared_ptr<markup_node>>> m_rows;
    };
}