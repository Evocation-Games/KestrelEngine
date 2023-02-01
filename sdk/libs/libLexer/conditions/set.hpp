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
     * Template conditional for the lexer. Checks to see if a character is contained in the defined set.
     * @tparam tC       The first character in the set.
     * @tparam ttC      Subsequent characters in the set.
     */
    template<char tC, char...ttC>
    struct set
    {
        /**
         * Check to see if the specified character is contained in the list of characters defined on the condition.
         * @param __Chk     The character to test.
         * @return          The result of the test. True if the defined list of characters contains __Chk.
         */
        static auto contains(const std::string& __Chk) -> bool
        {
            std::vector<char> v = { tC, ttC... };
            for (const auto __ch : __Chk) {
                if (std::find(v.begin(), v.end(), __ch) == v.end()) {
                    return false;
                }
            }
            return true;
        }

        /**
         * Check to see if the specified character is not contained in the list of characters defined on the condition.
         * @param __Chk     The character to test.
         * @return          The result of the test. True if the defined list of characters does not contain __Chk.
         */
        static auto not_contains(const std::string& __Chk) -> bool
        {
            return !contains(__Chk);
        }
    };
}