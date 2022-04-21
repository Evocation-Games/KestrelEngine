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

#include <memory>
#include <libGraphite/rsrc/file.hpp>
#include "scripting/state.hpp"
#include "util/hint.hpp"

namespace host::sandbox
{
    struct mod_reference: public scripting::lua::object
    {
    public:
        enum class bundle_type { simple, package };
        enum class bundle_origin { game, user };

        typedef luabridge::RefCountedPtr<host::sandbox::mod_reference> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        explicit mod_reference(const std::string& path, bundle_origin origin = bundle_origin::game, bundle_type type = bundle_type::simple);

        [[nodiscard]] auto validate_as_modpackage() const -> bool;
        [[nodiscard]] auto validate_as_simplemod() const -> bool;
        auto parse_modpackage() -> void;
        auto parse_simplemod() -> void;
        auto construct_simplemod() -> void;

        [[nodiscard]] lua_api auto has_initial_script() const -> bool;
        [[nodiscard]] lua_api auto user_provided() const -> bool;
        [[nodiscard]] lua_api auto is_loaded() const -> bool;
        [[nodiscard]] lua_api auto has_executed() const -> bool;
        [[nodiscard]] lua_api auto name() const -> std::string;
        [[nodiscard]] lua_api auto author() const -> std::string;
        [[nodiscard]] lua_api auto version() const -> std::string;
        [[nodiscard]] lua_api auto path() const -> std::string;
        [[nodiscard]] lua_api auto primary_namespace() const -> std::string;
        [[nodiscard]] lua_api auto description() const -> std::string;
        [[nodiscard]] lua_api auto category() const -> std::string;
        [[nodiscard]] lua_api auto package_id() const -> std::string;
        [[nodiscard]] lua_api auto scenario_id() const -> std::string;
        [[nodiscard]] lua_api auto is_scenario() const -> bool;
        [[nodiscard]] lua_api auto enabled() const -> bool;

        lua_api auto set_enabled(bool f) -> void;

        auto load_resources() -> void;
        auto execute() -> void;

        lua_api auto lua_load_resources() -> void;
        lua_api auto lua_execute() -> void;

    private:
        std::string m_name;
        std::string m_version;
        std::string m_author;
        std::string m_primary_namespace;
        std::string m_description;
        std::string m_category;
        std::string m_package_id;
        std::string m_scenario_id;
        int64_t m_lua_entry_script { INT64_MIN };
        std::string m_path;
        bundle_type m_bundle;
        bundle_origin m_origin;
        std::vector<std::shared_ptr<graphite::rsrc::file>> m_mod_files;
        bool m_enabled { false };
        bool m_parsed { false };
        bool m_loaded { false };
        bool m_executed { false };
        bool m_is_active { true };

    };
}
