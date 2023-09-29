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

#include <cctype>
#include <cstdlib>
#include <libFoundation/glob/sorting/alpha_numeric_sort.hpp>

/*
The Alphanum Algorithm is an improved sorting algorithm for strings
containing numbers.  Instead of sorting numbers in ASCII order like a
standard sort, this algorithm sorts numbers in numeric order.
The Alphanum Algorithm is discussed at http://www.DaveKoelle.com
This implementation is Copyright (c) 2008 Dirk Jagdmann <doj@cubic.org>.
It is a cleanroom implementation of the algorithm and not derived by
other's works. In contrast to the versions written by Dave Koelle this
source code is distributed with the libpng/zlib license.
This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:
        1. The origin of this software must not be misrepresented; you
             must not claim that you wrote the original software. If you use
             this software in a product, an acknowledgment in the product
             documentation would be appreciated but is not required.
        2. Altered source versions must be plainly marked as such, and
             must not be misrepresented as being the original software.
        3. This notice may not be removed or altered from any source
             distribution. */

auto foundation::sorting::alpha_numeric::sort(const char *l, const char *r) -> int
{
    enum mode_t { STRING, NUMBER } mode = STRING;

    while (*l && *r) {
        if (mode == STRING) {
            char l_char, r_char;
            while ((l_char = *l) && (r_char = *r)) {
                // check if this are digit characters
                const bool l_digit = ::isdigit(l_char), r_digit = ::isdigit(r_char);
                // if both characters are digits, we continue in NUMBER mode
                if (l_digit && r_digit) {
                    mode = NUMBER;
                    break;
                }
                // if only the left character is a digit, we have a result
                if (l_digit) return -1;
                // if only the right character is a digit, we have a result
                if (r_digit) return +1;
                // compute the difference of both characters
                const int diff = l_char - r_char;
                // if they differ we have a result
                if (diff != 0) return diff;
                // otherwise process the next characters
                ++l;
                ++r;
            }
        }
        else { // mode==NUMBER
            // get the left number
            char *end;
            unsigned long l_int = ::strtoul(l, &end, 0);
            l = end;

            // get the right number
            unsigned long r_int = ::strtoul(r, &end, 0);
            r = end;

            // if the difference is not equal to zero, we have a comparison result
            const long diff = l_int - r_int;
            if (diff != 0) return diff;

            // otherwise we process the next substring in STRING mode
            mode = STRING;
        }
    }

    if (*r) return -1;
    if (*l) return +1;
    return 0;
}