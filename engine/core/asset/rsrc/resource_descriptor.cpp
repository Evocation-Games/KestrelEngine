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

#include "core/asset/rsrc/resource_descriptor.hpp"
#include "core/asset/rsrc/namespace.hpp"
#include <libGraphite/rsrc/manager.hpp>

// MARK: - Lua

auto asset::resource_descriptor::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void
{
    lua->global_namespace()
        .beginNamespace("Legacy")
            .beginNamespace("Macintosh")
                .beginClass<asset::resource_descriptor>("ResourceDescriptor")
                    .addStaticFunction("identified", &resource_descriptor::identified)
                    .addStaticFunction("typed", &resource_descriptor::typed)
                    .addStaticFunction("named", &resource_descriptor::named)
                    .addStaticFunction("typedIdentified", &resource_descriptor::typed_identified)
                    .addStaticFunction("identifiedNamed", &resource_descriptor::identified_named)
                    .addStaticFunction("typedNamed", &resource_descriptor::typed_named)
                    .addStaticFunction("typedIdentifiedNamed", &resource_descriptor::typed_identified_named)
                    .addProperty("type", &resource_descriptor::type, false)
                    .addProperty("id", &resource_descriptor::id, false)
                    .addProperty("name", &resource_descriptor::name, false)
                    .addProperty("key", &resource_descriptor::key)
                    .addFunction("valid", &resource_descriptor::valid)
                    .addFunction("hasType", &resource_descriptor::has_type)
                    .addFunction("hasId", &resource_descriptor::has_id)
                    .addFunction("hasName", &resource_descriptor::has_name)
                    .addFunction("isNamespaced", &resource_descriptor::is_namespaced)
                    .addFunction("withType", &resource_descriptor::with_type)
                    .addFunction("withId", &resource_descriptor::with_id)
                    .addFunction("normalizedId", &resource_descriptor::normalized_id)
                    .addProperty("fromNamespace", &resource_descriptor::from_namespace)
                    .addProperty("ignoringNamespace", &resource_descriptor::ignoring_namespace)
                    .addProperty("ignoringType", &resource_descriptor::ignoring_type)
                    .addProperty("ignoringId", &resource_descriptor::ignoring_id)
                    .addProperty("ignoringName", &resource_descriptor::ignoring_name)
                    .addProperty("description", &resource_descriptor::description)
                    .addFunction("matchingResources", &resource_descriptor::matching_resources)
                    .addFunction("bestResource", &resource_descriptor::best_resource)
                    .addFunction("whenId", &resource_descriptor::when_id)
                    .addFunction("whenNotId", &resource_descriptor::when_not_id)
                    .addFunction("whenLessThanId", &resource_descriptor::when_less_than_id)
                    .addFunction("whenGreaterThanId", &resource_descriptor::when_greater_than_id)
                    .addFunction("whenIdInRange", &resource_descriptor::when_id_in_range)
                    .addFunction("whenIdNotInRange", &resource_descriptor::when_id_not_in_range)
                .endClass()
            .endNamespace()
        .endNamespace();
}

// MARK: - Constructors

auto asset::resource_descriptor::identified(int64_t id) -> lua_reference
{
    auto r = asset::resource_descriptor::lua_reference(new resource_descriptor());
    r->id = id;
    r->m_variant = variant::identified;
    return r;
}

auto asset::resource_descriptor::typed(const std::string& type) -> lua_reference
{
    auto r = asset::resource_descriptor::lua_reference(new resource_descriptor());
    r->type = type;
    r->m_variant = variant::typed;
    return r;
}

auto asset::resource_descriptor::named(const std::string& name) -> lua_reference
{
    auto r = asset::resource_descriptor::lua_reference(new resource_descriptor());
    r->name = name;
    r->m_variant = variant::named;
    return r;
}

auto asset::resource_descriptor::typed_identified(const std::string& type, int64_t id) -> lua_reference
{
    auto r = asset::resource_descriptor::lua_reference(new resource_descriptor());
    r->id = id;
    r->type = type;
    r->m_variant = variant::typed_identified;
    return r;
}

