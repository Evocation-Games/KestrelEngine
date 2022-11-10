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

#include <libKestrel/ui/entity/scene_entity.hpp>
#include <libKestrel/kestrel.hpp>
#include <libKestrel/graphics/canvas/canvas.hpp>
#include <libKestrel/graphics/image/static_image.hpp>
#include <libKestrel/graphics/legacy/spriteworld/sprite.hpp>
#include <libKestrel/graphics/legacy/macintosh/picture.hpp>
#include <libKestrel/graphics/legacy/macintosh/color_icon.hpp>
#include <libKestrel/graphics/renderer/common/blending.hpp>
#include <libKestrel/graphics/renderer/common/renderer.hpp>

// MARK: - Construction

kestrel::ui::scene_entity::scene_entity(const std::shared_ptr<ecs::entity>& entity)
    : m_entity(entity), m_position(0), m_frame(0), m_body(kestrel::session().current_scene()->physics_world().create_physics_body())
{
}

kestrel::ui::scene_entity::scene_entity(const luabridge::LuaRef& entity_provider)
    : m_entity(spawn_entity(entity_provider)), m_position(0), m_frame(0), m_body(kestrel::session().current_scene()->physics_world().create_physics_body())
{
}

kestrel::ui::scene_entity::scene_entity(const image::static_image::lua_reference &image)
    : m_entity(spawn_entity(image)), m_position(0), m_frame(0), m_body(kestrel::session().current_scene()->physics_world().create_physics_body())
{
}

kestrel::ui::scene_entity::scene_entity(const graphics::canvas::lua_reference &canvas)
    : m_entity(spawn_entity(canvas)), m_position(), m_frame(0), m_body(kestrel::session().current_scene()->physics_world().create_physics_body())
{
}

kestrel::ui::scene_entity::scene_entity(const std::shared_ptr<scene_entity>& entity)
    : m_entity(entity->m_entity),
      m_anchor(entity->m_anchor),
      m_position(entity->m_position),
      m_frame_count(entity->m_frame_count),
      m_frame(entity->m_frame),
      m_next_frame_on_draw(entity->m_next_frame_on_draw),
      m_loops(entity->m_loops),
      m_mouse_over(entity->m_mouse_over),
      m_mouse_dragged(entity->m_mouse_dragged),
      m_pressed(entity->m_pressed),
      m_started(entity->m_started),
      m_finished(entity->m_finished),
      m_children(entity->m_children),
      m_animator(entity->m_animator),
      m_on_mouse_down_internal(entity->m_on_mouse_down_internal),
      m_on_mouse_release_internal(entity->m_on_mouse_release_internal),
      m_on_mouse_drag_internal(entity->m_on_mouse_drag_internal),
      m_on_mouse_enter_internal(entity->m_on_mouse_enter_internal),
      m_on_mouse_exit_internal(entity->m_on_mouse_exit_internal),
      m_continuous_mouse_down_action(entity->m_continuous_mouse_down_action),
      m_body(entity->m_body)
{
    if (entity->m_on_animation_start.state() && entity->m_on_animation_start.isFunction()) {
        m_on_animation_start = entity->m_on_animation_start;
    }
    if (entity->m_on_animation_finish.state() && entity->m_on_animation_finish.isFunction()) {
        m_on_animation_finish = entity->m_on_animation_finish;
    }
    if (entity->m_on_layout.state() && entity->m_on_layout.isFunction()) {
        m_on_layout = entity->m_on_layout;
    }
    if (entity->m_on_mouse_enter.state() && entity->m_on_mouse_enter.isFunction()) {
        m_on_mouse_enter = entity->m_on_mouse_enter;
    }
    if (entity->m_on_mouse_exit.state() && entity->m_on_mouse_exit.isFunction()) {
        m_on_mouse_exit = entity->m_on_mouse_exit;
    }
    if (entity->m_on_mouse_down.state() && entity->m_on_mouse_down.isFunction()) {
        m_on_mouse_down = entity->m_on_mouse_down;
    }
    if (entity->m_on_mouse_release.state() && entity->m_on_mouse_release.isFunction()) {
        m_on_mouse_release = entity->m_on_mouse_release;
    }
    if (entity->m_on_mouse_drag.state() && entity->m_on_mouse_drag.isFunction()) {
        m_on_mouse_drag = entity->m_on_mouse_drag;
    }
}

