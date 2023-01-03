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

#pragma once

#include <vector>
#include <memory>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/command/node.hpp>

namespace kestrel::command
{
    struct lua_api(Kestrel.Command.Program, Available_0_9) program
    {
    public:
        has_constructable_lua_api(program);

        explicit program(const std::vector<node::lua_reference>& nodes);

        [[nodiscard]] auto nodes() const -> const std::vector<node::lua_reference>&;

        lua_getter(successfulExecutionOccurred, Available_0_9) auto successful_execution_occurred() const -> bool;
        lua_function(onSuccess, Available_0_9) auto on_success(const luabridge::LuaRef& callback) -> void;
        auto successful_execution_did_occur() -> void;

    private:
        bool m_successful_execution_occurred { false };
        std::vector<node::lua_reference> m_nodes;
        luabridge::LuaRef m_success_callback { nullptr };
    };
}
