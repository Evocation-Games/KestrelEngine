// Copyright (c) 2022 Tom Hancocks
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
#include <cmath>
#include <complex>
#include <libKestrel/kestrel.hpp>
#include <libKestrel/graphics/renderer/common/renderer.hpp>
#include <libKestrel/exceptions/incompatible_driver_exception.hpp>
#include <libKestrel/exceptions/bad_data_file_exception.hpp>
#include <libKestrel/ui/imgui/dockspace.hpp>
#include <libKestrel/task/async_queue.hpp>
#include <libKestrel/sound/audio_manager.hpp>
#include <libKestrel/sandbox/file/files.hpp>
#include <libKestrel/lua/support/vector.hpp>
#include <libResourceCore/manager.hpp>
#include <libKestrel/shared/shared_library_manager.hpp>
#include <libToolbox/font/manager.hpp>

#if TARGET_MACOS
#   include <libKestrel/platform/macos/application.h>
#endif

// MARK: - Storage

static struct kestrel_storage
{
    kestrel::configuration base_configuration;
    kestrel::ui::session instance;
    kestrel::shared_lib::manager native_extensions;
    std::shared_ptr<kestrel::lua::runtime> runtime;
    kestrel::ui::imgui::dockspace dockspace;
    kestrel::task::async_queue async_queue;
    struct {
        struct {
            bool fullscreen { false };
            std::uint16_t width { 1280 };
            std::uint16_t height { 720 };
            double scale { 0.0 };
        } video;
        struct {
            bool ready { false };
            bool enabled { false };
            luabridge::LuaRef load_action { nullptr };
            luabridge::LuaRef unload_action { nullptr };
        } imgui;
        struct {
            kestrel::lua::vector<std::string> files;
        } audio;
        struct {
            std::unordered_map<std::string, std::string> files;
        } fonts;
    } active_configuration;
    struct {

    } throttle;
} s_kestrel_session;

// MARK: - Forward Declarations

namespace kestrel::renderer
{
    auto tick() -> void;
}

// MARK: - Private

namespace kestrel::video
{
    auto determine_size() -> void
    {
        s_kestrel_session.active_configuration.video.width = s_kestrel_session.base_configuration.video.width;
        s_kestrel_session.active_configuration.video.height = s_kestrel_session.base_configuration.video.height;
    }

    auto determine_scale() -> void
    {
        double scale = s_kestrel_session.base_configuration.video.scale;
#if TARGET_MACOS
        double default_scale = platform::macos::application::screen_scale_factor();
#else
        double default_scale = 1.0;
#endif
        s_kestrel_session.active_configuration.video.scale = (scale <= 0.0) ? default_scale : scale;
    }
}

namespace kestrel::environment
{
    auto prepare() -> void
    {
        // Make sure we have configured the Lua Runtime.
        lua::enroll_lua_api(s_kestrel_session.runtime);

        s_kestrel_session.runtime->global_namespace()
            .

        // Configure the main tick function for the renderer
        renderer::set_tick_function(renderer::tick);

        // Locate and execute script #0 to enter the game itself.
        s_kestrel_session.instance.present_scene({
            new ui::game_scene(resource::descriptor::identified(0))
        });
    }
}

namespace kestrel
{
    auto launch() -> result
    {
        // Make sure the fonts are fully loaded.
        toolbox::font::manager::shared_manager().update_font_table();

        // Configure the audio manager
        try {
            sound::manager::shared_manager().set_api(s_kestrel_session.base_configuration.audio.desired_api);
        }
        catch (const incompatible_driver_exception& e) {
            return result::incompatible_audio_driver;
        }

        // Finally, setup the renderer and enter the game loop.
        try {
            renderer::initialize(s_kestrel_session.base_configuration.renderer.desired_api, environment::prepare);
        }
        catch (const incompatible_driver_exception& e) {
            return result::incompatible_renderer;
        }

        // Once we reach this point, we are exiting successfully...
        return result::success;
    }
}

namespace kestrel::async
{
    auto execute_tasks() -> void
    {
        s_kestrel_session.async_queue.execute_tasks();
    }
}