auto asset::resource_descriptor::identified_named(int64_t id, const std::string& name) -> lua_reference
{
    auto r = asset::resource_descriptor::lua_reference(new resource_descriptor());
    r->id = id;
    r->name = name;
    r->m_variant = variant::identified_named;
    return r;
}

auto asset::resource_descriptor::typed_named(const std::string& type, const std::string& name) -> lua_reference
{
    auto r = asset::resource_descriptor::lua_reference(new resource_descriptor());
    r->type = type;
    r->name = name;
    r->m_variant = variant::typed_named;
    return r;
}

auto asset::resource_descriptor::typed_identified_named(const std::string& type, int64_t id, const std::string& name) -> lua_reference
{
    auto r = asset::resource_descriptor::lua_reference(new resource_descriptor());
    r->id = id;
    r->name = name;
    r->type = type;
    r->m_variant = variant::typed_identified_named;
    return r;
}

auto asset::resource_descriptor::reference(const std::string &ns,
                                           const std::string &type,
                                           int64_t id,
                                           const std::string &name) -> resource_descriptor::lua_reference
{
    auto r = asset::resource_descriptor::lua_reference(new resource_descriptor());
    r->namespaces = { ns };
    r->id = id;
    r->name = name;
    r->type = type;
    r->m_variant = variant::resolved;
    return r;
}

auto asset::resource_descriptor::reference(const std::string &type,
                                           int64_t id,
                                           const std::string &name) -> resource_descriptor::lua_reference
{
    auto r = asset::resource_descriptor::lua_reference(new resource_descriptor());
    r->namespaces = { resource_namespace::universal()->primary_name() };
    r->id = id;
    r->name = name;
    r->type = type;
    r->m_variant = variant::typed_identified_named;
    return r;
}

// MARK: - Accessors

auto asset::resource_descriptor::valid() -> bool
{
    if (m_variant == variant::resolved) {
        return true;
    }

    resolve();
    return m_resolved_resources.size() > 0;
}

auto asset::resource_descriptor::description() const -> std::string
{
    std::string description;

    if (has_type()) {
        description += type + " ";
    }

    if (has_id()) {
        description += "#" + std::to_string(id) + " ";
    }

    if (has_name()) {
        description += name + " ";
    }

    if (is_namespaced()) {
        description += "ns<";
        for (const auto& ns : namespaces) {
            if (resource_namespace(ns).is_global()) {
                description += "global ";
            }
            else {
                description += ns + " ";
            }
        }
        description += ">";
    }

    return description;
}

auto asset::resource_descriptor::key() const -> std::string
{
    std::string prefix;
    std::string value{"?"};

    if (is_namespaced()) {
        // TODO: Currently only taking the first namespace.
        prefix = namespaces[0];
    }

    if (has_id()) {
        value = std::to_string(id);
    }

    return prefix.empty() ? value : prefix + "." + value;
}

auto asset::resource_descriptor::has_type() const -> bool
{
    return m_variant == variant::typed
        || m_variant == variant::typed_identified_named
        || m_variant == variant::typed_identified
        || m_variant == variant::typed_named
        || m_variant == variant::resolved;
}

auto asset::resource_descriptor::has_id() const -> bool
{
    return m_variant == variant::identified
        || m_variant == variant::identified_named
        || m_variant == variant::typed_identified
        || m_variant == variant::typed_identified_named
        || m_variant == variant::resolved;
}

auto asset::resource_descriptor::has_name() const -> bool
{
    return m_variant == variant::named
        || m_variant == variant::identified_named
        || m_variant == variant::typed_named
        || m_variant == variant::typed_identified_named
        || m_variant == variant::resolved;
}

auto asset::resource_descriptor::is_namespaced() const -> bool
{
    return !namespaces.empty();
}

auto asset::resource_descriptor::with_type(const std::string &type) const -> lua_reference
{
    lua_reference ref;

    if (has_id() && has_name()) {
        ref = resource_descriptor::typed_identified_named(type, id, name);
    }
    else if (has_id()) {
        ref = resource_descriptor::typed_identified(type, id);
    }
    else if (has_name()) {
        ref = resource_descriptor::typed_named(type, name);
    }
    else {
        ref = resource_descriptor::typed(type);
    }

    ref->namespaces = namespaces;
    return ref;
}

