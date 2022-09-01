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

#include "core/ui/entity/scene_entity.hpp"
#include "core/environment.hpp"

#include "core/graphics/common/canvas.hpp"
#include "core/graphics/common/text.hpp"
#include "core/asset/static_image.hpp"
#include "core/asset/legacy/spriteworld/sprite.hpp"
#include "core/asset/legacy/macintosh/picture.hpp"
#include "core/asset/legacy/macintosh/color_icon.hpp"
#include "renderer/common/blending.hpp"
#include "renderer/common/renderer.hpp"

// MARK: - Lua

static auto blend_mode_light() -> int
{
    return static_cast<int>(renderer::blending::light);
}

static auto blend_mode_normal() -> int
{
    return static_cast<int>(renderer::blending::normal);
}

auto ui::scene_entity::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("BlendMode")
            .addProperty("Normal", &blend_mode_normal)
            .addProperty("Light", &blend_mode_light)
        .endNamespace()
        .beginClass<ui::scene_entity>("SceneEntity")
            .addConstructor<auto(*)(const luabridge::LuaRef&)->void, lua_reference>()
            .addProperty("position", &scene_entity::position, &scene_entity::set_position)
            .addProperty("drawPosition", &scene_entity::draw_position, &scene_entity::set_draw_position)
            .addProperty("drawSize", &scene_entity::draw_size, &scene_entity::set_draw_size)
            .addProperty("size", &scene_entity::size, &scene_entity::set_size)
            .addProperty("halfSize", &scene_entity::half_size)
            .addProperty("renderSize", &scene_entity::render_size, &scene_entity::set_render_size)
            .addProperty("ignorePositioningFrameScaler", &scene_entity::ignore_positioning_frame_scaler, &scene_entity::set_ignore_positioning_frame_scaler)
            .addProperty("frameCount", &scene_entity::frame_count)
            .addProperty("frame", &scene_entity::current_frame, &scene_entity::set_current_frame) // TODO: Deprecate this variant.
            .addProperty("currentFrame", &scene_entity::current_frame, &scene_entity::set_current_frame)
            .addProperty("nextFrameOnDraw", &scene_entity::advances_to_next_frame_on_draw, &scene_entity::set_advances_to_next_frame_on_draw)
            .addProperty("loops", &scene_entity::animation_loops, &scene_entity::set_animation_loops)
            .addProperty("axisAnchor", &scene_entity::lua_anchor_point, &scene_entity::set_lua_anchor_point)
            .addProperty("alpha", &scene_entity::alpha, &scene_entity::set_alpha)
            .addProperty("blend", &scene_entity::blend_mode, &scene_entity::set_blend_mode)
            .addProperty("clippingArea", &scene_entity::clipping_area, &scene_entity::set_clipping_area)
            .addProperty("clippingOffset", &scene_entity::clipping_offset, &scene_entity::set_clipping_offset)
            .addProperty("children", &scene_entity::children)
            .addProperty("animator", &scene_entity::animator, &scene_entity::set_animator)
            .addFunction("setSprite", &scene_entity::set_sprite)
            .addFunction("addChildEntity", &scene_entity::add_child_entity)
            .addFunction("eachChild", &scene_entity::each_child)
            .addFunction("configureAnimation", &scene_entity::configure_animation_frames)
            .addFunction("nextFrame", &scene_entity::next_frame)
            .addFunction("onAnimationStart", &scene_entity::on_animation_start)
            .addFunction("onAnimationFinish", &scene_entity::on_animation_finish)
            .addFunction("layout", &scene_entity::layout)
            .addFunction("onLayout", &scene_entity::on_layout)
            .addFunction("draw", &scene_entity::draw)
            .addFunction("onMouseEnter", &scene_entity::on_mouse_enter)
            .addFunction("onMouseExit", &scene_entity::on_mouse_exit)
            .addFunction("onMouseDown", &scene_entity::on_mouse_down)
            .addFunction("onMouseRelease", &scene_entity::on_mouse_release)
            .addFunction("onMouseDrag", &scene_entity::on_mouse_drag)
            .addFunction("sendEvent", &scene_entity::send_event)
            .addFunction("hitTest", &scene_entity::hit_test)
        .endClass();
}

// MARK: - Construction

