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

#pragma once

#include <libKestrel/util/availability.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/lua/support/vector.hpp>
#include <libGraphite/rsrc/file.hpp>
#include <libResource/reference.hpp>

namespace kestrel::resource
{
    class lua_api(Resource.Descriptor, Available_0_8) descriptor
    {
    public:
        has_constructable_lua_api(descriptor);

        lua_data(containers, const, Available_0_8) lua::vector<std::string> containers;
        lua_data(type, const, Available_0_8) std::string type;
        lua_data(id, const, Available_0_8) graphite::rsrc::resource::identifier id { INT64_MIN };
        lua_data(name, const, Available_0_8) std::string name;
        const graphite::rsrc::file *file { nullptr };

        explicit descriptor() = default;
        descriptor(descriptor&) = default;
        descriptor(const descriptor&) = default;
        explicit descriptor(const ::resource::reference& reference);

        static auto file_constrained(const graphite::rsrc::file *file) -> lua_reference;

        lua_getter(hash, Available_0_8) [[nodiscard]] auto hash() const -> std::string;

        lua_function(identified, Available_0_8) static auto identified(graphite::rsrc::resource::identifier id) -> lua_reference;
        lua_function(typed, Available_0_8) static auto typed(const std::string& type) -> lua_reference;
        lua_function(named, Available_0_8) static auto named(const std::string& name) -> lua_reference;
        lua_function(typedIdentified, Available_0_8) static auto typed_identified(const std::string& type, int64_t id) -> lua_reference;
        lua_function(identifiedNamed, Available_0_8) static auto identified_named(graphite::rsrc::resource::identifier id, const std::string& name) -> lua_reference;
        lua_function(typedNamed, Available_0_8) static auto typed_named(const std::string& type, const std::string& name) -> lua_reference;
        lua_function(typedIdentifiedNamed, Available_0_8) static auto typed_identified_named(const std::string& type, graphite::rsrc::resource::identifier id, const std::string& name) -> lua_reference;

        static auto reference(const std::string& type, graphite::rsrc::resource::identifier id, const std::string& name) -> lua_reference;
        static auto reference(const std::string& ns, const std::string& type, graphite::rsrc::resource::identifier id, const std::string& name) -> lua_reference;

        lua_function(hasType, Available_0_8) [[nodiscard]] auto has_type() const -> bool;
        lua_function(hasId, Available_0_8) [[nodiscard]] auto has_id() const -> bool;
        lua_function(hasName, Available_0_8) [[nodiscard]] auto has_name() const -> bool;
        lua_function(hasContainer, Available_0_8) [[nodiscard]] auto has_container() const -> bool;

        lua_getter(description, Available_0_8) [[nodiscard]] auto description() const -> std::string;

        lua_function(withType, Available_0_8) [[nodiscard]] auto with_type(const std::string& type) const -> lua_reference;
        lua_function(withId, Available_0_8) [[nodiscard]] auto with_id(graphite::rsrc::resource::identifier id) const -> lua_reference;

        lua_function(normalizedId, Available_0_8) [[nodiscard]] auto normalized_id(graphite::rsrc::resource::identifier index_base,
                                                                                   graphite::rsrc::resource::identifier first_id) const -> lua_reference;

        lua_getter(ignoringType, Available_0_8) [[nodiscard]] auto ignoring_type() const -> lua_reference;
        lua_getter(ignoringId, Available_0_8) [[nodiscard]] auto ignoring_id() const -> lua_reference;
        lua_getter(ignoringName, Available_0_8) [[nodiscard]] auto ignoring_name() const -> lua_reference;
        lua_getter(ignoringContainer, Available_0_8) [[nodiscard]] auto ignoring_container() const -> lua_reference;

        lua_getter(container, Available_0_8) [[nodiscard]] auto container() const -> lua_reference;

        lua_function(valid, Available_0_8) auto valid() -> bool;

        lua_function(matchingResources, Available_0_8) auto matching_resources() -> lua::vector<lua_reference>;
        lua_function(bestResource, Available_0_8) auto best_resource() -> lua_reference;

        lua_function(whenId, Available_0_8) [[nodiscard]] auto when_id(graphite::rsrc::resource::identifier id) const -> bool;
        lua_function(whenNotId, Available_0_8) [[nodiscard]] auto when_not_id(graphite::rsrc::resource::identifier id) const -> bool;
        lua_function(whenLessThanId, Available_0_8) [[nodiscard]] auto when_less_than_id(graphite::rsrc::resource::identifier id) const -> bool;
        lua_function(whenGreaterThanId, Available_0_8) [[nodiscard]] auto when_greater_than_id(graphite::rsrc::resource::identifier id) const -> bool;
        lua_function(whenIdInRange, Available_0_8) [[nodiscard]] auto when_id_in_range(graphite::rsrc::resource::identifier lower_id,
                                                                                       graphite::rsrc::resource::identifier upper_id) const -> bool;
        lua_function(whenIdNotInRange, Available_0_8) [[nodiscard]] auto when_id_not_in_range(graphite::rsrc::resource::identifier lower_id,
                                                                                              graphite::rsrc::resource::identifier upper_id) const -> bool;

        auto load() -> const graphite::rsrc::resource *;

    private:
        enum class variant
        {
            none,
            identified,
            typed,
            named,
            typed_identified,
            identified_named,
            typed_named,
            typed_identified_named,
            resolved,
        };

        enum variant m_variant { variant::none };
        bool m_resolved { false };
        lua::vector<lua_reference> m_resolved_resources {};

        auto resolve() -> void;
        auto resolve_identified() -> void;
        auto resolve_typed() -> void;
        auto resolve_named() -> void;
        auto resolve_typed_identified() -> void;
        auto resolve_identified_named() -> void;
        auto resolve_typed_named() -> void;
        auto resolve_typed_identified_named() -> void;

    };
}