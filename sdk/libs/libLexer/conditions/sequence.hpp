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
#include <vector>

namespace lexer::condition
{
    /**
     * Template conditional for the lexer. Defines a condition for matching a sequence of specified characters.
     * @tparam tC       The first character to be matched.
     * @tparam ttC      The subsequent characters to be matched, repeated as many times as required.
     */
    template<char tC, char...ttC>
    struct sequence
    {
        /**
         * Test to see if the specified string contains the same sequence of characters as defined in the condition.
         * @param __Chk     The string to check.
         * @return          The result of the test. True if the string contains the same sequence of characters as the
         *                  condition.
         */
        static auto yes(const std::string& __Chk) -> bool
        {
            std::vector<char> v = { tC, ttC... };
            for (auto i = 0; i < v.size(); ++i) {
                if (__Chk[i] != v[i]) {
                    return false;
                }
            }
            return true;
        }

        /**
         * Test to see if the specified string does not contain the same sequence of characters as
         * defined in the condition.
         * @param __Chk     The string to check.
         * @return          The result of the test. True if the string does not contain the same sequence of characters
         *                  as the condition.
         */
        static auto no(const std::string& __Chk) -> bool
        {
            return !yes(__Chk);
        }
    };
}