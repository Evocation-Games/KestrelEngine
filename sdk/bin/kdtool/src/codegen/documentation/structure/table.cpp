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

#include "codegen/documentation/structure/table.hpp"

// MARK: - Structure

kdtool::codegen::documentation::table::table(std::size_t cols)
    : m_column_count(cols)
{}

kdtool::codegen::documentation::table::table(const std::initializer_list<text *> &cols)
    : m_column_count(cols.size()), m_headings(cols)
{}

kdtool::codegen::documentation::table::table(const std::initializer_list<std::string> &cols)
    : m_column_count(cols.size())
{
    for (auto heading : cols) {
        m_headings.emplace_back(new text(heading));
    }
}

// MARK: - Row Management

auto kdtool::codegen::documentation::table::add_row(const std::initializer_list<std::string>& cols) -> void
{
    std::vector<text *> row;
    for (auto column : cols) {
        row.emplace_back(new text(column));
    }
    m_rows.emplace_back(row);
}

auto kdtool::codegen::documentation::table::add_row(const std::initializer_list<text *>& cols) -> void
{
    std::vector<text *> row;
    for (auto column : cols) {
        row.emplace_back(column);
    }
    m_rows.emplace_back(row);
}

// MARK: - Emit

auto kdtool::codegen::documentation::table::emit() const -> std::vector<std::string>
{
    std::vector<std::string> out;

    auto build_row = [] (const std::vector<text *>& columns) -> std::string {
        std::string out = "|";
        for (const auto* column : columns) {
            out += " " + column->value() + " |";
        }
        return std::move(out);
    };

    out.emplace_back("");
    if (!m_headings.empty()) {
        out.emplace_back(build_row(m_headings));

        std::string divider_row = "|";
        for (auto n = 0; n < m_headings.size(); ++n) {
            divider_row += " --- |";
        }
        out.emplace_back(divider_row);
    }

    for (auto& row : m_rows) {
        out.emplace_back(build_row(row));
    }
    out.emplace_back("");

    return std::move(out);
}