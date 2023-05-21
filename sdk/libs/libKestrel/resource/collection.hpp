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

#include <unordered_map>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/resource/descriptor.hpp>
#include <libKestrel/resource/key.hpp>

namespace kestrel::resource
{
    struct lua_api(Resource.Collection, Available_0_8) collection
    {
    public:
        has_constructable_lua_api(collection);

        lua_constructor(Avavailable_0_8) collection() = default;
        explicit collection(const std::vector<std::pair<key, luabridge::LuaRef>>& resources);

        lua_function(add, Available_0_8) auto add_resource(const descriptor::lua_reference& descriptor, const luabridge::LuaRef& resource) -> void;
        lua_function(remove, Available_0_9) auto remove_resource(const descriptor::lua_reference& descriptor) -> void;

        lua_getter(size, Available_0_8) [[nodiscard]] auto size() const -> std::size_t;
        lua_getter(randomItem, Available_0_8) [[nodiscard]] auto random() const -> luabridge::LuaRef;
        lua_function(has, Available_0_8) [[nodiscard]] auto has(const descriptor::lua_reference& descriptor) const -> bool;
        lua_function(get, Available_0_8) [[nodiscard]] auto get(const descriptor::lua_reference& descriptor) const -> luabridge::LuaRef;
        lua_function(at, Available_0_8) [[nodiscard]] auto at(std::int32_t idx) const -> luabridge::LuaRef;
        lua_function(each, Available_0_8) auto each(const luabridge::LuaRef& block) const -> void;
        lua_function(filter, Available_0_9) auto filter(const luabridge::LuaRef& block) -> void;
        lua_function(filtered, Available_0_9) auto filtered(const luabridge::LuaRef& block) -> lua_reference;
        lua_function(mapValues, Available_0_9) auto map_values(const luabridge::LuaRef& block) -> lua_reference;

    private:
        std::vector<std::pair<key, luabridge::LuaRef>> m_resources;
    };
}
