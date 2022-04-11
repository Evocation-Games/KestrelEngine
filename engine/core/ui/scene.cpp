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

#include "core/ui/scene.hpp"
#include "core/ui/session.hpp"
#include "renderer/common/renderer.hpp"

// MARK: - Construction

ui::scene::scene(std::shared_ptr<class session>& game_session, const scripting::lua::script &script)
    : m_session(game_session), m_script(script)
{

}

// MARK: - Accessors

auto ui::scene::session() const -> std::shared_ptr<class session>
{
    return m_session;
}

auto ui::scene::is_passthrough_render() const -> bool
{
    return m_passthrough_render;
}

auto ui::scene::set_passthrough_render(bool f) -> void
{
    m_passthrough_render = f;
}

// MARK: - Blocks

auto ui::scene::add_update_block(const std::function<auto()->void>& block) -> void
{
    m_update_blocks.emplace_back(block);
}

auto ui::scene::add_render_block(const std::function<auto()->void>& block) -> void
{
    m_render_blocks.emplace_back(block);
}

auto ui::scene::add_key_event_block(const std::function<auto(const event&)->void>& block) -> void
{
    m_key_event_blocks.emplace_back(block);
}

auto ui::scene::add_mouse_event_block(const std::function<auto(const event&)->void>& block) -> void
{
    m_mouse_event_blocks.emplace_back(block);
}

auto ui::scene::add_timed_event(const std::shared_ptr<rtc::timed_event>& event) -> void
{
    m_timed_events.emplace_back(event);
}

auto ui::scene::invoke_update_blocks() -> void
{
    for (const auto& block : m_update_blocks) {
        block();
    }
}

auto ui::scene::invoke_render_blocks() -> void
{
    for (const auto& block : m_render_blocks) {
        block();
    }
}

auto ui::scene::receive_event(const event &e) -> void
{
    if (e.is_mouse_event()) {
        for (const auto& block : m_mouse_event_blocks) {
            block(e);
        }
    }

    if (e.is_key_event()) {
        auto owner = session();
        if (owner) {
            if (e.has(event::type::key_up) && e.key() == hid::f1) {
                owner->console().set_size({ owner->size().width, owner->size().height / 2.0 });
                owner->console().toggle_visibility();
                return;
            }

            if (owner->console().is_visible()) {
                owner->console().receive(e);
                return;
            }
        }

        for (const auto& block : m_key_event_blocks) {
            block(e);
        }
    }
}

auto ui::scene::check_timed_events() -> void
{
    for (const auto& event : m_timed_events) {
        if (event->should_fire()) {
            event->fire();
        }
    }
}

// MARK: - Scene Timing

auto ui::scene::current_time() const -> double
{
    return rtc::clock::global().since(m_starting_time).count();
}

// MARK: - Responder Chain

auto ui::scene::assign_key_responder(const std::shared_ptr<class key_responder> &responder) -> bool
{
    m_key_responder = responder;
    return true;
}

auto ui::scene::key_responder() const -> std::shared_ptr<class key_responder>
{
    return m_key_responder.lock();
}

// MARK: - Lifecycle

auto ui::scene::start() -> void
{
    m_script.execute();
}

auto ui::scene::update() -> void
{
    invoke_update_blocks();
}

auto ui::scene::render() -> void
{
    invoke_render_blocks();
}

// MARK: - Drawing

auto ui::scene::draw_entity(const std::shared_ptr<graphics::entity>& entity) const -> void
{
    math::rect frame { entity->get_position(), entity->get_size() };

    auto sprite = entity->sprite_sheet()->at(static_cast<int>(entity->get_sprite_index()));
    auto uv_x = static_cast<float>(sprite.point().x);
    auto uv_y = static_cast<float>(sprite.point().y);
    auto uv_w = static_cast<float>(sprite.size().width);
    auto uv_h = static_cast<float>(sprite.size().height);

    if (entity->has_clipping_area()) {
        uv_x = static_cast<float>(sprite.point().x + entity->clipping_offset_uv().x);
        uv_y = static_cast<float>(sprite.point().y + entity->clipping_offset_uv().y);
        uv_w = static_cast<float>(entity->clipping_area_uv().width);
        uv_h = static_cast<float>(entity->clipping_area_uv().height);
    }

    math::rect tex_coords { uv_x, uv_y, uv_w, uv_h };

    renderer::draw_quad(entity->texture(), frame, tex_coords, renderer::blending::normal, static_cast<float>(entity->get_alpha()), 1.0);
}