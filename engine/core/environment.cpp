// Copyright (c) 2020 Tom Hancocks
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

#include <dirent.h>
#include <iostream>
#include "core/environment.hpp"
#include "core/graphics/opengl/opengl_session_window.hpp"
#include <libGraphite/rsrc/manager.hpp>

// MARK: - Helpers

static auto ends_with(const std::string& string, const std::string& ending) -> bool
{
    if (string.length() >= ending.length()) {
        return (string.compare(string.length() - ending.length(), ending.length(), ending) == 0);
    }
    else {
        return false;
    }
}

// MARK: - Construction

environment::environment(int argc, const char **argv)
{
    for (auto i = 1; i < argc; ++i) {
        m_options.emplace_back(std::string(argv[i]));
    }

    // Configure the lua runtime
    m_lua_runtime = std::make_shared<scripting::lua::state>();
    m_lua_runtime->prepare_lua_environment();

    // Cache the data path locations... first assign the defaults, and then search for an alternate path in the
    // options.
    m_kestrel_core_path = kestrel_core_path();
    m_game_data_path = game_data_path();

    // Load all resource files.
    load_kestrel_core();
//    load_game_data();
}

// MARK: - Run Loop

auto environment::launch() -> int
{
    // Determine which graphics mode we are running in, and instantiate the correct session_window
    // subclass.
    // TODO: Add in alternate modes as they are implemented, and bind to appropriate platforms.
    m_game_window = std::make_shared<graphics::opengl::session_window>(shared_from_this());

    // Locate and execute script #0 to enter the game itself, and then enter a run loop.
    auto main_script = m_lua_runtime->load_script(0);
    m_lua_runtime->run(main_script);

    // Enter the main run loop, keep calling tick on the session window until such time as it is no
    // longer in existence or alive.
    while (m_game_window && m_game_window->is_running()) {
        m_game_window->tick();
    }

    return m_status;
}

// MARK: - Loading

auto environment::load_kestrel_core() -> void
{
    auto file = std::make_shared<graphite::rsrc::file>(m_kestrel_core_path);
    graphite::rsrc::manager::shared_manager().import_file(file);
}

auto environment::load_game_data() -> void
{
    load_data_files(m_game_data_path);
}

auto environment::load_data_files(const std::string &path) -> void
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            std::string file_path(path + "/");
            file_path.append(ent->d_name);
            if (ends_with(file_path, ".ndat")) {
                std::cout << "loading data file: " << file_path << std::endl;
                auto file = std::make_shared<graphite::rsrc::file>(file_path);
                graphite::rsrc::manager::shared_manager().import_file(file);
            }
        }
        closedir(dir);
    }
    else {
        perror("Failed to load scenario files");
    }
}

// MARK: - Platform Specifics

#if __APPLE__

auto environment::kestrel_core_path() const -> std::string
{
    return "../Resources/GameCore.ndat";
}

auto environment::game_data_path() const -> std::string
{
    return "../Resources/DataFiles";
}

#elif __linux__

auto environment::kestrel_core_path() const -> std::string
{
    return "GameCore.ndat";
}

auto environment::game_data_path() const -> std::string
{
    return "DataFiles";
}

#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#   if defined(_WIN64)

auto environment::kestrel_core_path() const -> std::string
{
    return "GameCore.ndat";
}

auto environment::game_data_path() const -> std::string
{
    return "DataFiles";
}

#   else
#       error "32-bit Windows is not supported."
#   endif
#else
#   error "Unknown Target Platform"
#endif
