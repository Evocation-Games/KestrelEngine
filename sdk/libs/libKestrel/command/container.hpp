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

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/command/program.hpp>
#include <libKestrel/command/property.hpp>
#include <libGraphite/util/hashing.hpp>

namespace kestrel::command
{
    class lua_api(Kestrel.Command.Container, Available_0_9) container
    {
    public:
        has_constructable_lua_api(container);

        lua_constructor(Available_0_9) container() = default;

        lua_function(execute, Available_0_9) auto execute() -> void;

        lua_function(addProgram, Available_0_9) auto add_program(const command::program::lua_reference& program) -> void;

        lua_function(setProperty, Available_0_9) auto set_property(const property::lua_reference& property) -> void;
        lua_function(createProperty, Available_0_9) auto create_property(const std::string& name, const std::int32_t expected_arguments) -> property::lua_reference;

    private:
        std::vector<command::program::lua_reference> m_programs;
        std::unordered_map<graphite::hashing::value64, property::lua_reference> m_properties;

        auto execute_program(const program::lua_reference& program) -> void;
        auto execute_node(const node::lua_reference& node, const property::lua_reference& property = { nullptr }) -> bool;
    };
}