// Copyright (c) 2020 Tom Hancocks
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

extern "C" {
#   include "LuaJIT/src/lua.h"
#   include "LuaJIT/src/lauxlib.h"
#   include "LuaJIT/src/lualib.h"
#   include "LuaJIT/src/luajit_rolling.h"
}

#include <string>
#include <memory>
#include <functional>
#include "LuaBridge/LuaBridge.h"
#include "LuaBridge/RefCountedPtr.h"
#include "LuaBridge/Vector.h"

#include <libResourceCore/structure/instance.hpp>
#include <libKestrel/lua/runtime/stack.hpp>

class environment;

namespace kestrel::lua
{
    class script;

    class runtime: public std::enable_shared_from_this<runtime>
    {
    public:
        runtime();
        ~runtime();

        auto stack() -> std::shared_ptr<lua::stack>;

        auto internal_state() const -> lua_State*;
        auto error_string() const -> std::string;

        auto global_namespace() const -> luabridge::Namespace;

        auto function(const std::string& name) const -> luabridge::LuaRef;
        auto function(const char *name) const -> luabridge::LuaRef;

        auto run(const lua::script& script) -> void;
        auto run(resource_core::identifier id, const std::string& name,const std::string& script) -> void;
        auto run(resource_core::identifier id, const std::string& name, const script &script) -> void;

        auto dump() -> void;
        auto purge() -> void;

        [[nodiscard]] auto null() const -> luabridge::LuaRef;
        [[nodiscard]] auto table() const -> luabridge::LuaRef;

    private:
        auto prepare_lua_runtime() -> void;
        auto install_internal_lua_overrides() const -> void;

        static auto lua_print(lua_State *L) -> int;
        static auto lua_abort(lua_State *L) -> int;

        auto execute(resource_core::identifier id, const std::string& name) -> void;

    private:
        lua_State *m_state { nullptr };
        std::shared_ptr<lua::stack> m_stack;
    };


    template<typename T>
    static inline auto ref_isa(const luabridge::LuaRef& ref) -> bool
    {
        return (ref.state() && ref.isUserdata() && ref.template isInstance<T>());
    }

    template<typename T>
    static inline auto type_check(const luabridge::LuaRef& ref) -> bool
    {
        return luabridge::detail::Userdata::isInstance<T>(ref.state(), -1);
    }

    extern auto enroll_lua_api(const std::shared_ptr<runtime>&) -> void;

}
