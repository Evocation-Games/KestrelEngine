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

#include "core/file/files.hpp"

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
    m_current_save_file = files::user_saves()->file(name);
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