ui::scene_entity::scene_entity(const std::shared_ptr<graphics::entity>& entity)
    : m_entity(entity), m_position(0), m_frame(0)
{

}

ui::scene_entity::scene_entity(const luabridge::LuaRef& entity_provider)
    : m_entity(spawn_entity(entity_provider)), m_position(0), m_frame(0)
{

}

ui::scene_entity::scene_entity(const asset::static_image::lua_reference &image)
    : m_entity(spawn_entity(image)), m_position(0), m_frame(0)
{

}

ui::scene_entity::scene_entity(const graphics::canvas::lua_reference &canvas)
    : m_entity(spawn_entity(canvas)), m_position(), m_frame(0)
{

}

ui::scene_entity::scene_entity(const std::shared_ptr<scene_entity>& entity)
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
      m_continuous_mouse_down_action(entity->m_continuous_mouse_down_action)
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

template<class T>
static inline auto lua_type_check(const luabridge::LuaRef& ref) -> bool
{
    return luabridge::detail::Userdata::isInstance<T>(ref.state(), -1);
}

auto ui::scene_entity::spawn_entity(const luabridge::LuaRef& entity_provider) -> std::shared_ptr<graphics::entity>
{
    if (entity_provider.isUserdata()) {
        if (lua_type_check<graphics::canvas>(entity_provider)) {
            auto canvas = entity_provider.cast<graphics::canvas::lua_reference>();
            auto entity = canvas->spawn_entity({ 0, 0 });
            return entity;
        }
        else if (lua_type_check<graphics::text>(entity_provider)) {
            auto text = entity_provider.cast<graphics::text>();
            return text.spawn_entity({ 0, 0 });
        }
        else if (lua_type_check<asset::legacy::spriteworld::sprite>(entity_provider)) {
            auto sprite = entity_provider.cast<asset::legacy::spriteworld::sprite>();
            return sprite.spawn_entity({ 0, 0 });
        }
        else if (lua_type_check<asset::legacy::macintosh::quickdraw::picture>(entity_provider)) {
            auto pict = entity_provider.cast<asset::legacy::macintosh::quickdraw::picture>();
            return pict.spawn_entity({ 0, 0 });
        }
        else if (lua_type_check<asset::legacy::macintosh::quickdraw::color_icon>(entity_provider)) {
            auto cicn = entity_provider.cast<asset::legacy::macintosh::quickdraw::color_icon>();
            return cicn.spawn_entity({ 0, 0 });
        }
        else if (lua_type_check<asset::static_image>(entity_provider)) {
            auto image = entity_provider.cast<asset::static_image>();
            return image.spawn_entity({ 0, 0 });
        }
    }
    return nullptr;
}

auto ui::scene_entity::spawn_entity(const asset::static_image::lua_reference &image) -> std::shared_ptr<graphics::entity>
{
    if (image.get()) {
        return image->spawn_entity({ 0, 0 });
    }
    return {};
}

auto ui::scene_entity::spawn_entity(const graphics::canvas::lua_reference &canvas) -> std::shared_ptr<graphics::entity>
{
    if (canvas.get()) {
        return canvas->spawn_entity({ 0, 0 });
    }
    return {};
}

auto ui::scene_entity::set_sprite(const luabridge::LuaRef& sprite) -> void
{
    if (sprite.isNil()) {
        return;
    }
    m_entity = ui::scene_entity::spawn_entity(sprite);
}

// MARK: - Accessors

auto ui::scene_entity::position() const -> math::point
{
    return m_position;
}

auto ui::scene_entity::draw_position() const -> math::point
{
    auto offset = origin_for_axis(render_size(), m_anchor);
    return m_entity->get_position() + offset;
}

auto ui::scene_entity::anchor_point() const -> enum layout::axis_origin
{
    return m_anchor;
}

auto ui::scene_entity::lua_anchor_point() const -> std::int32_t
{
    return static_cast<std::int32_t>(anchor_point());
}

auto ui::scene_entity::size() const -> math::size
{
    return m_entity->get_size();
}

auto ui::scene_entity::half_size() const -> math::size
{
    return m_entity->get_size() / 2;
}

auto ui::scene_entity::render_size() const -> math::size
{
    return m_entity->get_render_size();
}

auto ui::scene_entity::draw_size() const -> math::size
{
    return m_entity->get_draw_size();
}

