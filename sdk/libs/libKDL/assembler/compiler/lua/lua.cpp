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

#include <iostream>
#include <stdexcept>
#include <cstring>
#include <libKDL/assembler/compiler/lua/lua.hpp>
#include <libGraphite/data/writer.hpp>
#include <libKDL/exception/lua_exception.hpp>

struct source
{
    std::string code;
    bool accessed { false };
};

struct compilation_result
{
    graphite::data::writer buffer;
};

// MARK: - Compilation

static auto lua_reader(lua_State *L, void *data, std::size_t *size) -> const char *
{
    auto script = reinterpret_cast<struct source *>(data);
    if (script->accessed) {
        *size = 0;
        return nullptr;
    }
    script->accessed = true;
    *size = script->code.size();
    return script->code.c_str();
}

static auto lua_writer(lua_State *L, const void *p, std::size_t sz, void *ud) -> int
{
    auto result = reinterpret_cast<struct compilation_result *>(ud);
    auto bytes = reinterpret_cast<const std::uint8_t *>(p);
    std::vector<std::uint8_t> v(bytes, bytes + sz);
    result->buffer.write_bytes(v);
    return LUA_OK;
}

static auto ldump_writer(lua_State *L, const void *p, std::size_t sz, void *ud) -> int
{
    (void)L;
    luaL_addlstring((luaL_Buffer*)ud, (const char *)p, sz);
    return LUA_OK;
}

static auto strmemdup(const char *s, std::size_t n) -> char *
{
    char *res;
    if (s == nullptr) {
        return nullptr;
    }
    res = (char *)calloc(n + 1, 1);
    memcpy(res, s, n);
    res[n] = '\0';
    return res;
}

auto kdl::assembler::compiler::lua::compile(const std::string &source, const std::string& path) -> graphite::data::block
{
    auto L = luaL_newstate();
    lua_gc(L, LUA_GCSTOP, 0);
    luaL_openlibs(L);

    struct source src;
    src.code = source;
    if (lua_load(L, lua_reader, &src, "") != LUA_OK) {
        auto reason = lua_tostring(L, -1);
        std::cerr << std::endl << "LUA_EXCEPTION @ " << path << std::endl << reason << std::endl;
        throw lua_exception(reason, path);
    }

    struct compilation_result result;
    if (lua_dump(L, lua_writer, &result) != LUA_OK) {
        auto reason = lua_tostring(L, -1);
        std::cerr << std::endl << "LUA_EXCEPTION @ " << path << std::endl << reason << std::endl;
        throw lua_exception(reason, path);
    }

    lua_close(L);
    return { *result.buffer.data() };
}
