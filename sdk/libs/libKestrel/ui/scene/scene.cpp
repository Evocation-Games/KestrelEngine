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

#include <libKestrel/ui/scene/scene.hpp>
#include <libKestrel/kestrel.hpp>
#include <libKestrel/graphics/renderer/common/renderer.hpp>

// MARK: - Construction

kestrel::ui::scene::scene(const lua::script &script)
    : m_script(script)
{
}

// MARK: - Accessors

auto kestrel::ui::scene::is_passthrough_render() const -> bool
{
    return m_passthrough_render;
}

auto kestrel::ui::scene::set_passthrough_render(bool f) -> void
{
    m_passthrough_render = f;
}

auto kestrel::ui::scene::scaling_factor() const -> double
{
    return m_scaling_factor;
}

auto kestrel::ui::scene::set_scaling_factor(double f) -> void
{
    m_scaling_factor = f;
}

// MARK: - Blocks

auto kestrel::ui::scene::add_update_block(const std::function<auto()->void>& block) -> void
{
    m_update_blocks.emplace_back(block);
}

auto kestrel::ui::scene::add_render_block(const std::function<auto()->void>& block) -> void
{
    m_render_blocks.emplace_back(block);
}

auto kestrel::ui::scene::add_key_event_block(const std::function<auto(const event&)->void>& block) -> void
{
    m_key_event_blocks.emplace_back(block);
}

auto kestrel::ui::scene::add_mouse_event_block(const std::function<auto(const event&)->void>& block) -> void
{
    m_mouse_event_blocks.emplace_back(block);
}

auto kestrel::ui::scene::add_timed_event(const std::shared_ptr<rtc::timed_event>& event) -> void
{
    m_timed_events.emplace_back(event);
}

auto kestrel::ui::scene::invoke_update_blocks() -> void
{
    for (const auto& block : m_update_blocks) {
        block();
    }
}

auto kestrel::ui::scene::invoke_render_blocks() -> void
{
    for (const auto& block : m_render_blocks) {
        block();
    }
}

auto kestrel::ui::scene::receive_event(const event &e) -> void
{
    if (e.is_mouse_event()) {
        for (const auto& block : m_mouse_event_blocks) {
            block(e);
        }
    }

    if (e.is_key_event()) {
        // TODO: Console fixes
//        auto owner = session();
//        if (owner) {
//            if (e.has(event::type::key_up) && e.key() == hid::f1) {
//                owner->console().set_size({ owner->size().width, owner->size().height / 2.0 });
//                owner->console().toggle_visibility();
//                return;
//            }
//
//            if (owner->console().is_visible()) {
//                owner->console().receive(e);
//                return;
//            }
//        }

        for (const auto& block : m_key_event_blocks) {
            block(e);
        }
    }
}

auto kestrel::ui::scene::check_timed_events() -> void
{
    for (const auto& event : m_timed_events) {
        if (event->should_fire()) {
            event->fire();
        }
    }
}

// MARK: - Scene Timing

auto kestrel::ui::scene::current_time() const -> double
{
    return rtc::clock::global().since(m_starting_time).count();
}

// MARK: - Responder Chain

auto kestrel::ui::scene::assign_key_responder(const std::shared_ptr<responder_chain::key_responder> &responder) -> bool
{
    m_key_responder = responder;
    return true;
}

auto kestrel::ui::scene::key_responder() const -> std::shared_ptr<responder_chain::key_responder>
{
    return m_key_responder.lock();
}

// MARK: - Lifecycle

auto kestrel::ui::scene::start() -> void
{
    m_script.execute();
}

auto kestrel::ui::scene::update() -> void
{
    invoke_update_blocks();
}

auto kestrel::ui::scene::render() -> void
{
    invoke_render_blocks();
}

// MARK: - Drawing

auto kestrel::ui::scene::draw_entity(const std::shared_ptr<ecs::entity>& entity) const -> void
{
    if (entity->sprite_sheet() == nullptr || entity->sprite_sheet()->sprite_count() == 0) {
        return;
    }

    math::rect frame { entity->get_position(), entity->get_size() };
    if (!entity->ignores_scene_scaling_factor()) {
        if (auto scene = entity->scene()) {
            frame.set_size(frame.size() * static_cast<float>(scene->scaling_factor()));
        }
    }

    auto sprite = entity->sprite_sheet()->at(static_cast<int>(entity->get_sprite_index()));
    auto uv_x = static_cast<float>(sprite.point().x());
    auto uv_y = static_cast<float>(sprite.point().y());
    auto uv_w = static_cast<float>(sprite.size().width());
    auto uv_h = static_cast<float>(sprite.size().height());

    if (entity->has_clipping_area()) {
        uv_x = static_cast<float>(sprite.point().x() + entity->clipping_offset_uv().x());
        uv_y = static_cast<float>(sprite.point().y() + entity->clipping_offset_uv().y());
        uv_w = static_cast<float>(entity->clipping_area_uv().width());
        uv_h = static_cast<float>(entity->clipping_area_uv().height());
    }
    else if (entity->has_scaled_texture()) {
        uv_x = static_cast<float>(sprite.point().x() + entity->scaled_texture_area().origin().x());
        uv_y = static_cast<float>(sprite.point().y() + entity->scaled_texture_area().origin().y());
        uv_w = static_cast<float>(entity->scaled_texture_area().size().width());
        uv_h = static_cast<float>(entity->scaled_texture_area().size().height());
    }

    auto shader_program = entity->shader().get() ? entity->shader()->program() : nullptr;

    math::rect tex_coords { uv_x, uv_y, uv_w, uv_h };
    renderer::draw_quad(entity->texture(), frame, tex_coords, entity->blend(), static_cast<float>(entity->get_alpha()), renderer::scale_factor(), shader_program, entity->shader_attachments());

    if (renderer::hitbox_debug() && entity->body()->hitbox().is_valid()) {
        const auto& hb = entity->body()->hitbox();
        auto sheet = entity->sprite_sheet();
        auto color = *entity->hitbox_color().get();

        // Determine what type of hitbox needs to be drawn...
        if (hb.type() == physics::hitbox::type::polygon) {
            const auto& poly = hb.polygon() * hb.scale_factor();
            for (auto n = 0; n < poly.triangle_count(); ++n) {
                const auto& tri = poly.triangle_at(n);
                math::point a = frame.origin() + tri.a.to_point();
                math::point b = frame.origin() + tri.b.to_point();
                math::point c = frame.origin() + tri.c.to_point();

                renderer::draw_line(a, b, renderer::blending::normal, color, 1);
                renderer::draw_line(b, c, renderer::blending::normal, color, 1);
                renderer::draw_line(c, a, renderer::blending::normal, color, 1);
            }
        }
    }
}