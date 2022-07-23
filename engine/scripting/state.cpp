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

#include "scripting/state.hpp"
#include "scripting/script.hpp"
#include "core/environment.hpp"

#include "core/asset/rsrc/resource_data.hpp"
#include "core/asset/rsrc/resource_writer.hpp"
#include "core/asset/rsrc/namespace.hpp"
#include "core/asset/rsrc/resource_descriptor.hpp"
#include "math/angle.hpp"
#include "math/angular_difference.hpp"
#include "math/point.hpp"
#include "math/size.hpp"
#include "math/rect.hpp"
#include "core/graphics/common/color.hpp"
#include "core/asset/legacy/macintosh/picture.hpp"
#include "core/asset/legacy/macintosh/color_icon.hpp"
#include "core/asset/legacy/macintosh/sound.hpp"
#include "core/asset/legacy/macintosh/dialog.hpp"
#include "core/asset/legacy/macintosh/item_list.hpp"
#include "core/asset/spritesheet.hpp"
#include "core/asset/static_image.hpp"
#include "core/asset/scene_interface.hpp"
#include "core/graphics/common/text.hpp"
#include "core/event/event.hpp"
#include "core/asset/legacy/spriteworld/sprite.hpp"
#include "core/graphics/common/canvas.hpp"
#include "util/lua_vector.hpp"
#include "core/file/files.hpp"
#include "core/file/file_reference.hpp"
#include "core/file/directory_reference.hpp"
#include "core/file/mod_reference.hpp"
#include "core/file/resource_file_reference.hpp"
#include "core/audio/codec/mp3.hpp"
#include "core/ui/game_scene.hpp"
#include "core/ui/layout/positioning_frame.hpp"
#include "core/ui/widgets/label_widget.hpp"
#include "core/ui/widgets/text_widget.hpp"
#include "core/ui/widgets/custom_widget.hpp"
#include "core/ui/widgets/image_widget.hpp"
#include "core/ui/widgets/button_widget.hpp"
#include "core/ui/widgets/textarea_widget.hpp"
#include "core/ui/widgets/list_widget.hpp"
#include "core/ui/widgets/grid_widget.hpp"
#include "core/ui/widgets/sprite_widget.hpp"
#include "core/ui/stencils/button_stencil.hpp"
#include "core/ui/imgui/imgui.hpp"
#include "core/ui/control_definition.hpp"
#include "core/ui/dialog/dialog_configuration.hpp"
#include "core/ui/dialog/dialog.hpp"
#include "core/ui/font/manager.hpp"
#include "renderer/common/lua_api.hpp"
#include "renderer/common/animator.hpp"
#include "core/task/async_queue.hpp"

// MARK: - Destruction

scripting::lua::state::~state()
{
    lua_close(m_state);
}

// MARK: - Custom Print

static int scripting_lua_state_print(lua_State *state)
{
    if (auto env = environment::active_environment().lock()) {
        int nargs = lua_gettop(state);

        for (int i = 1; i <= nargs; ++i) {
            if (lua_isstring(state, i)) {
                auto str = lua_tostring(state, i);
                env->lua_out(str);
            }
            else {

            }
        }
    }

    return 0;
}

// MARK: - Lua

