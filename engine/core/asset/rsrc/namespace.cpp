// Copyright (c) 2021 Tom Hancocks
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

#include "core/asset/rsrc/namespace.hpp"
#include <LuaBridge/Vector.h>
#include <libGraphite/rsrc/manager.hpp>

// MARK: - Lua

auto asset::resource_namespace::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginClass<asset::resource_namespace>("Namespace")
            .addConstructor<auto(*)(const std::string&)->void, asset::resource_namespace::lua_reference>()
            .addStaticFunction("global", &resource_namespace::global)
            .addProperty("exists", &resource_namespace::contains_resources)
            .addProperty("name", &resource_namespace::name)
            .addProperty("containsResources", &resource_namespace::contains_resources)
            .addFunction("firstResourceOfType", &resource_namespace::first_resource_of)
            .addFunction("isNamed", &resource_namespace::has_name)
            .addFunction("resourcesOfType", &resource_namespace::resources_of_type)
            .addFunction("resource", &resource_namespace::resource_for_id)
        .endClass();
}

// MARK: - Construction

asset::resource_namespace::resource_namespace(const std::string &name)
    : m_name(name)
{

}

auto asset::resource_namespace::global() -> asset::resource_namespace::lua_reference
{
    return asset::resource_namespace::lua_reference(new resource_namespace());
}

// MARK: - Accessors

auto asset::resource_namespace::contains_resources() const -> bool
{
    if (!m_name.has_value() || m_name->empty()) {
        return true;
    }
    const auto& name = m_name.value();

    for (const auto& file : graphite::rsrc::manager::shared_manager().files()) {
        for (const auto& type : file->types()) {
            const auto& attributes = type->attributes();
            if ((attributes.find("namespace") != attributes.end()) && (attributes.at("namespace") == name)) {
                return true;
            }
        }
    }

    // We never found reference to the namespace at this point.
    return false;
}

auto asset::resource_namespace::name() const -> std::string
{
    return m_name.has_value() ? m_name.value() : "";
}

auto asset::resource_namespace::has_name(const std::string &name) const -> bool
{
    return (this->name() == name);
}

auto asset::resource_namespace::first_resource_of(const std::string& type) const -> asset::resource::lua_reference
{
    return resources_of_type(type)[0];
}

auto asset::resource_namespace::resources_of_type(const std::string& type) const -> std::vector<asset::resource::lua_reference>
{
    std::vector<asset::resource::lua_reference> resources;
    std::map<std::string, std::string> attributes;

    if (m_name.has_value()) {
        attributes["namespace"] = m_name.value();
    }

    for (const auto& weak_type : graphite::rsrc::manager::shared_manager().get_type(type, attributes)) {
        if (const auto& strong_type = weak_type.lock()) {
            for (const auto& resource : strong_type->resources()) {
                if (m_name.has_value()) {
                    resource_namespace ns(m_name.value());
                    resources.emplace_back(asset::resource::lua_reference(new asset::resource(ns, type, resource->id(), resource->name())));
                }
                else {
                    resources.emplace_back(asset::resource::lua_reference(new asset::resource(type, resource->id(), resource->name())));
                }
            }
        }
    }

    return resources;
}

auto asset::resource_namespace::resource_for_id(const std::string &type, int64_t id) const -> asset::resource::lua_reference
{
    if (m_name.has_value()) {
        resource_namespace ns(m_name.value());
        return asset::resource::lua_reference( new asset::resource(ns, type, id, "<unnamed>") );
    }
    else {
        return asset::resource::lua_reference( new asset::resource(type, id, "<unnamed>") );
    }
}


