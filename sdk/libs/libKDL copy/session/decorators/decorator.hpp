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

#include <vector>
#include <libKDL/lexer/lexeme.hpp>

namespace kdl::session::decorator
{
    struct key
    {
        static constexpr const char *api_export_name = "name";
        static constexpr const char *conversion_name = "conversion";
        static constexpr const char *format_if = "if";
        static constexpr const char *api_name = "api";
        static constexpr const char *synthesize_name = "synthesize";
        static constexpr const char *example_name = "example";
    };

    struct instance
    {
    public:
        explicit instance(const lexer::lexeme& name, const std::vector<lexer::lexeme>& values);

        [[nodiscard]] auto name() const -> lexer::lexeme;
        [[nodiscard]] auto has_associated_values() const -> bool;
        [[nodiscard]] auto associated_value_count() const -> std::size_t;
        [[nodiscard]] auto associated_value(std::int32_t idx) const -> lexer::lexeme;
        [[nodiscard]] auto has_associated_value(lexer::lexeme lx) const -> bool;
        [[nodiscard]] auto has_associated_value(const std::string& value) const -> bool;

    private:
        lexer::lexeme m_name;
        std::vector<lexer::lexeme> m_associated_values;
    };
}