auto scripting::lua::state::prepare_lua_environment(const std::shared_ptr<environment>& env) -> void
{
    m_state = luaL_newstate();
    luaL_openlibs(m_state);

    // Configure the stack and any dependancies that the primary state has.
    m_stack = std::make_shared<lua::stack>(shared_from_this());

    // Register and establish the API for the Lua Environment
    env->prepare_lua_interface();

    asset::resource_descriptor::enroll_object_api_in_state(shared_from_this());
    asset::resource_namespace::enroll_object_api_in_state(shared_from_this());
    asset::resource_data::enroll_object_api_in_state(shared_from_this());
    asset::resource_writer::enroll_object_api_in_state(shared_from_this());
    asset::spritesheet::enroll_object_api_in_state(shared_from_this());
    asset::scene_interface::enroll_object_api_in_state(shared_from_this());
    asset::legacy::spriteworld::sprite::enroll_object_api_in_state(shared_from_this());
    asset::legacy::macintosh::quickdraw::picture::enroll_object_api_in_state(shared_from_this());
    asset::legacy::macintosh::quickdraw::color_icon::enroll_object_api_in_state(shared_from_this());
    asset::legacy::macintosh::quicktime::sound::enroll_object_api_in_state(shared_from_this());
    asset::static_image::enroll_object_api_in_state(shared_from_this());
    asset::legacy::macintosh::toolbox::dialog::enroll_object_api_in_state(shared_from_this());
    asset::legacy::macintosh::toolbox::item_list::enroll_object_api_in_state(shared_from_this());

    math::angle::enroll_object_api_in_state(shared_from_this());
    math::angular_difference::enroll_object_api_in_state(shared_from_this());
    math::point::enroll_object_api_in_state(shared_from_this());
    math::size::enroll_object_api_in_state(shared_from_this());
    math::rect::enroll_object_api_in_state(shared_from_this());

    graphics::color::enroll_object_api_in_state(shared_from_this());
    graphics::text::enroll_object_api_in_state(shared_from_this());
    graphics::canvas::enroll_object_api_in_state(shared_from_this());

    ui::game_scene::enroll_object_api_in_state(shared_from_this());
    ui::scene_entity::enroll_object_api_in_state(shared_from_this());
    ui::control_definition::enroll_object_api_in_state(shared_from_this());
    ui::layout::positioning_frame::enroll_object_api_in_state(shared_from_this());
    ui::widgets::label_widget::enroll_object_api_in_state(shared_from_this());
    ui::widgets::text_widget::enroll_object_api_in_state(shared_from_this());
    ui::widgets::custom_widget::enroll_object_api_in_state(shared_from_this());
    ui::widgets::image_widget::enroll_object_api_in_state(shared_from_this());
    ui::widgets::button_widget::enroll_object_api_in_state(shared_from_this());
    ui::widgets::textarea_widget::enroll_object_api_in_state(shared_from_this());
    ui::widgets::list_widget::enroll_object_api_in_state(shared_from_this());
    ui::widgets::grid_widget::enroll_object_api_in_state(shared_from_this());
    ui::widgets::sprite_widget::enroll_object_api_in_state(shared_from_this());
    ui::stencils::button_stencil::enroll_object_api_in_state(shared_from_this());
    ui::dialog_configuration::enroll_object_api_in_state(shared_from_this());
    ui::dialog::enroll_object_api_in_state(shared_from_this());
    ui::font::manager::enroll_object_api_in_state(shared_from_this());
    ui::font::reference::enroll_object_api_in_state(shared_from_this());

    ui::imgui::window::enroll_object_api_in_state(shared_from_this());
    ui::imgui::label::enroll_object_api_in_state(shared_from_this());
    ui::imgui::button::enroll_object_api_in_state(shared_from_this());
    ui::imgui::textfield::enroll_object_api_in_state(shared_from_this());
    ui::imgui::textarea::enroll_object_api_in_state(shared_from_this());
    ui::imgui::code_editor::enroll_object_api_in_state(shared_from_this());
    ui::imgui::image::enroll_object_api_in_state(shared_from_this());
    ui::imgui::slider::enroll_object_api_in_state(shared_from_this());
    ui::imgui::same_line::enroll_object_api_in_state(shared_from_this());
    ui::imgui::dockspace::enroll_object_api_in_state(shared_from_this());
    ui::imgui::combo::enroll_object_api_in_state(shared_from_this());
    ui::imgui::tabbar::enroll_object_api_in_state(shared_from_this());
    ui::imgui::checkbox::enroll_object_api_in_state(shared_from_this());
    ui::imgui::table::enroll_object_api_in_state(shared_from_this());
    ui::imgui::box::enroll_object_api_in_state(shared_from_this());
    ui::imgui::separator::enroll_object_api_in_state(shared_from_this());
    ui::imgui::scrollarea::enroll_object_api_in_state(shared_from_this());

    ui::imgui::style::enroll_api_in_lua_state(shared_from_this());

    event::enroll_object_api_in_state(shared_from_this());

    host::sandbox::file_reference::enroll_object_api_in_state(shared_from_this());
    host::sandbox::resource_file_reference::enroll_object_api_in_state(shared_from_this());
    host::sandbox::directory_reference::enroll_object_api_in_state(shared_from_this());
    host::sandbox::mod_reference::enroll_object_api_in_state(shared_from_this());
    host::sandbox::files::enroll_object_api_in_state(shared_from_this());

    audio::asset::mp3::enroll_object_api_in_state(shared_from_this());

    renderer::lua::api::enroll_namespace_api_in_state(shared_from_this());
    renderer::animator::enroll_object_api_in_state(shared_from_this());

    task::async_queue::enroll_object_api_in_state(shared_from_this());

    util::lua_vector<asset::resource_descriptor::lua_reference>::enroll_object_api_in_state("ResourceSet", shared_from_this());
    util::lua_vector<std::string>::enroll_object_api_in_state("StringVector", shared_from_this());
    util::lua_vector<host::sandbox::file_reference::lua_reference>::enroll_object_api_in_state("DirectoryContentsVector", shared_from_this());
    util::lua_vector<host::sandbox::mod_reference::lua_reference>::enroll_object_api_in_state("ModList", shared_from_this());

    luabridge::getGlobalNamespace(m_state)
        .addFunction("print", scripting_lua_state_print);
}


