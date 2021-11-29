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
#include <utility>
#include <libGraphite/rsrc/manager.hpp>
#include "core/environment.hpp"
#include "scripting/state.hpp"
#include "core/graphics/opengl/opengl_session_window.hpp"
#include "core/graphics/common/scene.hpp"
#include "core/graphics/common/font.hpp"
#include "core/audio/audio_manager.hpp"

#if __APPLE__
#include "core/graphics/metal/metal_session_window.h"
#include "core/support/macos/cocoa/application.h"
#include "core/support/macos/cocoa/font.h"
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
    if (std::find(m_options.begin(), m_options.end(), "--game") != m_options.end()) {
        for (auto i = 0; i < m_options.size(); ++i) {
            if (m_options[i] == "--game") {
                m_kestrel_core_path = m_options[++i];
                break;
            }
        }
    }
    else {
        m_kestrel_core_path = kestrel_core_path();
    }

    if (std::find(m_options.begin(), m_options.end(), "--data") != m_options.end()) {
        for (auto i = 0; i < m_options.size(); ++i) {
            if (m_options[i] == "--data") {
                m_game_data_path = m_options[++i];
                break;
            }
        }
    }
    else {
        m_game_data_path = game_data_path();
    }

    if (std::find(m_options.begin(), m_options.end(), "--fonts") != m_options.end()) {
        for (auto i = 0; i < m_options.size(); ++i) {
            if (m_options[i] == "--fonts") {
                m_game_fonts_path = m_options[++i];
                break;
            }
        }
    }
    else {
        m_game_fonts_path = game_fonts_path();
    }

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
auto environment::launch_metal(const double& scale) -> int
{
    auto app = cocoa::application();;
    return app.run(m_options, [this, scale] () {
        this->m_game_window = std::make_shared<graphics::metal::session_window>(this->shared_from_this(), scale);
        this->m_game_window->set_title("Kestrel - Metal");
        this->m_game_window->set_size({ 800, 600 });
        this->m_gl = gl_type::metal;
        this->prepare_common();
    });
}
#endif

auto environment::launch_opengl(const double& scale) -> int
{
    m_game_window = std::make_shared<graphics::opengl::session_window>(shared_from_this(), scale);
    m_gl = gl_type::open_gl;
    return launch_common();
}

auto environment::prepare_common() -> void
{
    // Ensure Lua is fully configured and ready to go.
    become_active_environment();
    m_lua_runtime->prepare_lua_environment(shared_from_this());

    // Ensure the audio manager is started and configured.
    audio::manager::shared_manager().configure();

    // Locate and execute script #0 to enter the game itself, and then enter a run loop.
    auto state = m_lua_runtime->shared_from_this();
    scripting::lua::script main_script(state, asset::resource_descriptor::identified(0));
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
    double scale = 0.0;
    if (std::find(m_options.begin(), m_options.end(), "--scale") != m_options.end()) {
        for (auto i = 0; i < m_options.size(); ++i) {
            if (m_options[i] == "--scale") {
                scale = std::atof(m_options[++i].c_str());
                break;
            }
        }
    }

#if __APPLE__
    if (scale <= 0) {
        scale = cocoa::application::screen_scale_factor();
    }

    // Check if we're being forced to open the game in a certain graphics mode. If we are then we can ignore the
    // metal check.
    if (std::find(m_options.begin(), m_options.end(), "--opengl") != m_options.end()) {
        // We are loading in OpenGL
    }
    else {
        // We are going to try for Metal, but if the computer is not able to then we will default to OpenGL.
        auto metal = true;
        if (metal) {
            return launch_metal(scale);
        }
    }
#else
    if (scale <= 0) {
        scale = 1.0;
    }
#endif

    return launch_opengl(scale);
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
    load_font_files(m_game_fonts_path);
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
                auto file = std::make_shared<graphite::rsrc::file>(file_path);
                graphite::rsrc::manager::shared_manager().import_file(file);
            }
            else if (ends_with(file_path, ".mp3")) {
                m_audio_files.emplace_back(file_path);
            }
        }
        closedir(dir);
    }
    else {
        perror("Failed to load scenario files");
    }
}

auto environment::load_font_files(const std::string &path) -> void
{
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            std::string file_path(path + "/");
            file_path.append(ent->d_name);
            if (ends_with(file_path, ".ttf")) {
                auto name = graphics::font::font_name_at_path(file_path);
                m_custom_fonts[name] = file_path;
            }
        }
        closedir(dir);
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

