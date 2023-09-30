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

#include <iostream>
#include <libKDL/diagnostic/diagnostic.hpp>

// MARK: - Construction

kdl::diagnostic::diagnostic(const lexer::lexeme& lexeme, std::string reason, const std::vector<std::string>& info)
    : m_line(lexeme.line()),
      m_offset(lexeme.offset()),
      m_pos(0),
      m_reason(std::move(reason)),
      m_info(info),
      m_source_line(lexeme.text())
{
    if (auto owner = lexeme.owner().lock()) {
        m_file = owner->path().string();
    }
}

kdl::diagnostic::diagnostic(std::string reason, const std::vector<std::string>& info)
    : m_reason(std::move(reason)),
      m_info(info)
{}

kdl::diagnostic::diagnostic(const tokenizer::token& tk, std::string reason, const std::vector<std::string>& info)
    : diagnostic(tk.source(), std::move(reason), info)
{}

kdl::diagnostic::diagnostic(const interpreter::token& tk, std::string reason, const std::vector<std::string>& info)
    : m_reason(std::move(reason)),
      m_info(info),
      m_line(tk.line()),
      m_file(tk.file()),
      m_source_line(tk.string_value())
{}

// MARK: - Reporting

auto kdl::diagnostic::report() -> void
{
    std::cerr << location()
              << reason()
              << source_code()
              << additional_info()
              << std::endl;
}

auto kdl::diagnostic::location() const -> std::string
{
    std::string result = "\n";
    if (m_line > 0) {
        result += "L" + std::to_string(m_line) + ": ";
    }

    if (!m_file.empty()) {
        result += m_file;
    }

    result += "\n";
    return result;
}

auto kdl::diagnostic::reason() const -> std::string
{
    return m_reason + "\n";
}

auto kdl::diagnostic::source_code() const -> std::string
{
    std::string result;
    if (!m_source_line.empty()) {
        result += "\n";
        result += "\t" + m_source_line + "\n";
        result += "\n";
    }
    return result;
}

auto kdl::diagnostic::additional_info() const -> std::string
{
    std::string result;
    for (const auto& info : m_info) {
        result += " <attached-info> " + info + "\n";
    }
    return result;
}
