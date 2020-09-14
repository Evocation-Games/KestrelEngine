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

#include "core/graphics/common/lua_scene_wrapper.hpp"
#include "core/graphics/common/scene.hpp"
#include "core/environment.hpp"
#include <utility>

// MARK: - Lua

auto graphics::lua_scene_wrapper::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    luabridge::getGlobalNamespace(lua->internal_state())
        .beginClass<graphics::lua_scene_wrapper>("Scene")
            .addStaticFunction("current", &graphics::lua_scene_wrapper::current)
            .addProperty("centerPoint", &graphics::lua_scene_wrapper::center_point)
            .addProperty("size", &graphics::lua_scene_wrapper::size)
            .addProperty("name", &graphics::lua_scene_wrapper::name)
            .addFunction("present", &graphics::lua_scene_wrapper::present)
            .addFunction("render", &graphics::lua_scene_wrapper::render)
            .addFunction("onKeyEvent", &graphics::lua_scene_wrapper::key_event)
            .addFunction("onMouseEvent", &graphics::lua_scene_wrapper::mouse_event)
        .endClass();
}

// MARK: - Construction

graphics::lua_scene_wrapper::lua_scene_wrapper(std::shared_ptr<graphics::scene> scene)
    : m_scene(std::move(scene))
{

}

auto graphics::lua_scene_wrapper::current() -> graphics::lua_scene_wrapper::lua_reference
{
    auto scene = environment::active_environment().lock()->current_scene();
    return graphics::lua_scene_wrapper::lua_reference(new graphics::lua_scene_wrapper(scene));
}

// MARK: - Interface

auto graphics::lua_scene_wrapper::present() const -> void
{
    // TODO: Perform a check to ensure the scene isn't already presented!
    environment::active_environment().lock()->present_scene(m_scene);
}

auto graphics::lua_scene_wrapper::render(const luabridge::LuaRef &block) const -> void
{
    m_scene->add_render_block(block);
}

auto graphics::lua_scene_wrapper::key_event(const luabridge::LuaRef &block) const -> void
{
    m_scene->add_key_event_block(block);
}

auto graphics::lua_scene_wrapper::mouse_event(const luabridge::LuaRef &block) const -> void
{
    m_scene->add_mouse_event_block(block);
}

// MARK: - Accessors

auto graphics::lua_scene_wrapper::size() const -> math::size
{
    if (auto env = environment::active_environment().lock()) {
        return env->window()->get_size();
    }
    else {
        return math::size(0);
    }
}

auto graphics::lua_scene_wrapper::center_point() const -> math::point
{
    auto size = this->size();
    return math::point(size.width, size.height) / 2.0;
}

auto graphics::lua_scene_wrapper::name() const -> std::string
{
    return m_scene->get_name();
}
