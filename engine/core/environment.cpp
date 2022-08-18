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
#include "util/hint.hpp"
#include "scripting/state.hpp"
#include "core/environment.hpp"
#include "core/graphics/common/font.hpp"
#include "core/audio/audio_manager.hpp"
#include "renderer/common/renderer.hpp"
#include "core/ui/session.hpp"
#include "core/ui/scene.hpp"
#include "core/ui/game_scene.hpp"

#if TARGET_MACOS
#include "core/support/macos/cocoa/font.h"
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
    : m_status(0)
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
                host::sandbox::files::shared_files().set_game_path(m_options[++i], host::sandbox::files::path_type::core);
                break;
            }
        }
    }

    if (std::find(m_options.begin(), m_options.end(), "--support") != m_options.end()) {
        for (auto i = 0; i < m_options.size(); ++i) {
            if (m_options[i] == "--support") {
                host::sandbox::files::shared_files().set_game_path(m_options[++i], host::sandbox::files::path_type::support);
                break;
            }
        }
    }

    if (std::find(m_options.begin(), m_options.end(), "--scenarios") != m_options.end()) {
        for (auto i = 0; i < m_options.size(); ++i) {
            if (m_options[i] == "--scenarios") {
                host::sandbox::files::shared_files().set_game_path(m_options[++i], host::sandbox::files::path_type::scenario);
                break;
            }
        }
    }

    if (std::find(m_options.begin(), m_options.end(), "--mods") != m_options.end()) {
        for (auto i = 0; i < m_options.size(); ++i) {
            if (m_options[i] == "--mods") {
                host::sandbox::files::shared_files().set_game_path(m_options[++i], host::sandbox::files::path_type::mods);
                break;
            }
        }
    }

    if (std::find(m_options.begin(), m_options.end(), "--fonts") != m_options.end()) {
        for (auto i = 0; i < m_options.size(); ++i) {
            if (m_options[i] == "--fonts") {
                host::sandbox::files::shared_files().set_game_path(m_options[++i], host::sandbox::files::path_type::fonts);
                break;
            }
        }
    }

    // Load all resource files.
    load_kestrel_core();
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

#if TARGET_MACOS
auto environment::launch_metal(const double& scale) -> int
{
    renderer::initialize(renderer::api::metal, [&] {
        prepare_common();
        renderer::set_tick_function([&] {
            tick();
        });
    });
    return 0;
}
#endif

auto environment::launch_opengl(const double& scale) -> int
{
    renderer::initialize(renderer::api::opengl, [&] {
        prepare_common();
        renderer::set_tick_function([&] {
            tick();
        });
    });
    return 0;
}

auto environment::prepare_common() -> void
{
    // Setup a new game session.
    m_game_session = std::make_shared<ui::session>();

    // Ensure Lua is fully configured and ready to go.
    become_active_environment();
    m_lua_runtime->prepare_lua_environment(shared_from_this());

    // Locate and execute script #0 to enter the game itself, and then enter a run loop.
    m_game_session->present_scene({
        new ui::game_scene(asset::resource_descriptor::identified(0))
    });
}

static uint64_t s_throttle = 0;

auto environment::tick() -> void
{
    auto frame_start_time = rtc::clock::global().current();
    auto render_required = renderer::frame_render_required();

    auto session = this->session();

    if (render_required) {
        renderer::camera camera;
        renderer::start_frame(camera, m_imgui.enabled && m_imgui.ready);
    }

    if (session) {
        rtc::clock::global().tick();
        session->tick(render_required);
    }

    if (render_required) {
        if (m_imgui.enabled && m_imgui.ready) {
            m_imgui.dockspace.draw();
        }

        renderer::end_frame();
    }

    if (m_imgui.enabled && !m_imgui.ready) {
        m_imgui.ready = true;
        renderer::enable_imgui();

        if (m_imgui.imgui_load_action.state() && m_imgui.imgui_load_action.isFunction()) {
            m_imgui.imgui_load_action();
        }
    }
    else if (m_imgui.ready && !m_imgui.enabled) {
        m_imgui.ready = false;
        m_imgui.dockspace.erase();
        renderer::disable_imgui();

        if (m_imgui.imgui_unload_action.state() && m_imgui.imgui_unload_action.isFunction()) {
            m_imgui.imgui_unload_action();
        }
    }

    m_async_task_queue.execute_tasks();
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

    audio::manager::library audio_lib = audio::manager::library::openal;

#if TARGET_MACOS
    if (scale <= 0) {
        scale = cocoa::application::screen_scale_factor();
    }

    // Check if we're being forced to open the game using OpenAL rather than CoreAudio
    audio_lib = audio::manager::library::core_audio;
    if (std::find(m_options.begin(), m_options.end(), "--openal") != m_options.end()) {
        audio_lib = audio::manager::library::openal;
    }

    // Check if we're being forced to open the game in a certain graphics mode. If we are then we can ignore the
    // metal check.
    if (std::find(m_options.begin(), m_options.end(), "--opengl") != m_options.end()) {
        // We are loading in OpenGL
    }
    else {
        // We are going to try for Metal, but if the computer is not able to then we will default to OpenGL.
        if (renderer::supports_metal()) {
            audio::manager::shared_manager().set_library(audio_lib);
            return launch_metal(scale);
        }
    }
#else
    if (scale <= 0) {
        scale = 1.0;
    }

#endif

    audio::manager::shared_manager().set_library(audio_lib);
    return launch_opengl(scale);
}

