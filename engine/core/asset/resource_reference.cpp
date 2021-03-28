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

#include "core/asset/resource_reference.hpp"
#include <libGraphite/rsrc/manager.hpp>

// MARK: - Lua Integration

auto asset::resource_reference::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginClass<asset::resource_reference>("Resource")
            .addStaticFunction("named", &resource_reference::using_named)
            .addStaticFunction("id", &resource_reference::using_id)
            .addStaticFunction("idWithType", &resource_reference::using_typed_id)
            .addStaticFunction("namedWithType", &resource_reference::using_typed_named)
            .addStaticFunction("find", &resource_reference::find)
            .addStaticFunction("allWithType", &resource_reference::all_of_type)
            .addProperty("exists", &resource_reference::exists)
        .endClass();
}

// MARK: - Construction

asset::resource_reference::resource_reference(int64_t id)
        : m_id(id)
{

}

asset::resource_reference::resource_reference(const std::string &name)
        : m_name(name)
{

}

asset::resource_reference::resource_reference(const std::string &type, int64_t id)
        : m_type(type), m_id(id)
{

}

asset::resource_reference::resource_reference(const std::string &type, const std::string &name)
        : m_type(type), m_name(name)
{

}

// MARK: - Accessors

auto asset::resource_reference::id() const -> std::optional<int64_t>
{
    return m_id;
}

auto asset::resource_reference::name() const -> std::optional<std::string>
{
    return m_name;
}

auto asset::resource_reference::type() const -> std::optional<std::string>
{
    return m_type;
}

// MARK: - Helpers

auto asset::resource_reference::find(const std::string& type, int64_t id) -> bool
{
    return !graphite::rsrc::manager::shared_manager().find(type, id).expired();
}

auto asset::resource_reference::using_id(int64_t id) -> resource_reference::lua_reference
{
    return resource_reference::lua_reference(new resource_reference(id));
}

auto asset::resource_reference::using_named(const std::string& name) -> resource_reference::lua_reference
{
    return resource_reference::lua_reference(new resource_reference(name));
}

auto asset::resource_reference::using_typed_id(const std::string& type, int64_t id) -> resource_reference::lua_reference
{
    return resource_reference::lua_reference(new resource_reference(type, id));
}

auto asset::resource_reference::using_typed_named(const std::string& type, const std::string& name) -> resource_reference::lua_reference
{
    return resource_reference::lua_reference(new resource_reference(type, name));
}

auto asset::resource_reference::all_of_type(const std::string &type) -> util::lua_vector<resource_reference::lua_reference>
{
    std::map<int64_t, resource_reference::lua_reference> resources;
    for (const auto& t : graphite::rsrc::manager::shared_manager().get_type(type)) {
        for (const auto& r : t.lock()->resources()) {
            resources[r->id()] = resource_reference::lua_reference(new resource_reference(type, r->id()));
        }
    }

    util::lua_vector<resource_reference::lua_reference> found_resources;
    found_resources.reserve(resources.size());
    for (const auto& r : resources) {
        found_resources.emplace_back(r.second);
    }
    found_resources.sort([&] (const resource_reference::lua_reference& a, const resource_reference::lua_reference& b) -> bool {
        return (a->id().value() < b->id().value());
    });

    return found_resources;
}


auto asset::resource_reference::exists() const -> bool
{
    return !graphite::rsrc::manager::shared_manager().find(m_type.value(), m_id.value()).expired();
}