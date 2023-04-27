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

#pragma once

#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/lua/script.hpp>
#include <libKestrel/platform.hpp>
#include <libKestrel/configuration.hpp>
#include <libKestrel/event/event.hpp>
#include <libKestrel/session/session.hpp>
#include <libKestrel/ui/imgui/dockspace.hpp>
#include <libKestrel/resource/container.hpp>
#include <libKestrel/ui/scene/definition.hpp>

namespace lua_api(Kestrel, Available_0_8) kestrel
{
    has_lua_api;

    enum class result : std::int32_t
    {
        success,
        unknown_error,
        incompatible_renderer,
        incompatible_audio_driver,
        bad_data_file,
    };

    // MARK: - Configuration
    lua_getter(platform, Available_0_8) auto platform_family() -> std::int32_t;
    lua_getter(audioDriver, Available_0_8) auto audio_driver() -> std::int32_t;
    lua_getter(graphicsAPI, Available_0_8) auto renderer_api() -> std::int32_t;
    lua_getter(platformName, Available_0_8) auto platform_family_name() -> std::string;
    lua_getter(graphicsLayerName, Available_0_8) auto renderer_api_name() -> std::string;
    lua_getter(audioDriverName, Available_0_8) auto audio_driver_name() -> std::string;

    lua_function(start, Available_0_9) auto configure_window(const std::string& title, const math::size& size) -> void;

    lua_function(setGameWindowTitle, Available_0_8) auto set_game_window_title(const std::string& title) -> void;
    lua_function(setGameWindowSize, Available_0_8) auto set_game_window_size(const math::size& size) -> void;
    lua_function(nativeScreenSize, Available_0_8) auto native_screen_size() -> math::size;
    lua_function(effectiveGameSize, Available_0_8) auto effective_game_size() -> math::size;

    lua_getter(fullscreen, Available_0_8) auto is_fullscreen() -> bool;
    lua_setter(fullscreen, Available_0_8) auto set_fullscreen(bool f) -> void;

    lua_function(scaleFactor, Available_0_8) auto scale() -> double;

    lua_function(hasLaunchOption, Available_0_8) auto has_launch_option(const std::string& option) -> std::int32_t;
    lua_function(launchOptionCount, Available_0_8) auto launch_option_count() -> std::int32_t;
    lua_function(launchOptionAt, Available_0_8) auto launch_option_at(std::int32_t idx) -> std::string;

    // MARK: - Session
    auto start(const configuration& cfg) -> result;
    auto session() -> ui::session&;
    lua_function(loadScenarioData, Available_0_8) auto load_scenario_data() -> void;

    // MARK: - Lua
    auto lua_runtime() -> std::shared_ptr<lua::runtime>;
    lua_function(importScript, Available_0_8) auto import_script(const resource::descriptor::lua_reference& ref) -> void;
    lua_function(runScript, Available_0_8) auto run_script(const std::string& script) -> void;

    // MARK: - ImGui
    auto imgui_dockspace() -> ui::imgui::dockspace&;
    lua_function(loadImGui, Available_0_8) auto load_imgui_environment(const luabridge::LuaRef& callback) -> void;
    lua_function(unloadImGui, Available_0_8) auto unload_imgui_environment(const luabridge::LuaRef& callback) -> void;

    // MARK: - Scene Management
    lua_function(SceneContainer, Available_0_9) auto get_scene_container(const std::string& name) -> resource::container::lua_reference;
    lua_function(Scene, Available_0_9) auto get_scene_definition(const luabridge::LuaRef& name) -> ui::scene_definition::lua_reference;
    auto scene_definition(const std::string& name) -> ui::scene_definition::lua_reference;
    auto scene_definition(const resource::descriptor::lua_reference& ref) -> ui::scene_definition::lua_reference;
    auto current_scene() -> ui::game_scene::lua_reference;
    auto create_backing_scene(const lua::script& script, const std::string& name = "untitled-scene") -> std::shared_ptr<ui::scene>;
    lua_function(presentScene, Available_0_8) auto push_scene(const ui::game_scene::lua_reference& scene) -> void;
    auto pop_scene() -> void;

    // MARK: - Event Management
    auto post_event(const event& event) -> void;

    // MARK: - Async Tasks
    lua_getter(hasTasks, Available_0_8) auto has_async_tasks() -> bool;
    lua_getter(remainingTasks, Available_0_8) auto remaining_async_tasks() -> std::size_t;
    lua_function(enqueueTask, Available_0_8) auto enqueue_async_task(const std::string& name, const luabridge::LuaRef& task) -> void;

    // MARK: - Fonts
    auto bundled_font_named(const std::string& name) -> std::string;

    // MARK: - Audio
    lua_getter(allAudioFiles, Available_0_8) auto all_audio_files() -> lua::vector<std::string>;

    // MARK: - Garbage Collection
    auto purge_garbage_collector() -> void;
}