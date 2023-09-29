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
     * Template conditional for lexer. Checks to see if a character is a member of the set of characters that
     * are valid for a decimal (base-10) number.
     *
     * @note
     *  [0-9]
     */
    struct decimal_set
    {
        /**
         * Check to see if the specified string exists entirely within the decimal character set, which includes
         * the noted characters.
         *
         * @note [0-9]
         *
         * @param __Chk     The string to test.
         * @return          The result of the test. True if the character matches against the noted character set.
         */
        static auto contains(const std::string& __Chk) -> bool
        {
            for (const auto __ch : __Chk) {
                auto condition = (__ch >= '0' && __ch <= '9');
                if (!condition) {
                    return false;
                }
            }
            return true;
        }
    };
}