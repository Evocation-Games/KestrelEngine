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

#include "core/asset/rsrc/resource.hpp"
#include "core/asset/rsrc/namespace.hpp"
#include <algorithm>
#include <LuaBridge/LuaBridge.h>
#include <LuaBridge/Vector.h>
#include <libGraphite/rsrc/manager.hpp>

// MARK: - Lua

auto asset::resource::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginNamespace("Legacy")
            .beginNamespace("Macintosh")
                .beginClass<asset::resource>("Resource")
                    .addProperty("exists", &resource::exists)
                    .addStaticFunction("allWithType", &resource::all_of_type)
                    .addStaticFunction("id", &resource::using_id)
                    .addStaticFunction("idWithType", &resource::using_typed_id)
                    .addFunction("describe", &resource::describe)
                .endClass()
            .endNamespace()
        .endNamespace();
}

// MARK: - Construction

asset::resource::resource(int64_t id)
    : m_id(id)
{

}

asset::resource::resource(const std::string& type, int64_t id, const std::string& name)
    : m_type(type), m_id(id), m_name(name)
{

}

asset::resource::resource(const resource_namespace& ns, int64_t id, const std::string& name)
    : m_namespace_name(ns.name()), m_id(id), m_name(name)
{

}

asset::resource::resource(const resource_namespace& ns, const std::string& type, int64_t id, const std::string& name)
        : m_namespace_name(ns.name()), m_type(type), m_id(id), m_name(name)
{

}

// MARK: - Helpers

auto asset::resource::using_id(int64_t id) -> resource::lua_reference
{
    return resource::lua_reference(new resource(id));
}

auto asset::resource::using_typed_id(const std::string& type, int64_t id) -> resource::lua_reference
{
    return resource::lua_reference(new resource(*resource_namespace::global().get(), type, id, "<unnamed>"));
}

// MARK: - Accessors

auto asset::resource::ns() const -> std::optional<resource_namespace>
{
    return m_namespace_name.has_value() ? resource_namespace(m_namespace_name.value()) : resource_namespace();
}

auto asset::resource::type() const -> std::optional<std::string>
{
    return m_type;
}

auto asset::resource::id() const -> std::optional<int64_t>
{
    return m_id;
}

auto asset::resource::name() const -> std::optional<std::string>
{
    return m_name;
}


// MARK: - Resource Access

auto asset::resource::exists() const -> bool
{
    std::map<std::string, std::string> attributes;
    if (m_namespace_name.has_value() && !m_namespace_name.value().empty()) {
        attributes["namespace"] = m_namespace_name.value();
    }
    return !graphite::rsrc::manager::shared_manager().find(m_type.value(), m_id.value(), attributes).expired();
}

auto asset::resource::all_of_type(const std::string &type) -> util::lua_vector<asset::resource::lua_reference>
{
    std::map<std::tuple<std::string, int64_t>, resource::lua_reference> resources;
    for (const auto& f : graphite::rsrc::manager::shared_manager().files()) {
        for (const auto& t : f->types()) {

            std::string ns;
            const auto& attributes = t->attributes();
            if (attributes.find("namespace") != attributes.end()) {
                ns = attributes.at("namespace");
            }

            if (t->code() == type) {
                for (const auto& r : t->resources()) {
                    resources[std::make_tuple(ns, r->id())] = resource::lua_reference(new resource(resource_namespace(ns), type, r->id(), r->name()));
                }
            }
        }
    }

    util::lua_vector<resource::lua_reference> found_resources;
    found_resources.reserve(resources.size());
    for (const auto& r : resources) {
        found_resources.emplace_back(r.second);
    }
    found_resources.sort([&] (const resource::lua_reference& a, const resource::lua_reference& b) -> bool {
        return (a->id().value() < b->id().value());
    });
    return found_resources;
}

auto asset::resource::describe() const -> void
{
    std::cout << "resource: #" << (id().has_value() ? id().value() : -1) << " - "
              << (m_name.has_value() ? m_name.value() : "<no name>")
              << (m_namespace_name.has_value() && !m_namespace_name->empty() ? ", ns<" + m_namespace_name.value() + ">" : ", global-ns")
              << std::endl;
}
