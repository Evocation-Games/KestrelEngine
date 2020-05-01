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


#include <iostream>
#include "foundation/scene/scene.hpp"
#include "foundation/scene/stack.hpp"

// MARK: - Lua Integration

auto kestrel::scene::register_object() -> void
{
    luabridge::getGlobalNamespace(lua::active_state())
        .beginClass<kestrel::scene>("Scene")
            .addConstructor<auto(*)(std::string)->void, scene::lua_reference>()
            .addProperty("name", &scene::get_name, &scene::set_name)
            .addStaticFunction("current", &scene::current)
            .addFunction("attachScript", &scene::attach_script)
            .addFunction("setRenderer", &scene::set_renderer)
            .addFunction("addTimedCallback", &scene::add_timed_callback)
            .addFunction("present", &scene::present)
        .endClass();
}

// MARK: - Construction

auto kestrel::scene::create(std::string name) -> scene::lua_reference
{
    auto ptr = scene::lua_reference(new kestrel::scene(name));
    ptr->set_name(name);
    return ptr;
}

kestrel::scene::scene(std::string name)
    : m_name(name)
{
    std::cout << "constructing a new scene" << std::endl;
}

auto kestrel::scene::current() -> scene::lua_reference
{
    return scene_stack::global().current();
}


// MARK: - Scripts & Callback

auto kestrel::scene::attach_script(assets::resource_reference::lua_reference ref) -> void
{
    if (ref->id().has_value()) {
        m_script = lua::script(ref->id().value());
    }
    // TODO: Use Resource Name as a reference if required...
}

auto kestrel::scene::set_renderer(luabridge::LuaRef callback) -> void
{
    m_renderer = callback;
}

// MARK: - Properties

auto kestrel::scene::set_name(std::string name) -> void
{
    m_name = name;
}

auto kestrel::scene::get_name() const -> std::string
{
    return m_name;
}

// MARK: - Events

auto kestrel::scene::add_timed_callback(double delay, luabridge::LuaRef callback) -> void
{
    timed_event event(delay);
    event.set_callback(callback);
    m_timed_callbacks.emplace_back(event);
}

// MARK: - Life Cycle

auto kestrel::scene::present() -> void
{
    scene_stack::global().push(this);
    begin();
}

auto kestrel::scene::begin() -> void
{
    m_script.execute();
}

auto kestrel::scene::render() -> void
{
    // Iterate over all of the timed callbacks and attempt to fire any that are due,
    // and then clean it up if it is no longer needed.
    auto callback = m_timed_callbacks.begin();
    while (callback != m_timed_callbacks.end()) {
        callback->fire();
        if (callback->dead()) {
            callback = m_timed_callbacks.erase(callback);
        }
        else {
            ++callback;
        }
    }

    // Call the renderer to draw the contents of the scene.
    m_renderer();
}