auto ui::scene_entity::frame_count() const -> int32_t
{
    return m_frame_count;
}

auto ui::scene_entity::current_frame() const -> int32_t
{
    return m_frame;
}

auto ui::scene_entity::advances_to_next_frame_on_draw() const -> bool
{
    return m_next_frame_on_draw;
}

auto ui::scene_entity::animation_loops() const -> bool
{
    return m_loops;
}

auto ui::scene_entity::alpha() const -> double
{
    return m_entity->get_alpha();
}

auto ui::scene_entity::blend_mode() const -> int
{
    return m_entity->get_blend_lua();
}

auto ui::scene_entity::clipping_area() const -> math::size
{
    return m_entity->clipping_area();
}

auto ui::scene_entity::clipping_offset() const -> math::point
{
    return m_entity->clipping_offset();
}

auto ui::scene_entity::children() const -> util::lua_vector<lua_reference>
{
    return m_children;
}

auto ui::scene_entity::animator() const -> renderer::animator::lua_reference
{
    return m_animator;
}

auto ui::scene_entity::ignore_positioning_frame_scaler() const -> bool
{
    return m_ignore_positioning_frame_scaler;
}


// MARK: - Setters

auto ui::scene_entity::set_position(const math::point& v) -> void
{
    m_position = v;
}

auto ui::scene_entity::set_draw_position(const math::point& v) -> void
{
    auto offset = origin_for_axis(render_size(), m_anchor);
    m_entity->set_position(v - offset);
}

auto ui::scene_entity::set_anchor_point(enum layout::axis_origin v) -> void
{
    m_anchor = v;
}

auto ui::scene_entity::set_lua_anchor_point(std::int32_t v) -> void
{
    set_anchor_point(static_cast<enum layout::axis_origin>(v));
}

auto ui::scene_entity::set_size(const math::size& v) -> void
{
    m_entity->set_size(v);
}

auto ui::scene_entity::set_render_size(const math::size& v) -> void
{
    m_entity->set_render_size(v);
}

auto ui::scene_entity::set_draw_size(const math::size& v) -> void
{
    m_entity->set_draw_size(v);
}

auto ui::scene_entity::set_current_frame(int32_t v) -> void
{
    if (v >= 0 && v < m_frame_count) {
        m_entity->set_sprite_index(static_cast<int>(v));
    }
    m_frame = v;
}

auto ui::scene_entity::set_advances_to_next_frame_on_draw(bool v) -> void
{
    m_next_frame_on_draw = v;
}

auto ui::scene_entity::set_animation_loops(bool v) -> void
{
    m_loops = v;
}

auto ui::scene_entity::set_alpha(double v) -> void
{
    m_entity->set_alpha(v);
}

auto ui::scene_entity::set_blend_mode(int v) -> void
{
    m_entity->set_blend_lua(v);
}

auto ui::scene_entity::set_clipping_area(const math::size& v) -> void
{
    m_entity->set_clipping_area(v);
}

auto ui::scene_entity::set_clipping_offset(const math::point& v) -> void
{
    m_entity->set_clipping_offset(v);
}

auto ui::scene_entity::set_animator(const renderer::animator::lua_reference &animator) -> void
{
    m_animator = animator;
}

auto ui::scene_entity::set_continuous_mouse_down_action(bool continuous) -> void
{
    m_continuous_mouse_down_action = continuous;
}

auto ui::scene_entity::set_ignore_positioning_frame_scaler(bool f) -> void
{
    m_ignore_positioning_frame_scaler = f;
}

// MARK: - Child Entity Management

auto ui::scene_entity::add_child_entity(const lua_reference& child) -> void
{
    m_children.emplace_back(child);
}

auto ui::scene_entity::each_child(const luabridge::LuaRef& body) const -> void
{
    for (auto i = 0; i < m_children.size(); ++i) {
        const auto& child = m_children.at(i);
        body(child);
    }
}

// MARK: - Animation Management

auto ui::scene_entity::configure_animation_frames(int32_t frame_count) -> void
{
    m_frame_count = frame_count;
}

auto ui::scene_entity::next_frame() -> void
{
    if (m_frame_count == 0 || m_animator.get()) {
        return;
    }

    constrain_frame(m_frame + 1);
}

auto ui::scene_entity::constrain_frame(int32_t frame) -> void
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

