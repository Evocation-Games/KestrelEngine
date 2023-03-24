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

#include <libKestrel/lua/support/vector.hpp>
#include <libKestrel/resource/descriptor.hpp>
#include <libKestrel/resource/container.hpp>
#include <libGraphite/util/hashing.hpp>
#include <libGraphite/rsrc/manager.hpp>

// MARK: - Construction

auto kestrel::resource::descriptor::file_constrained(const graphite::rsrc::file* file) -> lua_reference
{
    auto r = lua_reference(new descriptor());
    r->file = file;
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
                                              std::int64_t id,
                                              const std::string &name) -> lua_reference
{
    auto r = lua_reference(new descriptor());
    r->containers = lua::vector<std::string>(ns);
    r->id = id;
    r->name = name;
    r->type = type;
    r->m_variant = variant::resolved;
    return r;
}

auto kestrel::resource::descriptor::reference(const std::string &type,
                                              std::int64_t id,
                                              const std::string &name) -> lua_reference
{
    auto r = lua_reference(new descriptor());
    r->containers = lua::vector<std::string>(container::universal()->primary_name());
    r->id = id;
    r->name = name;
    r->type = type;
    r->m_variant = variant::typed_identified_named;
    return r;
}

// MARK: - Accessors

auto kestrel::resource::descriptor::hash() const -> std::string
{
    std::string hash;
    if (!containers.empty()) {
        for (const auto& c : containers) {
            hash += c + ":";
        }
    }
    hash += std::to_string(id);
    return hash;
}

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

    if (has_container()) {
        description += "container<";
        for (const auto& c : containers) {
            if (resource::container(c).is_global()) {
                description += "global ";
            }
            else {
                description += c + " ";
            }
        }

        if (description.size() > 1 && description.back() == ' ') {
            description.erase(description.size() - 1);
        }

        description += ">";
    }

    if (description.size() > 1 && description.back() == ' ') {
        description.erase(description.size() - 1);
    }

    return description;
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

auto kestrel::resource::descriptor::has_container() const -> bool
{
    return !containers.empty();
}

// MARK: - Modifiers

auto kestrel::resource::descriptor::with_type(const std::string& type) const -> lua_reference
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

    ref->containers = containers;
    ref->file = file;
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

    ref->containers = containers;
    ref->file = file;
    return ref;
}


auto kestrel::resource::descriptor::normalized_id(int64_t index_base, int64_t first_id) const -> lua_reference
{
    return with_id((id - index_base) + first_id);
}

auto kestrel::resource::descriptor::container() const -> lua_reference
{
    lua_reference ref(new descriptor());
    ref->containers = containers;
    ref->file = file;
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

    ref->containers = containers;
    ref->file = file;
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

    ref->containers = containers;
    ref->file = file;
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

    ref->containers = containers;
    ref->file = file;
    return ref;
}

