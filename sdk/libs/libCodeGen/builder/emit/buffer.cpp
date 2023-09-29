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
#include <fstream>
#include <libCodeGen/builder/emit/buffer.hpp>
#include <libCodeGen/languages/markup/markdown.hpp>

// MARK: - Helpers

std::int32_t codegen::language::markdown::column_counter = 0;

// MARK: - Saving

auto codegen::emit::buffer::save(const foundation::filesystem::path& path) -> void
{
    // Make sure the directory exists...
    path.create_directory();

    // Save the file
    std::ofstream file(path.string(), std::ofstream::out);
    if (file.is_open()) {
        file << m_output << std::endl;
        file.flush();
        file.close();
    }
    else {
        std::cerr << "Failed to write to file: " << path.string() << std::endl;
    }
}

// MARK: - Writing

auto codegen::emit::buffer::write(const std::shared_ptr<ast::node>& node) -> void
{
    write(node->emit());
}

auto codegen::emit::buffer::write(const std::vector<segment> &segments) -> void
{
    if (segments.empty()) {
        return;
    }

    for (const auto& segment : segments) {
        write(segment);
    }
}

auto codegen::emit::buffer::write(const segment& segment) -> void
{
    if (segment.indentation() == indentation_mode::outdent_before) {
        outdent();
    }
    else if (segment.indentation() == indentation_mode::indent_before) {
        indent();
    }

    if (segment.line_break() == line_break_mode::before || segment.line_break() == line_break_mode::full) {
        newline();
    }

    write(segment.string());
    write(segment.sub_segments());

    if (segment.indentation() == indentation_mode::outdent_after) {
        outdent();
    }
    else if (segment.indentation() == indentation_mode::indent_after) {
        indent();
    }

    if (segment.line_break() == line_break_mode::after || segment.line_break() == line_break_mode::full) {
        newline();
    }
}

auto codegen::emit::buffer::write(const std::string &str) -> void
{
    m_output += str;
}

// MARK: - Helpers

auto codegen::emit::buffer::indent() -> void
{
    m_indentation++;
}

auto codegen::emit::buffer::outdent() -> void
{
    m_indentation--;
    if (m_indentation < 0) {
        m_indentation = 0;
    }
}

auto codegen::emit::buffer::newline() -> void
{
    m_output += "\n" + indentation_string();
}

auto codegen::emit::buffer::indentation_string() const -> std::string
{
    std::string out;
    for (auto i = 0; i < m_indentation; ++i) {
        out += "    ";
    }
    return std::move(out);
}