namespace kestrel::renderer
{
    auto tick() -> void
    {
        auto frame_start_time = rtc::clock::global().current();
        auto render_required = renderer::frame_render_required();
        auto& imgui = s_kestrel_session.active_configuration.imgui;

        if (render_required) {
            renderer::camera camera;
            renderer::start_frame(camera, imgui.enabled && imgui.ready);
        }

        rtc::clock::global().tick();
        s_kestrel_session.instance.tick(render_required);

        if (render_required) {
            if (imgui.enabled && imgui.ready) {
                s_kestrel_session.dockspace.draw();
            }
            renderer::end_frame();
        }

        if (imgui.enabled && !imgui.ready) {
            imgui.ready = true;
            renderer::enable_imgui();
            if (imgui.load_action.state() && imgui.load_action.isFunction()) {
                imgui.load_action();
            }
        }
        else if (imgui.ready && !imgui.enabled) {
            imgui.ready = false;
            s_kestrel_session.dockspace.erase();
            renderer::disable_imgui();
            if (imgui.unload_action.state() && imgui.unload_action.isFunction()) {
                imgui.unload_action();
            }
        }

        async::execute_tasks();
    }
}

namespace kestrel::loader
{
    auto load_core() -> void
    {
        auto ref = sandbox::files::game_core();
        if (!ref.get()) {
            throw bad_data_file_exception("Missing game core file reference.");
        }

        auto file = new resource_core::file(ref->path());
        resource_core::manager::shared_manager().import_file(file);
        toolbox::font::manager::shared_manager().update_font_table();
    }

    auto load_support() -> void
    {
        auto ref = sandbox::files::game_support();
        if (ref.get()) {
            sandbox::file_reference file_ref(ref->path());
            if (file_ref.exists()) {
                auto file = new resource_core::file(ref->path());
                resource_core::manager::shared_manager().import_file(file);
                toolbox::font::manager::shared_manager().update_font_table();
            }
        }
    }

    auto load_data() -> void
    {
        auto ref = sandbox::files::game_data();
        if (!ref.get()) {
            throw bad_data_file_exception("Missing game scenario & data references.");
        }

        const auto& files = ref->contents(s_kestrel_session.base_configuration.data_files.include_dot_files);
        const auto& exts = s_kestrel_session.base_configuration.data_files.recognized_extensions;

        for (const auto& data_file_ref : files) {
            if (std::find(exts.begin(), exts.end(), data_file_ref->extension()) != exts.end()) {
                auto file = new resource_core::file(data_file_ref->path());
                resource_core::manager::shared_manager().import_file(file);
            }
            else if (data_file_ref->extension() == "mp3") {
                s_kestrel_session.active_configuration.audio.files.emplace_back(data_file_ref->path());
            }
            else {
                // TODO: Issue a soft warning?
            }
        }

        toolbox::font::manager::shared_manager().update_font_table();
    }

    auto load_fonts() -> void
    {
        auto ref = sandbox::files::game_fonts();
        if (!ref.get()) {
            throw bad_data_file_exception("Missing game font references.");
        }

        const auto& files = ref->contents(s_kestrel_session.base_configuration.data_files.include_dot_files);
        for (const auto& font_file_ref : files) {
            if (font_file_ref->extension() == "ttf") {
                auto name = graphics::font::font_name_at_path(font_file_ref->path());
                s_kestrel_session.active_configuration.fonts.files.emplace(std::pair(name, font_file_ref->path()));
            }
        }

        toolbox::font::manager::shared_manager().update_font_table();
    }

    auto load_mods() -> void
    {
        auto mods = sandbox::files::all_active_scenario_mods();
        if (mods.empty()) {
            return;
        }

        for (const auto& mod_ref : mods) {
            if (!mod_ref->enabled()) {
                continue;
            }

            if (!mod_ref->is_loaded()) {
                mod_ref->load_resources();
            }

            if (!mod_ref->has_executed()) {
                mod_ref->execute();
            }
        }

        toolbox::font::manager::shared_manager().update_font_table();
    }
}

// MARK: - Session

auto kestrel::start(const configuration& cfg) -> result
{
    // Setup the absolute basics...
    s_kestrel_session.base_configuration = cfg;
    s_kestrel_session.runtime = std::make_shared<lua::runtime>();

    // Prepare the sandbox...
    auto& files = sandbox::files::shared_files();
    files.set_game_path(cfg.data_files.core, sandbox::files::path_type::core);
    files.set_game_path(cfg.data_files.support, sandbox::files::path_type::support);
    files.set_game_path(cfg.data_files.scenarios, sandbox::files::path_type::scenario);
    files.set_game_path(cfg.data_files.mods, sandbox::files::path_type::mods);
    files.set_game_path(cfg.data_files.fonts, sandbox::files::path_type::fonts);

    // Setup the active configuration...
    video::determine_size();
    video::determine_scale();

    // Load data files
    try {
        loader::load_core();
        loader::load_support();
    }
    catch (bad_data_file_exception& e) {
        std::cerr << e.what() << std::endl;
        return result::bad_data_file;
    }
    catch (...) {
        return result::unknown_error;
    }

    return launch();
}

