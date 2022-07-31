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

#include <vector>
#include <array>
#include <string>
#include "core/file/file_reference.hpp"
#include "core/file/directory_reference.hpp"
#include "core/file/mod_reference.hpp"
#include "scripting/state.hpp"
#include "util/hint.hpp"
#include "util/lua_vector.hpp"

namespace host::sandbox
{

    class files: public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<host::sandbox::files> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

        enum class path_type
        {
            core, fonts, scenario, current_scenario, mods, data, saves, support
        };

    public:
        files(const files&) = delete;
        files& operator=(const files &) = delete;
        files(files &&) = delete;
        files & operator=(files &&) = delete;

        static auto shared_files() -> files&;

        auto set_game_path(const std::string& path, enum path_type type) -> void;
        [[nodiscard]] auto game_path(enum path_type type) const -> std::string;
        [[nodiscard]] auto game_file(enum path_type type) const -> file_reference::lua_reference;

        auto set_user_path(const std::string& path, enum path_type type) -> void;
        [[nodiscard]] auto user_path(enum path_type type) const -> std::string;
        [[nodiscard]] auto user_file(enum path_type type) const -> file_reference::lua_reference;

        [[nodiscard]] auto game_packages(enum path_type type, bool load = false) const -> util::lua_vector<mod_reference::lua_reference>;
        [[nodiscard]] auto user_packages(enum path_type type, bool load = false) const -> util::lua_vector<mod_reference::lua_reference>;
        [[nodiscard]] auto all_packages(enum path_type type, bool load = false) const -> util::lua_vector<mod_reference::lua_reference>;

        [[nodiscard]] auto scenario_data_files() -> util::lua_vector<file_reference::lua_reference>;

        [[nodiscard]] auto mods() -> util::lua_vector<mod_reference::lua_reference>;
        [[nodiscard]] auto scenarios() -> util::lua_vector<mod_reference::lua_reference>;

        [[nodiscard]] auto get_current_save_file() const -> host::sandbox::file_reference::lua_reference;
        auto set_save_file_name(const std::string& name) -> void;

        lua_api static auto game() -> host::sandbox::directory_reference::lua_reference;
        lua_api static auto user() -> host::sandbox::directory_reference::lua_reference;

        lua_api static auto active_scenario() -> mod_reference::lua_reference;
        lua_api static auto scenario_data() -> host::sandbox::directory_reference::lua_reference;
        lua_api static auto set_active_scenario(mod_reference::lua_reference scenario) -> void;

        lua_api static auto game_core() -> host::sandbox::file_reference::lua_reference;
        lua_api static auto game_support() -> host::sandbox::file_reference::lua_reference;
        lua_api static auto game_scenarios() -> host::sandbox::directory_reference::lua_reference;
        lua_api static auto game_data() -> host::sandbox::directory_reference::lua_reference;
        lua_api static auto game_fonts() -> host::sandbox::directory_reference::lua_reference;
        lua_api static auto game_mods() -> host::sandbox::directory_reference::lua_reference;

        lua_api static auto user_mods() -> host::sandbox::directory_reference::lua_reference;
        lua_api static auto user_saves() -> host::sandbox::directory_reference::lua_reference;
        lua_api static auto current_save_file() -> host::sandbox::file_reference::lua_reference;

        lua_api static auto set_save_file(const std::string& name) -> void;
        lua_api static auto save() -> void;

        lua_api static auto all_scenarios() -> util::lua_vector<mod_reference::lua_reference>;
        lua_api static auto all_scenario_mods(const mod_reference::lua_reference& scenario) -> util::lua_vector<mod_reference::lua_reference>;
        lua_api static auto all_active_scenario_mods() -> util::lua_vector<mod_reference::lua_reference>;


    private:
        std::array<std::string, 8> m_game_paths;
        std::array<std::string, 8> m_user_paths;

        host::sandbox::file_reference::lua_reference m_current_save_file { nullptr };
        host::sandbox::mod_reference::lua_reference m_active_scenario { nullptr };

        host::sandbox::directory_reference::lua_reference m_active_scenario_dir { nullptr };

        util::lua_vector<mod_reference::lua_reference> m_mods;
        util::lua_vector<mod_reference::lua_reference> m_scenarios;

        files();

        [[nodiscard]] auto packages(const directory_reference::lua_reference& directory, mod_reference::bundle_origin origin) const -> util::lua_vector<mod_reference::lua_reference>;
        [[nodiscard]] auto mods(const directory_reference::lua_reference& directory, mod_reference::bundle_origin origin) const -> util::lua_vector<mod_reference::lua_reference>;

        auto load_mods(const util::lua_vector<file_reference::lua_reference>& mods, mod_reference::bundle_origin) -> util::lua_vector<mod_reference::lua_reference>;
    };

}