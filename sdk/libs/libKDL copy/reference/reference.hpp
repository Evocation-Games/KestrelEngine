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

#include <libLexer/lexer.hpp>

namespace kdl::session
{
    class session;
    struct type_definition;
}

namespace kdl::session::resources
{
    struct reference
    {
    public:
        typedef std::int64_t identifier;

        reference() = default;
        explicit reference(identifier id, type_definition *type = nullptr, const std::string& name = "");
        explicit reference(const lexer::lexeme& lx, const session *session = nullptr);

        [[nodiscard]] auto id() const -> identifier;
        [[nodiscard]] auto type() const -> const type_definition *;
        [[nodiscard]] auto namespace_name() const -> std::string;

        [[nodiscard]] auto has_type() const -> bool;
        [[nodiscard]] auto has_namespace() const -> bool;
        [[nodiscard]] auto is_auto() const -> bool;
        [[nodiscard]] auto is_namespace(const std::string& name) const -> bool;
        [[nodiscard]] auto is_type(const std::string& type) const -> bool;

        [[nodiscard]] auto with(identifier id) const -> reference;

        [[nodiscard]] auto lexeme() const -> lexer::lexeme;

    private:
        static constexpr identifier auto_value = std::numeric_limits<identifier>::min();

        identifier m_id { auto_value };
        const type_definition *m_type { nullptr };
        std::string m_namespace;
    };
}