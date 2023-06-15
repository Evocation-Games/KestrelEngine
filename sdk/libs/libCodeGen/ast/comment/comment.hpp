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
#include <sstream>
#include <vector>
#include <libCodeGen/ast/core/node.hpp>

namespace codegen::ast
{
    template<language::comment_support L>
    struct comment : public node
    {
        comment() = default;
        explicit comment(const std::string& text)
        {
            std::string line;
            std::stringstream in(text);
            while(std::getline(in, line)) {
                m_lines.emplace_back(line);
            }
        }

        [[nodiscard]] virtual auto lines() const -> const std::vector<std::string>& { return m_lines; }

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            std::vector<emit::segment> out;
            for (const auto& line : lines()) {
                out.emplace_back(L::comment(line));
            }
            return emit::segment(out);
        }

    private:
        std::vector<std::string> m_lines;
    };
}