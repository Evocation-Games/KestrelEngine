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
    template<language::component_definition_support L>
    struct component_files : public node
    {
    public:
        explicit component_files(const std::string& path)
            : m_path(path)
        {}

        [[nodiscard]] auto emit() const -> emit::segment override
        {
            std::vector<emit::segment> out;
            for (const auto& file : m_files) {
                emit::segment entry({
                    emit::segment("\"" + file + "\""),
                    emit::segment(";", emit::line_break_mode::after)
                });
                out.emplace_back(entry);
            }

            return {
                L::component_files_decl(m_path),
                L::begin_compound_statement(),
                emit::segment(out),
                L::end_compound_statement()
            };
        }

        auto add(const std::string& name) -> void
        {
            m_files.emplace_back(name);
        }

    private:
        std::string m_path;
        std::vector<std::string> m_files;
    };

}