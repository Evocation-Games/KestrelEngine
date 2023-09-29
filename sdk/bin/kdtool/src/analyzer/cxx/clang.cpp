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

#include "analyzer/cxx/clang.hpp"
#include <libFoundation/string/split.hpp>

// MARK: - Strings

auto clang::string(CXString string) -> std::string
{
    if (!string.data) {
        return "";
    }
    std::string s(clang_getCString(string));
    clang_disposeString(string);
    return s;
}

// MARK: - Cursors

auto clang::spelling(CXCursor cursor) -> std::string
{
    return string(clang_getCursorSpelling(cursor));
}

auto clang::spelling(CXType type) -> std::string
{
    return string(clang_getTypeSpelling(type));
}

auto clang::split_spelling(CXCursor cursor, const std::string &delimiter) -> std::vector<std::string>
{
    return foundation::string::split(spelling(cursor), delimiter);
}

// MARK: - Static

auto clang::is_static(CXCursor cursor) -> bool
{
    return (
        (clang_CXXMethod_isStatic(cursor))
    );
}

// MARK: - Documentation

auto clang::documentation(CXCursor cursor) -> std::string
{
    return string(clang_Cursor_getRawCommentText(cursor));
}

// MARK: - Location Information

auto clang::filename(CXCursor cursor) -> std::string
{
    CXFile file;
    auto sourceLocation = clang_getCursorLocation(cursor);
    clang_getFileLocation(sourceLocation, &file, nullptr, nullptr, nullptr);
    return clang::string(clang_getFileName(file));
}