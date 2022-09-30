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

#include <libTesting/testing.hpp>
#include <libKestrel/lua/support/vector.hpp>

// MARK: - Construction

TEST(lua_vector_construction)
{
    kestrel::lua::vector<std::int32_t> v({ 1, 2, 3, 4, 5 });
    test::equal(v.size(), 5);
}

// MARK: - Lua Interface

TEST(lua_vector_luaEmpty)
{
    kestrel::lua::vector<std::int32_t> v;
    test::is_true(v.empty());
}

TEST(lua_vector_luaSize)
{
    kestrel::lua::vector<std::int32_t> v({ 1, 2, 3, 4, 5 });
    test::equal(v.size(), 5);
}

TEST(lua_vector_luaAt)
{
    kestrel::lua::vector<std::int32_t> v({ 10, 11, 12, 13, 14 });
    test::equal(v.lua_at(1), 10);
    test::equal(v.lua_at(2), 11);
    test::equal(v.lua_at(3), 12);
    test::equal(v.lua_at(4), 13);
    test::equal(v.lua_at(5), 14);
}

TEST(lua_vector_at)
{
    kestrel::lua::vector<std::int32_t> v({ 10, 11, 12, 13, 14 });
    test::equal(v.at(0), 10);
    test::equal(v.at(1), 11);
    test::equal(v.at(2), 12);
    test::equal(v.at(3), 13);
    test::equal(v.at(4), 14);
}

TEST(lua_vector_emplaceBack)
{
    kestrel::lua::vector<std::int32_t> v;
    test::is_true(v.empty());
    v.emplace_back(16);
    test::is_false(v.empty());
    test::equal(v.at(0), 16);
}

TEST(lua_vector_appendVector)
{
    kestrel::lua::vector<std::int32_t> v({ 1, 2, 3 });
    kestrel::lua::vector<std::int32_t> u({ 4, 5 });
    test::equal(v.size(), 3);
    v.append(u);
    test::equal(v.size(), 5);
    test::equal(v.at(3), 4);
    test::equal(v.at(4), 5);
}