// MARK: - Accessors

auto scripting::lua::state::stack() -> std::shared_ptr<lua::stack>
{
    return m_stack;
}

auto scripting::lua::state::internal_state() const -> lua_State*
{
    return m_state;
}

auto scripting::lua::state::error_string() const -> std::string
{
    return m_stack->pop_string();
}

// MARK: - Lua Interaction

auto scripting::lua::state::function(const std::string& name) const -> luabridge::LuaRef
{
    return function(name.c_str());
}

auto scripting::lua::state::function(const char *name) const -> luabridge::LuaRef
{
    return luabridge::getGlobal(m_state, name);
}

auto scripting::lua::state::run(const lua::script& script) -> void
{
    if (script.is_object()) {
        run(script.id(), script.name(), script);
    }
    else {
        run(script.id(), script.name(), script.code());
    }
}

static const char *code_reader(lua_State *L, void *object, size_t *s)
{
    auto obj = reinterpret_cast<scripting::lua::script::script_object *>(object);
    if (obj == nullptr) {
        *s = 0;
        return nullptr;
    }
    *s = obj->len;
    return reinterpret_cast<const char *>(obj->data);
}

auto scripting::lua::state::run(const int64_t& id, const std::string& name, const lua::script &script) -> void
{
    if (lua_load(m_state, code_reader, script.object(), name.c_str(), "b") != LUA_OK) {
        if (auto env = environment::active_environment().lock()) {
            env->lua_out(error_string(), true);
        }
        else {
            throw std::runtime_error(error_string());
        }
    }

    if (lua_pcall(m_state, 0, LUA_MULTRET, 0) != LUA_OK) {
        auto error_string = this->error_string();

        lua_Debug info;
        int level = 0;
        while (lua_getstack(m_state, level, &info)) {
            lua_getinfo(m_state, "nSl", &info);
            fprintf(stderr, "  [%d] %s:%d -- %s [%s]\n",
                    level, info.short_src, info.currentline,
                    (info.name ? info.name : "<unknown>"), info.what);
            ++level;
        }

        if (auto env = environment::active_environment().lock()) {
            env->lua_out(error_string, true);
        }
        else {
            throw std::runtime_error(error_string);
        }
    }
}

auto scripting::lua::state::run(const int64_t& id, const std::string& name, const std::string& script) -> void
{
    if (luaL_loadstring(m_state, script.c_str()) != LUA_OK) {
        if (auto env = environment::active_environment().lock()) {
            env->lua_out(error_string(), true);
        }
        else {
            throw std::runtime_error(error_string());
        }
    }

    if (lua_pcall(m_state, 0, LUA_MULTRET, 0) != LUA_OK) {
        auto error_string = this->error_string();
        
        lua_Debug info;
        int level = 0;
        while (lua_getstack(m_state, level, &info)) {
            lua_getinfo(m_state, "nSl", &info);
            fprintf(stderr, "  [%d] %s:%d -- %s [%s]\n",
                    level, info.short_src, info.currentline,
                    (info.name ? info.name : "<unknown>"), info.what);
            ++level;
        }

        if (auto env = environment::active_environment().lock()) {
            env->lua_out(error_string, true);
        }
        else {
            throw std::runtime_error(error_string);
        }
    }
}

// MARK: - Namespaces

auto scripting::lua::state::global_namespace() const -> luabridge::Namespace
{
    return luabridge::getGlobalNamespace(m_state);
}
