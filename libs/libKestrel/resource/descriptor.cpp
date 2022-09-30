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
#include <libGraphite/util/hashing.hpp>
#include <libGraphite/rsrc/manager.hpp>

// MARK: - Lua API

contruct_lua_api(kestrel::resource::descriptor)
{
    runtime->global_namespace()
        .beginClass<descriptor>("ResourceDescriptor")
            .addStaticFunction("identified", &descriptor::identified)
            .addStaticFunction("typed", &descriptor::typed)
            .addStaticFunction("named", &descriptor::named)
            .addStaticFunction("typedIdentified", &descriptor::typed_identified)
            .addStaticFunction("identifiedNamed", &descriptor::identified_named)
            .addStaticFunction("typedNamed", &descriptor::typed_named)
            .addStaticFunction("typedIdentifiedNamed", &descriptor::typed_identified_named)
            .addProperty("type", &descriptor::type, false)
            .addProperty("id", &descriptor::id, false)
            .addProperty("name", &descriptor::name, false)
            .addProperty("key", &descriptor::key)
            .addProperty("hash", &descriptor::hash)
            .addFunction("valid", &descriptor::valid)
            .addFunction("hasType", &descriptor::has_type)
            .addFunction("hasId", &descriptor::has_id)
            .addFunction("hasName", &descriptor::has_name)
            .addFunction("isNamespaced", &descriptor::is_namespaced)
            .addFunction("withType", &descriptor::with_type)
            .addFunction("withId", &descriptor::with_id)
            .addFunction("normalizedId", &descriptor::normalized_id)
            .addProperty("fromNamespace", &descriptor::from_namespace)
            .addProperty("ignoringNamespace", &descriptor::ignoring_namespace)
            .addProperty("ignoringType", &descriptor::ignoring_type)
            .addProperty("ignoringId", &descriptor::ignoring_id)
            .addProperty("ignoringName", &descriptor::ignoring_name)
            .addProperty("description", &descriptor::description)
            .addFunction("matchingResources", &descriptor::matching_resources)
            .addFunction("bestResource", &descriptor::best_resource)
            .addFunction("whenId", &descriptor::when_id)
            .addFunction("whenNotId", &descriptor::when_not_id)
            .addFunction("whenLessThanId", &descriptor::when_less_than_id)
            .addFunction("whenGreaterThanId", &descriptor::when_greater_than_id)
            .addFunction("whenIdInRange", &descriptor::when_id_in_range)
            .addFunction("whenIdNotInRange", &descriptor::when_id_not_in_range)
        .endClass();
}

// MARK: - Construction

auto kestrel::resource::descriptor::file_constrained(const graphite::rsrc::file* file) -> lua_reference
{
    auto r = lua_reference(new descriptor());
    r->m_file_constraint = file;
    return r;
}

auto kestrel::resource::descriptor::identified(int64_t id) -> lua_reference
{
    auto r = lua_reference(new descriptor());
    r->id = id;
    r->m_variant = variant::identified;
    return r;
}

auto kestrel::resource::descriptor::typed(const std::string& type) -> lua_reference
{
    auto r = lua_reference(new descriptor());
    r->type = type;
    r->m_variant = variant::typed;
    return r;
}

auto kestrel::resource::descriptor::named(const std::string& name) -> lua_reference
{
    auto r = lua_reference(new descriptor());
    r->name = name;
    r->m_variant = variant::named;
    return r;
}

auto kestrel::resource::descriptor::typed_identified(const std::string& type, int64_t id) -> lua_reference
{
    auto r = lua_reference(new descriptor());
    r->id = id;
    r->type = type;
    r->m_variant = variant::typed_identified;
    return r;
}

auto kestrel::resource::descriptor::identified_named(int64_t id, const std::string& name) -> lua_reference
{
    auto r = lua_reference(new descriptor());
    r->id = id;
    r->name = name;
    r->m_variant = variant::identified_named;
    return r;
}

