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

#include <libCodeGen/languages/markup/markdown.hpp>
#include <libCodeGen/spec/markup/node.hpp>

auto codegen::markdown::lc_name() const -> std::string
{
    return "markdown";
}

auto codegen::markdown::extension() const -> std::string
{
    return "md";
}

auto codegen::markdown::line_break() -> std::string
{
    return "\n";
}

auto codegen::markdown::horizontal_rule() -> std::string
{
    return "\n-\n";
}

auto codegen::markdown::plain(const std::string& str) -> std::string
{
    return str;
}

auto codegen::markdown::bold(const std::string& str) -> std::string
{
    return "**" + str + "**";
}

auto codegen::markdown::italic(const std::string& str) -> std::string
{
    return "_" + str + "_";
}

auto codegen::markdown::strikethrough(const std::string& str) -> std::string
{
    return "~~" + str + "~~";
}

auto codegen::markdown::inline_code(const std::string& str) -> std::string
{
    return "`" + str + "`";
}


auto codegen::markdown::heading(const node& body, std::int32_t size) -> std::string
{
    switch (size) {
        case 1:     return "# " + body->value(shared_from_this());
        case 2:     return "## " + body->value(shared_from_this());
        case 3:     return "### " + body->value(shared_from_this());
        case 4:     return "#### " + body->value(shared_from_this());
        case 5:     return "##### " + body->value(shared_from_this());
        case 6:     return "###### " + body->value(shared_from_this());
        default:    return "### " + body->value(shared_from_this());
    }
}

auto codegen::markdown::anchor(const node& body, const std::string& href, bool wants_extension) -> std::string
{
    return "[" + body->value(shared_from_this()) + "](" + href + (wants_extension ? "." + extension() : "") + ")";
}


auto codegen::markdown::list(const node& body) -> std::vector<std::string>
{
    std::vector<std::string> out;
    out.emplace_back("");
    for (const auto& item : body->emit(shared_from_this())) {
        if (out.back() == item) {
            continue;
        }
        out.emplace_back(item);
    }
    return std::move(out);
}

auto codegen::markdown::list_item(const node& body) -> std::vector<std::string>
{
    std::vector<std::string> out;
    for (const auto& line : body->emit(shared_from_this())) {
        if (out.empty()) {
            out.emplace_back(" - " + line);
        }
        else {
            out.emplace_back("   " + line);
        }
    }
    return out;
}

auto codegen::markdown::preformatted(const std::string& body) -> std::vector<std::string>
{
    std::vector<std::string> out;
    out.emplace_back("```");
    out.emplace_back(body);
    out.emplace_back("```");
    return std::move(out);
}

auto codegen::markdown::blockquote(const node& body) -> std::vector<std::string>
{
    std::vector<std::string> out;
    for (const auto& line : body->emit(shared_from_this())) {
        out.emplace_back("> " + line);
    }
    return std::move(out);
}

auto codegen::markdown::table(const node& body) -> std::vector<std::string>
{
    return body->emit(shared_from_this());
}

auto codegen::markdown::table_row(const node& body) -> std::vector<std::string>
{
    std::string row;

    for (const auto& cell : body->emit(shared_from_this())) {
        if (!row.empty()) {
            row += " | ";
        }
        row += cell;
    }

    return {
        "| " + row + " |",
    };
}

auto codegen::markdown::table_header_row(const node& body) -> std::vector<std::string>
{
    std::string headings;
    std::string seperator;

    for (const auto& cell : body->emit(shared_from_this())) {
        if (!seperator.empty()) {
            headings += " | ";
            seperator += " | ";
        }
        headings += cell;
        seperator += "---";
    }

    return {
        "| " + headings + " |",
        "| " + seperator + " |",
    };
}
