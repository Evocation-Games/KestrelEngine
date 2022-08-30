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

#include "core/ui/layout/positioning_frame.hpp"
#include "renderer/common/renderer.hpp"
#include "core/environment.hpp"
#include "core/ui/layout/aspect_ratio.hpp"

// MARK: - Constants

static auto s_scaling_mode_none = static_cast<std::int32_t>(ui::layout::scaling_mode::normal);
static auto s_scaling_aspect_fill = static_cast<std::int32_t>(ui::layout::scaling_mode::aspect_fill);
static auto s_scaling_aspect_fit = static_cast<std::int32_t>(ui::layout::scaling_mode::aspect_fit);
static auto s_scaling_scale_fill = static_cast<std::int32_t>(ui::layout::scaling_mode::scale_fill);
static auto s_scaling_scale_fit = static_cast<std::int32_t>(ui::layout::scaling_mode::scale_fit);

static auto s_axis_top_left = static_cast<std::int32_t>(ui::layout::axis_origin::top_left);
static auto s_axis_center_left = static_cast<std::int32_t>(ui::layout::axis_origin::center_left);
static auto s_axis_bottom_left = static_cast<std::int32_t>(ui::layout::axis_origin::bottom_left);
static auto s_axis_top_center = static_cast<std::int32_t>(ui::layout::axis_origin::top_center);
static auto s_axis_center = static_cast<std::int32_t>(ui::layout::axis_origin::center);
static auto s_axis_bottom_center = static_cast<std::int32_t>(ui::layout::axis_origin::bottom_center);
static auto s_axis_top_right = static_cast<std::int32_t>(ui::layout::axis_origin::top_right);
static auto s_axis_center_right = static_cast<std::int32_t>(ui::layout::axis_origin::center_right);
static auto s_axis_bottom_right = static_cast<std::int32_t>(ui::layout::axis_origin::bottom_right);

// MARK: - Lua

auto ui::layout::positioning_frame::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void
{
    lua->global_namespace()
        .beginNamespace("ScalingMode")
            .addProperty("Normal", &s_scaling_mode_none, false)
            .addProperty("AspectFill", &s_scaling_aspect_fill, false)
            .addProperty("AspectFit", &s_scaling_aspect_fit, false)
            .addProperty("ScaleFill", &s_scaling_scale_fill, false)
            .addProperty("ScaleFit", &s_scaling_scale_fit, false)
        .endNamespace()
        .beginNamespace("AxisOrigin")
            .addProperty("TopLeft", &s_axis_top_left, false)
            .addProperty("CenterLeft", &s_axis_center_left, false)
            .addProperty("BottomLeft", &s_axis_bottom_left, false)
            .addProperty("TopCenter", &s_axis_top_center, false)
            .addProperty("Center", &s_axis_center, false)
            .addProperty("BottomCenter", &s_axis_bottom_center, false)
            .addProperty("TopRight", &s_axis_top_right, false)
            .addProperty("CenterRight", &s_axis_center_right, false)
            .addProperty("BottomRight", &s_axis_bottom_right, false)
        .endNamespace()
        .beginClass<positioning_frame>("PositioningFrame")
            .addConstructor<auto(*)(const math::size&, std::int32_t, std::int32_t)->void, lua_reference>()
            .addProperty("axisOrigin", &positioning_frame::axis_origin, &positioning_frame::set_axis_origin)
            .addProperty("axisDisplacement", &positioning_frame::axis_displacement, &positioning_frame::set_axis_displacement)
            .addProperty("axisDirection", &positioning_frame::axis_direction, &positioning_frame::set_axis_direction)
            .addProperty("scalingFactor", &positioning_frame::scaling_factor, &positioning_frame::set_scaling_factor)
            .addFunction("translatePoint", &positioning_frame::translate_point_to)
            .addFunction("convertPoint", &positioning_frame::translate_point_from)
            .addFunction("positionEntity", &positioning_frame::position_entity)
            .addFunction("positionEntityWithOffset", &positioning_frame::position_entity_with_offset)
            .addFunction("positionForEntity", &positioning_frame::lua_entity_position)
            .addFunction("sizeForEntity", &positioning_frame::lua_entity_size)
        .endClass();
}

