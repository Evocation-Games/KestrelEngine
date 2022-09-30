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

#include <libKestrel/exceptions/lua_runtime_exception.hpp>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/runtime/stack.hpp>
#include <libKestrel/lua/script.hpp>
#include <libGraphite/rsrc/resource.hpp>

// MARK: - Construction

kestrel::lua::runtime::runtime()
    : m_stack(std::make_shared<lua::stack>(shared_from_this()))
{
    prepare_lua_runtime();
}

// MARK: - Destruction

kestrel::lua::runtime::~runtime()
{
    lua_close(m_state);
}

// MARK: - Custom Print

// MARK: - Lua

auto kestrel::lua::runtime::prepare_lua_runtime() -> void
{
    m_state = luaL_newstate();
    luaL_openlibs(m_state);

}

// MARK: - Accessors

auto kestrel::lua::runtime::stack() -> std::shared_ptr<lua::stack>
{
    return m_stack;
}

auto kestrel::lua::runtime::internal_state() const -> lua_State*
{
    return m_state;
}

auto kestrel::lua::runtime::error_string() const -> std::string
{
    return m_stack->pop_string();
}

// MARK: - Lua Interaction

auto kestrel::lua::runtime::function(const std::string& name) const -> luabridge::LuaRef
{
    return function(name.c_str());
}

auto kestrel::lua::runtime::function(const char *name) const -> luabridge::LuaRef
{
    return luabridge::getGlobal(m_state, name);
}

auto kestrel::lua::runtime::run(const lua::script& script) -> void
{
    run(script.id(), script.name(), script.code());
}

auto kestrel::lua::runtime::run(graphite::rsrc::resource::identifier id, const std::string& name, const script& script) -> void
{
}

auto kestrel::lua::runtime::run(graphite::rsrc::resource::identifier id, const std::string& name, const std::string& script) -> void
{
    if (luaL_loadstring(m_state, script.c_str()) != LUA_OK) {
        throw lua_runtime_exception(error_string());
    }

    if (lua_pcall(m_state, 0, LUA_MULTRET, 0) != LUA_OK) {
        std::string stack_string;
        auto error_string = this->error_string();
        
        lua_Debug info;
        int level = 0;
        while (lua_getstack(m_state, level, &info)) {
            lua_getinfo(m_state, "nSl", &info);

            stack_string += "  [" + std::to_string(level) + "] ";
            stack_string += std::string(info.short_src) + ":L" + std::to_string(info.currentline) + " -- ";
            stack_string += std::string(info.name ? info.name : "<unknown>");
            stack_string += " [" + std::string(info.what) + "]\n";

            ++level;
        }

        throw lua_runtime_exception(error_string + "\nSTACK:\n" + stack_string);
    }
}

// MARK: - Namespaces

auto kestrel::lua::runtime::global_namespace() const -> luabridge::Namespace
{
    return luabridge::getGlobalNamespace(m_state);
}