auto environment::game_fonts_path() const -> std::string
{
    return cocoa::application::bundle_path() + "/Contents/Resources/Fonts";
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

auto environment::game_fonts_path() const -> std::string
{
    return "Fonts";
}

#elif (defined(_WIN32) || defined(_WIN64))

auto environment::kestrel_core_path() const -> std::string
{
    return "GameCore.ndat";
}

auto environment::game_data_path() const -> std::string
{
    return "DataFiles";
}

auto environment::game_fonts_path() const -> std::string
{
    return "Fonts";
}

#else
#   error "Unknown Target Platform"
#endif

// MARK: - Lua Interface

auto environment::prepare_lua_interface() -> void
{
    luabridge::getGlobalNamespace(m_lua_runtime->internal_state())
        .beginNamespace("Kestrel")
            .addFunction("setGameWindowTitle", &environment::set_game_window_title)
            .addFunction("setGameWindowSize", &environment::set_game_window_size)
            .addFunction("importScript", &environment::import_script)
            .addFunction("scene", &environment::create_scene)
            .addFunction("scaleFactor", &environment::scale)
            .addFunction("playAudioFile", &environment::play_audio_file)
            .addFunction("stopAudioFile", &environment::stop_audio_file)
            .addProperty("fileSystem", &environment::filesystem)
            .addProperty("platform", &environment::platform)
            .addProperty("platformName", &environment::platform_name)
            .addProperty("graphicsLayerName", &environment::gl_name)
            .addProperty("allAudioFiles", &environment::audio_files)
        .endNamespace();
}

auto environment::set_game_window_title(const std::string &title) -> void
{
    $_active_environment.lock()->m_game_window->set_title(title);
}

auto environment::set_game_window_size(const double& width, const double& height) -> void
{
    $_active_environment.lock()->m_game_window->set_size({ width, height });
}

auto environment::scale() -> double
{
    return $_active_environment.lock()->m_game_window->get_scale_factor();
}

auto environment::load_script(const asset::resource_descriptor::lua_reference &ref) -> scripting::lua::script
{
    if (auto env = $_active_environment.lock()) {
        return scripting::lua::script(m_lua_runtime->shared_from_this(), ref);
    }
    throw std::runtime_error("Missing environment");
}

auto environment::import_script(const asset::resource_descriptor::lua_reference& ref) -> void
{
    if (auto env = $_active_environment.lock()) {
        scripting::lua::script script(env->m_lua_runtime->shared_from_this(), ref);
        env->m_lua_runtime->run(script);
    }
}

auto environment::gc_purge() -> void
{
    lua_gc(m_lua_runtime->internal_state(), LUA_GCCOLLECT, 0);
}


// MARK: - Accessors

auto environment::window() -> std::shared_ptr<graphics::session_window>
{
    return m_game_window;
}

// MARK: - Audio

auto environment::all_audio_files() -> util::lua_vector<std::string>
{
    return m_audio_files;
}

auto environment::audio_files() -> util::lua_vector<std::string>
{
    if (auto env = active_environment().lock()) {
        return env->all_audio_files();
    }
    else {
        return {};
    }
}

auto environment::play_audio_file(const std::string& file) -> void
{
    audio::manager::shared_manager().play_background_audio(file);
}

auto environment::stop_audio_file() -> void
{
    audio::manager::shared_manager().stop_background_audio();
}


// MARK: - Graphics Layer Specific

auto environment::create_texture(const math::size &size,
                                 std::vector<uint32_t> data) const -> std::shared_ptr<graphics::texture>
{
    return m_game_window->create_texture(size, std::move(data));
}

auto environment::create_texture(const math::size &size, const uint8_t *data) const -> std::shared_ptr<graphics::texture>
{
    return m_game_window->create_texture(size, data);
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

auto environment::create_scene(const std::string &name, const asset::resource_descriptor::lua_reference &script) -> graphics::lua_scene_wrapper::lua_reference
{
    if (auto env = $_active_environment.lock()) {
        auto scene = env->m_game_window->new_scene(name, env->load_script(script));
        return graphics::lua_scene_wrapper::lua_reference(new graphics::lua_scene_wrapper(scene));
    }
    else {
        return nullptr;
    }
}

auto environment::gl_name() -> std::string
{
    if (auto env = $_active_environment.lock()) {
        switch (env->m_gl) {
            case gl_type::open_gl:
                return "OpenGL";
            case gl_type::metal:
                return "Metal";
            default:
                break;
        }
    }
    return "Unknown";
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

// MARK: - Bundled Resources

auto environment::bundled_font_named(const std::string &name) const -> std::optional<std::string>
{
    if (m_custom_fonts.find(name) != m_custom_fonts.end()) {
        return m_custom_fonts.at(name);
    }
    else {
        return {};
    }
}

// MARK: - File System

auto environment::filesystem() -> host::filesystem::lua_reference
{
    return $_active_environment.lock()->m_filesystem;
}

// MARK: - Host Platform

#if __APPLE__

auto environment::platform() -> environment::platform_type
{
    return platform_type::mac_os;
}

auto environment::platform_name() -> std::string
{
    return "macOS";
}

#elif __linux__

auto environment::platform() -> environment::platform_type
{
    return platform_type::unix_like;
}

auto environment::platform_name() -> std::string
{
    return "Linux";
}

#elif (_WIN32 || _WIN64)

auto environment::platform() -> environment::platform_type
{
    return platform_type::windows;
}

auto environment::platform_name() -> std::string
{
    return "Windows";
}

#endif
