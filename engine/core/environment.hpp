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

#if !defined(KESTREL_ENVIRONMENT_HPP)
#define KESTREL_ENVIRONMENT_HPP

#include <memory>
#include <string>
#include <vector>
#include "scripting/state.hpp"
#include "scripting/script.hpp"
#include "core/asset/resource_reference.hpp"
#include "util/hint.hpp"
#include "math/size.hpp"
#include "core/graphics/common/texture.hpp"
#include "core/asset/cache.hpp"
#include "core/graphics/common/lua_scene_wrapper.hpp"
#include "core/event/key.hpp"
#include "core/event/mouse.hpp"

namespace graphics
{
    class session_window;
    class scene;
}

class environment: public std::enable_shared_from_this<environment>
{
private:
    int m_status;
    std::vector<std::string> m_options;
    std::shared_ptr<graphics::session_window> m_game_window;
    std::string m_kestrel_core_path;
    std::string m_game_data_path;
    std::shared_ptr<scripting::lua::state> m_lua_runtime;
    std::shared_ptr<asset::cache> m_cache { std::make_shared<asset::cache>() };

    auto kestrel_core_path() const -> std::string;
    auto game_data_path() const -> std::string;

    auto load_kestrel_core() -> void;
    auto load_game_data() -> void;
    auto load_data_files(const std::string& path) -> void;

    auto load_script(const asset::resource_reference::lua_reference &ref) -> scripting::lua::script;

    lua_api static auto set_game_window_title(const std::string& title) -> void;
    lua_api static auto set_game_window_size(const double& width, const double& height) -> void;
    lua_api static auto import_script(const asset::resource_reference::lua_reference& ref) -> void;
    lua_api static auto create_scene(const std::string& name, const asset::resource_reference::lua_reference& script) -> graphics::lua_scene_wrapper::lua_reference;

public:
    environment(int argc, const char **argv);

    auto launch() -> int;

    static auto active_environment() -> std::weak_ptr<environment>;
    auto become_active_environment() -> void;

    auto prepare_lua_interface() -> void;

    auto cache() -> std::shared_ptr<asset::cache>;

    auto create_texture(const math::size& size, std::vector<uint32_t> data) const -> std::shared_ptr<graphics::texture>;
    auto current_scene() -> std::shared_ptr<graphics::scene>;
    auto present_scene(std::shared_ptr<graphics::scene> scene) -> void;

    auto post_key_event(const event::key& event) -> void;
    auto post_mouse_event(const event::mouse& event) -> void;
};

#endif //KESTREL_ENVIRONMENT_HPP