auto kestrel::resource::descriptor::typed_named(const std::string& type, const std::string& name) -> lua_reference
{
    auto r = lua_reference(new descriptor());
    r->type = type;
    r->name = name;
    r->m_variant = variant::typed_named;
    return r;
}

auto kestrel::resource::descriptor::typed_identified_named(const std::string& type, int64_t id, const std::string& name) -> lua_reference
{
    auto r = lua_reference(new descriptor());
    r->id = id;
    r->name = name;
    r->type = type;
    r->m_variant = variant::typed_identified_named;
    return r;
}

auto kestrel::resource::descriptor::reference(const std::string &ns,
                                           const std::string &type,
                                           int64_t id,
                                           const std::string &name) -> lua_reference
{
    auto r = lua_reference(new descriptor());
    r->namespaces = { ns };
    r->id = id;
    r->name = name;
    r->type = type;
    r->m_variant = variant::resolved;
    return r;
}

auto kestrel::resource::descriptor::reference(const std::string &type,
                                           int64_t id,
                                           const std::string &name) -> lua_reference
{
    auto r = lua_reference(new descriptor());
    r->namespaces = { resource_namespace::universal()->primary_name() };
    r->id = id;
    r->name = name;
    r->type = type;
    r->m_variant = variant::typed_identified_named;
    return r;
}

// MARK: - Accessors

auto kestrel::resource::descriptor::valid() -> bool
{
    if (m_variant == variant::resolved) {
        return true;
    }

    resolve();
    return !m_resolved_resources.empty();
}

auto kestrel::resource::descriptor::description() const -> std::string
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

auto kestrel::resource::descriptor::key() const -> std::string
{
    std::string value;

    if (is_namespaced() && !namespaces[0].empty()) {
        // TODO: Currently only taking the first namespace.
        value += namespaces[0] + ".";
    }

    if (has_id()) {
        value += std::to_string(id);
    }

    return value;
}

auto kestrel::resource::descriptor::hash() const -> std::string
{
    std::string value;

    if (is_namespaced()) {
        // TODO: Currently only taking the first namespace.
        value += namespaces[0] + ":";
    }

    if (has_type()) {
        value += type + ".";
    }

    if (has_id()) {
        value += "#" + std::to_string(id);
    }

    return std::to_string(graphite::hashing::xxh64(value.c_str(), value.size()));
}

auto kestrel::resource::descriptor::has_type() const -> bool
{
    return m_variant == variant::typed
           || m_variant == variant::typed_identified_named
           || m_variant == variant::typed_identified
           || m_variant == variant::typed_named
           || m_variant == variant::resolved;
}

auto kestrel::resource::descriptor::has_id() const -> bool
{
    return m_variant == variant::identified
           || m_variant == variant::identified_named
           || m_variant == variant::typed_identified
           || m_variant == variant::typed_identified_named
           || m_variant == variant::resolved;
}

auto kestrel::resource::descriptor::has_name() const -> bool
{
    return m_variant == variant::named
           || m_variant == variant::identified_named
           || m_variant == variant::typed_named
           || m_variant == variant::typed_identified_named
           || m_variant == variant::resolved;
}

auto kestrel::resource::descriptor::is_namespaced() const -> bool
{
    return !namespaces.empty();
}

// MARK: - Modifiers

auto kestrel::resource::descriptor::with_type(const std::string &type) const -> lua_reference
{
    lua_reference ref;

    if (has_id() && has_name()) {
        ref = descriptor::typed_identified_named(type, id, name);
    }
    else if (has_id()) {
        ref = descriptor::typed_identified(type, id);
    }
    else if (has_name()) {
        ref = descriptor::typed_named(type, name);
    }
    else {
        ref = descriptor::typed(type);
    }

    ref->namespaces = namespaces;
    ref->m_file_constraint = m_file_constraint;
    return ref;
}