auto asset::resource_descriptor::with_id(int64_t id) const -> lua_reference
{
    lua_reference ref;

    if (has_type() && has_name()) {
        ref = resource_descriptor::typed_identified_named(type, id, name);
    }
    else if (has_type()) {
        ref = resource_descriptor::typed_identified(type, id);
    }
    else if (has_name()) {
        ref = resource_descriptor::identified_named(id, name);
    }
    else {
        ref = resource_descriptor::identified(id);
    }

    ref->namespaces = namespaces;
    return ref;
}


auto asset::resource_descriptor::normalized_id(int64_t index_base, int64_t first_id) const -> lua_reference
{
    return with_id((id - index_base) + first_id);
}

auto asset::resource_descriptor::from_namespace() const -> lua_reference
{
    lua_reference ref(new resource_descriptor());
    ref->namespaces = namespaces;
    return ref;
}

auto asset::resource_descriptor::ignoring_type() const -> lua_reference
{
    lua_reference ref;

    if (has_id() && has_name()) {
        ref = resource_descriptor::identified_named(id, name);
    }
    else if (has_id()) {
        ref = resource_descriptor::identified(id);
    }
    else if (has_name()) {
        ref = resource_descriptor::named(name);
    }
    else {
        ref = lua_reference(new resource_descriptor());
    }

    ref->namespaces = namespaces;
    return ref;
}

auto asset::resource_descriptor::ignoring_id() const -> lua_reference
{
    lua_reference ref;

    if (has_type() && has_name()) {
        ref = resource_descriptor::typed_named(type, name);
    }
    else if (has_type()) {
        ref = resource_descriptor::typed(type);
    }
    else if (has_name()) {
        ref = resource_descriptor::named(name);
    }
    else {
        ref = lua_reference(new resource_descriptor());
    }

    ref->namespaces = namespaces;
    return ref;
}

auto asset::resource_descriptor::ignoring_name() const -> lua_reference
{
    lua_reference ref;

    if (has_id() && has_type()) {
        ref = resource_descriptor::typed_identified(type, id);
    }
    else if (has_type()) {
        ref = resource_descriptor::typed(type);
    }
    else if (has_id()) {
        ref = resource_descriptor::identified(id);
    }
    else {
        ref = lua_reference(new resource_descriptor());
    }

    ref->namespaces = namespaces;
    return ref;
}

auto asset::resource_descriptor::ignoring_namespace() const -> lua_reference
{
    lua_reference ref;

    if (has_type() && has_id() && has_name()) {
        ref = resource_descriptor::typed_identified_named(type, id, name);
    }
    else if (has_type() && has_id()) {
        ref = resource_descriptor::typed_identified(type, id);
    }
    else if (has_type() && has_name()) {
        ref = resource_descriptor::typed_named(type, name);
    }
    else if (has_id() && has_name()) {
        ref = resource_descriptor::identified_named(id, name);
    }
    else if (has_type()) {
        ref = resource_descriptor::typed(type);
    }
    else if (has_id()) {
        ref = resource_descriptor::identified(id);
    }
    else if (has_name()) {
        ref = resource_descriptor::named(name);
    }
    else {
        ref = lua_reference(new resource_descriptor());
    }

    return ref;
}

// MARK: - Resources

auto asset::resource_descriptor::matching_resources() -> util::lua_vector<lua_reference>
{
    resolve();
    return m_resolved_resources;
}

auto asset::resource_descriptor::best_resource() -> lua_reference
{
    resolve();
    if (m_variant == variant::resolved) {

    }
    else if (m_resolved_resources.size() > 0) {
        return m_resolved_resources.at(0);
    }
    else {
        return lua_reference();
    }
}

