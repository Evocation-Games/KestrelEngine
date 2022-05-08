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
#include "util/hint.hpp"
#include "math/size.hpp"
#include "math/point.hpp"
#include "util/lua_vector.hpp"
#include "scripting/state.hpp"
#include "core/event/event.hpp"
#include "core/graphics/common/entity.hpp"
#include "core/graphics/common/canvas.hpp"
#include "core/asset/static_image.hpp"

namespace ui
{
    struct scene_entity: public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<ui::scene_entity> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        explicit scene_entity(const std::shared_ptr<graphics::entity>& entity);
        explicit scene_entity(const luabridge::LuaRef& entity_provider);
        explicit scene_entity(const asset::static_image::lua_reference& image);
        explicit scene_entity(const graphics::canvas::lua_reference& canvas);

        [[nodiscard]] lua_api auto position() const -> math::point;
        [[nodiscard]] lua_api auto draw_position() const -> math::point;
        [[nodiscard]] lua_api auto centered() const -> bool;
        [[nodiscard]] lua_api auto size() const -> math::size;
        [[nodiscard]] lua_api auto half_size() const -> math::size;
        [[nodiscard]] lua_api auto render_size() const -> math::size;
        [[nodiscard]] lua_api auto frame_count() const -> int32_t;
        [[nodiscard]] lua_api auto current_frame() const -> int32_t;
        [[nodiscard]] lua_api auto advances_to_next_frame_on_draw() const -> bool;
        [[nodiscard]] lua_api auto animation_loops() const -> bool;
        [[nodiscard]] lua_api auto alpha() const -> double;
        [[nodiscard]] lua_api auto blend_mode() const -> int;
        [[nodiscard]] lua_api auto clipping_area() const -> math::size;
        [[nodiscard]] lua_api auto clipping_offset() const -> math::point;
        [[nodiscard]] lua_api auto children() const -> util::lua_vector<lua_reference>;

        lua_api auto set_position(const math::point& v) -> void;
        lua_api auto set_draw_position(const math::point& v) -> void;
        lua_api auto set_centered(bool v) -> void;
        lua_api auto set_size(const math::size& v) -> void;
        lua_api auto set_render_size(const math::size& v) -> void;
        lua_api auto set_current_frame(int32_t v) -> void;
        lua_api auto set_advances_to_next_frame_on_draw(bool v) -> void;
        lua_api auto set_animation_loops(bool v) -> void;
        lua_api auto set_alpha(double v) -> void;
        lua_api auto set_blend_mode(int v) -> void;
        lua_api auto set_clipping_area(const math::size& v) -> void;
        lua_api auto set_clipping_offset(const math::point& v) -> void;

        lua_api auto set_sprite(const luabridge::LuaRef& sprite) -> void;

        lua_api auto add_child_entity(const lua_reference& child) -> void;
        lua_api auto each_child(const luabridge::LuaRef& body) const -> void;

        lua_api auto configure_animation_frames(int32_t frame_count) -> void;
        lua_api auto next_frame() -> void;
        lua_api auto on_animation_start(const luabridge::LuaRef& callback) -> void;
        lua_api auto on_animation_finish(const luabridge::LuaRef& callback) -> void;

        lua_api auto layout() -> void;
        lua_api auto on_layout(const luabridge::LuaRef& callback) -> void;
        lua_api auto draw() -> void;

        lua_api auto on_mouse_enter(const luabridge::LuaRef& callback) -> void;
        lua_api auto on_mouse_exit(const luabridge::LuaRef& callback) -> void;
        lua_api auto on_mouse_down(const luabridge::LuaRef& callback) -> void;
        lua_api auto on_mouse_release(const luabridge::LuaRef& callback) -> void;
        lua_api auto on_mouse_drag(const luabridge::LuaRef& callback) -> void;
        lua_api auto send_event(const event& e) -> void;
        [[nodiscard]] lua_api auto hit_test(const math::point& p) const -> bool;

        [[nodiscard]] auto internal_entity() const -> std::shared_ptr<graphics::entity>;

    private:
        std::shared_ptr<graphics::entity> m_entity;
        bool m_centered { false };
        math::point m_position { 0 };
        int32_t m_frame_count { 1 };
        int32_t m_frame { 0 };
        bool m_next_frame_on_draw { false };
        bool m_loops { false };
        bool m_mouse_over { false };
        bool m_mouse_dragged { false };
        bool m_pressed { false };
        bool m_started { false };
        bool m_finished { false };
        util::lua_vector<lua_reference> m_children;
        luabridge::LuaRef m_on_animation_finish { nullptr };
        luabridge::LuaRef m_on_animation_start { nullptr };
        luabridge::LuaRef m_on_layout { nullptr };
        luabridge::LuaRef m_on_mouse_enter { nullptr };
        luabridge::LuaRef m_on_mouse_exit { nullptr };
        luabridge::LuaRef m_on_mouse_down { nullptr };
        luabridge::LuaRef m_on_mouse_release { nullptr };
        luabridge::LuaRef m_on_mouse_drag { nullptr };

        static auto spawn_entity(const luabridge::LuaRef& entity_provider) -> std::shared_ptr<graphics::entity>;
        static auto spawn_entity(const asset::static_image::lua_reference& image) -> std::shared_ptr<graphics::entity>;
        static auto spawn_entity(const graphics::canvas::lua_reference& canvas) -> std::shared_ptr<graphics::entity>;

    };
}

