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

#include "scripting/lua/lua_state.hpp"
#include "scripting/lua/lua_stack.hpp"

// MARK: - Helpers

static inline auto lua_state() -> lua_State *
{
    return kestrel::lua::state::global().internal_state();
}

// MARK: - Stack Peek Operations

auto kestrel::lua::stack::peek_string(kestrel::lua::stack::index i) -> std::string_view
{
    return std::string_view(lua_tostring(lua_state(), i));
}

// MARK: - Stack Pop Operations

auto kestrel::lua::stack::pop(int count) -> void
{
    lua_pop(lua_state(), count);
}

auto kestrel::lua::stack::pop_string() -> std::string_view
{
    auto str = peek_string();
    pop();
    return str;
}
