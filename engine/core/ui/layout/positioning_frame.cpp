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

auto ui::layout::positioning_frame::position_entity(const ui::scene_entity::lua_reference& entity) const -> void
{
    // Scale the values of the entity first, before moving on to positioning them in the frame.
    auto entity_size = this->entity_size(*entity.get());
    auto entity_position = this->entity_position(*entity.get());

    // Start constructing the viewport relative position
    math::point position = m_target.origin + ((m_axis_origin + m_axis_displacement) * m_scaling_factor) + entity_position;

    // Apply the drawing dimensions to the entity.
    entity->set_draw_position(position);
    entity->set_draw_size(entity_size);
}

auto ui::layout::positioning_frame::entity_position(const ui::scene_entity &entity) const -> math::point
{
    auto entity_position = entity.position() * m_scaling_factor;
    auto anchor_adjustment = entity_size(entity) * m_default_anchor;
    return math::point(entity_position.x - anchor_adjustment.width, entity_position.y - anchor_adjustment.height).round();
}

auto ui::layout::positioning_frame::entity_size(const ui::scene_entity &entity) const -> math::size
{
    auto entity_size = entity.render_size() * m_scaling_factor;
    return entity_size.round();
}

auto ui::layout::positioning_frame::lua_entity_position(const ui::scene_entity::lua_reference &entity) const -> math::point
{
    return entity_position(*entity.get());
}

auto ui::layout::positioning_frame::lua_entity_size(const ui::scene_entity::lua_reference &entity) const -> math::size
{
    return entity_size(*entity.get());
}

auto ui::layout::positioning_frame::translate_point_to(const math::point &point) const -> math::point
{
    return point + (m_target.origin + ((m_axis_origin + m_axis_displacement) * m_scaling_factor));
}

auto ui::layout::positioning_frame::translate_point_from(const math::point &point) const -> math::point
{
    return point - (m_target.origin + ((m_axis_origin + m_axis_displacement) * m_scaling_factor));
}

//
//// MARK: - Helpers
//
//auto ui::layout::positioning_frame::aspect_fill_size(const math::size &size) -> math::size
//{
//    // Fetch the viewport size, as this is what we'll be fitting to.
//    auto viewport_size = renderer::window_size();
//    math::size result(0);
//
//    if (size.width < viewport_size.width && size.height < viewport_size.height) {
//        return size;
//    }
//
//    if (viewport_size.width >= viewport_size.height) {
//        if (size.width >= size.height) {
//            // Landscape to Landscape
//            auto scale_factor = viewport_size.width / size.width;
//            result.width = viewport_size.width;
//            result.height = size.height * scale_factor;
//        }
//        else {
//            // Portrait to Landscape
//            auto scale_factor = viewport_size.height / size.height;
//            result.height = viewport_size.height;
//            result.width = size.width * scale_factor;
//        }
//    }
//    else {
//        // TODO: Portrait viewports are not supported currently.
//        throw std::runtime_error("Portrait viewports are not currently supported");
//    }
//
//    return result;
//}
//
//auto ui::layout::positioning_frame::ideal_target_size() -> math::size
//{
//    auto scale = 1.0;
//    if (auto env = environment::active_environment().lock()) {
//        scale = env->scale();
//    }
//    return renderer::window_size() * scale;
//}
//
//// MARK: - Construction
//
//ui::layout::positioning_frame::positioning_frame(const math::size &target_size, const math::point &origin, const math::point &anchor)
//{
//    m_viewport_size = renderer::window_size();
//    m_target_size = target_size;
//    m_effective_size = aspect_fill_size(target_size);
//
//    m_target_origin = origin;
//    m_anchor = anchor;
//
//    auto origin_x_ratio = m_target_origin.x / m_target_size.width;
//    auto origin_y_ratio = m_target_origin.y / m_target_size.height;
//
//    m_effective_origin.x = m_effective_size.width * origin_x_ratio;
//    m_effective_origin.y = m_effective_size.height * origin_y_ratio;
//    m_effective_origin = m_effective_origin.round();
//
//    m_viewport_origin.x = m_viewport_size.width * origin_x_ratio;
//    m_viewport_origin.y = m_viewport_size.height * origin_y_ratio;
//    m_viewport_origin = m_viewport_origin.round();
//
//    m_aspect_ratio = m_viewport_size.width / m_viewport_size.height;
//
////    m_viewport_offset.x = (m_effective_size.width - m_viewport_size.width) / 2;
////    m_viewport_offset.y = (m_effective_size.height - m_viewport_size.height) / 2;
////    m_viewport_offset = m_viewport_offset.round();
//
//    m_viewport_scale.x = m_effective_size.width / m_target_size.width;
//    m_viewport_scale.y = m_effective_size.height / m_target_size.height;
//}
//
//// MARK: - Methods
//
//auto ui::layout::positioning_frame::position_for_entity(const ui::scene_entity& entity) const -> math::point
//{
//    math::size entity_size = size_for_entity(entity).round();
//    math::point entity_origin_offset = math::point(entity_size.width * m_anchor.x, entity_size.height * m_anchor.y).round() * -1;
//    math::point entity_origin = ((entity.position() * m_viewport_scale) + entity_origin_offset).round();
//
//    // If the target size is different to the effective size, then scale the entity origin accordingly.
//    auto viewport_origin = (m_viewport_origin + m_viewport_offset);
//    auto position = (viewport_origin + entity_origin);
//    return position;
//}
//
//auto ui::layout::positioning_frame::size_for_entity(const scene_entity &entity) const -> math::size
//{
//    if (entity.ignore_positioning_frame_scaler()) {
//        return entity.render_size();
//    }
//    return entity.render_size() * math::size(m_viewport_scale.x, m_viewport_scale.y);
//}
//
//auto ui::layout::positioning_frame::lua_position_for_base_entity(const ui::scene_entity::lua_reference& entity) const -> math::point
//{
//    return position_for_entity(*entity.get());
//}
//
//auto ui::layout::positioning_frame::translate_point(math::point p) const -> math::point
//{
//    return (m_viewport_origin + m_viewport_offset) + p;
//}
//
//auto ui::layout::positioning_frame::convert_point(math::point p) const -> math::point
//{
//    return p - (m_viewport_origin + m_viewport_offset);
//}