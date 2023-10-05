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
#include <libKestrel/ui/entity/text_entity.hpp>
#include <libKestrel/ui/entity/line_entity.hpp>
#include <libKestrel/kestrel.hpp>
#include <libKestrel/graphics/canvas/canvas.hpp>
#include <libKestrel/graphics/image/static_image.hpp>
#include <libKestrel/graphics/legacy/spriteworld/sprite.hpp>
#include <libKestrel/graphics/legacy/macintosh/picture.hpp>
#include <libKestrel/graphics/legacy/macintosh/color_icon.hpp>
#include <libKestrel/graphics/renderer/common/blending.hpp>
#include <libKestrel/graphics/renderer/common/renderer.hpp>
#include <libKestrel/ui/layout/aspect_ratio.hpp>

// MARK: - Construction

kestrel::ui::scene_entity::scene_entity(const std::shared_ptr<ecs::entity>& entity)
    : m_entity(entity), m_position(0), m_frame(0)
{
    m_parent_bounds = { math::point(0), renderer::window_size() };
}

kestrel::ui::scene_entity::scene_entity(const luabridge::LuaRef& entity_provider)
    : m_entity(spawn_entity(entity_provider)), m_position(0), m_frame(0)
{
    m_parent_bounds = { math::point(0), renderer::window_size() };
}

kestrel::ui::scene_entity::scene_entity(const image::static_image::lua_reference &image)
    : m_entity(spawn_entity(image)), m_position(0), m_frame(0)
{
    m_parent_bounds = { math::point(0), renderer::window_size() };
}

kestrel::ui::scene_entity::scene_entity(const graphics::canvas::lua_reference &canvas)
    : m_entity(spawn_entity(canvas)), m_position(), m_frame(0)
{
    m_parent_bounds = { math::point(0), renderer::window_size() };
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
      m_parent_bounds(entity->m_parent_bounds)
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
    return m_entity->body();
}

auto kestrel::ui::scene_entity::position() const -> math::point
{
    return m_position;
}

auto kestrel::ui::scene_entity::anchor_point() const -> enum layout::axis_origin
{
    return m_anchor;
}

auto kestrel::ui::scene_entity::lua_anchor_point() const -> std::int32_t
{
    return static_cast<std::int32_t>(anchor_point());
}

auto kestrel::ui::scene_entity::scaling_mode() const -> enum layout::scaling_mode
{
    return m_scaling_mode;
}

auto kestrel::ui::scene_entity::lua_scaling_mode() const -> std::int32_t
{
    return static_cast<std::int32_t>(scaling_mode());
}

auto kestrel::ui::scene_entity::size() const -> math::size
{
    return m_entity->get_size();
}

