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
#include "lua/lua.h"
#include "lua/lualib.h"
#include "lauxlib.h"
}

#include <string>
#include <memory>
#include <functional>
#include "LuaBridge/LuaBridge.h"
#include "LuaBridge/RefCountedPtr.h"
#include "LuaBridge/Vector.h"
#include "scripting/stack.hpp"

class environment;

namespace scripting::lua
{
    class script;

    class state: public std::enable_shared_from_this<lua::state>
    {
    private:
        lua_State *m_state { nullptr };
        std::shared_ptr<lua::stack> m_stack;

    public:
        state() = default;
        ~state();

        auto prepare_lua_environment(const std::shared_ptr<environment>& env) -> void;

        auto stack() -> std::shared_ptr<lua::stack>;

        auto internal_state() const -> lua_State*;
        auto error_string() const -> std::string;

        auto global_namespace() const -> luabridge::Namespace;

        auto function(const std::string& name) const -> luabridge::LuaRef;
        auto function(const char *name) const -> luabridge::LuaRef;

        auto run(const lua::script& script) -> void;
        auto run(const int64_t& id, const std::string& name,const std::string& script) -> void;
        auto run(const int64_t& id, const std::string& name, const lua::script &script) -> void;

        auto dump_state() -> void;
    };

    class object
    {
    public:
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void {};
    };

    template<typename T>
    static inline auto ref_isa(const luabridge::LuaRef& ref) -> bool
    {
        return (ref.state() && ref.isUserdata() && ref.template isInstance<T>());
    }

}