kestrel::ui::scene_entity::~scene_entity()
{
    m_body->destroy();
}

// MARK: - Entity Spawning

auto kestrel::ui::scene_entity::spawn_entity(const luabridge::LuaRef& entity_provider) -> std::shared_ptr<ecs::entity>
{
    if (entity_provider.isUserdata()) {
        if (lua::type_check<graphics::canvas>(entity_provider)) {
            auto canvas = entity_provider.cast<graphics::canvas::lua_reference>();
            auto entity = canvas->spawn_entity({ 0, 0 });
            return entity;
        }
        else if (lua::type_check<image::legacy::spriteworld::sprite>(entity_provider)) {
            auto sprite = entity_provider.cast<image::legacy::spriteworld::sprite>();
            return sprite.spawn_entity({ 0, 0 });
        }
        else if (lua::type_check<image::legacy::macintosh::quickdraw::picture>(entity_provider)) {
            auto pict = entity_provider.cast<image::legacy::macintosh::quickdraw::picture>();
            return pict.spawn_entity({ 0, 0 });
        }
        else if (lua::type_check<image::legacy::macintosh::quickdraw::color_icon>(entity_provider)) {
            auto cicn = entity_provider.cast<image::legacy::macintosh::quickdraw::color_icon>();
            return cicn.spawn_entity({ 0, 0 });
        }
        else if (lua::type_check<image::static_image>(entity_provider)) {
            auto image = entity_provider.cast<image::static_image>();
            return image.spawn_entity({ 0, 0 });
        }
    }
    return nullptr;
}

auto kestrel::ui::scene_entity::spawn_entity(const image::static_image::lua_reference &image) -> std::shared_ptr<ecs::entity>
{
    if (image.get()) {
        return image->spawn_entity({ 0, 0 });
    }
    return {};
}

auto kestrel::ui::scene_entity::spawn_entity(const graphics::canvas::lua_reference &canvas) -> std::shared_ptr<ecs::entity>
{
    if (canvas.get()) {
        return canvas->spawn_entity({ 0, 0 });
    }
    return {};
}

auto kestrel::ui::scene_entity::set_sprite(const luabridge::LuaRef& sprite) -> void
{
    if (sprite.isNil()) {
        return;
    }
    m_entity = ui::scene_entity::spawn_entity(sprite);
}

// MARK: - Accessors

auto kestrel::ui::scene_entity::body() const -> physics::body::lua_reference
{
    return m_body;
}

auto kestrel::ui::scene_entity::position() const -> math::point
{
    return m_position;
}

auto kestrel::ui::scene_entity::draw_position() const -> math::point
{
    auto offset = origin_for_axis(render_size(), m_anchor);
    return m_entity->get_position() + offset;
}

auto kestrel::ui::scene_entity::anchor_point() const -> enum layout::axis_origin
{
    return m_anchor;
}

auto kestrel::ui::scene_entity::lua_anchor_point() const -> std::int32_t
{
    return static_cast<std::int32_t>(anchor_point());
}

auto kestrel::ui::scene_entity::size() const -> math::size
{
    return m_entity->get_size();
}

auto kestrel::ui::scene_entity::half_size() const -> math::size
{
    return m_entity->get_size() / 2;
}

auto kestrel::ui::scene_entity::render_size() const -> math::size
{
    return m_entity->get_render_size();
}

auto kestrel::ui::scene_entity::draw_size() const -> math::size
{
    return m_entity->get_draw_size();
}

