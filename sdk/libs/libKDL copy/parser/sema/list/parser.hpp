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

#include <tuple>
#include <vector>
#include <string>
#include <optional>
#include <unordered_map>
#include <libKDL/lexer/lexeme_stream.hpp>
#include <libKDL/session/interpreter/scope.hpp>

namespace kdl::sema::list
{
    /**
     * The kdl::sema::list::parser is responsible for parsing lists of items, such as
     *  ( arg0, arg1, arg2 )
     */
    struct parser
    {
    public:
        explicit parser(lexer::lexeme_stream& stream);

        auto set_list_start(enum lexer::lexeme::type lx) -> void;
        auto set_list_end(enum lexer::lexeme::type lx) -> void;
        auto set_delimiter(enum lexer::lexeme::type lx) -> void;

        auto add_valid_list_item(enum lexer::lexeme::type lx) -> void;
        auto add_valid_list_item(enum lexer::lexeme::type lx, const std::string& text) -> void;

        auto parse(session::interpreter::scope& scope, bool resolve_vars = true) -> std::vector<lexer::lexeme>;

    private:
        lexer::lexeme_stream& m_stream;
        enum lexer::lexeme::type m_list_start { lexer::lexeme::l_angle };
        enum lexer::lexeme::type m_list_end { lexer::lexeme::r_angle };
        enum lexer::lexeme::type m_delimit { lexer::lexeme::comma };
        std::vector<std::tuple<enum lexer::lexeme::type, std::optional<std::string>>> m_valid_lexemes;
    };
}