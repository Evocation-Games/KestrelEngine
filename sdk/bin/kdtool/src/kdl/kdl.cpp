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

#include "kdl.hpp"

// MARK: - Construction

kdtool::kdl::tool::tool(const std::string &path)
    : m_output_path(path),
      m_session(m_output, m_context)
{}

// MARK: - Configuration

auto kdtool::kdl::tool::add_file(const std::string& file) -> void
{
    m_session.import_file(file, definitions());
}

auto kdtool::kdl::tool::add_definition(const std::string &symbol) -> void
{
    m_definitions.emplace_back(symbol);
}

auto kdtool::kdl::tool::set_format(const std::string &format) -> void
{
    if (format == "extended" || format == "kestrel") {
        m_format = resource_core::file::format::extended;
    }
    else if (format == "classic" || format == "rsrc") {
        m_format = resource_core::file::format::classic;
    }
    else if (format == "rez") {
        m_format = resource_core::file::format::rez;
    }
    else {
        throw std::runtime_error("");
    }
}

// MARK: - Accessors

auto kdtool::kdl::tool::definitions() const -> std::vector<std::string>
{
    std::vector<std::string> definitions = m_definitions;
    switch (m_format) {
        case resource_core::file::format::extended: {
            definitions.emplace_back("extended");
            break;
        }
        case resource_core::file::format::classic: {
            definitions.emplace_back("classic");
            break;
        }
        case resource_core::file::format::rez: {
            definitions.emplace_back("rez");
            break;
        }
    }
    return definitions;
}

// MARK: - Build

auto kdtool::kdl::tool::build() -> void
{
    m_output.write(m_output_path, m_format);
}