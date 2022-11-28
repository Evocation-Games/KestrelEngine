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

#include "lua/ast/type.hpp"
#include "analyzer/clang.hpp"

// MARK: - Construction

luatool::lua_api::ast::type::type(CXCursor cursor)
    : m_name(clang::spelling(clang_getCursorType(cursor)))
{}

luatool::lua_api::ast::type::type(const std::string &name)
    : m_name(name)
{}

auto luatool::lua_api::ast::type::void_type() -> type
{
    return type("void");
}

auto luatool::lua_api::ast::type::int8() -> type
{
    return type("std::int8_t");
}

auto luatool::lua_api::ast::type::int16() -> type
{
    return type("std::int16_t");
}

auto luatool::lua_api::ast::type::int32() -> type
{
    return type("std::int32_t");
}

auto luatool::lua_api::ast::type::int64() -> type
{
    return type("std::int64_t");
}

auto luatool::lua_api::ast::type::uint8() -> type
{
    return type("std::uint8_t");
}

auto luatool::lua_api::ast::type::uint16() -> type
{
    return type("std::uint16_t");
}

auto luatool::lua_api::ast::type::uint32() -> type
{
    return type("std::uint32_t");
}

auto luatool::lua_api::ast::type::uint64() -> type
{
    return type("std::uint64_t");
}

auto luatool::lua_api::ast::type::string_type() -> type
{
    return type("std::string");
}

auto luatool::lua_api::ast::type::shared_ptr(const std::string &name) -> type
{
    return type("const std::shared_ptr<" + name + ">&");
}

// MARK: - Querying

auto luatool::lua_api::ast::type::string() const -> std::string
{
    return m_name;
}