auto kestrel::resource::descriptor::with_id(int64_t id) const -> lua_reference
{
    lua_reference ref;

    if (has_type() && has_name()) {
        ref = descriptor::typed_identified_named(type, id, name);
    }
    else if (has_type()) {
        ref = descriptor::typed_identified(type, id);
    }
    else if (has_name()) {
        ref = descriptor::identified_named(id, name);
    }
    else {
        ref = descriptor::identified(id);
    }

    ref->namespaces = namespaces;
    ref->m_file_constraint = m_file_constraint;
    return ref;
}


auto kestrel::resource::descriptor::normalized_id(int64_t index_base, int64_t first_id) const -> lua_reference
{
    return with_id((id - index_base) + first_id);
}

auto kestrel::resource::descriptor::from_namespace() const -> lua_reference
{
    lua_reference ref(new descriptor());
    ref->namespaces = namespaces;
    ref->m_file_constraint = m_file_constraint;
    return ref;
}

auto kestrel::resource::descriptor::ignoring_type() const -> lua_reference
{
    lua_reference ref;

    if (has_id() && has_name()) {
        ref = descriptor::identified_named(id, name);
    }
    else if (has_id()) {
        ref = descriptor::identified(id);
    }
    else if (has_name()) {
        ref = descriptor::named(name);
    }
    else {
        ref = lua_reference(new descriptor());
    }

    ref->namespaces = namespaces;
    ref->m_file_constraint = m_file_constraint;
    return ref;
}

auto kestrel::resource::descriptor::ignoring_id() const -> lua_reference
{
    lua_reference ref;

    if (has_type() && has_name()) {
        ref = descriptor::typed_named(type, name);
    }
    else if (has_type()) {
        ref = descriptor::typed(type);
    }
    else if (has_name()) {
        ref = descriptor::named(name);
    }
    else {
        ref = lua_reference(new descriptor());
    }

    ref->namespaces = namespaces;
    ref->m_file_constraint = m_file_constraint;
    return ref;
}

auto kestrel::resource::descriptor::ignoring_name() const -> lua_reference
{
    lua_reference ref;

    if (has_id() && has_type()) {
        ref = descriptor::typed_identified(type, id);
    }
    else if (has_type()) {
        ref = descriptor::typed(type);
    }
    else if (has_id()) {
        ref = descriptor::identified(id);
    }
    else {
        ref = lua_reference(new descriptor());
    }

    ref->namespaces = namespaces;
    ref->m_file_constraint = m_file_constraint;
    return ref;
}

auto kestrel::resource::descriptor::ignoring_namespace() const -> lua_reference
{
    lua_reference ref;

    if (has_type() && has_id() && has_name()) {
        ref = descriptor::typed_identified_named(type, id, name);
    }
    else if (has_type() && has_id()) {
        ref = descriptor::typed_identified(type, id);
    }
    else if (has_type() && has_name()) {
        ref = descriptor::typed_named(type, name);
    }
    else if (has_id() && has_name()) {
        ref = descriptor::identified_named(id, name);
    }
    else if (has_type()) {
        ref = descriptor::typed(type);
    }
    else if (has_id()) {
        ref = descriptor::identified(id);
    }
    else if (has_name()) {
        ref = descriptor::named(name);
    }
    else {
        ref = lua_reference(new descriptor());
    }

    ref->m_file_constraint = m_file_constraint;
    return ref;
}

// MARK: - Resources

auto kestrel::resource::descriptor::matching_resources() -> lua::vector<lua_reference>
{
    resolve();
    return m_resolved_resources;
}

auto kestrel::resource::descriptor::best_resource() -> lua_reference
{
    resolve();
    if (m_variant == variant::resolved) {
        return { nullptr };
    }
    else if (!m_resolved_resources.empty()) {
        return m_resolved_resources.at(0);
    }
    else {
        return { nullptr };
    }
}

