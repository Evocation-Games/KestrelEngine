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

#pragma once

#include <libGraphite/rsrc/manager.hpp>
#include "util/hint.hpp"
#include "scripting/state.hpp"
#include "util/lua_vector.hpp"

namespace asset
{

    struct resource_descriptor: public scripting::lua::object
    {
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

        std::vector<std::string> namespaces;
        std::string type;
        int64_t id { INT64_MIN };
        std::string name;

        typedef luabridge::RefCountedPtr<asset::resource_descriptor> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

        explicit resource_descriptor() = default;
        resource_descriptor(resource_descriptor&) = default;
        resource_descriptor(const resource_descriptor&) = default;

        static auto file_constrained(const graphite::rsrc::file *file) -> lua_reference;

        lua_api static auto identified(int64_t id) -> lua_reference;
        lua_api static auto typed(const std::string& type) -> lua_reference;
        lua_api static auto named(const std::string& name) -> lua_reference;
        lua_api static auto typed_identified(const std::string& type, int64_t id) -> lua_reference;
        lua_api static auto identified_named(int64_t id, const std::string& name) -> lua_reference;
        lua_api static auto typed_named(const std::string& type, const std::string& name) -> lua_reference;
        lua_api static auto typed_identified_named(const std::string& type, int64_t id, const std::string& name) -> lua_reference;

        static auto reference(const std::string& type, int64_t id, const std::string& name) -> lua_reference;
        static auto reference(const std::string& ns, const std::string& type, int64_t id, const std::string& name) -> lua_reference;

        lua_api [[nodiscard]] auto has_type() const -> bool;
        lua_api [[nodiscard]] auto has_id() const -> bool;
        lua_api [[nodiscard]] auto has_name() const -> bool;
        lua_api [[nodiscard]] auto is_namespaced() const -> bool;

        lua_api [[nodiscard]] auto description() const -> std::string;
        lua_api [[nodiscard]] auto key() const -> std::string;
        lua_api [[nodiscard]] auto hash() const -> std::string;

        lua_api [[nodiscard]] auto with_type(const std::string& type) const -> lua_reference;
        lua_api [[nodiscard]] auto with_id(int64_t id) const -> lua_reference;

        lua_api [[nodiscard]] auto normalized_id(int64_t index_base, int64_t first_id) const -> lua_reference;

        lua_api [[nodiscard]] auto ignoring_type() const -> lua_reference;
        lua_api [[nodiscard]] auto ignoring_id() const -> lua_reference;
        lua_api [[nodiscard]] auto ignoring_name() const -> lua_reference;
        lua_api [[nodiscard]] auto ignoring_namespace() const -> lua_reference;

        lua_api [[nodiscard]] auto from_namespace() const -> lua_reference;

        lua_api auto valid() -> bool;

        lua_api auto matching_resources() -> util::lua_vector<lua_reference>;
        lua_api auto best_resource() -> lua_reference;

        lua_api [[nodiscard]] auto when_id(int64_t id) const -> bool;
        lua_api [[nodiscard]] auto when_not_id(int64_t id) const -> bool;
        lua_api [[nodiscard]] auto when_less_than_id(int64_t id) const -> bool;
        lua_api [[nodiscard]] auto when_greater_than_id(int64_t id) const -> bool;
        lua_api [[nodiscard]] auto when_id_in_range(int64_t lower_id, int64_t upper_id) const -> bool;
        lua_api [[nodiscard]] auto when_id_not_in_range(int64_t lower_id, int64_t upper_id) const -> bool;

        auto load() -> const graphite::rsrc::resource *;

    private:
        enum variant m_variant { variant::none };
        bool m_resolved { false };
        util::lua_vector<lua_reference> m_resolved_resources {};
        const graphite::rsrc::file *m_file_constraint { nullptr };

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
