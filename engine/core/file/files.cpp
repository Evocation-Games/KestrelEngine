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
                    .addStaticProperty("gameFontsDirectory", &host::sandbox::files::game_fonts)
                    .addStaticProperty("gameModsDirectory", &host::sandbox::files::game_mods)
                    .addStaticProperty("savesDirectory", &host::sandbox::files::user_saves)
                    .addStaticProperty("modsDirectory", &host::sandbox::files::user_mods)
                    .addStaticProperty("currentSaveFile", &host::sandbox::files::current_save_file)
                    .addStaticFunction("setSaveFileName", &host::sandbox::files::set_save_file)
                    .addStaticFunction("save", &host::sandbox::files::save)
                    .addStaticFunction("preloadAllMods", &host::sandbox::files::preload_all_mods)
                    .addStaticFunction("preloadUserMods", &host::sandbox::files::preload_user_mods)
                    .addStaticFunction("preloadGameMods", &host::sandbox::files::preload_game_mods)
                    .addStaticFunction("allMods", &host::sandbox::files::all_mods)
                .endClass()
            .endNamespace()
        .endNamespace();
}

// MARK: - Construction

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
        auto file = std::make_shared<graphite::rsrc::file>(m_current_save_file->path());
        graphite::rsrc::manager::shared_manager().import_file(file);
    }
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

auto host::sandbox::files::game_core() -> host::sandbox::file_reference::lua_reference
{
    if (shared_files().m_game_core.get()) {
        return shared_files().m_game_core;
    }
    return game()->file("GameCore.kdat");
}

auto host::sandbox::files::game_data() -> host::sandbox::directory_reference::lua_reference
{
    if (shared_files().m_game_data.get()) {
        return shared_files().m_game_data;
    }
    return game()->directory("Scenario");
}

auto host::sandbox::files::game_fonts() -> host::sandbox::directory_reference::lua_reference
{
    if (shared_files().m_game_fonts.get()) {
        return shared_files().m_game_fonts;
    }
    return game()->directory("Fonts");
}

auto host::sandbox::files::game_mods() -> host::sandbox::directory_reference::lua_reference
{
    if (shared_files().m_game_mods.get()) {
        return shared_files().m_game_mods;
    }
    return game()->directory("Mods");
}

auto host::sandbox::files::user_mods() -> host::sandbox::directory_reference::lua_reference
{
    return user()->directory("Mods");
}

auto host::sandbox::files::user_saves() -> host::sandbox::directory_reference::lua_reference
{
    return user()->directory("Saves");
}

auto host::sandbox::files::current_save_file() -> host::sandbox::file_reference::lua_reference
{
    return shared_files().get_current_save_file();
}

auto host::sandbox::files::set_save_file(const std::string& name) -> void
{
    return shared_files().set_save_file_name(name);
}

// MARK: - Overrides

auto host::sandbox::files::set_game_core_path(const std::string& path) -> void
{
    m_game_core = { new file_reference(path) };
}

auto host::sandbox::files::set_game_data_path(const std::string& path) -> void
{
    m_game_data = { new directory_reference(path) };
}

auto host::sandbox::files::set_game_fonts_path(const std::string& path) -> void
{
    m_game_fonts = { new directory_reference(path) };
}

auto host::sandbox::files::set_game_mods_path(const std::string& path) -> void
{
    m_game_mods = { new directory_reference(path) };
}

// MARK: - Saving

auto host::sandbox::files::save() -> void
{
    if (current_save_file().get()) {
        for (auto& file : graphite::rsrc::manager::shared_manager().files()) {
            if (file->path() == current_save_file()->path()) {
                file->write(file->path(), graphite::rsrc::file::extended);
                return;
            }
        }
    }
}

// MARK: - Mod Discovery

auto host::sandbox::files::mods() const -> util::lua_vector<mod_reference::lua_reference>
{
    return util::lua_vector(m_mods);
}

