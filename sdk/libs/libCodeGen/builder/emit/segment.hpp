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
#include <libCodeGen/builder/emit/indentation_mode.hpp>
#include <libCodeGen/builder/emit/line_break_mode.hpp>

namespace codegen::emit
{
    struct segment
    {
        explicit segment(
            const std::string& str = "",
            enum line_break_mode line_break = line_break_mode::none,
            enum indentation_mode indentation = indentation_mode::none
        )
            : m_str(str), m_line_break(line_break), m_indentation_mode(indentation)
        {}

        segment(
            const std::initializer_list<struct segment>& segments,
            enum line_break_mode line_break = line_break_mode::none,
            enum indentation_mode indentation = indentation_mode::none
        )
            : m_sub_segments(segments), m_line_break(line_break), m_indentation_mode(indentation)
        {}

        explicit segment(
            const std::vector<struct segment>& segments,
            enum line_break_mode line_break = line_break_mode::none,
            enum indentation_mode indentation = indentation_mode::none
        )
            : m_sub_segments(segments), m_line_break(line_break), m_indentation_mode(indentation)
        {}

        [[nodiscard]] auto string() const -> std::string { return m_str; }
        [[nodiscard]] auto line_break() const -> enum line_break_mode { return m_line_break; }
        [[nodiscard]] auto indentation() const -> enum indentation_mode { return m_indentation_mode; }

        [[nodiscard]] auto sub_segments() const -> const std::vector<struct segment>& { return m_sub_segments; }

    private:
        std::string m_str;
        std::vector<struct segment> m_sub_segments;
        enum line_break_mode m_line_break { line_break_mode::none };
        enum indentation_mode m_indentation_mode { indentation_mode::none };
    };
}