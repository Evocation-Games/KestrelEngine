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

#include <libFoundation/string/trim.hpp>
#include <libDocumentation/parser/parser.hpp>

// MARK: - Construction

documentation::parser::parser(const std::string &text)
    : m_stream(text)
{}

// MARK: - Parsing / Building

auto documentation::parser::build() -> std::shared_ptr<struct object>
{
    m_object = std::make_shared<struct object>();

    std::string line;
    while (std::getline(m_stream, line)) {
        parse_line(line);
    }
    commit();

    return m_object;
}

auto documentation::parser::parse_line(const std::string &line) -> void
{
    auto str = foundation::string::trim(line);

    // Remove comment markers
    // TODO: Detect comment types for different languages...
    if (str.starts_with("/**")) {
        str.erase(0, 3);
    }
    if (str.starts_with("*/")) {
        str.erase(0, 2);
    }
    if (str.starts_with("*")) {
        str.erase(0, 1);
    }

    str += "\n";

    // Consume the line and switch states appropriately.
    str = foundation::string::trim(str);
    while (!str.empty()) {
        const auto section_name = parse_section_name(str);
        if (!section_name.empty()) {
            // We are entering a new section... construct the previous section
            // and store it.
            commit();

            // Setup the new section...
            m_buffer.clear();
            m_param_name.clear();
            m_section_name = section_name;
            str.erase(0, section_name.size());
            str = foundation::string::trim(str);

            // Continue into the next iteration, just in case another tag immediately
            // follows.
            continue;
        }

        if (m_section_name == section::name::param && m_param_name.empty()) {
            if (str[0] == ' ' || str[0] == '\t') {
                m_param_name = m_buffer;
                m_buffer.clear();
                str.erase(0, 1);
                continue;
            }
        }

        m_buffer += str[0];
        str.erase(0, 1);
    }
}

auto documentation::parser::commit() -> void
{
    m_buffer = foundation::string::trim_empty_lines(foundation::string::trim(m_buffer));

    if (m_section_name == section::name::param) {
        m_object->add(parameter(m_param_name, m_buffer));
    }
    else {
        m_object->add(section(m_section_name, m_buffer));
    }
}

// MARK: - Helper Functions

auto documentation::parser::parse_section_name(const std::string &line) -> std::string
{
    auto str = foundation::string::trim(line);
    std::string buffer;
    if (str[0] == '@') {
        for (auto i = 0; i <= str.size(); ++i) {
            if (str[i] == ' ' || str[i]== '\t' || str[i] == '\n' || str[i] == '\0') {
                break;
            }
            buffer += str[i];
        }
    }
    return buffer;
}
