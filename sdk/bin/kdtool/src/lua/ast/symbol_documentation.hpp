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

namespace kdtool::lua_api::ast
{
    struct symbol_documentation
    {
        struct parameter
        {
            parameter() = default;
            parameter(const std::string& name, const std::string& description);

            [[nodiscard]] auto name() const -> std::string;
            [[nodiscard]] auto description() const -> std::string;

        private:
            std::string m_name;
            std::string m_description;
        };

        symbol_documentation() = default;
        explicit symbol_documentation(const std::string& raw);

        [[nodiscard]] auto empty() const -> bool;
        [[nodiscard]] auto description() const -> std::string;
        [[nodiscard]] auto example() const -> std::string;
        [[nodiscard]] auto warning() const -> std::string;
        [[nodiscard]] auto return_description() const -> std::string;
        [[nodiscard]] auto parameters() const -> std::vector<parameter>;

        auto set_description(const std::string& description) -> void;
        auto set_example(const std::string& example) -> void;
        auto set_warning(const std::string& warning) -> void;
        auto set_return_description(const std::string& return_description) -> void;
        auto add_parameter(const parameter& parameter) -> void;

    private:
        bool m_empty { true };
        std::string m_description;
        std::string m_example;
        std::string m_warning;
        std::string m_return;
        std::vector<parameter> m_parameters;

        auto parse_raw_documentation(const std::string& raw) -> void;
    };
}