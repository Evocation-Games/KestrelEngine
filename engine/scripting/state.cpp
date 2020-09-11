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

#include "core/asset/resource_reference.hpp"
#include "core/asset/resource.hpp"
#include "math/angle.hpp"
#include "math/angular_difference.hpp"
#include "math/point.hpp"
#include "math/size.hpp"
#include "math/rect.hpp"
#include "math/vector.hpp"
#include "core/graphics/common/color.hpp"
#include "core/asset/macintosh_picture.hpp"
#include "core/asset/color_icon.hpp"
#include "core/asset/spritesheet.hpp"
#include "core/graphics/common/lua_scene_wrapper.hpp"
#include "core/asset/image.hpp"
#include "core/graphics/common/entity.hpp"
#include "core/graphics/common/text.hpp"
#include "core/event/key.hpp"
#include "core/asset/sprite.hpp"

// MARK: - Construction

scripting::lua::state::state()
{

}

// MARK: - Destruction

scripting::lua::state::~state()
{
    lua_close(m_state);
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
    asset::resource_reference::enroll_object_api_in_state(shared_from_this());
    asset::resource::enroll_object_api_in_state(shared_from_this());
    asset::image::enroll_object_api_in_state(shared_from_this());
    asset::spritesheet::enroll_object_api_in_state(shared_from_this());
    asset::sprite::enroll_object_api_in_state(shared_from_this());
    asset::macintosh_picture::enroll_object_api_in_state(shared_from_this());
    asset::color_icon::enroll_object_api_in_state(shared_from_this());

    math::angle::enroll_object_api_in_state(shared_from_this());
    math::angular_difference::enroll_object_api_in_state(shared_from_this());
    math::point::enroll_object_api_in_state(shared_from_this());
    math::size::enroll_object_api_in_state(shared_from_this());
    math::rect::enroll_object_api_in_state(shared_from_this());
    math::vector::enroll_object_api_in_state(shared_from_this());

    graphics::lua_scene_wrapper::enroll_object_api_in_state(shared_from_this());
    graphics::color::enroll_object_api_in_state(shared_from_this());
    graphics::entity::enroll_object_api_in_state(shared_from_this());
    graphics::text::enroll_object_api_in_state(shared_from_this());

    event::key::enroll_object_apu_in_state(shared_from_this());
    event::mouse::enroll_object_apu_in_state(shared_from_this());
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
    run(script.code());
}

auto scripting::lua::state::run(const std::string& script) -> void
{
    if (luaL_loadstring(m_state, script.c_str()) != LUA_OK) {
        throw std::runtime_error(error_string());
    }

    if (lua_pcall(m_state, 0, LUA_MULTRET, 0) != LUA_OK) {
        throw std::runtime_error(error_string());
    }
}

auto scripting::lua::state::load_script(const int64_t &id) -> lua::script
{
    return lua::script(shared_from_this(), id);
}

