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

namespace kdl::lexer::condition
{
    /**
     * Template condition for the lexer. Checks to see if a character is in the range specified between lc and uc.
     * @tparam lc   The lower character bound of the range.
     * @tparam uc   The upper character bound of the range.
     */
    template<char lc, char uc>
    struct range
    {
        /**
         * Check to see if the specified character, exists in between the lower and upper character bounds of the
         * condition.
         * @param __Chk     The character to check.
         * @return          The result of the test. True if the character falls between the lower and upper bounds.
         */
        static auto contains(const std::string& __Chk) -> bool
        {
            for (const auto __ch : __Chk) {
                if (__ch < lc || __ch > uc) {
                    return false;
                }
            }
            return true;
        }

        /**
         * Check to see if the specified character, does not exist in between the lower and upper character bounds of
         * the condition.
         * @param __Chk     The character to check.
         * @return          The result of the test. True if the character falls outside of the lower and upper bounds.
         */
        static auto not_contains(const std::string& __Chk) -> bool
        {
            return !contains(__Chk);
        }
    };
}