// MARK: - Construction

ui::layout::positioning_frame::positioning_frame(const math::size &target_size, enum axis_origin origin, enum scaling_mode scaling)
{
    m_viewport = renderer::window_size();
    m_axis_origin = origin;
    m_scaling_mode = scaling;

    auto ratio = aspect_ratio(target_size);
    auto corrected_target_size = calculate_size(scaling, m_viewport, target_size, ratio);
    auto target_position = position_for_axis(m_viewport, origin, corrected_target_size);

    // Center the target size into the viewport size,
    m_target = { target_position, corrected_target_size };

    // Calculate supplemental support values.
    m_axis_placement = origin_for_axis(m_target.size, origin);
}

ui::layout::positioning_frame::positioning_frame(const math::size &target_size, std::int32_t origin, std::int32_t scaling)
    : positioning_frame(target_size, static_cast<enum axis_origin>(origin), static_cast<enum scaling_mode>(scaling))
{
}

// MARK: - Accessors

auto ui::layout::positioning_frame::set_axis_origin(const enum axis_origin origin) -> void
{
    m_axis_origin = origin;
}

auto ui::layout::positioning_frame::axis_origin() const -> enum axis_origin
{
    return m_axis_origin;
}

auto ui::layout::positioning_frame::set_lua_axis_origin(std::int32_t origin) -> void
{
    set_axis_origin(static_cast<enum axis_origin>(origin));
}

auto ui::layout::positioning_frame::lua_axis_origin() const -> std::int32_t
{
    return static_cast<std::int32_t>(axis_origin());
}

auto ui::layout::positioning_frame::set_axis_displacement(const math::point &displacement) -> void
{
    m_axis_displacement = displacement;
}

auto ui::layout::positioning_frame::axis_displacement() const -> math::point
{
    return m_axis_displacement;
}

auto ui::layout::positioning_frame::set_axis_direction(const math::point &direction) -> void
{
    m_axis_direction = direction;
}

auto ui::layout::positioning_frame::axis_direction() const -> math::point
{
    return m_axis_direction;
}

auto ui::layout::positioning_frame::set_scaling_factor(double factor) -> void
{
    m_scaling_factor = factor;
}

auto ui::layout::positioning_frame::scaling_factor() const -> double
{
    return m_scaling_factor;
}

// MARK: - Calculations

auto ui::layout::positioning_frame::position_entity_with_offset(const luabridge::LuaRef& entity, const math::point& offset) const -> void
{
    if (scripting::lua::ref_isa<scene_entity>(entity)) {
        position_scene_entity_with_offset(entity.cast<scene_entity::lua_reference>(), offset);
    }
    else if (scripting::lua::ref_isa<text_entity>(entity)) {
        position_text_entity_with_offset(entity.cast<text_entity::lua_reference>(), offset);
    }
}

auto ui::layout::positioning_frame::position_entity(const luabridge::LuaRef& entity) const -> void
{
    if (scripting::lua::ref_isa<scene_entity>(entity)) {
        position_scene_entity_with_offset(entity.cast<scene_entity::lua_reference>(), {});
    }
    else if (scripting::lua::ref_isa<text_entity>(entity)) {
        position_text_entity_with_offset(entity.cast<text_entity::lua_reference>(), {});
    }
}

auto ui::layout::positioning_frame::lua_entity_position(const luabridge::LuaRef& entity) const -> math::point
{
    if (scripting::lua::ref_isa<scene_entity>(entity)) {
        return lua_scene_entity_position(entity.cast<scene_entity::lua_reference>());
    }
    else if (scripting::lua::ref_isa<text_entity>(entity)) {
        return lua_text_entity_position(entity.cast<text_entity::lua_reference>());
    }
    return {};
}

