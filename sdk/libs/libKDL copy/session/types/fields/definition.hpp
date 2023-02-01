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
#include <libKDL/lexer/lexeme.hpp>
#include <libKDL/session/types/fields/repeatable.hpp>
#include <libKDL/session/types/fields/value.hpp>
#include <libKDL/session/decorators/collection.hpp>

namespace kdl::session
{
    struct type_field
    {
    public:
        explicit type_field(const lexer::lexeme& name);
        explicit type_field(const std::string& name);

        [[nodiscard]] auto name() const -> lexer::lexeme;

        [[nodiscard]] auto decorators() const -> const decorator::collection&;
        auto set_decorators(const std::vector<decorator::instance>& decorators) -> void;
        auto set_decorators(const decorator::collection& collection) -> void;

        auto repeatable() -> type_field_repeatable&;
        auto make_repeatable(std::int32_t lower, std::int32_t upper) -> type_field_repeatable&;

        [[nodiscard]] auto is_command_encoder() const -> bool;
        [[nodiscard]] auto wants_lua_setter() const -> bool;
        [[nodiscard]] auto should_synthesize() const -> bool;

        auto add_value(const type_field_value& value) -> void;
        [[nodiscard]] auto expected_values() const -> std::size_t;
        auto value_at(std::int32_t idx) -> type_field_value&;
        auto value_named(const lexer::lexeme& name) -> type_field_value&;

    private:
        lexer::lexeme m_name;
        type_field_repeatable m_repeatable;
        std::vector<type_field_value> m_values;
        decorator::collection m_decorators;
    };
}