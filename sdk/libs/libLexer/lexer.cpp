// Copyright (c) 2019-2020 Tom Hancocks
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

#include <algorithm>
#include <libFoundation/system/filesystem/file.hpp>
#include <libLexer/lexer.hpp>
#include <libLexer/conditions/match.hpp>
#include <libLexer/conditions/sequence.hpp>
#include <libLexer/conditions/range.hpp>
#include <libLexer/conditions/set.hpp>
#include <libLexer/conditions/identifier_set.hpp>
#include <libLexer/conditions/decimal_set.hpp>
#include <libLexer/conditions/hexadecimal_set.hpp>
#include <libLexer/exception/exception.hpp>
#include <libLexer/exception/unrecognised_character_exception.hpp>

// MARK: - Constructor

lexer::lexer::lexer(const std::shared_ptr<foundation::filesystem::file>& source)
    : m_source(source)
{
}

lexer::lexer::lexer(const std::string& str)
    : m_source(std::make_shared<foundation::filesystem::file>("", str))
{
}

// MARK: - Comments

auto lexer::lexer::set_comment_style(enum comment_style style) -> void
{
    m_comment_style = style;
}

// MARK: - Keywords

auto lexer::lexer::add_keyword(const std::string &keyword) -> void
{
    m_keywords.insert(keyword);
}

// MARK: - Lexical Analysis

auto lexer::lexer::analyze() -> lexical_result
{
    if (!m_lexemes.empty()) {
        return lexical_result(m_lexemes);
    }

    // Loop through the source code as long as there are characters available to consume.
    while (available()) {

        // Consume any leading whitespace
        consume_while(condition::set<' ', '\t'>::contains);

        // Check if we're looking at a newline. If we are the simply consume it and increment the current line number.
        if (test_if(condition::match<'\n'>::yes)) {
            advance();
            m_line++;
            m_offset = 0;
            continue;
        }
        else if (test_if(condition::match<'\r'>::yes)) {
            advance();
            continue;
        }

        // Check for a comment. If we're looking at a comment then we need to consume the entire line. We need to
        // advance past the character at the end of the match.
        if (test_if(condition::match<'`'>::yes) && m_comment_style == comment_style::KDL) {
            consume_while(condition::match<'\n'>::no);
            continue;
        }
        else if (test_if(condition::sequence<'/', '/'>::yes, 0, 2) && m_comment_style == comment_style::CXX) {
            consume_while(condition::match<'\n'>::no);
            continue;
        }
        else if (test_if(condition::sequence<'-', '-'>::yes, 0, 2) && m_comment_style == comment_style::LUA) {
            consume_while(condition::match<'\n'>::no);
            continue;
        }

        // Literals
        else if (test_if(condition::match<'"'>::yes)) {
            // We're looking at a string literal.
            // The string continues until a corresponding '"' is found.
            advance();
            consume_while(condition::match<'"'>::no);
            m_lexemes.emplace_back(m_slice, lexeme_type::string, m_pos, m_offset, m_line, m_source);
            advance();
        }
        else if (test_if(condition::match<'\''>::yes)) {
            // We're looking at a string literal.
            // The string continues until a corresponding ' is found.
            advance();
            consume_while(condition::match<'\''>::no);
            m_lexemes.emplace_back(m_slice, lexeme_type::string, m_pos, m_offset, m_line, m_source);
            advance();
        }
        else if (test_if(condition::match<'0'>::yes) && test_if(condition::set<'x', 'X'>::contains, 1)) {
            // We're looking at a hexadecimal number
            advance(2);
            consume_while(condition::hexadecimal_set::contains);
            m_lexemes.emplace_back("0x" + m_slice, lexeme_type::integer, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::decimal_set::contains) || (test_if(condition::match<'-'>::yes) && test_if(condition::decimal_set::contains, 1))) {
            // We're looking at a number
            auto negative = test_if(condition::match<'-'>::yes);
            if (negative) {
                advance();
            }

            consume_while(condition::decimal_set::contains);
            auto number_text = m_slice;
            if (negative) {
                number_text.insert(0, 1, '-');
            }

            m_lexemes.emplace_back(number_text, lexeme_type::integer, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::identifier_set::limited_contains)) {
            consume_while(condition::identifier_set::contains);

            if (m_keywords.contains(m_slice)) {
                m_lexemes.emplace_back(m_slice, lexeme_type::keyword, m_pos, m_offset, m_line, m_source);
            }
            else {
                m_lexemes.emplace_back(m_slice, lexeme_type::identifier, m_pos, m_offset, m_line, m_source);
            }
        }

        // Symbols
        else if (test_if(condition::match<';'>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::semi, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::match<'{'>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::l_brace, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::match<'}'>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::r_brace, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::match<'['>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::l_bracket, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::match<']'>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::r_bracket, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::match<'('>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::l_paren, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::match<')'>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::r_paren, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::sequence<'<', '<'>::yes, 0, 2)) {
            m_lexemes.emplace_back(read(0, 2), lexeme_type::left_shift, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::sequence<'>', '>'>::yes, 0, 2)) {
            m_lexemes.emplace_back(read(0, 2), lexeme_type::right_shift, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::match<'<'>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::l_angle, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::match<'>'>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::r_angle, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::match<'='>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::equals, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::match<'+'>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::plus, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::match<'-'>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::minus, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::match<'*'>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::star, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::match<'/'>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::slash, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::match<'&'>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::amp, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::match<'.'>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::dot, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::match<','>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::comma, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::match<'|'>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::pipe, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::match<'^'>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::carat, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::match<':'>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::colon, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::match<'!'>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::exclaim, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::match<'@'>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::at, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::match<'#'>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::hash, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::match<'?'>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::question, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::match<'%'>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::percent, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::match<'~'>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::tilde, m_pos, m_offset, m_line, m_source);
        }
        else if (test_if(condition::match<'$'>::yes)) {
            m_lexemes.emplace_back(read(), lexeme_type::dollar, m_pos, m_offset, m_line, m_source);
        }

        // Unrecognised character encountered
        else if (available()) {
            throw unrecognised_character_exception(dummy());
        }
    }

    return lexical_result(m_lexemes);
}

