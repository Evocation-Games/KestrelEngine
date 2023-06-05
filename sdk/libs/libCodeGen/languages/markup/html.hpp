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
#include <memory>
#include <libCodeGen/languages/markup/markup.hpp>

namespace codegen
{
    struct html : public markup_language, public std::enable_shared_from_this<html>
    {
        [[nodiscard]] auto lc_name() const -> std::string override;
        [[nodiscard]] auto extension() const -> std::string override;

        [[nodiscard]] auto line_break() -> std::string override;
        [[nodiscard]] auto horizontal_rule() -> std::string override;

        [[nodiscard]] auto plain(const std::string& str) -> std::string override;
        [[nodiscard]] auto bold(const std::string& str) -> std::string override;
        [[nodiscard]] auto italic(const std::string& str) -> std::string override;
        [[nodiscard]] auto strikethrough(const std::string& str) -> std::string override;
        [[nodiscard]] auto inline_code(const std::string& str) -> std::string override;

        [[nodiscard]] auto heading(const node& body, std::int32_t size) -> std::string override;
        [[nodiscard]] auto anchor(const node& body, const std::string& href) -> std::string override;

        [[nodiscard]] auto table(const node& body) -> std::vector<std::string> override;
        [[nodiscard]] auto table_row(const node& body) -> std::vector<std::string> override;
        [[nodiscard]] auto table_header_row(const node& body) -> std::vector<std::string> override;

        [[nodiscard]] auto list(const node& body) -> std::vector<std::string> override;
        [[nodiscard]] auto list_item(const node& body) -> std::string override;

        [[nodiscard]] auto preformatted(const std::string& body) -> std::vector<std::string> override;
        [[nodiscard]] auto blockquote(const node& body) -> std::vector<std::string> override;
    };
}