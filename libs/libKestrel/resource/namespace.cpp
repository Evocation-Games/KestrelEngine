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

#include <libKestrel/resource/descriptor.hpp>
#include <libKestrel/resource/namespace.hpp>
#include <libGraphite/rsrc/manager.hpp>

// MARK: - Lua

contruct_lua_api(kestrel::resource::resource_namespace)
{
    runtime->global_namespace()
        .beginClass<resource_namespace>("Namespace")
            .addConstructor<auto(*)(const std::string&)->void, lua_reference>()
            .addStaticFunction("global", &resource_namespace::global)
            .addStaticFunction("any", &resource_namespace::universal)
            .addProperty("hasAssociatedResources", &resource_namespace::contains_resources)
            .addProperty("primaryName", &resource_namespace::primary_name)
            .addProperty("containsResources", &resource_namespace::contains_resources)
            .addFunction("firstResourceOfType", &resource_namespace::first_resource_of)
            .addFunction("hasName", &resource_namespace::has_name)
            .addFunction("resourcesOfType", &resource_namespace::resources_of_type)
            .addFunction("resource", &resource_namespace::resource_for_id)
            .addFunction("each", &resource_namespace::lua_each_name)
            .addFunction("identifiedResource", &resource_namespace::identified_resource)
            .addFunction("typedResource", &resource_namespace::typed_resource)
            .addFunction("namedResource", &resource_namespace::named_resource)
            .addFunction("typedIdentifiedResource", &resource_namespace::typed_identified_resource)
            .addFunction("identifiedNamedResource", &resource_namespace::identified_named_resource)
            .addFunction("typedNamedResource", &resource_namespace::typed_named_resource)
            .addFunction("typedIdentifiedNamedResource", &resource_namespace::typed_identified_named_resource)
//            .addFunction("scene", &resource_namespace::scene)
//            .addFunction("pushScene", &resource_namespace::push_scene)
        .endClass();
}

// MARK: - Construction

kestrel::resource::resource_namespace::resource_namespace(const std::string &name)
    : m_names({ name })
{
}

kestrel::resource::resource_namespace::resource_namespace(const std::vector<std::string>& names)
    : m_names(names.empty() ? std::vector<std::string>({ universal_name }) : names)
{
}

auto kestrel::resource::resource_namespace::global() -> lua_reference
{
    return { new resource_namespace(global_name) };
}

auto kestrel::resource::resource_namespace::universal() -> lua_reference
{
    return { new resource_namespace(universal_name) };
}

// MARK: - Accessors

auto kestrel::resource::resource_namespace::primary_name() const -> std::string
{
    if (is_universal()) {
        return universal_name;
    }
    return m_names.empty() ? global_name : m_names[0];
}

auto kestrel::resource::resource_namespace::count() const -> std::size_t
{
    return m_names.size();
}

auto kestrel::resource::resource_namespace::text() const -> std::string
{
    std::string s;
    for (const auto& n : m_names) {
        s.append(" " + n);
    }
    s.erase(0, 1);
    return s;
}

auto kestrel::resource::resource_namespace::contains_resources() const -> bool
{
    // This may or may not be the correct way of doing this. Theoretically universal should always have at least 1
    // resource.
    if (has_name(universal_name)) {
        return true;
    }

    for (const auto& file : graphite::rsrc::manager::shared_manager().file_references()) {
        for (const auto& type_hash : file->types()) {
            const auto& type = file->type(type_hash);
            const auto& attributes = type->attributes();
            const auto& it = attributes.find(graphite::rsrc::attribute::hash_for_name(attribute_name));

            if (has_name(it->second.string_value())) {
                return true;
            }
            else if (has_name(global_name)) {
                // No namespace is on the resource, and thus it is in the global namespace.
                // Check for global.
                return true;
            }
        }
    }

    // We never found reference to the namespace at this point.
    return false;
}

auto kestrel::resource::resource_namespace::has_name(const std::string &name) const -> bool
{
    return (std::find(m_names.begin(), m_names.end(), name) != m_names.end());
}

auto kestrel::resource::resource_namespace::is_global() const -> bool
{
    return has_name(global_name);
}

