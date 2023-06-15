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

#include <sstream>
#include <libCodeGen/languages/language.hpp>

namespace codegen::language
{
    struct markdown
    {
        // Helpers
        [[nodiscard]] static inline auto heading_prefix(std::int32_t level) -> std::string
        {
            switch (level) {
                case 1:     return "#";
                case 2:     return "##";
                case 4:     return "###";
                case 5:     return "####";
                case 6:     return "#####";
                default:    return "###";
            }
        }

        // Metadata
        [[nodiscard]] static auto name() -> std::string { return "Markdown"; }
        [[nodiscard]] static auto lc_name() -> std::string { return "markdown"; };
        [[nodiscard]] static auto extension() -> std::string { return "md"; };

        // Mark-up Support
        [[nodiscard]] static auto text(const std::string& str) -> emit::segment
        {
            return emit::segment(str);
        }

        [[nodiscard]] static auto bold(const std::string& str) -> emit::segment
        {
            return emit::segment("**" + str + "**");
        }

        [[nodiscard]] static auto italic(const std::string& str) -> emit::segment
        {
            return emit::segment("_" + str + "_");
        }

        [[nodiscard]] static auto strikethrough(const std::string& str) -> emit::segment
        {
            return emit::segment("~~" + str + "~~");
        }

        [[nodiscard]] static auto inline_code(const std::string& str) -> emit::segment
        {
            return emit::segment("`" + str + "`");
        }

        // Headings

        [[nodiscard]] static auto heading(const std::string& heading, std::int32_t level) -> emit::segment
        {
            return emit::segment(heading_prefix(level) + " " + heading, emit::line_break_mode::full);
        }

        // Formatting

        [[nodiscard]] static auto preformatted(const std::string& text) -> emit::segment
        {
            return emit::segment("```\n" + text + "\n```", emit::line_break_mode::full);
        }

        [[nodiscard]] static auto blockquote(const std::string& text) -> emit::segment
        {
            std::string out;
            std::string line;
            std::stringstream in(text);

            while (std::getline(in, line)) {
                out += " > " + line + "\n";
            }

            return emit::segment(out, emit::line_break_mode::full);
        }

        // Lists

        [[nodiscard]] static auto begin_list() -> emit::segment
        {
            return emit::segment("", emit::line_break_mode::after, emit::indentation_mode::indent_after);
        }

        [[nodiscard]] static auto begin_list_item() -> emit::segment
        {
            return emit::segment("- ", emit::line_break_mode::none);
        }

        [[nodiscard]] static auto end_list_item() -> emit::segment
        {
            return emit::segment("", emit::line_break_mode::after);
        }

        [[nodiscard]] static auto end_list() -> emit::segment
        {
            return emit::segment("", emit::line_break_mode::before, emit::indentation_mode::outdent_before);
        }

        // Tables

        [[nodiscard]] static auto begin_table() -> emit::segment
        {
            return emit::segment("", emit::line_break_mode::before);
        }

        [[nodiscard]] static auto begin_table_header() -> emit::segment
        {
            column_counter = 0;
            return begin_table_row();
        }

        [[nodiscard]] static auto end_table_header() -> emit::segment
        {
            std::string out = "|";
            for (auto n = 0; n < column_counter; ++n) {
                out += " --- |";
            }
            return emit::segment(out, emit::line_break_mode::full);
        }

        [[nodiscard]] static auto begin_table_row() -> emit::segment
        {
            return emit::segment("|", emit::line_break_mode::before);
        }

        [[nodiscard]] static auto end_table_row() -> emit::segment
        {
            return emit::segment();
        }

        [[nodiscard]] static auto begin_table_cell() -> emit::segment
        {
            return emit::segment(" ");
        }

        [[nodiscard]] static auto end_table_cell() -> emit::segment
        {
            return emit::segment(" |");
        }

        [[nodiscard]] static auto begin_table_header_cell() -> emit::segment
        {
            return begin_table_cell();
        }

        [[nodiscard]] static auto end_table_header_cell() -> emit::segment
        {
            column_counter++;
            return end_table_cell();
        }

        [[nodiscard]] static auto end_table() -> emit::segment
        {
            return emit::segment("", emit::line_break_mode::after);
        }

    private:
        static std::int32_t column_counter;
    };
}