auto kestrel::ui::scene_entity::frame_count() const -> std::int32_t
{
    return m_frame_count;
}

auto kestrel::ui::scene_entity::current_frame() const -> std::int32_t
{
    return m_frame;
}

auto kestrel::ui::scene_entity::advances_to_next_frame_on_draw() const -> bool
{
    return m_next_frame_on_draw;
}

auto kestrel::ui::scene_entity::animation_loops() const -> bool
{
    return m_loops;
}

auto kestrel::ui::scene_entity::alpha() const -> double
{
    return m_entity->get_alpha();
}

auto kestrel::ui::scene_entity::blend_mode() const -> std::int32_t
{
    return m_entity->get_blend_lua();
}

auto kestrel::ui::scene_entity::clipping_area() const -> math::size
{
    return m_entity->clipping_area();
}

auto kestrel::ui::scene_entity::clipping_offset() const -> math::point
{
    return m_entity->clipping_offset();
}

auto kestrel::ui::scene_entity::children() const -> lua::vector<lua_reference>
{
    return m_children;
}

auto kestrel::ui::scene_entity::animator() const -> renderer::animator::lua_reference
{
    return m_animator;
}

auto kestrel::ui::scene_entity::ignore_positioning_frame_scaler() const -> bool
{
    return m_ignore_positioning_frame_scaler;
}

auto kestrel::ui::scene_entity::continuous_mouse_down_action() const -> bool
{
    return m_continuous_mouse_down_action;
}

// MARK: - Setters

auto kestrel::ui::scene_entity::set_position(const math::point& v) -> void
{
    m_position = v;
}

auto kestrel::ui::scene_entity::set_draw_position(const math::point& v) -> void
{
    auto offset = origin_for_axis(render_size(), m_anchor);
    m_entity->set_position(v - offset);
}

auto kestrel::ui::scene_entity::set_anchor_point(enum layout::axis_origin v) -> void
{
    m_anchor = v;
}

auto kestrel::ui::scene_entity::set_lua_anchor_point(std::int32_t v) -> void
{
    set_anchor_point(static_cast<enum layout::axis_origin>(v));
}

auto kestrel::ui::scene_entity::set_size(const math::size& v) -> void
{
    m_entity->set_size(v);
}

auto kestrel::ui::scene_entity::set_render_size(const math::size& v) -> void
{
    m_entity->set_render_size(v);
}

auto kestrel::ui::scene_entity::set_draw_size(const math::size& v) -> void
{
    m_entity->set_draw_size(v);
}

auto kestrel::ui::scene_entity::set_current_frame(std::uint32_t v) -> void
{
    if (v >= 0 && v < m_frame_count) {
        m_entity->set_sprite_index(v);
    }
    m_frame = v;
}

auto kestrel::ui::scene_entity::set_advances_to_next_frame_on_draw(bool v) -> void
{
    m_next_frame_on_draw = v;
}

auto kestrel::ui::scene_entity::set_animation_loops(bool v) -> void
{
    m_loops = v;
}

auto kestrel::ui::scene_entity::set_alpha(double v) -> void
{
    m_entity->set_alpha(v);
}

auto kestrel::ui::scene_entity::set_blend_mode(std::int32_t v) -> void
{
    m_entity->set_blend_lua(v);
}

auto kestrel::ui::scene_entity::set_clipping_area(const math::size& v) -> void
{
    m_entity->set_clipping_area(v);
}

auto kestrel::ui::scene_entity::set_clipping_offset(const math::point& v) -> void
{
    m_entity->set_clipping_offset(v);
}

auto kestrel::ui::scene_entity::set_animator(const renderer::animator::lua_reference &animator) -> void
{
    m_animator = animator;
}

auto kestrel::ui::scene_entity::set_continuous_mouse_down_action(bool continuous) -> void
{
    m_continuous_mouse_down_action = continuous;
}