auto environment::become_active_environment() -> void
{
    $_active_environment = shared_from_this();
}

// MARK: - Loading

auto environment::load_kestrel_core() -> void
{
    auto file_ref = host::sandbox::files::game_core();
    if (file_ref.get()) {
        auto file = new graphite::rsrc::file(file_ref->path());
        graphite::rsrc::manager::shared_manager().import_file(file);
    }
    else {
        throw std::runtime_error("Could not locate game core.");
    }

    auto support_ref = host::sandbox::files::game_support();
    if (support_ref.get()) {
        auto file = new graphite::rsrc::file(support_ref->path());
        graphite::rsrc::manager::shared_manager().import_file(file);
    }
}

auto environment::load_game_data() -> void
{
    auto env = environment::active_environment().lock();
    if (!env) {
        return;
    }

    // Scenario Data
    auto scenario_ref = host::sandbox::files::game_data();
    if (scenario_ref.get()) {
        const auto& files = scenario_ref->contents(false);
        for (auto i = 0; i < files.size(); ++i) {
            const auto& file_ref = files.at(i);
            if (file_ref->extension() == "ndat" || file_ref->extension() == "kdat" || file_ref->extension() == "rez" || file_ref->extension() == "rsrc") {
                auto file = new graphite::rsrc::file(file_ref->path());
                graphite::rsrc::manager::shared_manager().import_file(file);
            }
            else if (file_ref->extension() == "mp3") {
                env->m_audio_files.emplace_back(file_ref->path());
            }
        }
    }
    else {
        throw std::runtime_error("Could not locate game scenario & data");
    }

    // Font Files
    auto fonts_ref = host::sandbox::files::game_fonts();
    if (fonts_ref.get() && fonts_ref->exists()) {
        const auto& files = fonts_ref->contents(false);
        for (auto i = 0; i < files.size(); ++i) {
            const auto& file_ref = files.at(i);
            if (file_ref->extension() == "ttf") {
                auto name = graphics::font::font_name_at_path(file_ref->path());
                env->m_custom_fonts[name] = file_ref->path();
            }
        }
    }

    // Mods
    auto mods_ref = host::sandbox::files::all_active_scenario_mods();
    if (!mods_ref.empty()) {
        for (auto i = 0; i < mods_ref.size(); ++i) {
            const auto& mod = mods_ref.at(i);
            if (!mod->enabled()) {
                continue;
            }

            if (!mod->is_loaded()) {
                mod->load_resources();
            }

            if (!mod->has_executed()) {
                mod->execute();
            }
        }
    }
}

// MARK: - Lua Interface

auto environment::prepare_lua_interface() -> void
{
    m_lua_runtime->global_namespace()
        .beginNamespace("Kestrel")
            .addProperty("platform", &environment::platform)
            .addProperty("platformName", &environment::platform_name)
            .addProperty("graphicsLayerName", &environment::gl_name)
            .addProperty("allAudioFiles", &environment::audio_files)
            .addProperty("tasksRemaining", &environment::tasks_remaining)
            .addProperty("hasTasks", &environment::has_tasks)
            .addFunction("presentScene", &environment::present_scene)
            .addFunction("setGameWindowTitle", &environment::set_game_window_title)
            .addFunction("setGameWindowSize", &environment::set_game_window_size)
            .addFunction("nativeScreenSize", &environment::native_screen_size)
            .addFunction("effectiveGameSize", &environment::effective_game_size)
            .addFunction("setFullscreen", &environment::set_fullscreen)
            .addFunction("importScript", &environment::import_script)
            .addFunction("runScript", &environment::run_script)
            .addFunction("scaleFactor", &environment::scale)
            .addFunction("loadImGui", &environment::start_imgui_environment_callback)
            .addFunction("unloadImGui", &environment::end_imgui_environment)
            .addFunction("loadScenarioData", &environment::load_game_data)
            .addFunction("enqueueTask", &environment::enqueue_task)
        .endNamespace();
}