auto kestrel::session() -> ui::session&
{
    return s_kestrel_session.instance;
}

auto kestrel::load_scenario_data() -> void
{
    loader::load_data();
    loader::load_fonts();
    loader::load_mods();
}

// MARK: - Configuration

auto kestrel::platform_family() -> std::int32_t
{
    return static_cast<std::int32_t>(platform::current());
}

auto kestrel::renderer_api() -> std::int32_t
{
    return static_cast<std::int32_t>(s_kestrel_session.base_configuration.renderer.desired_api);
}

auto kestrel::audio_driver() -> std::int32_t
{
    return static_cast<std::int32_t>(s_kestrel_session.base_configuration.audio.desired_api);
}

auto kestrel::platform_family_name() -> std::string
{
    switch (platform::current()) {
        case platform::family::macos:       return "macOS";
        case platform::family::windows:     return "Windows";
        case platform::family::gnu_linux:   return "Linux";
        case platform::family::unknown:     return "Unknown";
    }
}

auto kestrel::renderer_api_name() -> std::string
{
    switch (s_kestrel_session.base_configuration.renderer.desired_api) {
        case renderer::api::opengl:         return "OpenGL";
        case renderer::api::metal:          return "Metal";
        case renderer::api::none:           return "None";
    }
}

auto kestrel::audio_driver_name() -> std::string
{
    switch (s_kestrel_session.base_configuration.audio.desired_api) {
        case sound::api::core_audio:    return "CoreAudio";
        case sound::api::openal:        return "OpenAL";
        case sound::api::none:          return "None";
    }
}

auto kestrel::configure_window(const std::string &title, const math::size &size) -> void
{
    renderer::set_window_title(title);
    renderer::set_window_size(size);
}

auto kestrel::set_game_window_title(const std::string &title) -> void
{
    renderer::set_window_title(title);
}

auto kestrel::set_game_window_size(const math::size &size) -> void
{
    renderer::set_window_size(size);
}

auto kestrel::native_screen_size() -> math::size
{
    return renderer::native_screen_size();
}

auto kestrel::effective_game_size() -> math::size
{
    // The engine is intended to run at a 16:9 aspect ratio. Take the current game size and scale it to
    // an appropriate game size.
    // TODO: This only works with landscape orientations
    auto width = renderer::window_size().width();
    auto height = ceilf(width / 1.7777778f);
    return { width, height };
}

auto kestrel::set_fullscreen(bool f) -> void
{
    renderer::set_fullscreen(f);
}

auto kestrel::is_fullscreen() -> bool
{
    return false;
}

auto kestrel::scale() -> double
{
    return renderer::scale_factor();
}

// MARK; - Launch Options

auto kestrel::has_launch_option(const std::string &option) -> std::int32_t
{
    for (std::int32_t i = 0; i < s_kestrel_session.base_configuration.unparsed_options.size(); ++i) {
        if (s_kestrel_session.base_configuration.unparsed_options.at(i) == option) {
            return i + 1;
        }
    }
    return 0;
}

auto kestrel::launch_option_at(std::int32_t idx) -> std::string
{
    if (idx < 1 || idx >= s_kestrel_session.base_configuration.unparsed_options.size()) {
        return "";
    }
    return s_kestrel_session.base_configuration.unparsed_options.at(idx - 1);
}

auto kestrel::launch_option_count() -> std::int32_t
{
    return static_cast<std::int32_t>(s_kestrel_session.base_configuration.unparsed_options.size());
}

// MARK: - ImGui

auto kestrel::imgui_dockspace() -> ui::imgui::dockspace &
{
    return s_kestrel_session.dockspace;
}

auto kestrel::load_imgui_environment(const luabridge::LuaRef &callback) -> void
{
    auto& imgui = s_kestrel_session.active_configuration.imgui;
    imgui.enabled = true;
    if (callback.state()) {
        imgui.load_action = callback;
    }
    else {
        imgui.load_action = { s_kestrel_session.runtime->internal_state(), nullptr };
    }
}

