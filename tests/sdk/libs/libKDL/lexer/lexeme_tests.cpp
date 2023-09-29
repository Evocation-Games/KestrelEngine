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

#include <libTesting/testing.hpp>
#include <libKDL/lexer/lexeme.hpp>

using namespace kdl::lexer;

// MARK: - Helpers

static auto stub_owner_file() -> std::shared_ptr<kdl::host::filesystem::file>
{
    static std::shared_ptr<kdl::host::filesystem::file> stub;
    if (!stub) {
        stub = std::make_shared<kdl::host::filesystem::file>("/path/to/source.kdl", "example-contents");
    }
    return stub;
}

// MARK: - Tests

TEST(lexer_lexeme_constructStringValueLexemeCorrectly)
{
    lexeme lx("example", lexeme::string);
    test::equal(lx.text(), "example");
    test::equal(lx.type(), lexeme::string);
}

TEST(lexer_lexeme_constructIntegerLexemeCorrectly)
{
    lexeme lx(50);
    test::equal(lx.text(), "50");
    test::equal(lx.type(), lexeme::integer);
}

TEST(lexer_lexeme_constructBasicLexemeCorrectly)
{
    lexeme lx("example", lexeme::identifier, 1, 2, 3, stub_owner_file());
    test::equal(lx.text(), "example");
    test::equal(lx.type(), lexeme::identifier);
    test::equal(lx.offset(), 2);
    test::equal(lx.line(), 3);
    test::equal(lx.source_directory().string(), "/path/to");
}

TEST(lexer_lexeme_constructComponentsLexemeCorrectly)
{
    std::vector<std::string> components({"test", "components"});

    lexeme lx(components, lexeme::string, 1, 2, 3, stub_owner_file());
    test::equal(lx.text(), "test.components");
    test::equal(lx.components(), components);
    test::equal(lx.type(), lexeme::string);
    test::equal(lx.offset(), 2);
    test::equal(lx.line(), 3);
    test::equal(lx.source_directory().string(), "/path/to");
}

TEST(lexer_lexeme_reportsSourceLocationCorrectly)
{
    lexeme lx("example", lexeme::identifier, 1, 2, 3, stub_owner_file());
    test::equal(lx.location(), "/path/to/source.kdl:L3:2");
}

TEST(lexer_lexeme_isLexeme_reportsCorrectly)
{
    lexeme lx("example", lexeme::identifier);
    lexeme expected("example", lexeme::identifier);
    lexeme unexpected("foo", lexeme::identifier);
    lexeme unexpected2("example", lexeme::percentage);

    test::is_true(lx.is(expected));
    test::is_false(lx.is(unexpected));
    test::is_false(lx.is(unexpected2));
}

TEST(lexer_lexeme_isLexemeType_reportsCorrectly)
{
    lexeme lx("example", lexeme::identifier);
    test::is_true(lx.is(lexeme::identifier));
    test::is_false(lx.is(lexeme::res_id));
}

TEST(lexer_lexeme_isLexemeString_reportsCorrectly)
{
    lexeme lx("example", lexeme::identifier);
    test::is_true(lx.is("example"));
    test::is_false(lx.is("foo"));
}

TEST(lexer_lexeme_isLexemeTypeString_reportsCorrectly)
{
    lexeme lx("example", lexeme::identifier);

    test::is_true(lx.is(lexeme::identifier, "example"));
    test::is_false(lx.is(lexeme::identifier, "foo"));
    test::is_false(lx.is(lexeme::string, "example"));
}

TEST(lexer_lexeme_reportsBasicTextCorrectly)
{
    lexeme lx("example", lexeme::identifier);
    test::equal(lx.text(), "example");
}

TEST(lexer_lexeme_reportsComponentsTextCorrectly)
{
    std::vector<std::string> components({"test", "components"});
    lexeme lx(components, lexeme::string, 1, 2, 3, stub_owner_file());
    test::equal(lx.text(), "test.components");
}

TEST(lexer_lexeme_reportsComponentsCorrectly)
{
    std::vector<std::string> components({"test", "components"});
    lexeme lx(components, lexeme::string, 1, 2, 3, stub_owner_file());
    test::equal(lx.components(), components);

    lexeme single("single", lexeme::string);
    test::equal(single.components(), std::vector<std::string>({ "single" }));
}

TEST(lexer_lexeme_reportsIntegerValueCorrectly)
{
    lexeme lx(128);
    test::equal(lx.value<std::int32_t>(), 128);
}

TEST(lexer_lexeme_reportsLeftAssociativityCorrectly)
{
    test::is_false(lexeme("", lexeme::carat).left_associative());
    test::is_true(lexeme("", lexeme::star).left_associative());
    test::is_true(lexeme("", lexeme::plus).left_associative());
    test::is_true(lexeme("", lexeme::minus).left_associative());
    test::is_true(lexeme("", lexeme::slash).left_associative());
    test::is_true(lexeme("", lexeme::pipe).left_associative());
    test::is_true(lexeme("", lexeme::amp).left_associative());
    test::is_true(lexeme("", lexeme::tilde).left_associative());
}