auto environment::set_game_window_title(const std::string &title) -> void
{
    renderer::set_window_title(title);
}

auto environment::set_game_window_size(const math::size& size) -> void
{
    renderer::set_window_size(size);
}

auto environment::native_screen_size() -> math::size
{
    return renderer::native_screen_size();
}

auto environment::effective_game_size() -> math::size
{
    // The engine is intended to run at a 16:9 aspect ratio. Take the current game size and scale it to
    // an appropriate game size.
    // TODO: This only works with landscape orientations
    auto width = renderer::window_size().width;
    auto height = std::ceil(width / 1.7777778);
    return math::size(width, height);
}

auto environment::set_fullscreen(bool f) -> void
{
    renderer::set_fullscreen(f);
}

auto environment::scale() -> double
{
    return renderer::scale_factor();
}

auto environment::start_imgui_environment() -> void
{
    if (auto env = $_active_environment.lock()) {
        env->m_imgui.enabled = true;
    }
}

auto environment::start_imgui_environment_callback(const luabridge::LuaRef& callback) -> void
{
    if (auto env = $_active_environment.lock()) {
        env->m_imgui.enabled = true;
        if (callback.state()) {
            env->m_imgui.imgui_load_action = callback;
        }
        else {
            env->m_imgui.imgui_load_action = { env->m_lua_runtime->internal_state(), nullptr };
        }
    }
}

auto environment::end_imgui_environment(const luabridge::LuaRef& callback) -> void
{
    if (auto env = $_active_environment.lock()) {
        env->m_imgui.enabled = false;
        env->m_imgui.imgui_unload_action = callback;
    }
}

auto environment::import_script(const asset::resource_descriptor::lua_reference& ref) -> void
{
    if (auto env = $_active_environment.lock()) {
        scripting::lua::script script(env->m_lua_runtime->shared_from_this(), ref);
        env->m_lua_runtime->run(script);
    }
}

auto environment::run_script(const std::string &script_string) -> void
{
    if (auto env = $_active_environment.lock()) {
        scripting::lua::script script(env->m_lua_runtime->shared_from_this(), script_string);
        env->m_lua_runtime->run(script);
    }
}

auto environment::gc_purge() -> void
{
    lua_gc(m_lua_runtime->internal_state(), LUA_GCCOLLECT, 0);
}

auto environment::issue_lua_command(const std::string& lua) -> void
{
    scripting::lua::script script(m_lua_runtime->shared_from_this(), lua);
    script.execute();
}

auto environment::lua_out(const std::string& message, bool error) -> void
{
    auto session = this->session();
    if (session) {
        session->console().write((error ? "***" : "") + message);
    }
}

auto environment::lua_runtime() -> std::shared_ptr<scripting::lua::state>
{
    return m_lua_runtime;
}

auto environment::enqueue_task(const std::string& name, const luabridge::LuaRef &task) -> void
{
    if (auto env = $_active_environment.lock()) {
        env->m_async_task_queue.enqueue_task(name, task);
    }
}

auto environment::tasks_remaining() -> std::size_t
{
    if (auto env = $_active_environment.lock()) {
        return env->m_async_task_queue.tasks_remaining();
    }
    return 0;
}

auto environment::has_tasks() -> bool
{
    if (auto env = $_active_environment.lock()) {
        return env->m_async_task_queue.has_tasks_available();
    }
    return false;
}

// MARK: - Accessors

auto environment::session() -> std::shared_ptr<ui::session>
{
    return m_game_session;
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

// MARK: - Graphics Layer Specific

auto environment::gl_name() -> std::string
{
    return renderer::api_name();
}

// MARK: - Scene Management

auto environment::present_scene(const ui::game_scene::lua_reference &scene) -> void
{
    if (auto env = $_active_environment.lock()) {
        auto session = env->session();
        if (session) {
            session->present_scene(scene);
        }
    }
}

// MARK: - Event Posting

auto environment::post_event(const event &e) -> void
{
    if (m_imgui.enabled && m_imgui.ready) {
        m_imgui.dockspace.receive_event(e);
        return;
    }

    auto session = this->session();
    if (!session) {
        return;
    }

    session->receive_event(e);
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

// MARK: - Host Platform

#if TARGET_MACOS

auto environment::platform() -> environment::platform_type
{
    return platform_type::mac_os;
}

auto environment::platform_name() -> std::string
{
    return "macOS";
}

#elif TARGET_LINUX

auto environment::platform() -> environment::platform_type
{
    return platform_type::unix_like;
}

auto environment::platform_name() -> std::string
{
    return "Linux";
}

#elif TARGET_WINDOWS

auto environment::platform() -> environment::platform_type
{
    return platform_type::windows;
}

auto environment::platform_name() -> std::string
{
    return "Windows";
}

#endif