auto ui::scene_entity::on_animation_start(const luabridge::LuaRef& callback) -> void
{
    m_on_animation_start = callback;
}

auto ui::scene_entity::on_animation_finish(const luabridge::LuaRef& callback) -> void
{
    m_on_animation_finish = callback;
}

// MARK: -

auto ui::scene_entity::replace(const lua_reference &entity) -> void
{
    m_entity = entity->internal_entity();
}

// MARK: - Layout & Drawing

auto ui::scene_entity::layout() -> void
{
    if (m_on_layout.state() && m_on_layout.isFunction()) {
        m_on_layout(this);
    }
}

auto ui::scene_entity::on_layout(const luabridge::LuaRef& callback) -> void
{
    m_on_layout = callback;
}

auto ui::scene_entity::draw() -> void
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

    for (auto i = 0; i < m_children.size(); ++i) {
        auto child = m_children.at(i);

        child->set_draw_position(draw_position() + child->position());
        child->set_draw_size(child->render_size());
        child->draw();
    }
}

// MARK: - Mouse Events

auto ui::scene_entity::on_mouse_enter(const luabridge::LuaRef& callback) -> void
{
    m_on_mouse_enter = callback;
}

auto ui::scene_entity::on_mouse_exit(const luabridge::LuaRef& callback) -> void
{
    m_on_mouse_exit = callback;
}

auto ui::scene_entity::on_mouse_down(const luabridge::LuaRef& callback) -> void
{
    m_on_mouse_down = callback;
}

auto ui::scene_entity::on_mouse_release(const luabridge::LuaRef& callback) -> void
{
    m_on_mouse_release = callback;
}

auto ui::scene_entity::on_mouse_drag(const luabridge::LuaRef& callback) -> void
{
    m_on_mouse_drag = callback;
}

auto ui::scene_entity::on_mouse_enter_internal(const std::function<auto(const event&)->void>& callback) -> void
{
    m_on_mouse_enter_internal = callback;
}

auto ui::scene_entity::on_mouse_exit_internal(const std::function<auto(const event&)->void>& callback) -> void
{
    m_on_mouse_exit_internal = callback;
}

auto ui::scene_entity::on_mouse_down_internal(const std::function<auto(const event&)->void>& callback) -> void
{
    m_on_mouse_down_internal = callback;
}

auto ui::scene_entity::on_mouse_release_internal(const std::function<auto(const event&)->void>& callback) -> void
{
    m_on_mouse_release_internal = callback;
}

auto ui::scene_entity::on_mouse_drag_internal(const std::function<auto(const event&)->void>& callback) -> void
{
    m_on_mouse_drag_internal = callback;
}


auto ui::scene_entity::send_event(const event& e) -> void
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

        if (m_mouse_over && e.is_pressed() && !m_pressed) {
            m_pressed = true;
            m_mouse_down_event = e;

            if (m_on_mouse_down.state() && m_on_mouse_down.isFunction()) {
                m_on_mouse_down(event::lua_reference { new event(e) });
            }

            if (m_on_mouse_down_internal) {
                m_on_mouse_down_internal(e);
            }
        }
        else if (e.is_released() && m_pressed) {
            m_pressed = false;
            if (m_on_mouse_release.state() && m_on_mouse_release.isFunction()) {
                m_on_mouse_release(event::lua_reference { new event(e) });
            }

            if (m_on_mouse_release_internal) {
                m_on_mouse_release_internal(e);
            }
        }

        if (m_on_mouse_drag.state() && m_on_mouse_drag.isFunction() && e.is_pressed() && e.has_moved()) {
            m_on_mouse_drag(event::lua_reference { new event(e) });
            m_mouse_dragged = true;
            if (m_on_mouse_drag_internal) {
                m_on_mouse_drag_internal(e);
            }
        }
    }
}

auto ui::scene_entity::hit_test(const math::point& p) const -> bool
{
    math::rect frame { math::point(0), m_entity->get_size() };
    return frame.contains_point(p);
}

// MARK: - Entity

auto ui::scene_entity::internal_entity() const -> std::shared_ptr<graphics::entity>
{
    return m_entity;
}

auto ui::scene_entity::change_internal_entity(const std::shared_ptr<graphics::entity>& entity) -> void
{
    m_entity = entity;
}
