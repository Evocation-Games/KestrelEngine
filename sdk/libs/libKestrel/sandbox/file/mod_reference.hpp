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
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>

namespace kestrel::sandbox
{
    struct lua_api(Kestrel.Sandbox.ModReference, Available_0_8) mod_reference
    {
    public:
        has_constructable_lua_api(mod_reference);

        enum class bundle_type { simple, package };
        enum class bundle_origin { game, user };

        explicit mod_reference(const std::string& path, bundle_origin origin = bundle_origin::game, bundle_type type = bundle_type::simple);

        [[nodiscard]] auto validate_as_modpackage() const -> bool;
        [[nodiscard]] auto validate_as_simplemod() const -> bool;
        auto parse_modpackage() -> void;
        auto parse_simplemod() -> void;
        auto construct_simplemod() -> void;

        lua_getter(hasInitialScript, Available_0_8) [[nodiscard]] auto has_initial_script() const -> bool;
        lua_getter(userProvided, Available_0_8) [[nodiscard]] auto user_provided() const -> bool;
        lua_getter(isLoaded, Available_0_8) [[nodiscard]] auto is_loaded() const -> bool;
        lua_getter(hasExecuted, Available_0_8) [[nodiscard]] auto has_executed() const -> bool;
        lua_getter(name, Available_0_8) [[nodiscard]] auto name() const -> std::string;
        lua_getter(author, Available_0_8) [[nodiscard]] auto author() const -> std::string;
        lua_getter(version, Available_0_8) [[nodiscard]] auto version() const -> std::string;
        lua_getter(path, Available_0_8) [[nodiscard]] auto path() const -> std::string;
        lua_getter(primaryContainer, Available_0_8) [[nodiscard]] auto primary_container() const -> std::string;
        lua_getter(description, Available_0_8) [[nodiscard]] auto description() const -> std::string;
        lua_getter(category, Available_0_8) [[nodiscard]] auto category() const -> std::string;
        lua_getter(packageID, Available_0_8) [[nodiscard]] auto package_id() const -> std::string;
        lua_getter(scenarioID, Available_0_8) [[nodiscard]] auto scenario_id() const -> std::string;
        lua_getter(isScenario, Available_0_8) [[nodiscard]] auto is_scenario() const -> bool;
        lua_getter(enabled, Available_0_8) [[nodiscard]] auto enabled() const -> bool;

        lua_setter(enabled, Available_0_8) auto set_enabled(bool f) -> void;

        auto load_resources() -> void;
        auto execute() -> void;

        lua_function(loadResources, Available_0_8) auto lua_load_resources() -> void;
        lua_function(execute, Available_0_8) auto lua_execute() -> void;

    private:
        std::string m_name;
        std::string m_version;
        std::string m_author;
        std::string m_primary_container;
        std::string m_description;
        std::string m_category;
        std::string m_package_id;
        std::string m_scenario_id;
        graphite::rsrc::resource::identifier m_lua_entry_script { INT64_MIN };
        std::string m_path;
        bundle_type m_bundle;
        bundle_origin m_origin;
        std::vector<graphite::rsrc::file *> m_mod_files;
        bool m_enabled { false };
        bool m_parsed { false };
        bool m_loaded { false };
        bool m_executed { false };
        bool m_is_active { true };

    };
}
