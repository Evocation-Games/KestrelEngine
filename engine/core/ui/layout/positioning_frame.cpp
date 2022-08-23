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

// MARK: - Constants

// MARK: - Lua

auto ui::layout::positioning_frame::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void
{
    lua->global_namespace()
        .beginClass<positioning_frame>("PositioningFrame")
            .addConstructor<auto(*)(const math::size&, bool)->void>()
            .addProperty("axisOrigin", &positioning_frame::axis_origin, &positioning_frame::set_axis_origin)
            .addProperty("axisDisplacement", &positioning_frame::axis_displacement, &positioning_frame::set_axis_displacement)
            .addProperty("axisDirection", &positioning_frame::axis_direction, &positioning_frame::set_axis_direction)
            .addProperty("scalingFactor", &positioning_frame::scaling_factor, &positioning_frame::set_scaling_factor)
            .addProperty("defaultAnchor", &positioning_frame::default_anchor, &positioning_frame::set_default_anchor)
            .addFunction("translatePoint", &positioning_frame::translate_point_to)
            .addFunction("convertPoint", &positioning_frame::translate_point_from)
            .addFunction("positionEntity", &positioning_frame::position_entity)
            .addFunction("positionEntityWithOffset", &positioning_frame::position_entity_with_offset)
            .addFunction("positionForEntity", &positioning_frame::lua_entity_position)
            .addFunction("sizeForEntity", &positioning_frame::lua_entity_size)
        .endClass();
}

// MARK: - Construction

ui::layout::positioning_frame::positioning_frame(const math::size &target_size, bool constrain_to_viewport)
{
    m_viewport = renderer::window_size();

    // If we need to constrain the target size to the viewport, then do so now.
    auto corrected_target_size = target_size;
    if (constrain_to_viewport && (corrected_target_size.width >= m_viewport.width || corrected_target_size.height >= m_viewport.height)) {
        if (m_viewport.width >= m_viewport.height) {
            if (corrected_target_size.width >= corrected_target_size.height) {
                m_scaling_factor = m_viewport.width / corrected_target_size.width;
                corrected_target_size.width = m_viewport.width;
                corrected_target_size.height *=  m_scaling_factor;
            }
            else {
                m_scaling_factor = m_viewport.height / corrected_target_size.height;
                corrected_target_size.height = m_viewport.height;
                corrected_target_size.width *= m_scaling_factor;
            }
        }
    }

    // Center the target size into the viewport size,
    m_target = {
        { (m_viewport.width - corrected_target_size.width) / 2.0, (m_viewport.height - corrected_target_size.height) / 2.0 },
        corrected_target_size
    };

    // Calculate supplemental support values.
    m_axis_origin = math::point(m_target.size.width / 2.0, m_target.size.height / 2.0);
}

// MARK: - Accessors

auto ui::layout::positioning_frame::set_axis_origin(const math::point &origin) -> void
{
    m_axis_origin = origin;
}

auto ui::layout::positioning_frame::axis_origin() const -> math::point
{
    return m_axis_origin;
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

auto ui::layout::positioning_frame::set_default_anchor(const math::size &anchor) -> void
{
    m_default_anchor = anchor;
}

auto ui::layout::positioning_frame::default_anchor() const -> math::size
{
    return m_default_anchor;
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
}

auto ui::layout::positioning_frame::lua_entity_size(const luabridge::LuaRef& entity) const -> math::size
{
    if (scripting::lua::ref_isa<scene_entity>(entity)) {
        return lua_scene_entity_size(entity.cast<scene_entity::lua_reference>());
    }
    else if (scripting::lua::ref_isa<text_entity>(entity)) {
        return lua_text_entity_size(entity.cast<text_entity::lua_reference>());
    }
}

auto ui::layout::positioning_frame::position_scene_entity_with_offset(const ui::scene_entity::lua_reference& entity, const math::point& offset) const -> void
{
    // Scale the values of the entity first, before moving on to positioning them in the frame.
    auto entity_size = this->scene_entity_size(*entity.get());
    auto entity_position = this->scene_entity_position(*entity.get());

    // Start constructing the viewport relative position
    math::point position = m_target.origin + ((m_axis_origin + m_axis_displacement) * m_scaling_factor) + entity_position + (offset * m_scaling_factor);

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
    math::point position = m_target.origin + ((m_axis_origin + m_axis_displacement) * m_scaling_factor) + entity_position + (offset * m_scaling_factor);

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
    auto entity_position = entity.position() * m_scaling_factor;
    auto anchor_adjustment = scene_entity_size(entity) * m_default_anchor;
    return math::point(entity_position.x - anchor_adjustment.width, entity_position.y - anchor_adjustment.height).round();
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
    auto entity_position = entity.position() * m_scaling_factor;
    auto anchor_adjustment = text_entity_size(entity) * m_default_anchor;
    return math::point(entity_position.x - anchor_adjustment.width, entity_position.y - anchor_adjustment.height).round();
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
    return point + (m_target.origin + ((m_axis_origin + m_axis_displacement) * m_scaling_factor));
}

auto ui::layout::positioning_frame::translate_point_from(const math::point &point) const -> math::point
{
    return point - (m_target.origin + ((m_axis_origin + m_axis_displacement) * m_scaling_factor));
}