auto host::sandbox::files::internal_preload_all_mods() -> util::lua_vector<mod_reference::lua_reference>
{
    internal_preload_game_mods();
    internal_preload_user_mods();
    return util::lua_vector(m_mods);
}

auto host::sandbox::files::internal_preload_user_mods() -> util::lua_vector<mod_reference::lua_reference>
{
    if (m_user_mods_loaded) {
        if (auto env = environment::active_environment().lock()) {
            env->lua_out("Attempted to preload user mods, after they were already preloaded.", true);
        }
        return util::lua_vector(m_mods);
    }
    m_user_mods_loaded = true;

    // Discover user mods.
    const auto& user_mods = this->user_mods()->contents(false);
    return load_mods(user_mods, mod_reference::bundle_origin::user);
}


auto host::sandbox::files::internal_preload_game_mods() -> util::lua_vector<mod_reference::lua_reference>
{
    if (m_game_mods_loaded) {
        if (auto env = environment::active_environment().lock()) {
            env->lua_out("Attempted to preload game mods, after they were already preloaded.", true);
        }
        return util::lua_vector(m_mods);
    }
    m_game_mods_loaded = true;

    // Discover game mods.
    const auto& game_mods = this->game_mods()->contents(false);
    return load_mods(game_mods, mod_reference::bundle_origin::game);
}

auto host::sandbox::files::load_mods(const util::lua_vector<file_reference::lua_reference> &mods, mod_reference::bundle_origin) -> util::lua_vector<mod_reference::lua_reference>
{
    util::lua_vector<mod_reference::lua_reference> loaded_mods;

    auto env = environment::active_environment().lock();
    if (!env) {
        throw std::runtime_error("Missing environment whilst discovering mods.");
    }

    for (auto i = 0; i < mods.size(); ++i) {
        const auto& file = mods.at(i);

        if (file->is_directory() && file->extension() == "modpackage") {
            // This should be a bundle/package mod. We will need to setup the mod reference, and then have it validated
            // before including it.
            mod_reference::lua_reference mod {
                new mod_reference(file->path(), mod_reference::bundle_origin::user, mod_reference::bundle_type::package)
            };

            if (!mod->validate_as_modpackage()) {
                continue;
            }

            loaded_mods.emplace_back(mod);
            mod->parse_modpackage();
        }
        else if (file->extension() == "kdat" || file->extension() == "ndat" || file->extension() == "rez" || file->extension() == "rsrc") {
            // This is a simple mod.
            mod_reference::lua_reference mod {
                new mod_reference(file->path(), mod_reference::bundle_origin::user, mod_reference::bundle_type::simple)
            };

            loaded_mods.emplace_back(mod);
            if (mod->validate_as_simplemod()) {
                mod->parse_simplemod();
            }
            else {
                mod->construct_simplemod();
            }
        }
    }

    // Construct a small namespace containing the mods.
    for (auto i = 0; i < loaded_mods.size(); ++i) {
        const auto& mod = loaded_mods.at(i);

        env->lua_runtime()->global_namespace()
            .beginNamespace("Kestrel")
                .beginNamespace("Mods")
                    .addProperty(mod->primary_namespace().c_str(), mod.get())
                .endNamespace()
            .endNamespace();

        m_mods.emplace_back(mod);
    }

    return loaded_mods;
}

auto host::sandbox::files::preload_all_mods() -> util::lua_vector<mod_reference::lua_reference>
{
    return shared_files().internal_preload_all_mods();
}

auto host::sandbox::files::preload_user_mods() -> util::lua_vector<mod_reference::lua_reference>
{
    return shared_files().internal_preload_user_mods();
}

auto host::sandbox::files::preload_game_mods() -> util::lua_vector<mod_reference::lua_reference>
{
    return shared_files().internal_preload_game_mods();
}

auto host::sandbox::files::all_mods() -> util::lua_vector<mod_reference::lua_reference>
{
    return shared_files().mods();
}
