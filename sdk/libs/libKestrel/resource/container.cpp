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

#include <libKestrel/kestrel.hpp>
#include <libKestrel/resource/descriptor.hpp>
#include <libKestrel/resource/container.hpp>
#include <libResourceCore/manager.hpp>

// MARK: - Construction

kestrel::resource::container::container(const std::string &name)
    : m_names({ name })
{}

kestrel::resource::container::container(const std::vector<std::string>& names)
    : m_names(names.empty() ? std::vector<std::string>({ global_name }) : names)
{}

kestrel::resource::container::container(const lua::vector<std::string>& names)
    : m_names(names.empty() ? std::vector<std::string>({ global_name }) : names.to_vector())
{}

auto kestrel::resource::container::global() -> lua_reference
{
    return { new container(global_name) };
}

auto kestrel::resource::container::universal() -> lua_reference
{
    return { new container(universal_name) };
}

// MARK: - Accessors

auto kestrel::resource::container::primary_name() const -> std::string
{
    if (is_universal()) {
        return universal_name;
    }
    return m_names.empty() ? global_name : m_names[0];
}

auto kestrel::resource::container::count() const -> std::size_t
{
    return m_names.size();
}

auto kestrel::resource::container::text() const -> std::string
{
    std::string s;
    for (const auto& n : m_names) {
        s.append(" " + n);
    }
    s.erase(0, 1);
    return s;
}

auto kestrel::resource::container::contains_resources() const -> bool
{
    // This may or may not be the correct way of doing this. Theoretically universal should always have at least 1
    // resource.
    if (has_name(universal_name)) {
        return true;
    }

    for (const auto& file : resource_core::manager::shared_manager().file_references()) {
        for (const auto& type_hash : file->types()) {
            const auto& type = file->type(type_hash);
            const auto& attributes = type->attributes();
            const auto& it = attributes.find(resource_core::attribute::hash_for_name(attribute_name));

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

auto kestrel::resource::container::has_name(const std::string &name) const -> bool
{
    return (std::find(m_names.begin(), m_names.end(), name) != m_names.end());
}

auto kestrel::resource::container::is_global() const -> bool
{
    return has_name(global_name);
}

auto kestrel::resource::container::is_universal() const -> bool
{
    return has_name(universal_name);
}

// MARK: - Management

auto kestrel::resource::container::lua_add_container(const lua_reference &ns) -> void
{
    add_container(*ns.get());
}

auto kestrel::resource::container::add_container(const container &ns) -> void
{
    m_names.insert(m_names.end(), ns.m_names.begin(), ns.m_names.end());
}

// MARK: - Look Up

auto kestrel::resource::container::first_resource_of(const std::string& type) const -> descriptor::lua_reference
{
    return resources_of_type(type).at(0);
}

auto kestrel::resource::container::resources_of_type(const std::string& type) const -> lua::vector<descriptor::lua_reference>
{
    return typed_resource(type)->matching_resources();
}

auto kestrel::resource::container::resource_for_id(const std::string &type, int64_t id) const -> descriptor::lua_reference
{
    return typed_identified_resource(type, id)->best_resource();
}

// MARK: - Functional

auto kestrel::resource::container::lua_each_name(luabridge::LuaRef body) const -> void
{
    for (const auto& name : m_names) {
        body(lua_reference(new container(name)));
    }
}

auto kestrel::resource::container::each_name(const std::function<auto(const container&)->void> &body) const -> void
{
    for (const auto& name : m_names) {
        body(container(name));
    }
}

// MARK: - Resources

auto kestrel::resource::container::file_constraint(const resource_core::file *file) -> descriptor::lua_reference
{
    auto r = descriptor::file_constrained(file);
    each_name([&] (const container& ns) {
        r->containers.emplace_back(ns.primary_name());
    });
    return r;
}

auto kestrel::resource::container::identified_resource(int64_t id) const -> descriptor::lua_reference
{
    auto r = descriptor::identified(id);
    each_name([&] (const container& ns) {
        r->containers.emplace_back(ns.primary_name());
    });
    return r;
}

auto kestrel::resource::container::typed_resource(const std::string &type) const -> descriptor::lua_reference
{
    auto r = descriptor::typed(type);
    each_name([&] (const container& ns) {
        r->containers.emplace_back(ns.primary_name());
    });
    return r;
}

auto kestrel::resource::container::named_resource(const std::string &name) const -> descriptor::lua_reference
{
    auto r = descriptor::named(name);
    each_name([&] (const container& ns) {
        r->containers.emplace_back(ns.primary_name());
    });
    return r;
}

auto kestrel::resource::container::typed_identified_resource(const std::string &type, int64_t id) const -> descriptor::lua_reference
{
    auto r = descriptor::typed_identified(type, id);
    each_name([&] (const container& ns) {
        r->containers.emplace_back(ns.primary_name());
    });
    return r;
}

auto kestrel::resource::container::identified_named_resource(int64_t id, const std::string &name) const -> descriptor::lua_reference
{
    auto r = descriptor::identified_named(id, name);
    each_name([&] (const container& ns) {
        r->containers.emplace_back(ns.primary_name());
    });
    return r;
}

auto kestrel::resource::container::typed_named_resource(const std::string &type, const std::string &name) const -> descriptor::lua_reference
{
    auto r = descriptor::typed_named(type, name);
    each_name([&] (const container& ns) {
        r->containers.emplace_back(ns.primary_name());
    });
    return r;
}

auto kestrel::resource::container::typed_identified_named_resource(const std::string &type, int64_t id, const std::string &name) const -> descriptor::lua_reference
{
    auto r = descriptor::typed_identified_named(type, id, name);
    each_name([&] (const container& ns) {
        r->containers.emplace_back(ns.primary_name());
    });
    return r;
}

// MARK: - Scene

auto kestrel::resource::container::scene() const -> ui::game_scene::lua_reference
{
    return {
        new ui::game_scene(typed_identified_resource(lua::script::resource_type::code, ui::game_scene::initial_script_id))
    };
}

auto kestrel::resource::container::push_scene() const -> void
{
    kestrel::push_scene(scene());
}