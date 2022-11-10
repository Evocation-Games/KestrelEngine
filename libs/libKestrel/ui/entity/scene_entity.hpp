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

#pragma once

#include <memory>
#include <vector>
#include <variant>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/lua/support/vector.hpp>
#include <libKestrel/math/size.hpp>
#include <libKestrel/math/point.hpp>
#include <libKestrel/event/event.hpp>
#include <libKestrel/entity/entity.hpp>
#include <libKestrel/graphics/canvas/canvas.hpp>
#include <libKestrel/graphics/image/static_image.hpp>
#include <libKestrel/graphics/renderer/common/animator.hpp>
#include <libKestrel/ui/layout/axis_origin.hpp>
#include <libKestrel/physics/body.hpp>

namespace kestrel::ui
{
    struct lua_api(SceneEntity, Available_0_8) scene_entity
    {
    public:
        has_constructable_lua_api(scene_entity);

        scene_entity() = default;
        explicit scene_entity(const std::shared_ptr<ecs::entity>& entity);
        explicit scene_entity(const image::static_image::lua_reference& image);
        explicit scene_entity(const graphics::canvas::lua_reference& canvas);
        explicit scene_entity(const std::shared_ptr<scene_entity>& entity);

        luatool_type_fix(const luabridge::LuaRef&, entity_provider)
        lua_constructor(Available_0_8) explicit scene_entity(const luabridge::LuaRef& entity_provider);

        auto set_id(const std::string& id) -> void { m_id = id; }

        lua_getter(position, Available_0_8) [[nodiscard]] auto position() const -> math::point;
        lua_getter(drawPosition, Available_0_8) [[nodiscard]] auto draw_position() const -> math::point;
        [[nodiscard]] auto anchor_point() const -> enum layout::axis_origin;
        lua_getter(anchorPoint, Available_0_8) [[nodiscard]] auto lua_anchor_point() const -> std::int32_t;
        lua_getter(size, Available_0_8) [[nodiscard]] auto size() const -> math::size;
        lua_getter(halfSize, Available_0_8) [[nodiscard]] auto half_size() const -> math::size;
        lua_getter(renderSize, Available_0_8) [[nodiscard]] auto render_size() const -> math::size;
        lua_getter(drawSize, Available_0_8) [[nodiscard]] auto draw_size() const -> math::size;
        lua_getter(frameCount, Available_0_8) [[nodiscard]] auto frame_count() const -> std::int32_t;
        lua_getter(currentFrame, Available_0_8) [[nodiscard]] auto current_frame() const -> std::int32_t;
        lua_getter(nextFrameOnDraw, Available_0_8) [[nodiscard]] auto advances_to_next_frame_on_draw() const -> bool;
        lua_getter(loops, Available_0_8) [[nodiscard]] auto animation_loops() const -> bool;
        lua_getter(alpha, Available_0_8) [[nodiscard]] auto alpha() const -> double;
        lua_getter(blend, Available_0_8) [[nodiscard]] auto blend_mode() const -> std::int32_t;
        lua_getter(clippingArea, Available_0_8) [[nodiscard]] auto clipping_area() const -> math::size;
        lua_getter(clippingOffset, Available_0_8) [[nodiscard]] auto clipping_offset() const -> math::point;
        lua_getter(children, Available_0_8) [[nodiscard]] auto children() const -> lua::vector<lua_reference>;
        lua_getter(animator, Available_0_8) [[nodiscard]] auto animator() const -> renderer::animator::lua_reference;
        lua_getter(ignorePositioningFrameScaler, Available_0_8) [[nodiscard]] auto ignore_positioning_frame_scaler() const -> bool;
        lua_getter(continuous, Available_0_8) [[nodiscard]] auto continuous_mouse_down_action() const -> bool;
        lua_getter(body, Available_0_8) [[nodiscard]] auto body() const -> physics::body::lua_reference;

        lua_setter(position, Available_0_8) auto set_position(const math::point& v) -> void;
        lua_setter(drawPosition, Available_0_8) auto set_draw_position(const math::point& v) -> void;
        auto set_anchor_point(enum layout::axis_origin v) -> void;
        lua_setter(anchorPoint, Available_0_8) auto set_lua_anchor_point(std::int32_t v) -> void;
        lua_setter(size, Available_0_8) auto set_size(const math::size& v) -> void;
        lua_setter(renderSize, Available_0_8) auto set_render_size(const math::size& v) -> void;
        lua_setter(drawSize, Available_0_8) auto set_draw_size(const math::size& v) -> void;
        lua_setter(currentFrame, Available_0_8) auto set_current_frame(std::uint32_t v) -> void;
        lua_setter(nextFrameOnDraw, Available_0_8) auto set_advances_to_next_frame_on_draw(bool v) -> void;
        lua_setter(loops, Available_0_8) auto set_animation_loops(bool v) -> void;
        lua_setter(alpha, Available_0_8) auto set_alpha(double v) -> void;
        lua_setter(blend, Available_0_8) auto set_blend_mode(std::int32_t v) -> void;
        lua_setter(clippingArea, Available_0_8) auto set_clipping_area(const math::size& v) -> void;
        lua_setter(clippingOffset, Available_0_8) auto set_clipping_offset(const math::point& v) -> void;
        lua_setter(animator, Available_0_8) auto set_animator(const renderer::animator::lua_reference& animator) -> void;
        lua_setter(continuous, Available_0_8) auto set_continuous_mouse_down_action(bool continuous) -> void;
        lua_setter(ignorePositioningFrameScaler, Available_0_8) auto set_ignore_positioning_frame_scaler(bool f) -> void;

