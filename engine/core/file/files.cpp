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

#include <stdexcept>
#include <algorithm>
#include <libGraphite/rsrc/manager.hpp>
#include "core/file/files.hpp"
#include "core/environment.hpp"

// MARK: - Lua

auto host::sandbox::files::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginNamespace("Kestrel")
            .beginNamespace("Sandbox")
                .beginClass<host::sandbox::files>("Files")
                    .addStaticProperty("__userDir", &host::sandbox::files::user)
                    .addStaticProperty("__gameDir", &host::sandbox::files::game)
                    .addStaticProperty("__gameCore", &host::sandbox::files::game_core)
                    .addStaticProperty("gameDataDirectory", &host::sandbox::files::game_data)
                    .addStaticProperty("gameScenariosDirectory", &host::sandbox::files::game_scenarios)
                    .addStaticProperty("gameFontsDirectory", &host::sandbox::files::game_fonts)
                    .addStaticProperty("gameModsDirectory", &host::sandbox::files::game_mods)
                    .addStaticProperty("savesDirectory", &host::sandbox::files::user_saves)
                    .addStaticProperty("modsDirectory", &host::sandbox::files::user_mods)
                    .addStaticProperty("activeScenario", &host::sandbox::files::active_scenario)
                    .addStaticProperty("activeScenarioDirectory", &host::sandbox::files::scenario_data)
                    .addStaticProperty("currentSaveFile", &host::sandbox::files::current_save_file)
                    .addStaticFunction("setSaveFileName", &host::sandbox::files::set_save_file)
                    .addStaticFunction("save", &host::sandbox::files::save)
                    .addStaticFunction("allScenarioMods", &host::sandbox::files::all_scenario_mods)
                    .addStaticFunction("allActiveScenarioMods", &host::sandbox::files::all_active_scenario_mods)
                    .addStaticFunction("allScenarios", &host::sandbox::files::all_scenarios)
                    .addStaticFunction("setActiveScenario", &host::sandbox::files::set_active_scenario)
                .endClass()
            .endNamespace()
        .endNamespace();
}

// MARK: - Construction

host::sandbox::files::files()
{
    // Game Directories
    set_game_path(game()->file("GameCore.kdat")->path(), path_type::core);
    set_game_path(game()->file("Support.kdat")->path(), path_type::support);
    set_game_path(game()->directory("Scenario")->path(), path_type::scenario);
    set_game_path(game()->directory("Mods")->path(), path_type::mods);
    set_game_path(game()->directory("Fonts")->path(), path_type::fonts);

    // User Directories
    set_user_path(user()->directory("Mods")->path(), path_type::mods);
    set_user_path(user()->directory("Saves")->path(), path_type::saves);
}

auto host::sandbox::files::shared_files() -> host::sandbox::files&
{
    static host::sandbox::files instance;
    return instance;
}

auto host::sandbox::files::get_current_save_file() const -> host::sandbox::file_reference::lua_reference
{
    return m_current_save_file;
}

auto host::sandbox::files::set_save_file_name(const std::string& name) -> void
{
    // If we have an existing save file then we need to remove it from the resource manager.
    if (m_current_save_file.get()) {
        graphite::rsrc::manager::shared_manager().unload_file(m_current_save_file->path());
    }

    // Setup the new file and attempt to load it if it exists.
    m_current_save_file = files::user_saves()->file(name);
    if (m_current_save_file.get() && m_current_save_file->exists()) {
        auto file = new graphite::rsrc::file(m_current_save_file->path());
        graphite::rsrc::manager::shared_manager().import_file(file);
    }
}

// MARK: - Active Scenario

auto host::sandbox::files::active_scenario() -> mod_reference::lua_reference
{
    return shared_files().m_active_scenario;
}

auto host::sandbox::files::set_active_scenario(mod_reference::lua_reference scenario) -> void
{
    shared_files().m_active_scenario = scenario;
    shared_files().m_active_scenario_dir = { new directory_reference(scenario->path()) };
}

auto host::sandbox::files::scenario_data() -> host::sandbox::directory_reference::lua_reference
{
    return shared_files().m_active_scenario_dir;
}

auto host::sandbox::files::scenario_data_files() -> util::lua_vector<file_reference::lua_reference>
{
    return scenario_data()->contents(false);
}

// MARK: - Known Locations

#if __APPLE__
#include "core/support/macos/cocoa/application.h"

