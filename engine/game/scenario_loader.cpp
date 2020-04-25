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

#include "game/scenario_loader.hpp"
#include "libGraphite/rsrc/manager.hpp"

// MARK: - Scenario Path

#if __APPLE__
auto kestrel::game::default_scenario_path() -> std::string
{
    return "../Resources/GameCore.ndat";
}
#elif __linux__
auto kestrel::game::default_scenario_path() -> std::string
{
    // TODO: Determine what the best location for this is...
    return "GameCore.ndat";
}
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#   if defined(_WIN64)
auto kestrel::game::default_scenario_path() -> std::string
{
    // TODO: Determine what the best location for this is...
    return "GameCore.ndat";
}
#   else
#       error "32-bit Windows is not supported."
#   endif
#else
#   error "Unknown target platform"
#endif

// MARK: - Loader

auto kestrel::game::load_scenario(std::string path) -> void
{
    auto file = std::make_shared<graphite::rsrc::file>(path);
    graphite::rsrc::manager::shared_manager().import_file(file);
}