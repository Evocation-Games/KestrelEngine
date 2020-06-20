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

#include "scripting/state.hpp"
#include "scripting/script.hpp"

// MARK: - Construction

scripting::lua::state::state()
{

}

// MARK: - Destruction

scripting::lua::state::~state()
{
    lua_close(m_state);
}

// MARK: - Lua

auto scripting::lua::state::prepare_lua_environment() -> void
{
    m_state = luaL_newstate();
    luaL_openlibs(m_state);

    // Configure the stack and any dependancies that the primary state has.
    m_stack = std::make_shared<lua::stack>(shared_from_this());

    // Register and establish the API for the Lua Environment
}


// MARK: - Accessors

auto scripting::lua::state::stack() -> std::shared_ptr<lua::stack>
{
    return m_stack;
}

auto scripting::lua::state::internal_state() const -> lua_State*
{
    return m_state;
}

auto scripting::lua::state::error_string() const -> std::string
{
    return m_stack->pop_string();
}

// MARK: - Lua Interaction

auto scripting::lua::state::function(const std::string& name) const -> luabridge::LuaRef
{
    return function(name.c_str());
}

auto scripting::lua::state::function(const char *name) const -> luabridge::LuaRef
{
    return luabridge::getGlobal(m_state, name);
}

auto scripting::lua::state::run(const lua::script& script) -> void
{
    run(script.code());
}

auto scripting::lua::state::run(const std::string& script) -> void
{
    if (luaL_loadstring(m_state, script.c_str()) != LUA_OK) {
        throw std::runtime_error(error_string());
    }

    if (lua_pcall(m_state, 0, LUA_MULTRET, 0) != LUA_OK) {
        throw std::runtime_error(error_string());
    }
}

auto scripting::lua::state::load_script(const int64_t &id) -> lua::script
{
    return lua::script(shared_from_this(), id);
}

