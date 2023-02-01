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
#include <libKDL/session/types/binary_template/binary_template.hpp>
#include <libKDL/session/interpreter/scope.hpp>

namespace kdl::session
{
    class session;

    struct type_descriptor
    {
    public:
        struct built_in_type_names {
            static constexpr const char *rect = "Rect";
            static constexpr const char *size = "Size";
            static constexpr const char *point = "Point";
            static constexpr const char *color = "Color";
            static constexpr const char *integer = "Integer";
            static constexpr const char *string = "String";
            static constexpr const char *range = "Range";
            static constexpr const char *bitmask = "Bitmask";
            static constexpr const char *file = "File";
            static constexpr const char *image = "Image";
        };

        type_descriptor() = default;

        explicit type_descriptor(bool is_reference)
            : m_reference(is_reference) {};

        type_descriptor(bool is_reference, const lexer::lexeme& name)
            : m_reference(is_reference), m_name(name) {};

        type_descriptor(bool is_reference, const lexer::lexeme& name, const std::vector<lexer::lexeme>& hints)
            : m_reference(is_reference), m_name(name), m_hints(hints) {};

        inline auto set_reference(bool reference) -> void { m_reference = reference; }
        [[nodiscard]] inline auto is_reference() const -> bool { return m_reference; }

        inline auto set_name(const lexer::lexeme& name) -> void { m_name = name; }
        [[nodiscard]] inline auto name() -> lexer::lexeme { return m_name; }
        [[nodiscard]] inline auto has_name() const -> bool { return !m_name.text().empty(); }

        inline auto set_hints(const std::vector<lexer::lexeme>& hints) -> void { m_hints = hints; }
        [[nodiscard]] inline auto hints() const -> const std::vector<lexer::lexeme>& { return m_hints; }

        auto verify_binary_type(enum kdl::session::binary_template::data_type type, const lexer::lexeme& nested_type_name) const -> void;

        [[nodiscard]] auto type_matches(const lexer::lexeme& lx) const -> bool;

        auto inject(interpreter::scope& scope, const session *session) const -> void;

    private:
        bool m_reference { false };
        lexer::lexeme m_name { "", lexer::lexeme::identifier };
        std::vector<lexer::lexeme> m_hints;
    };
}