auto asset::resource_descriptor::load() -> std::weak_ptr<graphite::rsrc::resource>
{
    // TODO: Determine the best way of doing this check.
    if (!valid()) {
        return {};
    }

    // We need to be able to assume that we have a valid resource descriptor at this point so that
    // we can just directly load it without conditions.

    if (m_resolved_resources.size() == 0) {
        std::map<std::string, std::string> attributes {};
        resource_namespace ns(namespaces.at(0));
        if (!(ns.is_universal() || ns.is_global())) {
            attributes["namespace"] = ns.primary_name();
        }
        return graphite::rsrc::manager::shared_manager().find(type, id, attributes);
    }
    else {
        auto best = m_resolved_resources.at(0);
        std::map<std::string, std::string> attributes {};
        resource_namespace ns(best->namespaces.at(0));
        if (!(ns.is_universal() || ns.is_global())) {
            attributes["namespace"] = ns.primary_name();
        }
        return graphite::rsrc::manager::shared_manager().find(best->type, best->id, attributes);
    }
}

// MARK: - Resolution

auto asset::resource_descriptor::resolve() -> void
{
    if (m_resolved) {
        return;
    }

    switch (m_variant) {
        case variant::identified: {
            resolve_identified();
            break;
        }
        case variant::typed: {
            resolve_typed();
            break;
        }
        case variant::named: {
            resolve_named();
            break;
        }
        case variant::typed_identified: {
            resolve_typed_identified();
            break;
        }
        case variant::identified_named: {
            resolve_identified_named();
            break;
        }
        case variant::typed_named: {
            resolve_typed_named();
            break;
        }
        case variant::typed_identified_named: {
            resolve_typed_identified_named();
            break;
        }
        case variant::resolved: {
            break;
        }
    }
}

auto asset::resource_descriptor::resolve_identified() -> void
{
    resource_namespace ns(namespaces);
    const auto& rm = graphite::rsrc::manager::shared_manager();

    for (const auto& f : rm.files()) {
        for (const auto& t : f->types()) {
            std::string t_namespace = resource_namespace::universal()->primary_name();
            const auto& attributes = t->attributes();
            if (attributes.find("namespace") != attributes.end()) {
                t_namespace = attributes.at("namespace");
            }

            if (!ns.is_universal() && !ns.has_name(t_namespace)) {
                continue;
            }

            for (const auto& r : t->resources()) {
                if (r->id() == id) {
                    m_resolved_resources.emplace_back(resource_descriptor::reference(t_namespace, t->code(), r->id(), r->name()));
                }
            }
        }
    }

    m_resolved = true;
}

auto asset::resource_descriptor::resolve_typed() -> void
{
    resource_namespace ns(namespaces);
    const auto& rm = graphite::rsrc::manager::shared_manager();

    for (const auto& f : rm.files()) {
        for (const auto& t : f->types()) {
            std::string t_namespace = resource_namespace::universal()->primary_name();
            const auto& attributes = t->attributes();
            if (attributes.find("namespace") != attributes.end()) {
                t_namespace = attributes.at("namespace");
            }

            if (!ns.is_universal() && !ns.has_name(t_namespace)) {
                continue;
            }

            if (t->code() != type) {
                continue;
            }

            for (const auto& r : t->resources()) {
                m_resolved_resources.emplace_back(resource_descriptor::reference(t_namespace, t->code(), r->id(), r->name()));
            }
        }
    }

    m_resolved = true;
}

auto asset::resource_descriptor::resolve_named() -> void
{
    resource_namespace ns(namespaces);
    const auto& rm = graphite::rsrc::manager::shared_manager();

    for (const auto& f : rm.files()) {
        for (const auto& t : f->types()) {
            std::string t_namespace = resource_namespace::universal()->primary_name();
            const auto& attributes = t->attributes();
            if (attributes.find("namespace") != attributes.end()) {
                t_namespace = attributes.at("namespace");
            }

            if (!ns.is_universal() && !ns.has_name(t_namespace)) {
                continue;
            }

            for (const auto& r : t->resources()) {
                if (r->name() == name) {
                    m_resolved_resources.emplace_back(resource_descriptor::reference(t_namespace, t->code(), r->id(), r->name()));
                }
            }
        }
    }

    m_resolved = true;
}

