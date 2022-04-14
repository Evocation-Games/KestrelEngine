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

#include "core/graphics/common/canvas.hpp"
#include "core/graphics/common/text.hpp"
#include "core/asset/static_image.hpp"
#include "core/asset/legacy/spriteworld/sprite.hpp"
#include "core/asset/legacy/macintosh/picture.hpp"
#include "core/asset/legacy/macintosh/color_icon.hpp"

// MARK: - Lua

auto ui::scene_entity::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state> &lua) -> void
{
    lua->global_namespace()
        .beginNamespace("BlendMode")
            .addProperty("Normal", [] (const auto L) {
                return static_cast<int>(graphics::entity::blend::normal);
            })
            .addProperty("Light", [] (const auto L) {
                return static_cast<int>(graphics::entity::blend::light);
            })
        .endNamespace()
//        .beginNamespace("UI")
            .beginClass<ui::scene_entity>("SceneEntity")
                .addConstructor<auto(*)(const luabridge::LuaRef&)->void, lua_reference>()
                .addProperty("position", &scene_entity::position, &scene_entity::set_position)
                .addProperty("drawPosition", &scene_entity::draw_position, &scene_entity::set_draw_position)
                .addProperty("size", &scene_entity::size, &scene_entity::set_size)
                .addProperty("halfSize", &scene_entity::half_size)
                .addProperty("renderSize", &scene_entity::render_size, &scene_entity::set_render_size)
                .addProperty("frameCount", &scene_entity::frame_count)
                .addProperty("frame", &scene_entity::current_frame, &scene_entity::set_current_frame) // TODO: Deprecate this variant.
                .addProperty("currentFrame", &scene_entity::current_frame, &scene_entity::set_current_frame)
                .addProperty("nextFrameOnDraw", &scene_entity::advances_to_next_frame_on_draw, &scene_entity::set_advances_to_next_frame_on_draw)
                .addProperty("loops", &scene_entity::animation_loops, &scene_entity::set_animation_loops)
                .addProperty("centered", &scene_entity::centered, &scene_entity::set_centered)
                .addProperty("alpha", &scene_entity::alpha, &scene_entity::set_alpha)
                .addProperty("blend", &scene_entity::blend_mode, &scene_entity::set_blend_mode)
                .addProperty("clippingArea", &scene_entity::clipping_area, &scene_entity::set_clipping_area)
                .addProperty("clippingOffset", &scene_entity::clipping_offset, &scene_entity::set_clipping_offset)
                .addProperty("children", &scene_entity::children)
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
//        .endNamespace();
}

// MARK: - Construction

ui::scene_entity::scene_entity(const std::shared_ptr<graphics::entity>& entity)
    : m_entity(entity), m_position({ 0 }), m_frame(0)
{

}

ui::scene_entity::scene_entity(const luabridge::LuaRef& entity_provider)
    : m_entity(spawn_entity(entity_provider)), m_position({ 0 }), m_frame(0)
{

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
    auto pos = m_entity->get_position();
    if (m_centered) {
        auto hsize = half_size();
        return pos + math::point(hsize.width, hsize.height);
    }
    else {
        return pos;
    }
}

auto ui::scene_entity::centered() const -> bool
{
    return m_centered;
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

// MARK: - Setters

auto ui::scene_entity::set_position(const math::point& v) -> void
{
    m_position = v;
}

auto ui::scene_entity::set_draw_position(const math::point& v) -> void
{
    if (m_centered) {
        auto hsize = half_size();
        m_entity->set_position({ v.x - hsize.width, v.y - hsize.height });
    }
    else {
        m_entity->set_position(v);
    }
}

auto ui::scene_entity::set_centered(bool v) -> void
{
    m_centered = v;
}

auto ui::scene_entity::set_size(const math::size& v) -> void
{
    m_entity->set_size(v);
}

auto ui::scene_entity::set_render_size(const math::size& v) -> void
{
    m_entity->set_render_size(v);
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
    if (m_frame_count == 0) {
        return;
    }

    if (m_frame > m_frame_count && !m_finished) {
        if (m_loops) {
            set_current_frame(0);
            return;
        }
        else if (m_on_animation_finish.state() && m_on_animation_finish.isFunction()) {
            m_on_animation_finish();
        }
        m_finished = true;
    }
    else if (!m_finished) {
        if (m_frame == 0 && !m_started && m_on_animation_start.state() && m_on_animation_start.isFunction()) {
            m_started = true;
            m_on_animation_start();
        }
        set_current_frame(current_frame() + 1);
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

// MARK: - Layout & Drawing

auto ui::scene_entity::layout() -> void
{
    if (m_on_layout.state() && m_on_layout.isFunction()) {
        m_on_layout();
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

    m_entity->draw();

    if (m_next_frame_on_draw) {
        next_frame();
    }

    for (auto i = 0; i < m_children.size(); ++i) {
        m_children.at(i)->draw();
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

auto ui::scene_entity::send_event(const event& e) -> void
{
    if (e.is_mouse_event()) {
        auto point = e.location();

        if (!m_mouse_over && hit_test(point)) {
            m_mouse_over = true;
            if (m_on_mouse_enter.state() && m_on_mouse_enter.isFunction()) {
                m_on_mouse_enter(event::lua_reference { new event(e) });
            }
        }
        else if (m_mouse_over && !hit_test(point)) {
            m_mouse_over = false;
            if (m_on_mouse_exit.state() && m_on_mouse_exit.isFunction()) {
                m_on_mouse_exit(event::lua_reference { new event(e) });
            }
        }

        if (m_mouse_over && e.is_pressed() && !m_pressed) {
            m_pressed = true;
            if (m_on_mouse_down.state() && m_on_mouse_down.isFunction()) {
                m_on_mouse_down(event::lua_reference { new event(e) });
            }
        }
        else if (e.is_released() && m_pressed) {
            m_pressed = false;
            if (m_on_mouse_release.state() && m_on_mouse_release.isFunction()) {
                m_on_mouse_release(event::lua_reference { new event(e) });
            }
        }

        if (m_on_mouse_drag.state() && m_on_mouse_drag.isFunction() && e.is_pressed() && e.has_moved()) {
            m_on_mouse_drag(event::lua_reference { new event(e) });
            m_mouse_dragged = true;
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