auto kestrel::resource::resource_namespace::is_universal() const -> bool
{
    return has_name(universal_name);
}

// MARK: - Management

auto kestrel::resource::resource_namespace::lua_add_namespace(const lua_reference &ns) -> void
{
    add_namespace(*ns.get());
}

auto kestrel::resource::resource_namespace::add_namespace(const resource_namespace &ns) -> void
{
    m_names.insert(m_names.end(), ns.m_names.begin(), ns.m_names.end());
}

// MARK: - Look Up

auto kestrel::resource::resource_namespace::first_resource_of(const std::string& type) const -> descriptor::lua_reference
{
    return resources_of_type(type).at(0);
}

auto kestrel::resource::resource_namespace::resources_of_type(const std::string& type) const -> lua::vector<descriptor::lua_reference>
{
    return typed_resource(type)->matching_resources();
}

auto kestrel::resource::resource_namespace::resource_for_id(const std::string &type, int64_t id) const -> descriptor::lua_reference
{
    return typed_identified_resource(type, id)->best_resource();
}

// MARK: - Functional

auto kestrel::resource::resource_namespace::lua_each_name(luabridge::LuaRef body) const -> void
{
    for (const auto& name : m_names) {
        body(lua_reference(new resource_namespace(name)));
    }
}

auto kestrel::resource::resource_namespace::each_name(const std::function<auto(const resource_namespace&)->void> &body) const -> void
{
    for (const auto& name : m_names) {
        body(resource_namespace(name));
    }
}

// MARK: - Resources

auto kestrel::resource::resource_namespace::file_constraint(const graphite::rsrc::file *file) -> descriptor::lua_reference
{
    auto r = descriptor::file_constrained(file);
    each_name([&] (const resource_namespace& ns) {
        r->namespaces.emplace_back(ns.primary_name());
    });
    return r;
}

auto kestrel::resource::resource_namespace::identified_resource(int64_t id) const -> descriptor::lua_reference
{
    auto r = descriptor::identified(id);
    each_name([&] (const resource_namespace& ns) {
        r->namespaces.emplace_back(ns.primary_name());
    });
    return r;
}

auto kestrel::resource::resource_namespace::typed_resource(const std::string &type) const -> descriptor::lua_reference
{
    auto r = descriptor::typed(type);
    each_name([&] (const resource_namespace& ns) {
        r->namespaces.emplace_back(ns.primary_name());
    });
    return r;
}

auto kestrel::resource::resource_namespace::named_resource(const std::string &name) const -> descriptor::lua_reference
{
    auto r = descriptor::named(name);
    each_name([&] (const resource_namespace& ns) {
        r->namespaces.emplace_back(ns.primary_name());
    });
    return r;
}

auto kestrel::resource::resource_namespace::typed_identified_resource(const std::string &type, int64_t id) const -> descriptor::lua_reference
{
    auto r = descriptor::typed_identified(type, id);
    each_name([&] (const resource_namespace& ns) {
        r->namespaces.emplace_back(ns.primary_name());
    });
    return r;
}

auto kestrel::resource::resource_namespace::identified_named_resource(int64_t id, const std::string &name) const -> descriptor::lua_reference
{
    auto r = descriptor::identified_named(id, name);
    each_name([&] (const resource_namespace& ns) {
        r->namespaces.emplace_back(ns.primary_name());
    });
    return r;
}

auto kestrel::resource::resource_namespace::typed_named_resource(const std::string &type, const std::string &name) const -> descriptor::lua_reference
{
    auto r = descriptor::typed_named(type, name);
    each_name([&] (const resource_namespace& ns) {
        r->namespaces.emplace_back(ns.primary_name());
    });
    return r;
}

auto kestrel::resource::resource_namespace::typed_identified_named_resource(const std::string &type, int64_t id, const std::string &name) const -> descriptor::lua_reference
{
    auto r = descriptor::typed_identified_named(type, id, name);
    each_name([&] (const resource_namespace& ns) {
        r->namespaces.emplace_back(ns.primary_name());
    });
    return r;
}