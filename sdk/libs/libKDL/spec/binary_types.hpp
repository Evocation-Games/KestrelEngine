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

#pragma once

#include <vector>
#include <string>

namespace kdl::spec::binary_types
{
    static constexpr const char *DBYT = "DBYT";
    static constexpr const char *DWRD = "DWRD";
    static constexpr const char *DLNG = "DLNG";
    static constexpr const char *DQWD = "DQWD";
    static constexpr const char *HBYT = "HBYT";
    static constexpr const char *HWRD = "HWRD";
    static constexpr const char *HLNG = "HLNG";
    static constexpr const char *HQWD = "HQWD";
    static constexpr const char *RECT = "RECT";
    static constexpr const char *PSTR = "PSTR";
    static constexpr const char *CSTR = "CSTR";
    static constexpr const char *Cnnn = "Cnnn";
    static constexpr const char *LSTR = "LSTR";
    static constexpr const char *OSTR = "OSTR";
    static constexpr const char *CHAR = "CHAR";
    static constexpr const char *OCNT = "OCNT";
    static constexpr const char *LSTC = "LSTC";
    static constexpr const char *LSTE = "LSTE";
    static constexpr const char *BBIT = "BBIT";
    static constexpr const char *BOOL = "BOOL";
    static constexpr const char *RSRC = "RSRC";
    static constexpr const char *HEXD = "HEXD";
    static constexpr const char *NESTED = "Nested";

    static auto vector() -> std::vector<std::string>
    {
        return {
            DBYT, DWRD, DLNG, DQWD, HBYT, HWRD, HLNG,
            HQWD, RECT, PSTR, CSTR, Cnnn, LSTR, OSTR,
            CHAR, OCNT, LSTC, LSTE, BBIT, BOOL, HEXD,
            RSRC, NESTED
        };
    }
}