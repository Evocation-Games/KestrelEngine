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
#include <utility>
#include "scripting/state.hpp"
#include "core/graphics/common/scene.hpp"

#if __APPLE__
#include "core/graphics/metal/metal_session_window.h"
#include "core/support/macos/cocoa/application.h"
#endif

static std::weak_ptr<environment> $_active_environment;

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

    // Cache the data path locations... first assign the defaults, and then search for an alternate path in the
    // options.
    m_kestrel_core_path = kestrel_core_path();
    m_game_data_path = game_data_path();

    // Load all resource files.
    load_kestrel_core();
    load_game_data();
}

auto environment::active_environment() -> std::weak_ptr<environment>
{
    return $_active_environment;
}

// MARK: - Accessors

auto environment::cache() -> std::shared_ptr<asset::cache>
{
    return m_cache;
}

// MARK: - Run Loop

#if __APPLE__
auto environment::launch_metal() -> int
{
    auto app = cocoa::application();
    return app.run(m_options, [this] () {
        this->m_game_window = std::make_shared<graphics::metal::session_window>(this->shared_from_this());
        this->m_game_window->set_title("Kestrel - Metal");
        this->m_game_window->set_size({ 800, 600 });
        this->prepare_common();
    });
}
#endif

auto environment::launch_opengl() -> int
{
    m_game_window = std::make_shared<graphics::opengl::session_window>(shared_from_this());
    return launch_common();
}

auto environment::prepare_common() -> void
{
    // Ensure Lua is fully configured and ready to go.
    become_active_environment();
    m_lua_runtime->prepare_lua_environment(shared_from_this());

    // Locate and execute script #0 to enter the game itself, and then enter a run loop.
    auto main_script = m_lua_runtime->load_script(0);
    m_lua_runtime->run(main_script);
}

auto environment::launch_common() -> int
{
    prepare_common();

    // Enter the main run loop, keep calling tick on the session window until such time as it is no
    // longer in existence or alive.
    while (m_game_window && m_game_window->is_running()) {
        m_game_window->tick();
    }

    return m_status;
}

auto environment::launch() -> int
{
#if __APPLE__
    auto metal = false;
    if (metal) {
        return launch_metal();
    }
#endif

    return launch_opengl();
}

auto environment::become_active_environment() -> void
{
    $_active_environment = shared_from_this();
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
            if (ends_with(file_path, ".ndat") || ends_with(file_path, ".rez") || ends_with(file_path, ".kdat")) {
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
    return cocoa::application::bundle_path() + "/Contents/Resources/GameCore.ndat";
}

auto environment::game_data_path() const -> std::string
{
    return cocoa::application::bundle_path() + "/Contents/Resources/DataFiles";
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

// MARK: - Lua Interface

auto environment::prepare_lua_interface() -> void
{
    luabridge::getGlobalNamespace(m_lua_runtime->internal_state())
        .beginClass<environment>("Kestrel")
            .addStaticFunction("setGameWindowTitle", &environment::set_game_window_title)
            .addStaticFunction("setGameWindowSize", &environment::set_game_window_size)
            .addStaticFunction("importScript", &environment::import_script)
            .addStaticFunction("scene", &environment::create_scene)
        .endClass();
}

auto environment::set_game_window_title(const std::string &title) -> void
{
    $_active_environment.lock()->m_game_window->set_title(title);
}

auto environment::set_game_window_size(const double& width, const double& height) -> void
{
    $_active_environment.lock()->m_game_window->set_size({ width, height });
}

auto environment::load_script(const asset::resource_reference::lua_reference &ref) -> scripting::lua::script
{
    if (auto env = $_active_environment.lock()) {
        if (ref->id().has_value()) {
            return env->m_lua_runtime->load_script(ref->id().value());
        }
        else if (ref->name().has_value()) {
            throw std::logic_error("Unable to load script.");
        }
        else {
            throw std::logic_error("Unable to load script.");
        }
    }
    throw std::runtime_error("Missing environment");
}

auto environment::import_script(const asset::resource_reference::lua_reference& ref) -> void
{
    if (auto env = $_active_environment.lock()) {
        if (ref->id().has_value()) {
            auto scpt = env->m_lua_runtime->load_script(ref->id().value());
            env->m_lua_runtime->run(scpt);
        }
        else if (ref->name().has_value()) {

        }
        else {

        }
    }
}

// MARK: - Accessors

auto environment::window() -> std::shared_ptr<graphics::session_window>
{
    return m_game_window;
}


// MARK: - Graphics Layer Specific

auto environment::create_texture(const math::size &size,
                                 std::vector<uint32_t> data) const -> std::shared_ptr<graphics::texture>
{
    return m_game_window->create_texture(size, std::move(data));
}

auto environment::current_scene() -> std::shared_ptr<graphics::scene>
{
    return m_game_window->current_scene();
}

auto environment::present_scene(std::shared_ptr<graphics::scene> scene) -> void
{
    m_game_window->present_scene(std::move(scene));
}

auto environment::pop_scene() -> void
{
    m_game_window->pop_scene();
}

auto environment::create_scene(const std::string &name,
                               const asset::resource_reference::lua_reference &script) -> graphics::lua_scene_wrapper::lua_reference
{
    if (auto env = $_active_environment.lock()) {
        auto scene = env->m_game_window->new_scene(name, env->load_script(script));
        return graphics::lua_scene_wrapper::lua_reference(new graphics::lua_scene_wrapper(scene));
    }
    else {
        return nullptr;
    }
}

// MARK: - Event Posting

auto environment::post_key_event(const event::key &event) -> void
{
#if __APPLE__
    if ((event.code() == event::key::q) && (event.modifiers() & event::key::super)) {
        exit(0);
    }
#elif __linux__

#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#   if defined(_WIN64)

#   endif
#endif
    auto scene = current_scene();
    if (scene != nullptr) {
        scene->key_event(event);
    }
}

auto environment::post_mouse_event(const event::mouse &event) -> void
{
    auto scene = current_scene();
    if (scene != nullptr) {
        scene->mouse_event(event);
    }
}