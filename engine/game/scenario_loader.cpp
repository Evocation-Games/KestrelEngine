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
#include "game/scenario_loader.hpp"
#include "libGraphite/rsrc/manager.hpp"

// MARK: - Scenario Path

#if __APPLE__
auto kestrel::game::default_game_core_path() -> std::string
{
    return "../Resources/GameCore.ndat";
}

auto kestrel::game::default_scenario_path() -> std::string
{
    return "../Resources/DataFiles";
}
#elif __linux__
auto kestrel::game::default_game_core_path() -> std::string
{
    return "GameCore.ndat";
}

auto kestrel::game::default_scenario_path() -> std::string
{
    // TODO: Determine what the best location for this is...
    return "DataFiles";
}
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#   if defined(_WIN64)
auto kestrel::game::default_game_core_path() -> std::string
{
    return "GameCore.ndat";
}

auto kestrel::game::default_scenario_path() -> std::string
{
    // TODO: Determine what the best location for this is...
    return "DataFiles";
}
#   else
#       error "32-bit Windows is not supported."
#   endif
#else
#   error "Unknown target platform"
#endif

// MARK: - Loader

static auto ends_with(const std::string& string, const std::string& ending) -> bool
{
    if (string.length() >= ending.length()) {
        return (0 == string.compare(string.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

auto kestrel::game::load_game_core(std::string path) -> void
{
    auto file = std::make_shared<graphite::rsrc::file>(path);
    graphite::rsrc::manager::shared_manager().import_file(file);
}

auto kestrel::game::load_scenario(std::string path) -> void
{
    // TODO: Clean up... possibly pull out file system stuff to Diamane
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            std::string file_path(path + "/");
            file_path.append(ent->d_name);
            if (ends_with(file_path, ".ndat")) {
                std::cout << "Loading Scenario: " << file_path << std::endl;
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