auto ui::layout::positioning_frame::lua_entity_size(const luabridge::LuaRef& entity) const -> math::size
{
    if (scripting::lua::ref_isa<scene_entity>(entity)) {
        return lua_scene_entity_size(entity.cast<scene_entity::lua_reference>());
    }
    else if (scripting::lua::ref_isa<text_entity>(entity)) {
        return lua_text_entity_size(entity.cast<text_entity::lua_reference>());
    }
    return {};
}

auto ui::layout::positioning_frame::position_scene_entity_with_offset(const ui::scene_entity::lua_reference& entity, const math::point& offset) const -> void
{
    // Scale the values of the entity first, before moving on to positioning them in the frame.
    auto entity_size = this->scene_entity_size(*entity.get());
    auto entity_position = this->scene_entity_position(*entity.get());

    // Start constructing the viewport relative position
    math::point position = m_target.origin + (m_axis_placement * m_scaling_factor) + (m_axis_displacement * m_scaling_factor) + entity_position + (offset * m_scaling_factor);

    // Apply the drawing dimensions to the entity.
    entity->set_draw_position(position);
    entity->set_draw_size(entity_size);
}

auto ui::layout::positioning_frame::position_scene_entity(const ui::scene_entity::lua_reference& entity) const -> void
{
    position_scene_entity_with_offset(entity, math::point(0, 0));
}

auto ui::layout::positioning_frame::position_text_entity_with_offset(const ui::text_entity::lua_reference& entity, const math::point& offset) const -> void
{
    // Scale the values of the entity first, before moving on to positioning them in the frame.
    auto entity_size = this->text_entity_size(*entity.get());
    auto entity_position = this->text_entity_position(*entity.get());

    // Start constructing the viewport relative position
    math::point position = m_target.origin + (m_axis_placement * m_scaling_factor) + (m_axis_displacement * m_scaling_factor) + entity_position + (offset * m_scaling_factor);

    // Apply the drawing dimensions to the entity.
    entity->set_draw_position(position);
    entity->set_draw_size(entity_size);
}

auto ui::layout::positioning_frame::position_text_entity(const ui::text_entity::lua_reference& entity) const -> void
{
    position_text_entity_with_offset(entity, math::point(0, 0));
}

auto ui::layout::positioning_frame::scene_entity_position(const ui::scene_entity &entity) const -> math::point
{
    auto position = entity.position() * m_scaling_factor;
    return position.round();
}

auto ui::layout::positioning_frame::scene_entity_size(const ui::scene_entity &entity) const -> math::size
{
    auto entity_size = entity.render_size() * m_scaling_factor;
    return entity_size.round();
}

auto ui::layout::positioning_frame::lua_scene_entity_position(const ui::scene_entity::lua_reference &entity) const -> math::point
{
    return scene_entity_position(*entity.get());
}

auto ui::layout::positioning_frame::lua_scene_entity_size(const ui::scene_entity::lua_reference &entity) const -> math::size
{
    return scene_entity_size(*entity.get());
}


auto ui::layout::positioning_frame::text_entity_position(const ui::text_entity& entity) const -> math::point
{
    auto position = entity.position() * m_scaling_factor;
    return position.round();
}

auto ui::layout::positioning_frame::text_entity_size(const ui::text_entity& entity) const -> math::size
{
    auto entity_size = entity.render_size();
    return entity_size.round();
}

auto ui::layout::positioning_frame::lua_text_entity_position(const ui::text_entity::lua_reference& entity) const -> math::point
{
    return text_entity_position(*entity.get());
}

auto ui::layout::positioning_frame::lua_text_entity_size(const ui::text_entity::lua_reference& entity) const -> math::size
{
    return text_entity_size(*entity.get());
}

auto ui::layout::positioning_frame::translate_point_to(const math::point &point) const -> math::point
{
    return point + (m_target.origin + (m_axis_placement * m_scaling_factor) + (m_axis_displacement * m_scaling_factor));
}

auto ui::layout::positioning_frame::translate_point_from(const math::point &point) const -> math::point
{
    return point - (m_target.origin + (m_axis_placement * m_scaling_factor) + (m_axis_displacement * m_scaling_factor));
}
