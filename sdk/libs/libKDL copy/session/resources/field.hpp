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

#include <unordered_map>
#include <libHashing/hashing.hpp>
#include <libKDL/session/types/fields/definition.hpp>

namespace kdl::session::resources
{
    struct field
    {
    public:
        typedef std::int32_t lock;

        explicit field(const kdl::session::type_field *definition, const kdl::session::type_field_value *value);

        [[nodiscard]] auto name() const -> lexer::lexeme;

        auto named(const lexer::lexeme& name) -> field&;
        auto set_nested_fields(const std::unordered_map<hashing::value, resources::field>& fields) -> void;

        auto set_value(const lexer::lexeme& value) -> void;

    private:
        const kdl::session::type_field *m_definition;
        const kdl::session::type_field_value *m_value_definition;
        lexer::lexeme m_value { 0 };
        std::unordered_map<hashing::value, resources::field> m_nested_fields;
    };
}