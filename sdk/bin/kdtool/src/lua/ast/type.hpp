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
#include <clang-c/Index.h>

namespace kdtool::lua_api::ast
{
    /**
     * Represents the data type of a variable or return type.
     * TODO: Implement this in a more comprehensive way
     */
    struct type
    {
        type() = default;
        explicit type(CXCursor cursor);
        explicit type(const std::string& name);

        static auto void_type() -> type;
        static auto int8() -> type;
        static auto int16() -> type;
        static auto int32() -> type;
        static auto int64() -> type;
        static auto uint8() -> type;
        static auto uint16() -> type;
        static auto uint32() -> type;
        static auto uint64() -> type;
        static auto string_type() -> type;
        static auto shared_ptr(const std::string& name) -> type;

        [[nodiscard]] auto string() const -> std::string;

    private:
        std::string m_name { "int" };
    };
}
