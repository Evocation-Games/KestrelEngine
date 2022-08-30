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

#include "scripting/state.hpp"
#include "util/hint.hpp"
#include "math/point.hpp"
#include "math/size.hpp"
#include "math/rect.hpp"
#include "core/ui/entity/scene_entity.hpp"
#include "core/ui/entity/text_entity.hpp"
#include "core/ui/layout/axis_origin.hpp"
#include "core/ui/layout/scaling_mode.hpp"

namespace ui::layout
{
    struct positioning_frame : public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<ui::layout::positioning_frame> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        lua_api explicit positioning_frame(const math::size& target_size, std::int32_t origin, std::int32_t scaling);
        lua_api explicit positioning_frame(const math::size& target_size, enum axis_origin origin, enum scaling_mode scaling);

        auto set_axis_origin(const enum axis_origin origin) -> void;
        [[nodiscard]] auto axis_origin() const -> enum axis_origin;
        lua_api auto set_lua_axis_origin(std::int32_t origin) -> void;
        [[nodiscard]] lua_api auto lua_axis_origin() const -> std::int32_t;

        lua_api auto set_axis_displacement(const math::point& displacement) -> void;
        [[nodiscard]] lua_api auto axis_displacement() const -> math::point;

        lua_api auto set_axis_direction(const math::point& direction) -> void;
        [[nodiscard]] lua_api auto axis_direction() const -> math::point;

        lua_api auto set_scaling_factor(double factor) -> void;
        [[nodiscard]] lua_api auto scaling_factor() const -> double;

        lua_api auto position_entity(const luabridge::LuaRef& entity) const -> void;
        lua_api auto position_entity_with_offset(const luabridge::LuaRef& entity, const math::point& offset) const -> void;

        [[nodiscard]] lua_api auto lua_entity_position(const luabridge::LuaRef& entity) const -> math::point;
        [[nodiscard]] lua_api auto lua_entity_size(const luabridge::LuaRef& entity) const -> math::size;

        auto position_scene_entity(const ui::scene_entity::lua_reference& entity) const -> void;
        auto position_scene_entity_with_offset(const ui::scene_entity::lua_reference& entity, const math::point& offset) const -> void;

        auto position_text_entity(const ui::text_entity::lua_reference& entity) const -> void;
        auto position_text_entity_with_offset(const ui::text_entity::lua_reference& entity, const math::point& offset) const -> void;

        [[nodiscard]] auto scene_entity_position(const ui::scene_entity& entity) const -> math::point;
        [[nodiscard]] auto scene_entity_size(const ui::scene_entity& entity) const -> math::size;
        [[nodiscard]] auto lua_scene_entity_position(const ui::scene_entity::lua_reference& entity) const -> math::point;
        [[nodiscard]] auto lua_scene_entity_size(const ui::scene_entity::lua_reference& entity) const -> math::size;

        [[nodiscard]] auto text_entity_position(const ui::text_entity& entity) const -> math::point;
        [[nodiscard]] auto text_entity_size(const ui::text_entity& entity) const -> math::size;
        [[nodiscard]] auto lua_text_entity_position(const ui::text_entity::lua_reference& entity) const -> math::point;
        [[nodiscard]] auto lua_text_entity_size(const ui::text_entity::lua_reference& entity) const -> math::size;

        [[nodiscard]] lua_api auto translate_point_to(const math::point& point) const -> math::point;
        [[nodiscard]] lua_api auto translate_point_from(const math::point& point) const -> math::point;

    private:
        // The viewport size reflects the _real_ size of the viewport in pixels.
        math::size m_viewport { 1920, 1080 };

        // The target represents the bounds of the scene, to be centrally positioned in the viewport.
        math::rect m_target { 448, 156, 1024, 768 };

        // Scaling
        enum scaling_mode m_scaling_mode { scaling_mode::normal };
        double m_scaling_factor { 1.0 };

        // Axis configuration
        enum axis_origin m_axis_origin { axis_origin::center };
        math::point m_axis_placement { 960, 540 };
        math::point m_axis_direction { 1, 1 };
        math::point m_axis_displacement { 0, 0 };

        // Entity Anchor
        math::size m_default_anchor { 0.5, 0.5 };
    };
}