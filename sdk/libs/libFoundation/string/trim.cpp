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

#include <sstream>
#include <libFoundation/string/trim.hpp>

// MARK: - Basic

auto foundation::string::trim(const std::string &str, bool rhs) -> std::string
{
    auto s = str;
    while(s[0] == ' ' || s[0] == '\t') {
        s.erase(0, 1);
    }

    if (rhs) {
        while (s.ends_with(' ') || s.ends_with('\t')) {
            s.erase(s.size() - 1, 1);
        }
    }

    return std::move(s);
}

// MARK: - Complex

auto foundation::string::trim_empty_lines(const std::string &str) -> std::string
{
    std::string buffer;
    std::string line;
    std::stringstream in(str);
    while (std::getline(in, line)) {
        trim(line);
        if (!line.empty()) {
            if (!buffer.empty()) {
                buffer += "\n";
            }
            buffer += line;
        }
    }
    return std::move(buffer);
}
