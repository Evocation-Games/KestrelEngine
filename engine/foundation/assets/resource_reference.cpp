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

#include "foundation/assets/resource_reference.hpp"

// MARK: - Lua Integration

auto kestrel::assets::resource_reference::register_object() -> void
{
    luabridge::getGlobalNamespace(lua::active_state())
        .beginClass<assets::resource_reference>("Resource")
            .addStaticFunction("named", &resource_reference::using_named)
            .addStaticFunction("id", &resource_reference::using_id)
            .addStaticFunction("idWithType", &resource_reference::using_typed_id)
            .addStaticFunction("namedWithType", &resource_reference::using_typed_named)
        .endClass();
}

// MARK: - Construction

kestrel::assets::resource_reference::resource_reference(int64_t id)
    : m_id(id)
{

}

kestrel::assets::resource_reference::resource_reference(const std::string &name)
    : m_name(name)
{

}

kestrel::assets::resource_reference::resource_reference(const std::string &type, int64_t id)
    : m_type(type), m_id(id)
{

}

kestrel::assets::resource_reference::resource_reference(const std::string &type, const std::string &name)
    : m_type(type), m_name(name)
{

}

// MARK: - Accessors

auto kestrel::assets::resource_reference::id() const -> std::optional<int64_t>
{
    return m_id;
}

auto kestrel::assets::resource_reference::name() const -> std::optional<std::string>
{
    return m_name;
}

auto kestrel::assets::resource_reference::type() const -> std::optional<std::string>
{
    return m_type;
}

// MARK: - Helpers

auto kestrel::assets::resource_reference::using_id(int64_t id) -> resource_reference::lua_reference
{
    return resource_reference::lua_reference(new resource_reference(id));
}

auto kestrel::assets::resource_reference::using_named(const std::string& name) -> resource_reference::lua_reference
{
    return resource_reference::lua_reference(new resource_reference(name));
}

auto kestrel::assets::resource_reference::using_typed_id(const std::string& type, int64_t id) -> resource_reference::lua_reference
{
    return resource_reference::lua_reference(new resource_reference(type, id));
}

auto kestrel::assets::resource_reference::using_typed_named(const std::string& type, const std::string& name) -> resource_reference::lua_reference
{
    return resource_reference::lua_reference(new resource_reference(type, name));
}
