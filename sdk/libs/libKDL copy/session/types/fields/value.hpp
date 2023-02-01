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
#include <optional>
#include <unordered_map>
#include <libHashing/hashing.hpp>
#include <libKDL/lexer/lexeme.hpp>
#include <libKDL/session/types/binary_template/binary_template.hpp>
#include <libKDL/session/types/descriptor.hpp>
#include <libKDL/session/conversion/conversion.hpp>
#include <libKDL/session/decorators/collection.hpp>
#include <libKDL/session/interpreter/scope.hpp>
#include <libKDL/session/interpreter/expression.hpp>

namespace kdl::session
{
    struct type_field_value
    {
    public:
        explicit type_field_value(const lexer::lexeme& name);

        [[nodiscard]] auto base_name() const -> lexer::lexeme;
        [[nodiscard]] auto extended_name(const kdl::session::interpreter::scope& scope) const -> lexer::lexeme;
        [[nodiscard]] auto has_name_extensions() const -> bool;

        [[nodiscard]] auto decorators() const -> const decorator::collection&;
        auto set_decorators(const std::vector<decorator::instance>& decorators) -> void;
        auto set_decorators(const decorator::collection& collection) -> void;

        [[nodiscard]] auto has_export_name() const -> bool;
        [[nodiscard]] auto export_name() const -> lexer::lexeme;
        auto set_export_name(const lexer::lexeme& name) -> void;

        auto set_name_extensions(const std::vector<lexer::lexeme>& extensions) -> void;
        auto add_name_extension(const lexer::lexeme& extension) -> void;

        [[nodiscard]] auto has_explicit_type() const -> bool;
        [[nodiscard]] auto explicit_type() const -> type_descriptor;
        auto set_explicit_type(const type_descriptor& type) -> void;
        auto set_explicit_type(bool reference, const lexer::lexeme& name, const std::vector<lexer::lexeme>& hints = {}) -> void;

        [[nodiscard]] auto has_default_value() const -> bool;
        [[nodiscard]] auto default_value() const -> interpreter::expression;
        auto set_default_value(const interpreter::expression& expression) -> void;
        auto set_default_value(const lexer::lexeme& expression) -> void;
        auto set_default_value(const std::string& value) -> void;
        auto set_default_value(std::int64_t value) -> void;

        [[nodiscard]] auto symbols() const -> std::vector<lexer::lexeme>;
        [[nodiscard]] auto has_symbol(const lexer::lexeme& name) const -> bool;
        [[nodiscard]] auto has_symbol(const std::string& name) const -> bool;
        [[nodiscard]] auto value_for_symbol(const lexer::lexeme& name) const -> lexer::lexeme;
        [[nodiscard]] auto value_for_symbol(const std::string& name) const -> lexer::lexeme;
        auto add_symbol(const lexer::lexeme& name, const lexer::lexeme& value) -> void;
        auto add_symbol(const std::string& name, const std::string& value) -> void;
        auto add_symbol(const std::string& name, std::int64_t value) -> void;

        [[nodiscard]] auto has_conversion() const -> bool;
        [[nodiscard]] auto conversion() const -> conversion;
        auto set_conversion(const lexer::lexeme& input, const lexer::lexeme& output) -> void;

        [[nodiscard]] auto has_joined_values() const -> bool;
        [[nodiscard]] auto joined_values_count() const -> std::size_t;
        [[nodiscard]] auto joined_value_at(std::int32_t idx) const -> const type_field_value&;
        [[nodiscard]] auto has_joined_value_for(const lexer::lexeme& symbol) const -> bool;
        [[nodiscard]] auto has_joined_value_for(const std::string& symbol) const -> bool;
        [[nodiscard]] auto joined_value_for(const lexer::lexeme& symbol) const -> std::tuple<std::int32_t, lexer::lexeme>;
        [[nodiscard]] auto joined_value_for(const std::string& symbol) const -> std::tuple<std::int32_t, lexer::lexeme>;

    private:
        lexer::lexeme m_base_name;
        type_descriptor m_explicit_type;
        std::optional<interpreter::expression> m_default_value;
        std::vector<std::tuple<lexer::lexeme, lexer::lexeme>> m_symbols;
        std::vector<lexer::lexeme> m_name_extensions;
        std::vector<type_field_value> m_joined_values;
        decorator::collection m_decorators;
    };
}