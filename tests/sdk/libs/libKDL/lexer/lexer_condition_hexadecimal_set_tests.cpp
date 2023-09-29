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
#include <libKDL/lexer/conditions/hexadecimal_set.hpp>

using namespace kdl::lexer::condition;

// MARK: - Tests

TEST(lexer_condition_hexadecimalSet_containsDecimalNumerals)
{
    test::is_true(hexadecimal_set::contains("0"));
    test::is_true(hexadecimal_set::contains("1"));
    test::is_true(hexadecimal_set::contains("2"));
    test::is_true(hexadecimal_set::contains("3"));
    test::is_true(hexadecimal_set::contains("4"));
    test::is_true(hexadecimal_set::contains("5"));
    test::is_true(hexadecimal_set::contains("6"));
    test::is_true(hexadecimal_set::contains("7"));
    test::is_true(hexadecimal_set::contains("8"));
    test::is_true(hexadecimal_set::contains("9"));
}

TEST(lexer_condition_hexadecimalSet_containsHexNumerals)
{
    test::is_true(hexadecimal_set::contains("a"));
    test::is_true(hexadecimal_set::contains("b"));
    test::is_true(hexadecimal_set::contains("c"));
    test::is_true(hexadecimal_set::contains("d"));
    test::is_true(hexadecimal_set::contains("e"));
    test::is_true(hexadecimal_set::contains("f"));

    test::is_true(hexadecimal_set::contains("A"));
    test::is_true(hexadecimal_set::contains("B"));
    test::is_true(hexadecimal_set::contains("C"));
    test::is_true(hexadecimal_set::contains("D"));
    test::is_true(hexadecimal_set::contains("E"));
    test::is_true(hexadecimal_set::contains("F"));
}

TEST(lexer_condition_hexadecimalSet_doesNotContainUnexpectedCharacters)
{
    test::is_false(hexadecimal_set::contains("j"));
    test::is_false(hexadecimal_set::contains("£"));
    test::is_false(hexadecimal_set::contains("G"));
    test::is_false(hexadecimal_set::contains("."));
    test::is_false(hexadecimal_set::contains("x"));
    test::is_false(hexadecimal_set::contains("O"));
}