auto kestrel::ui::scene_entity::set_ignore_positioning_frame_scaler(bool f) -> void
{
    m_ignore_positioning_frame_scaler = f;
}

// MARK: - Child Entity Management

auto kestrel::ui::scene_entity::add_child_entity(const lua_reference& child) -> void
{
    m_children.emplace_back(child);
}

auto kestrel::ui::scene_entity::each_child(const luabridge::LuaRef& body) const -> void
{
    for (const auto& child : m_children) {
        body(child);
    }
}

// MARK: - Animation Management

auto kestrel::ui::scene_entity::configure_animation_frames(std::int32_t frame_count) -> void
{
    m_frame_count = frame_count;
}

auto kestrel::ui::scene_entity::next_frame() -> void
{
    if (m_frame_count == 0 || m_animator.get()) {
        return;
    }

    constrain_frame(m_frame + 1);
}

auto kestrel::ui::scene_entity::constrain_frame(std::uint32_t frame) -> void
{
    if (frame > m_frame_count && !m_finished) {
        if (m_loops) {
            set_current_frame(0);

            if (m_animator.get()) {
                m_animator->reset();
            }

            return;
        }
        else if (m_on_animation_finish.state() && m_on_animation_finish.isFunction()) {
            m_on_animation_finish();
        }
        m_finished = true;

        if (m_animator.get()) {
            m_animator->pause();
        }
    }
    else if (!m_finished) {
        if (frame == 0 && !m_started && m_on_animation_start.state() && m_on_animation_start.isFunction()) {
            m_started = true;
            m_on_animation_start();
        }
        set_current_frame(frame);
    }
}

auto kestrel::ui::scene_entity::on_animation_start(const luabridge::LuaRef& callback) -> void
{
    m_on_animation_start = callback;
}

auto kestrel::ui::scene_entity::on_animation_finish(const luabridge::LuaRef& callback) -> void
{
    m_on_animation_finish = callback;
}

// MARK: -

auto kestrel::ui::scene_entity::replace(const lua_reference &entity) -> void
{
    m_entity = entity->internal_entity();
}

// MARK: - Layout & Drawing

auto kestrel::ui::scene_entity::layout() -> void
{
    if (m_on_layout.state() && m_on_layout.isFunction()) {
        m_on_layout(this);
    }
}

auto kestrel::ui::scene_entity::on_layout(const luabridge::LuaRef& callback) -> void
{
    m_on_layout = callback;
}

auto kestrel::ui::scene_entity::draw() -> void
{
    if (!m_entity) {
        return;
    }

    if (m_pressed && m_continuous_mouse_down_action) {
        if (m_on_mouse_down.state() && m_on_mouse_down.isFunction()) {
            m_on_mouse_down(event::lua_reference(new event(m_mouse_down_event)));
        }
        if (m_on_mouse_down_internal) {
            m_on_mouse_down_internal(m_mouse_down_event);
        }
    }

    if (m_animator.get()) {
        m_animator->advance(renderer::last_frame_time());
        constrain_frame(m_animator->frame());
    }

    m_entity->draw();

    if (m_next_frame_on_draw && !m_animator.get()) {
        next_frame();
    }

    for (auto& child : m_children) {
        child->set_draw_position(draw_position() + child->position());
        child->set_draw_size(child->render_size());
        child->draw();
    }
}

// MARK: - Mouse Events

auto kestrel::ui::scene_entity::on_mouse_enter(const luabridge::LuaRef& callback) -> void
{
    m_on_mouse_enter = callback;
}

auto kestrel::ui::scene_entity::on_mouse_exit(const luabridge::LuaRef& callback) -> void
{
    m_on_mouse_exit = callback;
}

auto kestrel::ui::scene_entity::on_mouse_down(const luabridge::LuaRef& callback) -> void
{
    m_on_mouse_down = callback;
}

auto kestrel::ui::scene_entity::on_mouse_release(const luabridge::LuaRef& callback) -> void
{
    m_on_mouse_release = callback;
}

