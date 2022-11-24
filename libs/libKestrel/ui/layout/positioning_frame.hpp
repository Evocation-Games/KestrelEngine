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

#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/math/point.hpp>
#include <libKestrel/math/size.hpp>
#include <libKestrel/math/rect.hpp>
#include <libKestrel/ui/entity/scene_entity.hpp>
#include <libKestrel/ui/entity/text_entity.hpp>
#include <libKestrel/ui/entity/line_entity.hpp>
#include <libKestrel/ui/layout/axis_origin.hpp>
#include <libKestrel/ui/layout/scaling_mode.hpp>

namespace kestrel::ui::layout
{
    struct lua_api(PositioningFrame, Available_0_8) positioning_frame
    {
    public:
        has_constructable_lua_api(positioning_frame);

        explicit positioning_frame(const math::size& target_size, enum axis_origin origin, enum scaling_mode scaling);
        lua_constructor(Available_0_8) explicit positioning_frame(const math::size& target_size, std::int32_t origin, std::int32_t scaling);

        auto set_axis_origin(enum axis_origin origin) -> void;
        [[nodiscard]] auto axis_origin() const -> enum axis_origin;
        lua_setter(axisOrigin, Available_0_8) auto set_lua_axis_origin(std::int32_t origin) -> void;
        lua_getter(axisOrigin, Available_0_8) [[nodiscard]] auto lua_axis_origin() const -> std::int32_t;

        lua_setter(axisDisplacement, Available_0_8) auto set_axis_displacement(const math::point& displacement) -> void;
        lua_getter(axisDisplacement, Available_0_8) [[nodiscard]] auto axis_displacement() const -> math::point;

        lua_setter(axisDirection, Available_0_8) auto set_axis_direction(const math::point& direction) -> void;
        lua_getter(axisDirection, Available_0_8) [[nodiscard]] auto axis_direction() const -> math::point;

        lua_setter(scalingFactor, Available_0_8) auto set_scaling_factor(double factor) -> void;
        lua_getter(scalingFactor, Available_0_8) [[nodiscard]] auto scaling_factor() const -> double;

        lua_function(positionEntity, Available_0_8) auto position_entity(const luabridge::LuaRef& entity) const -> void;
        lua_function(positionEntityWithOffset, Available_0_8) auto position_entity_with_offset(const luabridge::LuaRef& entity, const math::point& offset) const -> void;

        lua_function(positionForEntity, Available_0_8) [[nodiscard]] auto lua_entity_position(const luabridge::LuaRef& entity) const -> math::point;
        lua_function(sizeForEntity, Available_0_8) [[nodiscard]] auto lua_entity_size(const luabridge::LuaRef& entity) const -> math::size;

        auto position_scene_entity(const ui::scene_entity::lua_reference& entity) const -> void;
        auto position_scene_entity_with_offset(const ui::scene_entity::lua_reference& entity, const math::point& offset) const -> void;

        auto position_text_entity(const ui::text_entity::lua_reference& entity) const -> void;
        auto position_text_entity_with_offset(const ui::text_entity::lua_reference& entity, const math::point& offset) const -> void;

        auto position_line_entity(const ui::line_entity::lua_reference& entity) const -> void;
        auto position_line_entity_with_offset(const ui::line_entity::lua_reference& entity, const math::point& offset) const -> void;

        [[nodiscard]] auto scene_entity_position(const ui::scene_entity& entity) const -> math::point;
        [[nodiscard]] auto scene_entity_size(const ui::scene_entity& entity) const -> math::size;
        lua_function(positionForEntity, Available_0_8) [[nodiscard]] auto lua_scene_entity_position(const ui::scene_entity::lua_reference& entity) const -> math::point;
        lua_function(sizeForEntity, Available_0_8) [[nodiscard]] auto lua_scene_entity_size(const ui::scene_entity::lua_reference& entity) const -> math::size;

        [[nodiscard]] auto text_entity_position(const ui::text_entity& entity) const -> math::point;
        [[nodiscard]] auto text_entity_size(const ui::text_entity& entity) const -> math::size;
        lua_function(positionForTextEntity, Available_0_8) [[nodiscard]] auto lua_text_entity_position(const ui::text_entity::lua_reference& entity) const -> math::point;
        lua_function(sizeForTextEntity, Available_0_8) [[nodiscard]] auto lua_text_entity_size(const ui::text_entity::lua_reference& entity) const -> math::size;

        [[nodiscard]] auto line_entity_start_position(const ui::line_entity& entity) const -> math::point;
        [[nodiscard]] auto line_entity_end_position(const ui::line_entity& entity) const -> math::point;
        lua_function(startPositionForLineEntity, Available_0_8) [[nodiscard]] auto lua_line_entity_start_position(const ui::line_entity::lua_reference& entity) const -> math::point;
        lua_function(endPositionForLineEntity, Available_0_8) [[nodiscard]] auto lua_line_entity_end_position(const ui::line_entity::lua_reference& entity) const -> math::point;

        lua_function(translatePointTo, Available_0_8) [[nodiscard]] auto translate_point_to(const math::point& point) const -> math::point;
        lua_function(translatePointFrom, Available_0_8) [[nodiscard]] auto translate_point_from(const math::point& point) const -> math::point;

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