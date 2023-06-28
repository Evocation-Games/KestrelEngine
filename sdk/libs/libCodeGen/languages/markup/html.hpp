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

#include <codecvt>
#include <locale>
#include <libCodeGen/languages/language.hpp>

namespace codegen::language
{
    struct html
    {
        // Helpers
        [[nodiscard]] static inline auto tag(const std::string& name, bool close = false, const std::vector<std::string>& styles = {}) -> std::string
        {
            return close ? "</" + name + ">"
                         : "<" + name + style_classes_attribute(styles) + ">";
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

        [[nodiscard]] static inline auto encode_text(const std::string& str) -> std::string
        {
            std::string out;

            for (const auto c : str) {
                switch (c) {
                    case '&':  out += "&amp;"; break;
                    case '<':  out += "&lt;"; break;
                    case '>':  out += "&gt;"; break;
                    case '\n':  out += "<br>"; break;
                    default:    out += c; break;
                }
            }

            return std::move(out);
        }

        // Metadata
        [[nodiscard]] static auto name() -> std::string { return "HTML"; }
        [[nodiscard]] static auto lc_name() -> std::string { return "html"; };
        [[nodiscard]] static auto extension() -> std::string { return "html"; };

        // Mark-up Support
        [[nodiscard]] static auto prologue(const std::string& title, const std::string& style) -> emit::segment
        {
            return {
                emit::segment("<html>", emit::line_break_mode::after, emit::indentation_mode::indent_after),
                emit::segment("<head>", emit::line_break_mode::after, emit::indentation_mode::indent_after),
                emit::segment("<meta charset=\"UTF-8\">", emit::line_break_mode::after),
                emit::segment("<title>" + title + "</title>", emit::line_break_mode::after),
                emit::segment("<link rel=\"stylesheet\" href=\"" + style + "\">", emit::line_break_mode::after),
                emit::segment("</head>", emit::line_break_mode::after, emit::indentation_mode::outdent_before),
                emit::segment("<body>", emit::line_break_mode::after, emit::indentation_mode::indent_after),
            };
        }

        [[nodiscard]] static auto epilogue() -> emit::segment
        {
            return {
                emit::segment("</body>", emit::line_break_mode::after, emit::indentation_mode::outdent_before),
                emit::segment("</html>", emit::line_break_mode::after, emit::indentation_mode::outdent_before),
            };
        }

        [[nodiscard]] static auto identifier_attribute(const std::string& id) -> std::string
        {
            if (id.empty()) {
                return "";
            }
            return " id=\"" + id + "\"";
        }

        [[nodiscard]] static auto style_classes_attribute(const std::vector<std::string>& styles) -> std::string
        {
            if (styles.empty()) {
                return "";
            }
            return " class=\"" + foundation::string::joined(styles, " ") + "\"";
        }

        [[nodiscard]] static auto text(const std::string& str, const std::vector<std::string>& styles) -> emit::segment
        {
            return emit::segment("<span" + style_classes_attribute(styles) + ">" + encode_text(str) + "</span>");
        }

        [[nodiscard]] static auto bold(const std::string& str, const std::vector<std::string>& styles) -> emit::segment
        {
            return emit::segment("<b" + style_classes_attribute(styles) + ">" + encode_text(str) + "</b>");
        }

        [[nodiscard]] static auto italic(const std::string& str, const std::vector<std::string>& styles) -> emit::segment
        {
            return emit::segment("<i" + style_classes_attribute(styles) + ">" + encode_text(str) + "</i>");
        }

        [[nodiscard]] static auto strikethrough(const std::string& str, const std::vector<std::string>& styles) -> emit::segment
        {
            return emit::segment("<s" + style_classes_attribute(styles) + ">" + encode_text(str) + "</s>");
        }

        [[nodiscard]] static auto inline_code(const std::string& str, const std::vector<std::string>& styles) -> emit::segment
        {
            return emit::segment("<code" + style_classes_attribute(styles) + ">" + encode_text(str) + "</code>");
        }

        // Dividers

        [[nodiscard]] static auto divider() -> emit::segment
        {
            return emit::segment("<hr/>", emit::line_break_mode::full);
        }

        // Anchors

        [[nodiscard]] static auto anchor(const std::string& str, const std::string& link, const std::vector<std::string>& styles) -> emit::segment
        {
            return emit::segment("<a" + style_classes_attribute(styles) + " href=\"" + link + "\">" + encode_text(str) + "</a>");
        }

        // Headings

        [[nodiscard]] static auto heading(const std::string& heading, std::int32_t level, const std::vector<std::string>& styles) -> emit::segment
        {
            const auto tag_name = heading_tag_name(level);
            return emit::segment(tag(tag_name, false, styles) + encode_text(heading) + tag(tag_name, true), emit::line_break_mode::full);
        }

        // Formatting

        [[nodiscard]] static auto preformatted(const std::string& text, const std::vector<std::string>& styles) -> emit::segment
        {
            return {
                emit::segment("<pre" + style_classes_attribute(styles) + ">", emit::line_break_mode::full, emit::indentation_mode::indent_after),
                emit::segment(text),
                emit::segment("</pre>", emit::line_break_mode::full, emit::indentation_mode::outdent_before)
            };
        }

        [[nodiscard]] static auto blockquote(const std::string& text, const std::vector<std::string>& styles) -> emit::segment
        {
            return {
                emit::segment("<blockquote" + style_classes_attribute(styles) + ">", emit::line_break_mode::full, emit::indentation_mode::indent_after),
                emit::segment(encode_text(text)),
                emit::segment("</blockquote>", emit::line_break_mode::full, emit::indentation_mode::outdent_before)
            };
        }

        // Lists

        [[nodiscard]] static auto begin_list(const std::vector<std::string>& styles) -> emit::segment
        {
            return emit::segment("<ul" + style_classes_attribute(styles) + ">", emit::line_break_mode::full, emit::indentation_mode::indent_after);
        }

        [[nodiscard]] static auto begin_sublist(const std::vector<std::string>& styles) -> emit::segment
        {
            return begin_list(styles);
        }

        [[nodiscard]] static auto begin_list_item(const std::vector<std::string>& styles) -> emit::segment
        {
            return emit::segment("<li" + style_classes_attribute(styles) + ">", emit::line_break_mode::full, emit::indentation_mode::indent_after);
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

        [[nodiscard]] static auto begin_table(const std::vector<std::string>& styles) -> emit::segment
        {
            return emit::segment("<table cellspacing=0 cellpadding=0" + style_classes_attribute(styles) + ">", emit::line_break_mode::before, emit::indentation_mode::indent_after);
        }

        [[nodiscard]] static auto begin_table_header(const std::vector<std::string>& styles) -> emit::segment
        {
            return begin_table_row(styles);
        }

        [[nodiscard]] static auto end_table_header() -> emit::segment
        {
            return end_table_row();
        }

        [[nodiscard]] static auto begin_table_row(const std::vector<std::string>& styles) -> emit::segment
        {
            return emit::segment("<tr" + style_classes_attribute(styles) + ">", emit::line_break_mode::before, emit::indentation_mode::indent_after);
        }

        [[nodiscard]] static auto end_table_row() -> emit::segment
        {
            return emit::segment("</tr>", emit::line_break_mode::before, emit::indentation_mode::outdent_before);
        }

        [[nodiscard]] static auto begin_table_cell(const std::vector<std::string>& styles) -> emit::segment
        {
            return emit::segment("<td" + style_classes_attribute(styles) + ">", emit::line_break_mode::before, emit::indentation_mode::indent_after);
        }

        [[nodiscard]] static auto end_table_cell() -> emit::segment
        {
            return emit::segment("</td>", emit::line_break_mode::before, emit::indentation_mode::outdent_before);
        }

        [[nodiscard]] static auto begin_table_header_cell(const std::vector<std::string>& styles) -> emit::segment
        {
            return emit::segment("<th" + style_classes_attribute(styles) + ">", emit::line_break_mode::before, emit::indentation_mode::indent_after);
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