auto kestrel::resource::descriptor::ignoring_container() const -> lua_reference
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

    ref->file = file;
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
        resource::container c(containers.at(0));
        if (!(c.is_universal() || c.is_global())) {
            attributes.emplace_back(resource::container::attribute_name, c.primary_name());
        }

        if (file) {
            return file->find(type, id, attributes);
        }
        else {
            return graphite::rsrc::manager::shared_manager().find(type, id, attributes);
        }
    }
    else {
        auto best = m_resolved_resources.at(0);
        std::vector<graphite::rsrc::attribute> attributes;
        resource::container c(best->containers.at(0));
        if (!(c.is_universal() || c.is_global())) {
            attributes.emplace_back(resource::container::attribute_name, c.primary_name());
        }

        if (file) {
            return file->find(best->type, best->id, attributes);
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
    resource::container c(containers);
    const auto& rm = graphite::rsrc::manager::shared_manager();

    std::vector<graphite::rsrc::file *> files;
    if (file) {
        files.emplace_back(const_cast<graphite::rsrc::file *>(file));
    }
    else {
        files = rm.file_references();
    }

    for (const auto& file : files) {
        for (const auto& type_hash : file->types()) {
            const auto& t = const_cast<struct graphite::rsrc::type *>(file->type(type_hash));

            std::string type_container = resource::container::global()->primary_name();
            const auto& attributes = t->attributes();
            const auto& it = attributes.find(graphite::rsrc::attribute::hash_for_name(resource::container::attribute_name));
            if (it != attributes.end()) {
                type_container = it->second.string_value();
            }

            if (!c.is_universal() && !c.has_name(type_container)) {
                continue;
            }

            for (const auto& resource : *t) {
                if (resource->id() == id) {
                    auto res = descriptor::reference(type_container, t->code(), resource->id(), resource->name());
                    res->file = file;
                    m_resolved_resources.emplace_back(res);
                }
            }
        }
    }

    m_resolved = true;
}

auto kestrel::resource::descriptor::resolve_typed() -> void
{
    resource::container c(containers);
    const auto& rm = graphite::rsrc::manager::shared_manager();

    std::vector<graphite::rsrc::file *> files;
    if (file) {
        files.emplace_back(const_cast<graphite::rsrc::file *>(file));
    }
    else {
        files = rm.file_references();
    }

    for (const auto& file : files) {
        for (const auto& type_hash : file->types()) {
            const auto& t = const_cast<graphite::rsrc::type *>(file->type(type_hash));

            std::string type_container = resource::container::global()->primary_name();
            const auto& attributes = t->attributes();
            const auto& it = attributes.find(graphite::rsrc::attribute::hash_for_name(resource::container::attribute_name));
            if (it != attributes.end()) {
                type_container = it->second.string_value();
            }

            if (!c.is_universal() && !c.has_name(type_container)) {
                continue;
            }

            if (t->code() != type) {
                continue;
            }

            for (const auto& resource : *t) {
                auto res = descriptor::reference(type_container, t->code(), resource->id(), resource->name());
                res->file = file;
                m_resolved_resources.emplace_back(res);
            }
        }
    }

    m_resolved = true;
}

auto kestrel::resource::descriptor::resolve_named() -> void
{
    resource::container c(containers);
    const auto& rm = graphite::rsrc::manager::shared_manager();

    std::vector<graphite::rsrc::file *> files;
    if (file) {
        files.emplace_back(const_cast<graphite::rsrc::file *>(file));
    }
    else {
        files = rm.file_references();
    }

    for (const auto& file : files) {
        for (const auto& type_hash : file->types()) {
            const auto& t = const_cast<graphite::rsrc::type *>(file->type(type_hash));

            std::string type_container = resource::container::global()->primary_name();
            const auto& attributes = t->attributes();
            const auto& it = attributes.find(graphite::rsrc::attribute::hash_for_name(resource::container::attribute_name));
            if (it != attributes.end()) {
                type_container = it->second.string_value();
            }

            if (!c.is_universal() && !c.has_name(type_container)) {
                continue;
            }

            for (const auto& resource : *t) {
                if (resource->name() == name) {
                    auto res = descriptor::reference(type_container, t->code(), resource->id(), resource->name());
                    res->file = file;
                    m_resolved_resources.emplace_back(res);
                }
            }
        }
    }

    m_resolved = true;
}

auto kestrel::resource::descriptor::resolve_typed_identified() -> void
{
    resource::container c(containers);
    const auto& rm = graphite::rsrc::manager::shared_manager();

    std::vector<graphite::rsrc::file *> files;
    if (file) {
        files.emplace_back(const_cast<graphite::rsrc::file *>(file));
    }
    else {
        files = rm.file_references();
    }

    for (const auto& file : files) {
        for (const auto& type_hash : file->types()) {
            const auto& t = const_cast<graphite::rsrc::type *>(file->type(type_hash));

            std::string type_container = resource::container::global()->primary_name();
            const auto& attributes = t->attributes();
            const auto& it = attributes.find(graphite::rsrc::attribute::hash_for_name(resource::container::attribute_name));
            if (it != attributes.end()) {
                type_container = it->second.string_value();
            }

            if (!c.is_universal() && !c.has_name(type_container)) {
                continue;
            }

            if (t->code() != type) {
                continue;
            }

            for (const auto& resource : *t) {
                if (resource->id() == id) {
                    auto res = descriptor::reference(type_container, t->code(), resource->id(), resource->name());
                    res->file = file;
                    m_resolved_resources.emplace_back(res);
                }
            }
        }
    }

    m_resolved = true;
}

auto kestrel::resource::descriptor::resolve_identified_named() -> void
{
    resource::container c(containers);
    const auto& rm = graphite::rsrc::manager::shared_manager();

    std::vector<graphite::rsrc::file *> files;
    if (file) {
        files.emplace_back(const_cast<graphite::rsrc::file *>(file));
    }
    else {
        files = rm.file_references();
    }

    for (const auto& file : files) {
        for (const auto& type_hash : file->types()) {
            const auto& t = const_cast<graphite::rsrc::type *>(file->type(type_hash));

            std::string type_container = resource::container::global()->primary_name();
            const auto& attributes = t->attributes();
            const auto& it = attributes.find(graphite::rsrc::attribute::hash_for_name(resource::container::attribute_name));
            if (it != attributes.end()) {
                type_container = it->second.string_value();
            }

            if (!c.is_universal() && !c.has_name(type_container)) {
                continue;
            }

            for (const auto& resource : *t) {
                if (resource->id() == id && resource->name() == name) {
                    auto res = descriptor::reference(type_container, t->code(), resource->id(), resource->name());
                    res->file = file;
                    m_resolved_resources.emplace_back(res);
                }
            }
        }
    }

    m_resolved = true;
}

auto kestrel::resource::descriptor::resolve_typed_named() -> void
{
    resource::container c(containers);
    const auto& rm = graphite::rsrc::manager::shared_manager();

    std::vector<graphite::rsrc::file *> files;
    if (file) {
        files.emplace_back(const_cast<graphite::rsrc::file *>(file));
    }
    else {
        files = rm.file_references();
    }

    for (const auto& file : files) {
        for (const auto& type_hash : file->types()) {
            const auto& t = const_cast<graphite::rsrc::type *>(file->type(type_hash));

            std::string type_container = resource::container::global()->primary_name();
            const auto& attributes = t->attributes();
            const auto& it = attributes.find(graphite::rsrc::attribute::hash_for_name(resource::container::attribute_name));
            if (it != attributes.end()) {
                type_container = it->second.string_value();
            }

            if (!c.is_universal() && !c.has_name(type_container)) {
                continue;
            }

            if (t->code() != type) {
                continue;
            }

            for (const auto& resource : *t) {
                if (resource->name() == name) {
                    auto res = descriptor::reference(type_container, t->code(), resource->id(), resource->name());
                    res->file = file;
                    m_resolved_resources.emplace_back(res);
                }
            }
        }
    }

    m_resolved = true;
}

auto kestrel::resource::descriptor::resolve_typed_identified_named() -> void
{
    resource::container c(containers);
    const auto& rm = graphite::rsrc::manager::shared_manager();

    std::vector<graphite::rsrc::file *> files;
    if (file) {
        files.emplace_back(const_cast<graphite::rsrc::file *>(file));
    }
    else {
        files = rm.file_references();
    }

    for (const auto& file : files) {
        for (const auto& type_hash : file->types()) {
            const auto& t = const_cast<graphite::rsrc::type *>(file->type(type_hash));

            std::string type_container = resource::container::global()->primary_name();
            const auto& attributes = t->attributes();
            const auto& it = attributes.find(graphite::rsrc::attribute::hash_for_name(resource::container::attribute_name));
            if (it != attributes.end()) {
                type_container = it->second.string_value();
            }

            if (!c.is_universal() && !c.has_name(type_container)) {
                continue;
            }

            if (t->code() != type) {
                continue;
            }

            for (const auto& resource : *t) {
                if (resource->id() == id && resource->name() == name) {
                    auto res = descriptor::reference(type_container, t->code(), resource->id(), resource->name());
                    res->file = file;
                    m_resolved_resources.emplace_back(res);
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
