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

#include "core/graphics/common/scene.hpp"
#include "core/graphics/common/entity.hpp"

// MARK: - Construction

graphics::scene::scene(const std::shared_ptr<graphics::session_window>& window, const scripting::lua::script &script, const std::string& name)
    : m_owner(window), m_script(script), m_name(name)
{

}

auto graphics::scene::start() -> void
{
    m_script.execute();
}

// MARK: - Properties

auto graphics::scene::get_name() const -> std::string
{
    return m_name;
}

auto graphics::scene::get_owner() const -> std::weak_ptr<graphics::session_window>
{
    return m_owner;
}

auto graphics::scene::get_passthrough_render() const -> bool
{
    return m_passthrough_render;
}

auto graphics::scene::set_passthrough_render(const bool f) -> void
{
    m_passthrough_render = f;
}

// MARK: - Render/Physics

auto graphics::scene::update() -> void
{
    // To be implemented in a subclass
}

auto graphics::scene::render() -> void
{
    // To be implemented in a subclass
}

auto graphics::scene::draw_entity(const graphics::entity::lua_reference& entity) const -> void
{
    // To be implemented in a subclass
}

auto graphics::scene::add_update_block(const luabridge::LuaRef &block) -> void
{
    m_update_blocks.emplace_back(block);
}

auto graphics::scene::invoke_update_blocks() -> void
{
    for (const auto& block : m_update_blocks) {
        if (block.isFunction()) {
            block();
        }
    }
}

auto graphics::scene::add_render_block(const luabridge::LuaRef &block) -> void
{
    m_render_blocks.emplace_back(block);
}

auto graphics::scene::invoke_render_blocks() -> void
{
    for (const auto& block : m_render_blocks) {
        if (block.isFunction()) {
            block();
        }
    }
}

// MARK: - Event Handler

auto graphics::scene::add_key_event_block(const luabridge::LuaRef &block) -> void
{
    m_key_event_blocks.emplace_back(block);
}

auto graphics::scene::key_event(const event::key &event) -> void
{
    if (auto owner = get_owner().lock()) {
        if (event.is_released() && event.code() == event::key::f1) {
            owner->console().set_size({ owner->get_size().width, owner->get_size().height / 2.0 });
            owner->console().toggle_visibility();
            return;
        }

        if (owner->console().is_visible()) {
            owner->console().receive(event);
            return;
        }
    }

    auto ref = event::key::lua_reference(new event::key(event));
    for (const auto& block : m_key_event_blocks) {
        block(ref);
    }
}

auto graphics::scene::add_mouse_event_block(const luabridge::LuaRef &block) -> void
{
    m_mouse_event_blocks.emplace_back(block);
}

auto graphics::scene::mouse_event(const event::mouse &event) -> void
{
    auto ref = event::mouse::lua_reference(new event::mouse(event));
    for (const auto& block : m_mouse_event_blocks) {
        block(ref);
    }
}

auto graphics::scene::add_timed_event(std::shared_ptr<rtc::timed_event> event) -> void
{
    m_timed_events.emplace_back(event);
}

auto graphics::scene::check_timed_events() -> void
{
    for (const auto& event : m_timed_events) {
        if (event->should_fire()) {
            event->fire();
        }
    }
}

// MARK: - Scene Time

auto graphics::scene::current_time() const -> double
{
    return rtc::clock::global().since(m_starting_time).count();
}
