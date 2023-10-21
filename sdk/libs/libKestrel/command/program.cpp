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

#include <libKestrel/command/program.hpp>

// MARK: - Construction

kestrel::command::program::program(const std::vector<node::lua_reference> &nodes)
    : m_nodes(nodes)
{}

// MARK: - Node Management

auto kestrel::command::program::nodes() const -> const std::vector<node::lua_reference> &
{
    return m_nodes;
}

// MARK: - Successful Callback Management

auto kestrel::command::program::successful_execution_occurred() const -> bool
{
    return m_successful_execution_occurred;
}

auto kestrel::command::program::on_success(const luabridge::LuaRef &callback) -> void
{
    if (callback.state() && callback.isFunction()) {
        m_success_callback = callback;
    }
    else {
        m_success_callback = { nullptr };
    }
}

auto kestrel::command::program::successful_execution_did_occur() -> void
{
    m_successful_execution_occurred = true;
    if (m_success_callback.state() && m_success_callback.isFunction()) {
        m_success_callback();
    }
}