auto kestrel::ui::scene_entity::half_size() const -> math::size
{
    return m_entity->get_size() / 2;
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

auto kestrel::ui::scene_entity::children() const -> lua::vector<luabridge::LuaRef>
{
    return {};
}

auto kestrel::ui::scene_entity::animator() const -> renderer::animator::lua_reference
{
    return m_animator;
}

auto kestrel::ui::scene_entity::continuous_mouse_down_action() const -> bool
{
    return m_continuous_mouse_down_action;
}

auto kestrel::ui::scene_entity::hidden() const -> bool
{
    return m_hidden;
}

// MARK: - Setters

auto kestrel::ui::scene_entity::set_position(const math::point& v) -> void
{
    m_position = v;
    update_position();
}

auto kestrel::ui::scene_entity::update_position() -> void
{
    auto scale_factor = 1.f;
    if (auto scene = internal_entity()->scene()) {
        if (!ignores_scene_scaling_factor()) {
            scale_factor = scene->scaling_factor();
        }
    }
    auto position = entity_position(m_parent_bounds.size(), this->anchor_point(), this->position() * scale_factor, this->size() * scale_factor);
    m_entity->set_position(position + m_parent_bounds.origin());
    update_children();
}

auto kestrel::ui::scene_entity::set_anchor_point(enum layout::axis_origin v) -> void
{
    m_anchor = v;
    update_position();
}

auto kestrel::ui::scene_entity::set_lua_anchor_point(std::int32_t v) -> void
{
    set_anchor_point(static_cast<enum layout::axis_origin>(v));
}

auto kestrel::ui::scene_entity::set_scaling_mode(enum layout::scaling_mode v) -> void
{
    m_scaling_mode = v;
    update_scaling();
}

auto kestrel::ui::scene_entity::set_lua_scaling_mode(std::int32_t v) -> void
{
    set_scaling_mode(static_cast<enum layout::scaling_mode>(v));
}

auto kestrel::ui::scene_entity::update_scaling() -> void
{
    if (m_scaling_mode == layout::scaling_mode::aspect_fill) {
        auto scaled = layout::calculate_size(
            m_scaling_mode,
            this->size(),
            m_entity->texture()->size(),
            layout::aspect_ratio(m_entity->texture()->size())
        );

        auto offset = ((scaled - this->size()) / 2.f) / scaled;
        auto size = this->size() / scaled;

        m_entity->set_scaled_texture_area(math::rect(offset.width(), offset.height(), size.width(), size.height()));
    }
}

auto kestrel::ui::scene_entity::set_ignores_scene_scaling_factor(bool f) -> void
{
    m_entity->set_ignores_scene_scaling_factor(f);
}

auto kestrel::ui::scene_entity::ignores_scene_scaling_factor() const -> bool
{
    return m_entity->ignores_scene_scaling_factor();
}

auto kestrel::ui::scene_entity::set_size(const math::size& v) -> void
{
    m_entity->set_size(v);
    update_position();
    update_scaling();
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

auto kestrel::ui::scene_entity::set_hidden(bool hidden) -> void
{
    m_hidden = hidden;
}

// MARK: - Child Entity Management

auto kestrel::ui::scene_entity::add_entity(const lua_reference & child) -> void
{
    add_child_entity({ kestrel::lua_runtime()->internal_state(), child });
}

auto kestrel::ui::scene_entity::add_child_entity(const luabridge::LuaRef& child) -> void
{
    m_children.emplace_back(child);
    update_children();
}

auto kestrel::ui::scene_entity::each_child(const luabridge::LuaRef& body) const -> void
{
    for (const auto& child : m_children) {
        body(child);
    }
}

auto kestrel::ui::scene_entity::remove_entity(const kestrel::ui::scene_entity::lua_reference &child) -> void
{
    for (auto i = 0; i < m_children.size(); ++i) {
        if (m_children.at(i) == child) {
            m_children.remove(i + 1);
            return;
        }
    }
}

auto kestrel::ui::scene_entity::update_children() -> void
{
    for (auto& child : m_children) {
        if (lua::ref_isa<scene_entity>(child)) {
            auto entity = child.cast<scene_entity::lua_reference>();
            entity->m_parent_bounds = {
                m_entity->get_position(), size()
            };
            entity->update_position();
        }
        else if (lua::ref_isa<text_entity>(child)) {
            auto entity = child.cast<text_entity::lua_reference>();
            entity->m_parent_bounds = {
                m_entity->get_position(), size()
            };
            entity->update_position();
        }
        else if (lua::ref_isa<line_entity>(child)) {
            auto entity = child.cast<line_entity::lua_reference>();
            entity->m_parent_bounds = {
                m_entity->get_position(), size()
            };
        }
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

auto kestrel::ui::scene_entity::set_parent_bounds(const math::rect &bounds) -> void
{
    m_parent_bounds = bounds;
    update_position();
}

auto kestrel::ui::scene_entity::parent_bounds() const -> math::rect
{
    return m_parent_bounds;
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

auto kestrel::ui::scene_entity::shader() const -> renderer::shader::source::lua_reference
{
    return m_entity->shader();
}

auto kestrel::ui::scene_entity::set_shader(const renderer::shader::source::lua_reference &shader) -> void
{
    m_entity->set_shader(shader);
}

auto kestrel::ui::scene_entity::bind_shader_attachment(std::int32_t idx, double v1) -> void
{
    m_entity->bind_shader_attachment(idx, v1);
}

auto kestrel::ui::scene_entity::bind_shader_attachment2(std::int32_t idx, double v1, double v2) -> void
{
    m_entity->bind_shader_attachment2(idx, v1, v2);
}

auto kestrel::ui::scene_entity::bind_shader_attachment3(std::int32_t idx, double v1, double v2, double v3) -> void
{
    m_entity->bind_shader_attachment3(idx, v1, v2, v3);
}

auto kestrel::ui::scene_entity::bind_shader_attachment4(std::int32_t idx, double v1, double v2, double v3, double v4) -> void
{
    m_entity->bind_shader_attachment4(idx, v1, v2, v3, v4);
}

auto kestrel::ui::scene_entity::draw() -> void
{
    if (!m_entity || m_hidden) {
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
        if (lua::ref_isa<scene_entity>(child)) {
            child.cast<scene_entity::lua_reference>()->draw();
        }
        else if (lua::ref_isa<text_entity>(child)) {
            child.cast<text_entity::lua_reference>()->draw();
        }
        else if (lua::ref_isa<line_entity>(child)) {
            child.cast<line_entity::lua_reference>()->draw();
        }
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
    for (auto& child : m_children) {
        if (lua::ref_isa<scene_entity>(child)) {
            child.cast<scene_entity::lua_reference>()->send_event(e);
        }
    }

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

        if (m_mouse_over && e.has(::ui::event::any_mouse_down) && !m_pressed) {
            m_pressed = true;
            m_mouse_down_event = e;

            if (m_on_mouse_down.state() && m_on_mouse_down.isFunction()) {
                m_on_mouse_down(event::lua_reference { new event(e) });
            }

            if (m_on_mouse_down_internal) {
                m_on_mouse_down_internal(e);
            }
        }
        else if (e.has(::ui::event::any_mouse_up) && m_pressed) {
            m_pressed = false;
            if (m_on_mouse_release.state() && m_on_mouse_release.isFunction()) {
                m_on_mouse_release(event::lua_reference { new event(e) });
            }

            if (m_on_mouse_release_internal) {
                m_on_mouse_release_internal(e);
            }
        }

        if (m_on_mouse_drag.state() && m_on_mouse_drag.isFunction() && e.has(::ui::event::any_mouse_down) && e.has(::ui::event::mouse_move)) {
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
    return m_entity->get_bounds().contains_point(p) && !m_hidden;
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

auto kestrel::ui::scene_entity::hitbox_color() const -> graphics::color::lua_reference
{
    return m_entity->hitbox_color();
}

auto kestrel::ui::scene_entity::set_hitbox_color(const graphics::color::lua_reference &color) -> void
{
    m_entity->set_hitbox_color(color);
}

auto kestrel::ui::scene_entity::setup_hitbox() -> void
{
    // The collision map is part of the sprite sheet, with each sprite frame having its own.
    m_entity->sprite_sheet()->build_hitboxes();
    m_entity->set_sprite_index(m_entity->get_sprite_index());
}