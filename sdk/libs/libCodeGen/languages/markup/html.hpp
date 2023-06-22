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

#include <libCodeGen/languages/language.hpp>

namespace codegen::language
{
    struct html
    {
        // Helpers
        [[nodiscard]] static inline auto tag(const std::string& name, bool close = false) -> std::string
        {
            return close ? "</" + name + ">"
                         : "<" + name + ">";
        }

        [[nodiscard]] static inline auto heading_tag_name(std::int32_t level) -> std::string
        {
            switch (level) {
                case 1:     return "h1";
                case 2:     return "h2";
                case 4:     return "h4";
                case 5:     return "h5";
                case 6:     return "h6";
                default:    return "h3";
            }
        }

        // Metadata
        [[nodiscard]] static auto name() -> std::string { return "HTML"; }
        [[nodiscard]] static auto lc_name() -> std::string { return "html"; };
        [[nodiscard]] static auto extension() -> std::string { return "html"; };

        // Mark-up Support
        [[nodiscard]] static auto text(const std::string& str) -> emit::segment
        {
            return emit::segment(str);
        }

        [[nodiscard]] static auto bold(const std::string& str) -> emit::segment
        {
            return emit::segment("<b>" + str + "</b>");
        }

        [[nodiscard]] static auto italic(const std::string& str) -> emit::segment
        {
            return emit::segment("<i>" + str + "</i>");
        }

        [[nodiscard]] static auto strikethrough(const std::string& str) -> emit::segment
        {
            return emit::segment("<s>" + str + "</s>");
        }

        [[nodiscard]] static auto inline_code(const std::string& str) -> emit::segment
        {
            return emit::segment("<code>" + str + "</code>");
        }

        // Dividers

        [[nodiscard]] static auto divider() -> emit::segment
        {
            return emit::segment("<hr/>", emit::line_break_mode::full);
        }

        // Anchors

        [[nodiscard]] static auto anchor(const std::string& str, const std::string& link) -> emit::segment
        {
            return emit::segment("<a href=\"" + link + "\">" + str + "</a>");
        }

        // Headings

        [[nodiscard]] static auto heading(const std::string& heading, std::int32_t level) -> emit::segment
        {
            const auto tag_name = heading_tag_name(level);
            return emit::segment(tag(tag_name) + heading + tag(tag_name, true), emit::line_break_mode::full);
        }

        // Formatting

        [[nodiscard]] static auto preformatted(const std::string& text) -> emit::segment
        {
            return {
                emit::segment("<pre>", emit::line_break_mode::full, emit::indentation_mode::indent_after),
                emit::segment(text),
                emit::segment("</pre>", emit::line_break_mode::full, emit::indentation_mode::outdent_before)
            };
        }

        [[nodiscard]] static auto blockquote(const std::string& text) -> emit::segment
        {
            return {
                emit::segment("<blockquote>", emit::line_break_mode::full, emit::indentation_mode::indent_after),
                emit::segment(text),
                emit::segment("</blockquote>", emit::line_break_mode::full, emit::indentation_mode::outdent_before)
            };
        }

        // Lists

        [[nodiscard]] static auto begin_list() -> emit::segment
        {
            return emit::segment("<ul>", emit::line_break_mode::full, emit::indentation_mode::indent_after);
        }

        [[nodiscard]] static auto begin_sublist() -> emit::segment
        {
            return begin_list();
        }

        [[nodiscard]] static auto begin_list_item() -> emit::segment
        {
            return emit::segment("<li>", emit::line_break_mode::full, emit::indentation_mode::indent_after);
        }

        [[nodiscard]] static auto end_list_item() -> emit::segment
        {
            return emit::segment("</li>", emit::line_break_mode::full, emit::indentation_mode::outdent_before);
        }

        [[nodiscard]] static auto end_sublist() -> emit::segment
        {
            return end_list();
        }

        [[nodiscard]] static auto end_list() -> emit::segment
        {
            return emit::segment("</ul>", emit::line_break_mode::full, emit::indentation_mode::outdent_before);
        }

        // Tables

        [[nodiscard]] static auto begin_table() -> emit::segment
        {
            return emit::segment("<table>", emit::line_break_mode::before, emit::indentation_mode::indent_after);
        }

        [[nodiscard]] static auto begin_table_header() -> emit::segment
        {
            return begin_table_row();
        }

        [[nodiscard]] static auto end_table_header() -> emit::segment
        {
            return end_table_row();
        }

        [[nodiscard]] static auto begin_table_row() -> emit::segment
        {
            return emit::segment("<tr>", emit::line_break_mode::before, emit::indentation_mode::indent_after);
        }

        [[nodiscard]] static auto end_table_row() -> emit::segment
        {
            return emit::segment("</tr>", emit::line_break_mode::before, emit::indentation_mode::outdent_before);
        }

        [[nodiscard]] static auto begin_table_cell() -> emit::segment
        {
            return emit::segment("<td>", emit::line_break_mode::before, emit::indentation_mode::indent_after);
        }

        [[nodiscard]] static auto end_table_cell() -> emit::segment
        {
            return emit::segment("</td>", emit::line_break_mode::before, emit::indentation_mode::outdent_before);
        }

        [[nodiscard]] static auto begin_table_header_cell() -> emit::segment
        {
            return emit::segment("<th>", emit::line_break_mode::before, emit::indentation_mode::indent_after);
        }

        [[nodiscard]] static auto end_table_header_cell() -> emit::segment
        {
            return emit::segment("</th>", emit::line_break_mode::before, emit::indentation_mode::outdent_before);
        }

        [[nodiscard]] static auto end_table() -> emit::segment
        {
            return emit::segment("</table>", emit::line_break_mode::full, emit::indentation_mode::outdent_before);
        }
    };
}