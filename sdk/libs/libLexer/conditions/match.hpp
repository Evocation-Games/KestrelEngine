// Copyright (c) 2022 Tom Hancocks
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

namespace lexer::condition
{
    /**
     * Template Conditional for the lexer. Defines a condition for matching a single specified character.
     * @tparam c    The character to be matched.
     */
    template<char c>
    struct match
    {
        /**
         * Test to see if the specified string matches the desired character.
         * @param __Chk     The string to check. It should be just one character long.
         * @return          The result of the test. True if the string equals the character.
         */
        static auto yes(const std::string& __Chk) -> bool
        {
            return __Chk == std::string(1, c);
        }

        /**
         * Test to see if the specified string does not match the desired character.
         * @param __Chk     The string to check. It should be just one character long.
         * @return          The result of the test. True if the string does not equal the character.
         */
        static auto no(const std::string& __Chk) -> bool
        {
            return !yes(__Chk);
        }
    };
}