auto kestrel::unload_imgui_environment(const luabridge::LuaRef &callback) -> void
{
    auto& imgui = s_kestrel_session.active_configuration.imgui;
    imgui.enabled = false;
    if (callback.state()) {
        imgui.unload_action = callback;
    }
    else {
        imgui.unload_action = { s_kestrel_session.runtime->internal_state(), nullptr };
    }
}

// MARK: - Lua Runtime

auto kestrel::lua_runtime() -> std::shared_ptr<lua::runtime>
{
    return s_kestrel_session.runtime;
}

auto kestrel::import_script(const resource::descriptor::lua_reference &ref) -> void
{
    lua::script script(s_kestrel_session.runtime, ref);
    script.execute();
}

auto kestrel::run_script(const std::string &script_str) -> void
{
    lua::script script(s_kestrel_session.runtime, script_str);
    script.execute();
}

// MARK: - Scenes

auto kestrel::get_scene_container(const std::string &name) -> resource::container::lua_reference
{
    // TODO: Check for a valid scene container?
    return { new resource::container(name) };
}

auto kestrel::get_scene_definition(const luabridge::LuaRef &name) -> ui::scene_definition::lua_reference
{
    if (!name.state() || name.isNil()) {
        return { nullptr };
    }

    if (lua::ref_isa<resource::container>(name)) {
        auto container = name.cast<resource::container::lua_reference>();
        return scene_definition(container->primary_name());
    }
    else if (name.isString()) {
        return scene_definition(name.tostring());
    }
    else {
        return { nullptr };
    }
}

auto kestrel::scene_definition(const std::string &name) -> ui::scene_definition::lua_reference
{
    // TODO: Check for a valid scene container?
    return { new ui::scene_definition(name) };
}

auto kestrel::scene_definition(const resource::descriptor::lua_reference &ref) -> ui::scene_definition::lua_reference
{
    // TODO: Check for a valid scene container?
    if (!ref->has_container()) {
        throw std::runtime_error("");
    }

    if (ref->has_id()) {
        return { new ui::scene_definition(ref->container()->name, ref->id) };
    }
    else {
        return { new ui::scene_definition(ref->container()->name) };
    }
}

auto kestrel::current_scene() -> ui::game_scene::lua_reference
{
    return s_kestrel_session.instance.current_scene();
}

auto kestrel::create_backing_scene(const lua::script &script, const std::string& name) -> std::shared_ptr<ui::scene>
{
    return s_kestrel_session.instance.new_scene(name, script);
}

auto kestrel::push_scene(const ui::game_scene::lua_reference &scene) -> void
{
    s_kestrel_session.instance.present_scene(scene);
}

auto kestrel::pop_scene() -> void
{
    s_kestrel_session.instance.pop_scene();
    purge_garbage_collector();
}

// MARK: - Event Management

auto kestrel::post_event(const event &event) -> void
{
    auto& imgui = s_kestrel_session.active_configuration.imgui;
    if (imgui.ready && imgui.enabled) {
        s_kestrel_session.dockspace.receive_event(event);
        return;
    }
    s_kestrel_session.instance.receive_event(event);
}

// MARK: - Async Tasks

auto kestrel::has_async_tasks() -> bool
{
    return s_kestrel_session.async_queue.has_tasks_available();
}

auto kestrel::remaining_async_tasks() -> std::size_t
{
    return s_kestrel_session.async_queue.tasks_remaining();
}

auto kestrel::enqueue_async_task(const std::string &name, const luabridge::LuaRef &task) -> void
{
    s_kestrel_session.async_queue.enqueue_task(name, task);
}

// MARK: - Fonts

auto kestrel::bundled_font_named(const std::string &name) -> std::string
{
    auto& fonts = s_kestrel_session.active_configuration.fonts.files;
    if (fonts.find(name) != fonts.end()) {
        return fonts.at(name);
    }
    return "";
}

// MARK: - Audio

auto kestrel::all_audio_files() -> lua::vector<std::string>
{
    return s_kestrel_session.active_configuration.audio.files;
}

// MARK: - Garbage Collection

auto kestrel::purge_garbage_collector() -> void
{
    s_kestrel_session.runtime->purge();
}