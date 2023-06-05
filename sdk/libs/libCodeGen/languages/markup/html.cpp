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

#include <libCodeGen/languages/markup/html.hpp>
#include <libCodeGen/spec/markup/node.hpp>

auto codegen::html::lc_name() const -> std::string
{
    return "html";
}

auto codegen::html::extension() const -> std::string
{
    return "html";
}

auto codegen::html::line_break() -> std::string
{
    return "<br/>";
}

auto codegen::html::horizontal_rule() -> std::string
{
    return "<hr/>";
}

auto codegen::html::plain(const std::string& str) -> std::string
{
    return str;
}

auto codegen::html::bold(const std::string& str) -> std::string
{
    return "<b>" + str + "</b>";
}

auto codegen::html::italic(const std::string& str) -> std::string
{
    return "<i>" + str + "</i>";
}

auto codegen::html::strikethrough(const std::string& str) -> std::string
{
    return "<s>" + str + "</s>";
}

auto codegen::html::inline_code(const std::string& str) -> std::string
{
    return "<code>" + str + "</code>";
}


auto codegen::html::heading(const node& body, std::int32_t size) -> std::string
{
    switch (size) {
        case 1:     return "<h1>" + body->value(shared_from_this()) + "</h1>";
        case 2:     return "<h2>" + body->value(shared_from_this()) + "</h2>";
        case 3:     return "<h3>" + body->value(shared_from_this()) + "</h3>";
        case 4:     return "<h4>" + body->value(shared_from_this()) + "</h4>";
        case 5:     return "<h5>" + body->value(shared_from_this()) + "</h5>";
        case 6:     return "<h6>" + body->value(shared_from_this()) + "</h6>";
        default:    return "<h3>" + body->value(shared_from_this()) + "</h3>";
    }
}

auto codegen::html::anchor(const node& body, const std::string& href) -> std::string
{
    return "<a href=\"" + href +  + "." + extension() + "\">" + body->value(shared_from_this()) + "</a>";
}


auto codegen::html::list(const node& body) -> std::vector<std::string>
{
    std::vector<std::string> out;
    out.emplace_back("  <ul>");

    for (const auto& item : body->emit(shared_from_this())) {
        out.emplace_back(item);
    }

    out.emplace_back("  </ul>");
    return std::move(out);
}

auto codegen::html::list_item(const node& body) -> std::string
{
    return "        <li>" + body->value(shared_from_this()) + "</li>";
}


auto codegen::html::preformatted(const std::string& body) -> std::vector<std::string>
{
    std::vector<std::string> out;
    out.emplace_back("  <pre>");
    out.emplace_back(body);
    out.emplace_back("  </pre>");
    return std::move(out);
}

auto codegen::html::blockquote(const node& body) -> std::vector<std::string>
{
    std::vector<std::string> out;
    out.emplace_back("  <blockquote>");
    for (const auto& line : body->emit(shared_from_this())) {
        out.emplace_back(line);
    }
    out.emplace_back("  </blockquote>");
    return std::move(out);
}

auto codegen::html::table(const node& body) -> std::vector<std::string>
{
    std::vector<std::string> out;
    out.emplace_back("  <table cellpadding=5 border=1>");

    for (const auto& line : body->emit(shared_from_this())) {
        out.emplace_back(line);
    }

    out.emplace_back("  </table>");
    return std::move(out);
}

auto codegen::html::table_row(const node& body) -> std::vector<std::string>
{
    std::vector<std::string> out;
    out.emplace_back("      <tr>");

    for (const auto& cell : body->emit(shared_from_this())) {
        out.emplace_back("          <td>" + cell + "</td>");
    }

    out.emplace_back("      </tr>");
    return std::move(out);
}

auto codegen::html::table_header_row(const node& body) -> std::vector<std::string>
{
    std::vector<std::string> out;
    out.emplace_back("      <tr>");

    for (const auto& cell : body->emit(shared_from_this())) {
        out.emplace_back("          <th>" + cell + "</th>");
    }

    out.emplace_back("      </tr>");
    return std::move(out);
}