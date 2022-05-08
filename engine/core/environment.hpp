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

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <type_traits>
#include "util/hint.hpp"
#include "math/size.hpp"
#include "util/lua_vector.hpp"
#include "scripting/state.hpp"
#include "scripting/script.hpp"
#include "core/file/files.hpp"
#include "core/event/event.hpp"
#include "core/asset/cache.hpp"
#include "core/graphics/common/texture.hpp"
#include "core/asset/rsrc/resource_descriptor.hpp"
#include "core/ui/session.hpp"
#include "core/ui/scene.hpp"
#include "core/ui/game_scene.hpp"
#include "core/ui/imgui/dockspace.hpp"
#include "renderer/common/context.hpp"
#include <imgui/imgui.h>

class environment: public std::enable_shared_from_this<environment>
{
public:
    enum platform_type { mac_os, unix_like, windows };
    enum gl_type { none, open_gl, metal };

#if TARGET_MACOS
    auto launch_metal(const double& scale = 1.0) -> int;
#endif
    auto launch_opengl(const double& scale = 1.0) -> int;

    auto prepare_common() -> void;
    auto tick() -> void;

    static auto load_kestrel_core() -> void;
    lua_api static auto load_game_data() -> void;

    lua_api static auto set_game_window_title(const std::string& title) -> void;
    lua_api static auto set_game_window_size(const double& width, const double& height) -> void;
    lua_api static auto import_script(const asset::resource_descriptor::lua_reference& ref) -> void;
    lua_api static auto run_script(const std::string& script) -> void;

    lua_api static auto present_scene(const ui::game_scene::lua_reference& scene) -> void;

    lua_api static auto scale() -> double;

    lua_api static auto gl_name() -> std::string;
    lua_api static auto platform() -> environment::platform_type;
    lua_api static auto platform_name() -> std::string;

    lua_api static auto audio_files() -> util::lua_vector<std::string>;
    lua_api static auto play_audio_file(const std::string& file) -> void;
    lua_api static auto stop_audio_file() -> void;

    lua_api static auto start_imgui_environment() -> void;
    lua_api static auto start_imgui_environment_callback(const luabridge::LuaRef& callback) -> void;
    lua_api static auto end_imgui_environment(const luabridge::LuaRef& callback) -> void;
    inline auto imgui_dockspace() -> ui::imgui::dockspace& { return m_imgui.dockspace; }

    auto all_audio_files() -> util::lua_vector<std::string>;

public:
    environment(int argc, const char **argv);

    auto launch() -> int;

    static auto active_environment() -> std::weak_ptr<environment>;
    auto become_active_environment() -> void;

    auto prepare_lua_interface() -> void;
    auto issue_lua_command(const std::string& lua) -> void;
    auto lua_out(const std::string& message, bool error = false) -> void;
    auto lua_runtime() -> std::shared_ptr<scripting::lua::state>;

    auto cache() -> std::shared_ptr<asset::cache>;

    auto gc_purge() -> void;

    auto session() -> std::shared_ptr<ui::session>;
    auto post_event(const event& event) -> void;

    auto bundled_font_named(const std::string& name) const -> std::optional<std::string>;

private:
    int m_status;
    std::shared_ptr<ui::session> m_game_session;
    std::shared_ptr<renderer::context> m_renderer_context;
    std::vector<std::string> m_options;
    util::lua_vector<std::string> m_audio_files;
    std::shared_ptr<scripting::lua::state> m_lua_runtime;
    std::shared_ptr<asset::cache> m_cache { std::make_shared<asset::cache>() };
    std::map<std::string, std::string> m_custom_fonts {};
    float m_current_estimated_fps { 0.f };

    struct {
        bool enabled { false };
        bool ready { false };
        ui::imgui::dockspace dockspace;
        luabridge::LuaRef imgui_load_action { nullptr };
        luabridge::LuaRef imgui_unload_action { nullptr };
    } m_imgui;
};