auto kestrel::resource::descriptor::load() -> const graphite::rsrc::resource *
{
    // TODO: Determine the best way of doing this check.
    if (!valid()) {
        return nullptr;
    }

    // We need to be able to assume that we have a valid resource descriptor at this point so that
    // we can just directly load it without conditions.

    if (m_resolved_resources.empty()) {
        std::vector<graphite::rsrc::attribute> attributes;
        resource_namespace ns(namespaces.at(0));
        if (!(ns.is_universal() || ns.is_global())) {
            attributes.emplace_back(graphite::rsrc::attribute("namespace", ns.primary_name()));
        }

        if (m_file_constraint) {
            return m_file_constraint->find(type, id, attributes);
        }
        else {
            return graphite::rsrc::manager::shared_manager().find(type, id, attributes);
        }
    }
    else {
        auto best = m_resolved_resources.at(0);
        std::vector<graphite::rsrc::attribute> attributes;
        resource_namespace ns(best->namespaces.at(0));
        if (!(ns.is_universal() || ns.is_global())) {
            attributes.emplace_back(graphite::rsrc::attribute(resource_namespace::attribute_name, ns.primary_name()));
        }

        if (m_file_constraint) {
            return m_file_constraint->find(best->type, best->id, attributes);
        }
        else {
            return graphite::rsrc::manager::shared_manager().find(best->type, best->id, attributes);
        }
    }
}

// MARK: - Resolution

