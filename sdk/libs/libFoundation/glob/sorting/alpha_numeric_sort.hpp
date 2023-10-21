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

namespace foundation::sorting::alpha_numeric
{
    /**
     compare l and r with strcmp() semantics, but using
     the "Alphanum Algorithm". This function is designed to read
     through the l and r strings only one time, for
     maximum performance. It does not allocate memory for
     substrings. It can either use the C-library functions isdigit()
     and atoi() to honour your locale settings, when recognizing
     digit characters when you "#define ALPHANUM_LOCALE=1" or use
     it's own digit character handling which only works with ASCII
     digit characters, but provides better performance.
     @param l NULL-terminated C-style string
     @param r NULL-terminated C-style string
     @return negative if l<r, 0 if l equals r, positive if l>r
    */
    auto sort(const char *l, const char *r) -> int;

}