auto asset::resource_descriptor::resolve_typed_identified() -> void
{
    resource_namespace ns(namespaces);
    const auto& rm = graphite::rsrc::manager::shared_manager();

    for (const auto& f : rm.files()) {
        for (const auto& t : f->types()) {
            std::string t_namespace = resource_namespace::global()->primary_name();
            const auto& attributes = t->attributes();
            if (attributes.find("namespace") != attributes.end()) {
                t_namespace = attributes.at("namespace");
            }

            if (!ns.is_universal() && !ns.has_name(t_namespace)) {
                continue;
            }

            if (t->code() != type) {
                continue;
            }

            for (const auto& r : t->resources()) {
                if (r->id() == id) {
                    m_resolved_resources.emplace_back(resource_descriptor::reference(t_namespace, t->code(), r->id(), r->name()));
                }
            }
        }
    }

    m_resolved = true;
}

auto asset::resource_descriptor::resolve_identified_named() -> void
{
    resource_namespace ns(namespaces);
    const auto& rm = graphite::rsrc::manager::shared_manager();

    for (const auto& f : rm.files()) {
        for (const auto& t : f->types()) {
            std::string t_namespace = resource_namespace::universal()->primary_name();
            const auto& attributes = t->attributes();
            if (attributes.find("namespace") != attributes.end()) {
                t_namespace = attributes.at("namespace");
            }

            if (!ns.is_universal() && !ns.has_name(t_namespace)) {
                continue;
            }

            for (const auto& r : t->resources()) {
                if (r->id() == id && r->name() == name) {
                    m_resolved_resources.emplace_back(resource_descriptor::reference(t_namespace, t->code(), r->id(), r->name()));
                }
            }
        }
    }

    m_resolved = true;
}

auto asset::resource_descriptor::resolve_typed_named() -> void
{
    resource_namespace ns(namespaces);
    const auto& rm = graphite::rsrc::manager::shared_manager();

    for (const auto& f : rm.files()) {
        for (const auto& t : f->types()) {
            std::string t_namespace = resource_namespace::universal()->primary_name();
            const auto& attributes = t->attributes();
            if (attributes.find("namespace") != attributes.end()) {
                t_namespace = attributes.at("namespace");
            }

            if (!ns.is_universal() && !ns.has_name(t_namespace)) {
                continue;
            }

            if (t->code() != type) {
                continue;
            }

            for (const auto& r : t->resources()) {
                if (r->name() == name) {
                    m_resolved_resources.emplace_back(resource_descriptor::reference(t_namespace, t->code(), r->id(), r->name()));
                }
            }
        }
    }

    m_resolved = true;
}

auto asset::resource_descriptor::resolve_typed_identified_named() -> void
{
    resource_namespace ns(namespaces);
    const auto& rm = graphite::rsrc::manager::shared_manager();

    for (const auto& f : rm.files()) {
        for (const auto& t : f->types()) {
            std::string t_namespace = resource_namespace::universal()->primary_name();
            const auto& attributes = t->attributes();
            if (attributes.find("namespace") != attributes.end()) {
                t_namespace = attributes.at("namespace");
            }

            if (!ns.is_universal() && !ns.has_name(t_namespace)) {
                continue;
            }

            if (t->code() != type) {
                continue;
            }

            for (const auto& r : t->resources()) {
                if (r->id() == id && r->name() == name) {
                    m_resolved_resources.emplace_back(resource_descriptor::reference(t_namespace, t->code(), r->id(), r->name()));
                }
            }
        }
    }

    m_resolved = true;
}

// MARK: - Conditions / When

auto asset::resource_descriptor::when_id(int64_t id) const -> bool
{
    return (this->id == id);
}

auto asset::resource_descriptor::when_not_id(int64_t id) const -> bool
{
    return (this->id != id);
}

auto asset::resource_descriptor::when_less_than_id(int64_t id) const -> bool
{
    return (this->id < id);
}

auto asset::resource_descriptor::when_greater_than_id(int64_t id) const -> bool
{
    return (this->id > id);
}

auto asset::resource_descriptor::when_id_in_range(int64_t lower_id, int64_t upper_id) const -> bool
{
    return (this->id >= lower_id && this->id <= upper_id);
}

auto asset::resource_descriptor::when_id_not_in_range(int64_t lower_id, int64_t upper_id) const -> bool
{
    return (this->id < lower_id || this->id > upper_id);
}

