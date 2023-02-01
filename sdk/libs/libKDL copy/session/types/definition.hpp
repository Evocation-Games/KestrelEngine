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
#include <unordered_map>
#include <libKDL/session/decorators/collection.hpp>
#include <libKDL/session/output/format.hpp>
#include <libKDL/session/types/constructor.hpp>
#include <libKDL/session/types/fields/definition.hpp>
#include <libKDL/session/types/binary_template/binary_template.hpp>

namespace kdl::session
{
    struct type_definition
    {
    public:
        type_definition(const std::string& name, const std::string& code);

        [[nodiscard]] auto name() const -> std::string;
        [[nodiscard]] auto code() const -> std::string;

        [[nodiscard]] auto is_format_compatible(const format& format) const -> bool;

        [[nodiscard]] auto decorators() const -> const decorator::collection&;
        auto set_decorators(const decorator::collection& collection) -> void;
        auto set_decorators(const std::vector<decorator::instance>& decorators) -> void;

        [[nodiscard]] auto constructors() const -> const std::vector<type_constructor>&;
        auto add_constructor(const type_constructor& constructor) -> void;

        auto binary_template() -> struct binary_template&;
        auto set_binary_template(const struct binary_template& tmpl) -> void;

        auto add_field(const lexer::lexeme& name) -> type_field&;
        auto add_field(const type_field& field) -> void;
        [[nodiscard]] auto field_named(const lexer::lexeme& name) const -> const type_field&;
        [[nodiscard]] auto all_fields() const -> const std::vector<type_field>&;

    private:
        decorator::collection m_decorators;
        std::string m_name;
        std::string m_code;
        struct binary_template m_template;
        std::vector<type_field> m_fields;
        std::vector<type_constructor> m_constructors;
    };
}