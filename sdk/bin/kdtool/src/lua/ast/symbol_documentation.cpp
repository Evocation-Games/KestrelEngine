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

#include <sstream>
#include "lua/ast/symbol_documentation.hpp"

// MARK: - Construction

kdtool::lua_api::ast::symbol_documentation::parameter::parameter(const std::string &name, const std::string &description)
    : m_name(name), m_description(description)
{}

kdtool::lua_api::ast::symbol_documentation::symbol_documentation(const std::string &raw)
    : m_empty(false)
{
    parse_raw_documentation(raw);
}

// MARK: - Accessors

auto kdtool::lua_api::ast::symbol_documentation::parameter::name() const -> std::string
{
    return m_name;
}

auto kdtool::lua_api::ast::symbol_documentation::parameter::description() const -> std::string
{
    return m_description;
}

auto kdtool::lua_api::ast::symbol_documentation::empty() const -> bool
{
    return m_empty;
}

auto kdtool::lua_api::ast::symbol_documentation::description() const -> std::string
{
    return m_description;
}

auto kdtool::lua_api::ast::symbol_documentation::example() const -> std::string
{
    return m_example;
}

auto kdtool::lua_api::ast::symbol_documentation::warning() const -> std::string
{
    return m_warning;
}

auto kdtool::lua_api::ast::symbol_documentation::return_description() const -> std::string
{
    return m_return;
}

auto kdtool::lua_api::ast::symbol_documentation::parameters() const -> std::vector<parameter>
{
    return m_parameters;
}

auto kdtool::lua_api::ast::symbol_documentation::set_description(const std::string &description) -> void
{
    m_description = description;
}

auto kdtool::lua_api::ast::symbol_documentation::set_example(const std::string &example) -> void
{
    m_example = example;
}

auto kdtool::lua_api::ast::symbol_documentation::set_warning(const std::string &warning) -> void
{
    m_warning = warning;
}

auto kdtool::lua_api::ast::symbol_documentation::set_return_description(const std::string &return_description) -> void
{
    m_return = return_description;
}

auto kdtool::lua_api::ast::symbol_documentation::add_parameter(const kdtool::lua_api::ast::symbol_documentation::parameter &parameter) -> void
{
    m_parameters.emplace_back(parameter);
}

// MARK: - Parsing

auto kdtool::lua_api::ast::symbol_documentation::parse_raw_documentation(const std::string &raw) -> void
{
    std::string description;
    std::string example;
    std::string warning;
    std::string return_information;
    std::string parameter_name;
    std::string template_parameter_name;
    std::string parameter_description;
    std::string template_parameter_description;
    std::istringstream in(raw);
    std::string line;

    enum class mode {
        description, example, parameter_name, parameter_description, warning, return_information,
        template_parameter_name, template_parameter_description,
    };
    enum mode mode = mode::description;

    auto trim = [] (std::string& str) {
        while (str[0] == ' ' || str[0] == '\t') {
            str.erase(0, 1);
        }
    };

    auto trim_empty_lines = [&] (std::string& str) -> std::string {
        std::string buffer;
        std::string line;
        std::istringstream in(str);
        while (std::getline(in, line)) {
            trim(line);
            if (!line.empty()) {
                if (!buffer.empty()) {
                    buffer += "\n";
                }
                buffer += line;
            }
        }
        return buffer;
    };

    auto parse_documentation_tag = [&] (std::string& line) {
        if (line.starts_with("@code")) {
            line.erase(0, 5);
            mode = mode::example;
            trim(line);
        }
        else if (line.starts_with("@warning")) {
            mode = mode::warning;
            line.erase(0, 8);
            trim(line);
        }
        else if (line.starts_with("@return")) {
            mode = mode::return_information;
            line.erase(0, 7);
            trim(line);
        }
        else if (line.starts_with("@example")) {
            mode = mode::example;
            line.erase(0, 8);
            trim(line);
        }
        else if (line.starts_with("@param")) {
            mode = mode::parameter_name;
            line.erase(0, 6);
            trim(line);
        }
        else if (line.starts_with("@tparam")) {
            mode = mode::template_parameter_name;
            line.erase(0, 7);
        }
    };

    auto parse_text = [&] (std::string& line) -> std::string {
        std::string buffer;
        while (!line.empty()) {
            auto c = line[0];
            if (c == '@') {
                parse_documentation_tag(line);
                return buffer;
            }
            else {
                buffer += c;
                line.erase(0, 1);
            }
        }
        return buffer + "\n";
    };

    auto build_parameter = [&] {
        if (!parameter_name.empty()) {
            add_parameter(parameter(
                trim_empty_lines(parameter_name),
                trim_empty_lines(parameter_description)
            ));
        }
        else {

        }
        parameter_name = "";
        parameter_description = "";
        template_parameter_name = "";
        template_parameter_description = "";
    };

    auto parse_parameter_name = [&] (std::string& line) {
        trim(line);

        std::string buffer;
        while (!line.empty()) {
            auto c = line[0];
            if (c == ' ' || c == '\t') {
                if (mode == mode::parameter_name) {
                    mode = mode::parameter_description;
                }
                else {
                    mode = mode::template_parameter_description;
                }
                trim(line);
                break;
            }
            else if (c == '@') {
                build_parameter();
                parse_documentation_tag(line);
                break;
            }
            line.erase(0, 1);
            buffer += c;
        }

        return buffer;
    };

    auto parse_parameter_description = [&] (std::string& line) {
        trim(line);

        std::string buffer;
        while (!line.empty()) {
            auto c = line[0];
            if (c == '@') {
                build_parameter();
                parse_documentation_tag(line);
                return buffer;
            }
            line.erase(0, 1);
            buffer += c;
        }

        return buffer + "\n";
    };

    auto parse_line = [&] (std::string& line) {
        trim(line);

        while (!line.empty()) {
            switch (mode) {
                case mode::description: {
                    description += parse_text(line);
                    break;
                }
                case mode::warning: {
                    warning += parse_text(line);
                    break;
                }
                case mode::example: {
                    example += parse_text(line);
                    break;
                }
                case mode::return_information: {
                    return_information += parse_text(line);
                    break;
                }
                case mode::parameter_name: {
                    parameter_name += parse_parameter_name(line);
                    break;
                }
                case mode::parameter_description: {
                    parameter_description += parse_parameter_description(line);
                    break;
                }
                case mode::template_parameter_name: {
                    template_parameter_name += parse_parameter_name(line);
                    break;
                }
                case mode::template_parameter_description: {
                    template_parameter_description += parse_parameter_description(line);
                    break;
                }
            }
        }
    };

    while (std::getline(in, line)) {
        trim(line);

        if (line.starts_with("/**")) {
            line.erase(0, 3);
        }
        if (line.starts_with("*/")) {
            line.erase(0, 2);
        }
        if (line.starts_with("*")) {
            line.erase(0, 1);
        }

        if (line.empty()) {
            line = "\n";
        }
        parse_line(line);
    }

    if (mode == mode::parameter_description || mode == mode::parameter_name) {
        build_parameter();
    }

    set_description(trim_empty_lines(description));
    set_warning(trim_empty_lines(warning));
    set_return_description(trim_empty_lines(return_information));
    set_example(trim_empty_lines(example));
}