auto kestrel::ui::scene_entity::on_mouse_drag(const luabridge::LuaRef& callback) -> void
{
    m_on_mouse_drag = callback;
}

auto kestrel::ui::scene_entity::on_mouse_enter_internal(const std::function<auto(const event&)->void>& callback) -> void
{
    m_on_mouse_enter_internal = callback;
}

auto kestrel::ui::scene_entity::on_mouse_exit_internal(const std::function<auto(const event&)->void>& callback) -> void
{
    m_on_mouse_exit_internal = callback;
}

auto kestrel::ui::scene_entity::on_mouse_down_internal(const std::function<auto(const event&)->void>& callback) -> void
{
    m_on_mouse_down_internal = callback;
}

auto kestrel::ui::scene_entity::on_mouse_release_internal(const std::function<auto(const event&)->void>& callback) -> void
{
    m_on_mouse_release_internal = callback;
}

auto kestrel::ui::scene_entity::on_mouse_drag_internal(const std::function<auto(const event&)->void>& callback) -> void
{
    m_on_mouse_drag_internal = callback;
}


auto kestrel::ui::scene_entity::send_event(const event& e) -> void
{
    if (e.is_mouse_event()) {
        auto point = e.location();

        if (!m_mouse_over && hit_test(point)) {
            m_mouse_over = true;
            if (m_on_mouse_enter.state() && m_on_mouse_enter.isFunction()) {
                m_on_mouse_enter(event::lua_reference { new event(e) });
            }

            if (m_on_mouse_enter_internal) {
                m_on_mouse_enter_internal(e);
            }
        }
        else if (m_mouse_over && !hit_test(point)) {
            m_mouse_over = false;
            if (m_on_mouse_exit.state() && m_on_mouse_exit.isFunction()) {
                m_on_mouse_exit(event::lua_reference { new event(e) });
            }

            if (m_on_mouse_exit_internal) {
                m_on_mouse_exit_internal(e);
            }
        }

        if (m_mouse_over && e.has(event_type::any_mouse_down) && !m_pressed) {
            m_pressed = true;
            m_mouse_down_event = e;

            if (m_on_mouse_down.state() && m_on_mouse_down.isFunction()) {
                m_on_mouse_down(event::lua_reference { new event(e) });
            }

            if (m_on_mouse_down_internal) {
                m_on_mouse_down_internal(e);
            }
        }
        else if (e.has(event_type::any_mouse_up) && m_pressed) {
            m_pressed = false;
            if (m_on_mouse_release.state() && m_on_mouse_release.isFunction()) {
                m_on_mouse_release(event::lua_reference { new event(e) });
            }

            if (m_on_mouse_release_internal) {
                m_on_mouse_release_internal(e);
            }
        }

        if (m_on_mouse_drag.state() && m_on_mouse_drag.isFunction() && e.has(event_type::any_mouse_down) && e.has(event_type::mouse_move)) {
            m_on_mouse_drag(event::lua_reference { new event(e) });
            m_mouse_dragged = true;
            if (m_on_mouse_drag_internal) {
                m_on_mouse_drag_internal(e);
            }
        }
    }
}

auto kestrel::ui::scene_entity::hit_test(const math::point& p) const -> bool
{
    math::rect frame { math::point(0), m_entity->get_draw_size() };
    return frame.contains_point(p);
}

// MARK: - Entity

auto kestrel::ui::scene_entity::internal_entity() const -> std::shared_ptr<ecs::entity>
{
    return m_entity;
}

auto kestrel::ui::scene_entity::change_internal_entity(const std::shared_ptr<ecs::entity>& entity) -> void
{
    m_entity = entity;
}

// MARK: - Collisions

auto kestrel::ui::scene_entity::build_collision_map() -> void
{
    // The collision map is part of the sprite sheet, with each sprite frame having its own.
    m_entity->sprite_sheet()->build_collision_maps();
}