        auto change_internal_entity(const std::shared_ptr<ecs::entity>& entity) -> void;
        lua_function(setSprite, Available_0_8) auto set_sprite(const luabridge::LuaRef& sprite) -> void;

        lua_function(addChildEntity, Available_0_8) auto add_child_entity(const lua_reference& child) -> void;
        lua_function(eachChild, Available_0_8) auto each_child(const luabridge::LuaRef& body) const -> void;

        lua_function(configureAnimation, Available_0_8) auto configure_animation_frames(std::int32_t frame_count) -> void;
        lua_function(nextFrame, Available_0_8) auto next_frame() -> void;
        lua_function(onAnimationStart, Available_0_8) auto on_animation_start(const luabridge::LuaRef& callback) -> void;
        lua_function(onAnimationFinish, Available_0_8) auto on_animation_finish(const luabridge::LuaRef& callback) -> void;

        lua_function(layout, Available_0_8) auto layout() -> void;
        lua_function(onLayout, Available_0_8) auto on_layout(const luabridge::LuaRef& callback) -> void;
        lua_function(draw, Available_0_8) auto draw() -> void;

        lua_function(onMouseEnter, Available_0_8) auto on_mouse_enter(const luabridge::LuaRef& callback) -> void;
        lua_function(onMouseExit, Available_0_8) auto on_mouse_exit(const luabridge::LuaRef& callback) -> void;
        lua_function(onMouseDown, Available_0_8) auto on_mouse_down(const luabridge::LuaRef& callback) -> void;
        lua_function(onMouseRelease, Available_0_8) auto on_mouse_release(const luabridge::LuaRef& callback) -> void;
        lua_function(onMouseDrag, Available_0_8) auto on_mouse_drag(const luabridge::LuaRef& callback) -> void;
        lua_function(sendEvent, Available_0_8) auto send_event(const event& e) -> void;
        lua_function(hitTest, Available_0_8) [[nodiscard]] auto hit_test(const math::point& p) const -> bool;

        auto on_mouse_enter_internal(const std::function<auto(const event&)->void>& callback) -> void;
        auto on_mouse_exit_internal(const std::function<auto(const event&)->void>& callback) -> void;
        auto on_mouse_down_internal(const std::function<auto(const event&)->void>& callback) -> void;
        auto on_mouse_release_internal(const std::function<auto(const event&)->void>& callback) -> void;
        auto on_mouse_drag_internal(const std::function<auto(const event&)->void>& callback) -> void;

        auto replace(const lua_reference& entity) -> void;

        [[nodiscard]] auto internal_entity() const -> std::shared_ptr<ecs::entity>;

        lua_function(setupHitbox, Available_0_8) auto setup_hitbox() -> void;
        lua_getter(hitboxColor, Available_0_8) [[nodiscard]] auto hitbox_color() const -> graphics::color::lua_reference;
        lua_setter(hitboxColor, Available_0_8) auto set_hitbox_color(const graphics::color::lua_reference& color) -> void;

    private:
        std::string m_id;
        std::shared_ptr<ecs::entity> m_entity;
        enum layout::axis_origin m_anchor { layout::axis_origin::center };
        math::point m_position { 0 };
        std::int32_t m_frame_count { 1 };
        std::int32_t m_frame { 0 };
        bool m_next_frame_on_draw { false };
        bool m_loops { false };
        bool m_mouse_over { false };
        bool m_mouse_dragged { false };
        bool m_pressed { false };
        bool m_started { false };
        bool m_finished { false };
        bool m_continuous_mouse_down_action { false };
        bool m_ignore_positioning_frame_scaler { false };
        event m_mouse_down_event;
        lua::vector<lua_reference> m_children;
        luabridge::LuaRef m_on_animation_finish { nullptr };
        luabridge::LuaRef m_on_animation_start { nullptr };
        luabridge::LuaRef m_on_layout { nullptr };
        luabridge::LuaRef m_on_mouse_enter { nullptr };
        luabridge::LuaRef m_on_mouse_exit { nullptr };
        luabridge::LuaRef m_on_mouse_down { nullptr };
        luabridge::LuaRef m_on_mouse_release { nullptr };
        luabridge::LuaRef m_on_mouse_drag { nullptr };
        renderer::animator::lua_reference m_animator { nullptr };
        std::function<auto(const event&)->void> m_on_mouse_enter_internal { nullptr };
        std::function<auto(const event&)->void> m_on_mouse_exit_internal { nullptr };
        std::function<auto(const event&)->void> m_on_mouse_down_internal { nullptr };
        std::function<auto(const event&)->void> m_on_mouse_release_internal { nullptr };
        std::function<auto(const event&)->void> m_on_mouse_drag_internal { nullptr };

        static auto spawn_entity(const luabridge::LuaRef& entity_provider) -> std::shared_ptr<ecs::entity>;
        static auto spawn_entity(const image::static_image::lua_reference& image) -> std::shared_ptr<ecs::entity>;
        static auto spawn_entity(const graphics::canvas::lua_reference& canvas) -> std::shared_ptr<ecs::entity>;

        auto constrain_frame(std::uint32_t frame) -> void;
    };
}