auto host::sandbox::files::game() -> host::sandbox::directory_reference::lua_reference
{
    return { new directory_reference(cocoa::application::bundle_path() + "/Contents/Resources") };
}

auto host::sandbox::files::user() -> host::sandbox::directory_reference::lua_reference
{
    return { new directory_reference(cocoa::application::application_support_path()) };
}

#elif __linux__

auto host::sandbox::files::game() -> host::sandbox::directory_reference::lua_reference
{
    std::string path { "" };
    if (const char *app_dir = std::getenv("APPDIR")) {
        path = std::string(app_dir) + "/usr/share" + path;
    }
    return { new directory_reference(path) };
}

auto host::sandbox::files::user() -> host::sandbox::directory_reference::lua_reference
{
    return { new directory_reference("") };
}

#else

auto host::sandbox::files::game() -> host::sandbox::directory_reference::lua_reference
{
    return { new directory_reference("Data") };
}

auto host::sandbox::files::user() -> host::sandbox::directory_reference::lua_reference
{
    return { new directory_reference("") };
}

#endif

auto host::sandbox::files::set_game_path(const std::string &path, enum path_type type) -> void
{
    m_game_paths[static_cast<int>(type)] = path;
}

auto host::sandbox::files::game_path(enum path_type type) const -> std::string
{
    return m_game_paths[static_cast<int>(type)];
}

auto host::sandbox::files::game_file(enum path_type type) const -> file_reference::lua_reference
{
    return { new file_reference(game_path(type)) };
}

auto host::sandbox::files::set_user_path(const std::string &path, enum path_type type) -> void
{
    m_user_paths[static_cast<int>(type)] = path;
}

auto host::sandbox::files::user_path(enum path_type type) const -> std::string
{
    return m_user_paths[static_cast<int>(type)];
}

auto host::sandbox::files::user_file(enum path_type type) const -> file_reference::lua_reference
{
    return { new file_reference(user_path(type)) };
}

auto host::sandbox::files::game_core() -> host::sandbox::file_reference::lua_reference
{
    return shared_files().game_file(path_type::core);
}

auto host::sandbox::files::game_support() -> host::sandbox::file_reference::lua_reference
{
    return shared_files().game_file(path_type::support);
}

auto host::sandbox::files::game_scenarios() -> host::sandbox::directory_reference::lua_reference
{
    return directory_reference::lua_reference {
        new directory_reference(shared_files().game_path(path_type::scenario))
    };
}

auto host::sandbox::files::game_data() -> host::sandbox::directory_reference::lua_reference
{
    return scenario_data();
}

auto host::sandbox::files::game_fonts() -> host::sandbox::directory_reference::lua_reference
{
    return directory_reference::lua_reference {
        new directory_reference(shared_files().game_path(path_type::fonts))
    };
}

auto host::sandbox::files::game_mods() -> host::sandbox::directory_reference::lua_reference
{
    return directory_reference::lua_reference {
        new directory_reference(shared_files().game_path(path_type::mods))
    };
}

auto host::sandbox::files::user_mods() -> host::sandbox::directory_reference::lua_reference
{
    return directory_reference::lua_reference {
        new directory_reference(shared_files().user_path(path_type::mods))
    };
}

auto host::sandbox::files::user_saves() -> host::sandbox::directory_reference::lua_reference
{
    return directory_reference::lua_reference {
        new directory_reference(shared_files().user_path(path_type::saves))
    };
}

// MARK: - Saving / Saves

auto host::sandbox::files::current_save_file() -> host::sandbox::file_reference::lua_reference
{
    return shared_files().get_current_save_file();
}

auto host::sandbox::files::set_save_file(const std::string& name) -> void
{
    return shared_files().set_save_file_name(name);
}

auto host::sandbox::files::save() -> void
{
    if (current_save_file().get()) {
        for (auto& file : graphite::rsrc::manager::shared_manager().file_references()) {
            if (file->path() == current_save_file()->path()) {
                file->write(file->path(), graphite::rsrc::file::format::extended);
                return;
            }
        }
    }
}

// MARK: - Mod Discovery