auto kestrel::resource::descriptor::resolve() -> void
{
    if (m_resolved) {
        return;
    }

    switch (m_variant) {
        case variant::none: {
            break;
        }
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

auto kestrel::resource::descriptor::resolve_identified() -> void
{
    resource_namespace ns(namespaces);
    const auto& rm = graphite::rsrc::manager::shared_manager();

    std::vector<graphite::rsrc::file *> files;
    if (m_file_constraint) {
        files.emplace_back(const_cast<graphite::rsrc::file *>(m_file_constraint));
    }
    else {
        files = rm.file_references();
    }

    for (const auto& file : files) {
        for (const auto& type_hash : file->types()) {
            const auto& t = const_cast<struct graphite::rsrc::type *>(file->type(type_hash));

            std::string type_namespace = resource_namespace::global()->primary_name();
            const auto& attributes = t->attributes();
            const auto& it = attributes.find(graphite::rsrc::attribute::hash_for_name(resource_namespace::attribute_name));
            if (it != attributes.end()) {
                type_namespace = it->second.string_value();
            }

            if (!ns.is_universal() && !ns.has_name(type_namespace)) {
                continue;
            }

            for (const auto& resource : *t) {
                if (resource->id() == id) {
                    auto res = descriptor::reference(type_namespace, t->code(), resource->id(), resource->name());
                    res->m_file_constraint = m_file_constraint;
                    m_resolved_resources.emplace_back(std::move(res));
                }
            }
        }
    }

    m_resolved = true;
}

auto kestrel::resource::descriptor::resolve_typed() -> void
{
    resource_namespace ns(namespaces);
    const auto& rm = graphite::rsrc::manager::shared_manager();

    std::vector<graphite::rsrc::file *> files;
    if (m_file_constraint) {
        files.emplace_back(const_cast<graphite::rsrc::file *>(m_file_constraint));
    }
    else {
        files = rm.file_references();
    }

    for (const auto& file : files) {
        for (const auto& type_hash : file->types()) {
            const auto& t = const_cast<graphite::rsrc::type *>(file->type(type_hash));

            std::string type_namespace = resource_namespace::global()->primary_name();
            const auto& attributes = t->attributes();
            const auto& it = attributes.find(graphite::rsrc::attribute::hash_for_name(resource_namespace::attribute_name));
            if (it != attributes.end()) {
                type_namespace = it->second.string_value();
            }

            if (!ns.is_universal() && !ns.has_name(type_namespace)) {
                continue;
            }

            if (t->code() != type) {
                continue;
            }

            for (const auto& resource : *t) {
                auto res = descriptor::reference(type_namespace, t->code(), resource->id(), resource->name());
                res->m_file_constraint = m_file_constraint;
                m_resolved_resources.emplace_back(std::move(res));
            }
        }
    }

    m_resolved = true;
}

auto kestrel::resource::descriptor::resolve_named() -> void
{
    resource_namespace ns(namespaces);
    const auto& rm = graphite::rsrc::manager::shared_manager();

    std::vector<graphite::rsrc::file *> files;
    if (m_file_constraint) {
        files.emplace_back(const_cast<graphite::rsrc::file *>(m_file_constraint));
    }
    else {
        files = rm.file_references();
    }

    for (const auto& file : files) {
        for (const auto& type_hash : file->types()) {
            const auto& t = const_cast<graphite::rsrc::type *>(file->type(type_hash));

            std::string type_namespace = resource_namespace::global()->primary_name();
            const auto& attributes = t->attributes();
            const auto& it = attributes.find(graphite::rsrc::attribute::hash_for_name(resource_namespace::attribute_name));
            if (it != attributes.end()) {
                type_namespace = it->second.string_value();
            }

            if (!ns.is_universal() && !ns.has_name(type_namespace)) {
                continue;
            }

            for (const auto& resource : *t) {
                if (resource->name() == name) {
                    auto res = descriptor::reference(type_namespace, t->code(), resource->id(), resource->name());
                    res->m_file_constraint = m_file_constraint;
                    m_resolved_resources.emplace_back(std::move(res));
                }
            }
        }
    }

    m_resolved = true;
}

auto kestrel::resource::descriptor::resolve_typed_identified() -> void
{
    resource_namespace ns(namespaces);
    const auto& rm = graphite::rsrc::manager::shared_manager();

    std::vector<graphite::rsrc::file *> files;
    if (m_file_constraint) {
        files.emplace_back(const_cast<graphite::rsrc::file *>(m_file_constraint));
    }
    else {
        files = rm.file_references();
    }

    for (const auto& file : files) {
        for (const auto& type_hash : file->types()) {
            const auto& t = const_cast<graphite::rsrc::type *>(file->type(type_hash));

            std::string type_namespace = resource_namespace::global()->primary_name();
            const auto& attributes = t->attributes();
            const auto& it = attributes.find(graphite::rsrc::attribute::hash_for_name(resource_namespace::attribute_name));
            if (it != attributes.end()) {
                type_namespace = it->second.string_value();
            }

            if (!ns.is_universal() && !ns.has_name(type_namespace)) {
                continue;
            }

            if (t->code() != type) {
                continue;
            }

            for (const auto& resource : *t) {
                if (resource->id() == id) {
                    auto res = descriptor::reference(type_namespace, t->code(), resource->id(), resource->name());
                    res->m_file_constraint = m_file_constraint;
                    m_resolved_resources.emplace_back(std::move(res));
                }
            }
        }
    }

    m_resolved = true;
}

auto kestrel::resource::descriptor::resolve_identified_named() -> void
{
    resource_namespace ns(namespaces);
    const auto& rm = graphite::rsrc::manager::shared_manager();

    std::vector<graphite::rsrc::file *> files;
    if (m_file_constraint) {
        files.emplace_back(const_cast<graphite::rsrc::file *>(m_file_constraint));
    }
    else {
        files = rm.file_references();
    }

    for (const auto& file : files) {
        for (const auto& type_hash : file->types()) {
            const auto& t = const_cast<graphite::rsrc::type *>(file->type(type_hash));

            std::string type_namespace = resource_namespace::global()->primary_name();
            const auto& attributes = t->attributes();
            const auto& it = attributes.find(graphite::rsrc::attribute::hash_for_name(resource_namespace::attribute_name));
            if (it != attributes.end()) {
                type_namespace = it->second.string_value();
            }

            if (!ns.is_universal() && !ns.has_name(type_namespace)) {
                continue;
            }

            for (const auto& resource : *t) {
                if (resource->id() == id && resource->name() == name) {
                    auto res = descriptor::reference(type_namespace, t->code(), resource->id(), resource->name());
                    res->m_file_constraint = m_file_constraint;
                    m_resolved_resources.emplace_back(std::move(res));
                }
            }
        }
    }

    m_resolved = true;
}

auto kestrel::resource::descriptor::resolve_typed_named() -> void
{
    resource_namespace ns(namespaces);
    const auto& rm = graphite::rsrc::manager::shared_manager();

    std::vector<graphite::rsrc::file *> files;
    if (m_file_constraint) {
        files.emplace_back(const_cast<graphite::rsrc::file *>(m_file_constraint));
    }
    else {
        files = rm.file_references();
    }

    for (const auto& file : files) {
        for (const auto& type_hash : file->types()) {
            const auto& t = const_cast<graphite::rsrc::type *>(file->type(type_hash));

            std::string type_namespace = resource_namespace::global()->primary_name();
            const auto& attributes = t->attributes();
            const auto& it = attributes.find(graphite::rsrc::attribute::hash_for_name(resource_namespace::attribute_name));
            if (it != attributes.end()) {
                type_namespace = it->second.string_value();
            }

            if (!ns.is_universal() && !ns.has_name(type_namespace)) {
                continue;
            }

            if (t->code() != type) {
                continue;
            }

            for (const auto& resource : *t) {
                if (resource->name() == name) {
                    auto res = descriptor::reference(type_namespace, t->code(), resource->id(), resource->name());
                    res->m_file_constraint = m_file_constraint;
                    m_resolved_resources.emplace_back(std::move(res));
                }
            }
        }
    }

    m_resolved = true;
}

auto kestrel::resource::descriptor::resolve_typed_identified_named() -> void
{
    resource_namespace ns(namespaces);
    const auto& rm = graphite::rsrc::manager::shared_manager();

    std::vector<graphite::rsrc::file *> files;
    if (m_file_constraint) {
        files.emplace_back(const_cast<graphite::rsrc::file *>(m_file_constraint));
    }
    else {
        files = rm.file_references();
    }

    for (const auto& file : files) {
        for (const auto& type_hash : file->types()) {
            const auto& t = const_cast<graphite::rsrc::type *>(file->type(type_hash));

            std::string type_namespace = resource_namespace::global()->primary_name();
            const auto& attributes = t->attributes();
            const auto& it = attributes.find(graphite::rsrc::attribute::hash_for_name(resource_namespace::attribute_name));
            if (it != attributes.end()) {
                type_namespace = it->second.string_value();
            }

            if (!ns.is_universal() && !ns.has_name(type_namespace)) {
                continue;
            }

            if (t->code() != type) {
                continue;
            }

            for (const auto& resource : *t) {
                if (resource->id() == id && resource->name() == name) {
                    auto res = descriptor::reference(type_namespace, t->code(), resource->id(), resource->name());
                    res->m_file_constraint = m_file_constraint;
                    m_resolved_resources.emplace_back(std::move(res));
                }
            }
        }
    }

    m_resolved = true;
}

// MARK: - Conditions / When

auto kestrel::resource::descriptor::when_id(int64_t id) const -> bool
{
    return (this->id == id);
}

auto kestrel::resource::descriptor::when_not_id(int64_t id) const -> bool
{
    return (this->id != id);
}

auto kestrel::resource::descriptor::when_less_than_id(int64_t id) const -> bool
{
    return (this->id < id);
}

auto kestrel::resource::descriptor::when_greater_than_id(int64_t id) const -> bool
{
    return (this->id > id);
}

auto kestrel::resource::descriptor::when_id_in_range(int64_t lower_id, int64_t upper_id) const -> bool
{
    return (this->id >= lower_id && this->id <= upper_id);
}

auto kestrel::resource::descriptor::when_id_not_in_range(int64_t lower_id, int64_t upper_id) const -> bool
{
    return (this->id < lower_id || this->id > upper_id);
}
