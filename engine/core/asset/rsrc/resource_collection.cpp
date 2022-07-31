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

#include "core/asset/rsrc/resource_collection.hpp"

// MARK: - Lua

auto asset::resource_collection::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("Legacy")
            .beginNamespace("Macintosh")
                .beginClass<resource_collection>("ResourceCollection")
                    .addConstructor<auto(*)()->void, lua_reference>()
                    .addProperty("size", &resource_collection::size)
                    .addFunction("add", &resource_collection::add_resource)
                    .addFunction("has", &resource_collection::has)
                    .addFunction("get", &resource_collection::get)
                .endClass()
            .endNamespace()
        .endNamespace();
}

// MARK: - Construction

// MARK: - Accesors

auto asset::resource_collection::size() const -> std::size_t
{
    return m_resources.size();
}

// MARK: - Item Management

auto asset::resource_collection::add_resource(const resource_descriptor::lua_reference &descriptor, const luabridge::LuaRef &resource) -> void
{
    asset::resource_key key(*descriptor.get());

    // Find an existing entry and replace it...
    for (auto& it : m_resources) {
        if (it.first == key) {
            it.second = resource;
            return;
        }
    }

    // Not found, so add new entry.
    m_resources.emplace_back(std::pair(key, resource));
}

auto asset::resource_collection::get(const resource_descriptor::lua_reference &descriptor) const -> luabridge::LuaRef
{
    asset::resource_key key(*descriptor.get());

    for (auto& it : m_resources) {
        if (it.first == key) {
            return it.second;
        }
    }

    return { nullptr };
}

auto asset::resource_collection::has(const resource_descriptor::lua_reference &descriptor) const -> bool
{
    asset::resource_key key(*descriptor.get());

    for (const auto& it : m_resources) {
        if (it.first == key) {
            return true;
        }
    }

    return false;
}