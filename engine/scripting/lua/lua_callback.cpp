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

#include "scripting/lua/lua_callback.hpp"

// MARK: - Lua Integration

auto kestrel::lua::callback::register_object() -> void
{
    luabridge::getGlobalNamespace(lua::active_state())
        .beginClass<lua::callback>("Callback")
            .addStaticFunction("namedFunction", &callback::named_function)
            .addStaticFunction("script", &callback::script_resource)
        .endClass();
}

// MARK: - Constructor

kestrel::lua::callback::callback()
{

}

kestrel::lua::callback::callback(std::string name)
    : m_name(name), m_type(function)
{

}

kestrel::lua::callback::callback(int64_t id)
    : m_id(id), m_type(script)
{

}

auto kestrel::lua::callback::named_function(std::string name) -> lua_callback
{
    return lua_callback(new callback(name));
}


auto kestrel::lua::callback::script_resource(int64_t id) -> lua_callback
{
    return lua_callback(new callback(id));
}

// MARK: - Calling

auto kestrel::lua::callback::call() -> void
{
    switch (m_type) {
        case function: {
            luabridge::getGlobal(lua::active_state(), m_name.c_str())();
            return;
        }
        case script: {
            return;
        }
        default: {
            return;
        }
    }
}

