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
#include <libKestrel/sandbox/file/file_reference.hpp>
#include <libKestrel/sandbox/file/directory_reference.hpp>
#include <libKestrel/sandbox/file/mod_reference.hpp>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/lua/support/vector.hpp>

namespace kestrel::sandbox
{
    class lua_api(Kestrel.Sandbox.Files, Available_0_8) files
    {
    public:
        has_constructable_lua_api(files);

        enum class path_type
        {
            core, fonts, scenario, current_scenario, mods, data, saves, support
        };

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

        [[nodiscard]] auto game_packages(enum path_type type, bool load = false) const -> lua::vector<mod_reference::lua_reference>;
        [[nodiscard]] auto user_packages(enum path_type type, bool load = false) const -> lua::vector<mod_reference::lua_reference>;
        [[nodiscard]] auto all_packages(enum path_type type, bool load = false) const -> lua::vector<mod_reference::lua_reference>;

        [[nodiscard]] auto scenario_data_files() -> lua::vector<file_reference::lua_reference>;

        [[nodiscard]] auto mods() -> lua::vector<mod_reference::lua_reference>;
        [[nodiscard]] auto scenarios() -> lua::vector<mod_reference::lua_reference>;

        [[nodiscard]] auto get_current_save_file() const -> file_reference::lua_reference;
        auto set_save_file_name(const std::string& name) -> void;

        lua_getter(__gameDir, Available_0_8) static auto game() -> directory_reference::lua_reference;
        lua_getter(__userDir, Available_0_8) static auto user() -> directory_reference::lua_reference;

        lua_getter(activeScenario, Available_0_8) static auto active_scenario() -> mod_reference::lua_reference;
        lua_getter(activeScenarioDirectory, Available_0_8) static auto scenario_data() -> directory_reference::lua_reference;
        lua_function(setActiveScenario, Available_0_8) static auto set_active_scenario(mod_reference::lua_reference scenario) -> void;

        lua_getter(__gameCore, Available_0_8) static auto game_core() -> file_reference::lua_reference;
        lua_getter(__gameSupport, Available_0_8) static auto game_support() -> file_reference::lua_reference;
        lua_getter(gameScenariosDirectory, Available_0_8) static auto game_scenarios() -> directory_reference::lua_reference;
        lua_getter(gameDataDirectory, Available_0_8) static auto game_data() -> directory_reference::lua_reference;
        lua_getter(gameFontsDirectory, Available_0_8) static auto game_fonts() -> directory_reference::lua_reference;
        lua_getter(gameModsDirectory, Available_0_8) static auto game_mods() -> directory_reference::lua_reference;

        lua_getter(modsDirectory, Available_0_8) static auto user_mods() -> directory_reference::lua_reference;
        lua_getter(savesDirectory, Available_0_8) static auto user_saves() -> directory_reference::lua_reference;
        lua_getter(currentSaveFile, Available_0_8) static auto current_save_file() -> file_reference::lua_reference;

        lua_function(setSaveFileName, Available_0_8) static auto set_save_file(const std::string& name) -> void;
        lua_function(save, Available_0_8) static auto save() -> void;

        lua_function(allScenarios, Available_0_8) static auto all_scenarios() -> lua::vector<mod_reference::lua_reference>;
        lua_function(allScenarioMods, Available_0_8) static auto all_scenario_mods(const mod_reference::lua_reference& scenario) -> lua::vector<mod_reference::lua_reference>;
        lua_function(allActiveScenarioMods, Available_0_8) static auto all_active_scenario_mods() -> lua::vector<mod_reference::lua_reference>;


    private:
        std::array<std::string, 8> m_game_paths;
        std::array<std::string, 8> m_user_paths;

        file_reference::lua_reference m_current_save_file { nullptr };
        mod_reference::lua_reference m_active_scenario { nullptr };

        directory_reference::lua_reference m_active_scenario_dir { nullptr };

        lua::vector<mod_reference::lua_reference> m_mods;
        lua::vector<mod_reference::lua_reference> m_scenarios;

        files();

        [[nodiscard]] auto packages(const directory_reference::lua_reference& directory, mod_reference::bundle_origin origin) const -> lua::vector<mod_reference::lua_reference>;
        [[nodiscard]] auto mods(const directory_reference::lua_reference& directory, mod_reference::bundle_origin origin) const -> lua::vector<mod_reference::lua_reference>;

        auto load_mods(const lua::vector<file_reference::lua_reference>& mods, mod_reference::bundle_origin) -> lua::vector<mod_reference::lua_reference>;
    };

}