// MARK: - Private Lexer

auto lexer::lexer::dummy(long offset) const -> lexeme
{
    return { "<no text available>", lexeme_type::string, m_pos + offset, m_offset + offset, m_line, m_source };
}

auto lexer::lexer::length() const -> std::size_t
{
    return m_source->string_contents().size();
}

auto lexer::lexer::advance(long offset) -> void
{
    m_pos += offset;
    m_offset += offset;
}

auto lexer::lexer::available(long offset, std::size_t length) const -> bool
{
    auto start = m_pos + offset;
    auto end = start + length;
    return (end <= this->length());
}

auto lexer::lexer::peek(long offset, std::size_t length) const -> std::string
{
    if (!available(offset, length)) {
        throw exception("Failed to peek " + std::to_string(length) + " characters from source.", dummy(offset));
    }
    return m_source->string_contents().substr(m_pos + offset, length);
}

auto lexer::lexer::read(long offset, std::size_t length) -> std::string
{
    auto str = peek(offset, length);
    advance(offset + std::int32_t(length));
    return str;
}

auto lexer::lexer::test_if(const std::function<auto(const std::string&) -> bool>& fn, long offset, std::size_t length) const -> bool
{
    return available(offset, length) && fn(peek(offset, length));
}

auto lexer::lexer::consume_while(const std::function<auto(const std::string&) -> bool>& fn, std::size_t size) -> bool
{
    m_slice.clear();
    while (available(0, size) && fn(peek(0, size))) {
        m_slice += read(0, size);
    }
    return !m_slice.empty();
}