auto host::sandbox::files::packages(const directory_reference::lua_reference& directory, mod_reference::bundle_origin origin) const -> util::lua_vector<mod_reference::lua_reference>
{
    util::lua_vector<mod_reference::lua_reference> packages;

    auto env = environment::active_environment().lock();
    if (!env) {
        throw std::runtime_error("Missing environment whilst finding packages.");
    }

    if (!directory.get()) {
        return {};
    }

    auto contents = directory->contents(false);
    for (auto i = 0; i < contents.size(); ++i) {
        const auto& file = contents.at(i);

        if (file->is_directory() && (file->extension() == "modpackage" || file->extension() == "scenario")) {
            mod_reference::lua_reference package {
                new mod_reference(file->path(), origin, mod_reference::bundle_type::package)
            };

            if (!package->validate_as_modpackage()) {
                continue;
            }

            packages.emplace_back(package);
            package->parse_modpackage();
        }
    }

    return packages;
}

auto host::sandbox::files::mods(const directory_reference::lua_reference& directory, mod_reference::bundle_origin origin) const -> util::lua_vector<mod_reference::lua_reference>
{
    util::lua_vector<mod_reference::lua_reference> mods;

    auto packages = this->packages(directory, origin);
    for (auto i = 0; i < packages.size(); ++i) {
        const auto& package = packages.at(i);
        if (package->is_scenario()) {
            continue;
        }
        mods.emplace_back(package);
    }

    const auto& contents = directory->contents(false);
    for (auto i = 0; i < contents.size(); ++i) {
        const auto& file = contents.at(i);

        if (file->extension() == "kdat" || file->extension() == "ndat" || file->extension() == "rez" || file->extension() == "rsrc") {
            mod_reference::lua_reference mod {
                new mod_reference(file->path(), origin, mod_reference::bundle_type::simple)
            };

            mods.emplace_back(mod);
            if (mod->validate_as_simplemod()) {
                mod->parse_simplemod();
            }
            else {
                mod->construct_simplemod();
            }
        }
    }

    return mods;
}

auto host::sandbox::files::game_packages(enum path_type type, bool load) const -> util::lua_vector<mod_reference::lua_reference>
{
    return packages(directory_reference::get(game_path(type)), mod_reference::bundle_origin::game);
}

auto host::sandbox::files::user_packages(enum path_type type, bool load) const -> util::lua_vector<mod_reference::lua_reference>
{
    return packages(directory_reference::get(user_path(type)), mod_reference::bundle_origin::user);
}

auto host::sandbox::files::all_packages(enum path_type type, bool load) const -> util::lua_vector<mod_reference::lua_reference>
{
    util::lua_vector<mod_reference::lua_reference> result;
    result.append(game_packages(type, load));
    result.append(user_packages(type, load));
    return result;
}


auto host::sandbox::files::mods() -> util::lua_vector<mod_reference::lua_reference>
{
    if (m_mods.empty()) {
        m_mods.append(mods(game_mods(), mod_reference::bundle_origin::game));
        m_mods.append(mods(user_mods(), mod_reference::bundle_origin::user));
    }
    return m_mods;
}

auto host::sandbox::files::scenarios() -> util::lua_vector<mod_reference::lua_reference>
{
    if (m_scenarios.empty()) {
        auto scenario_packages = all_packages(path_type::scenario);
        auto mod_packages = all_packages(path_type::mods);

        for (auto i = 0; i < scenario_packages.size(); ++i) {
            const auto& package = scenario_packages.at(i);
            if (package->is_scenario()) {
                m_scenarios.emplace_back(package);
            }
        }

        for (auto i = 0; i < mod_packages.size(); ++i) {
            const auto& package = mod_packages.at(i);
            if (package->is_scenario()) {
                m_scenarios.emplace_back(package);
            }
        }
    }

    return m_scenarios;
}

auto host::sandbox::files::all_scenarios() -> util::lua_vector<mod_reference::lua_reference>
{
    return shared_files().scenarios();
}

auto host::sandbox::files::all_active_scenario_mods() -> util::lua_vector<mod_reference::lua_reference>
{
    return all_scenario_mods(active_scenario());
}

auto host::sandbox::files::all_scenario_mods(const mod_reference::lua_reference& scenario) -> util::lua_vector<mod_reference::lua_reference>
{
    if (!scenario.get()) {
        return {};
    }

    auto package_id = scenario->package_id();

    util::lua_vector<mod_reference::lua_reference> scenario_mods;
    auto mods = shared_files().mods();

    for (auto i = 0; i < mods.size(); ++i) {
        const auto& mod = mods.at(i);
        if (mod->scenario_id() == package_id) {
            scenario_mods.emplace_back(mod);
        }
    }

    return scenario_mods;
}
