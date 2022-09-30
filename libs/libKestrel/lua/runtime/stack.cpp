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

// MARK: - Construction

kestrel::lua::stack::stack(std::weak_ptr<runtime> runtime)
    : m_runtime(runtime)
{
}

// MARK: - Stack

auto kestrel::lua::stack::pop(std::size_t count) -> void
{
    auto runtime = m_runtime.lock();
    if (!runtime) {
        throw lua_runtime_exception("Attempted to pop from stack without a parent state in place.");
    }
    lua_pop(runtime->internal_state(), count);
}

auto kestrel::lua::stack::peek_string(index i) -> std::string
{
    auto runtime = m_runtime.lock();
    if (!runtime) {
        throw lua_runtime_exception("Attempted to peek from stack without a parent state in place.");
    }
    return { lua_tostring(runtime->internal_state(), i) };
}

auto kestrel::lua::stack::pop_string() -> std::string
{
    auto runtime = m_runtime.lock();
    if (!runtime) {
        throw lua_runtime_exception("Attempted to pop_string from stack without a parent state in place.");
    }

    auto str = peek_string();
    pop();
    return str;
}

