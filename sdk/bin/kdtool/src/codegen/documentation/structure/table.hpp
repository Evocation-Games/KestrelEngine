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

#pragma once

#include <initializer_list>
#include "codegen/common/emit_node.hpp"
#include "codegen/documentation/structure/text.hpp"

namespace kdtool::codegen::documentation
{
    class table : public common::emit_node
    {
    public:
        explicit table(std::size_t cols);
        table(const std::initializer_list<std::string>& cols);
        table(const std::initializer_list<text *>& cols);

        auto add_row(const std::initializer_list<std::string>& cols) -> void;
        auto add_row(const std::initializer_list<text *>& cols) -> void;

        [[nodiscard]] auto emit() const -> std::vector<std::string> override;

    private:
        std::size_t m_column_count;
        std::vector<text *> m_headings;
        std::vector<std::vector<text *>> m_rows;
    };
}