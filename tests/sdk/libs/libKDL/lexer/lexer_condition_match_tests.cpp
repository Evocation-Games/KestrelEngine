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
#include <libKDL/lexer/conditions/match.hpp>

using namespace kdl::lexer::condition;

// MARK: - Tests

TEST(lexer_condition_matchTemplate_yes_returnsTrueIfEqual)
{
    test::is_true(match<'a'>::yes("a"));
    test::is_true(match<':'>::yes(":"));
    test::is_false(match<'a'>::yes("abc"));
}

TEST(lexer_condition_matchTemplate_yes_returnsFalseIfNotEqual)
{
    test::is_false(match<'a'>::yes("b"));
    test::is_false(match<'a'>::yes("bac"));
}

TEST(lexer_condition_matchTemplate_no_returnsFalseIfEqual)
{
    test::is_false(match<'a'>::no("a"));
}

TEST(lexer_condition_matchTemplate_no_returnsTrueIfNotEqual)
{
    test::is_true(match<'a'>::no("b"));
}
