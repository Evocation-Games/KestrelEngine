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

#include <string>
#include <memory>
#include <variant>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/lua/support/vector.hpp>
#include <libKestrel/command/argument.hpp>
#include <libGraphite/util/hashing.hpp>

namespace kestrel::command
{
    struct lua_api(Kestrel.Command.Property, Available_0_9) property
    {
    public:
        has_constructable_lua_api(property);

        lua_constructor(Available_0_9) explicit property(const std::string& name, const std::int32_t expected_arguments);

        lua_function(setReturnedPropertyForArgument, Available_0_9) auto set_property_for_argument(const std::string& argument, const lua_reference& property) -> void;
        lua_function(setReturnedValueForArgument, Available_0_9) auto set_value_for_argument(const std::string& argument, const luabridge::LuaRef& value) -> void;
        lua_function(setReturnedValue, Available_0_9) auto set_value(const luabridge::LuaRef& value) -> void;


        lua_getter(name, Available_0_9) [[nodiscard]] auto name() const -> std::string;
        [[nodiscard]] auto name_hash() const -> graphite::hashing::value64;

        lua_getter(expectedArgumentCount, Available_0_9) [[nodiscard]] auto expected_argument_count() const -> std::int32_t;

        [[nodiscard]] auto boolean_value() const -> bool;
        [[nodiscard]] auto boolean_value(graphite::hashing::value64 name_hash) const -> bool;
        [[nodiscard]] auto property_value(graphite::hashing::value64 name_hash) const -> lua_reference;

    private:
        enum variant_index : std::int32_t { property_index = 0, boolean_index };

        bool m_raw_value { false };
        std::string m_name;
        graphite::hashing::value64 m_name_hash;
        std::int32_t m_expected_arguments;
        std::unordered_map<graphite::hashing::value64, std::variant<lua_reference, bool>> m_properties;

        static auto hash(const std::string& name) -> graphite::hashing::value64;
    };
}