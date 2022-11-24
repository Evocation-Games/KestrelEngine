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

#include <libKestrel/kestrel.hpp>
#include <libKestrel/ui/layout/positioning_frame.hpp>
#include <libKestrel/graphics/renderer/common/renderer.hpp>
#include <libKestrel/ui/layout/aspect_ratio.hpp>

// MARK: - Construction

kestrel::ui::layout::positioning_frame::positioning_frame(const math::size &target_size, enum axis_origin origin, enum scaling_mode scaling)
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

kestrel::ui::layout::positioning_frame::positioning_frame(const math::size &target_size, std::int32_t origin, std::int32_t scaling)
    : positioning_frame(target_size, static_cast<enum axis_origin>(origin), static_cast<enum scaling_mode>(scaling))
{
}

// MARK: - Accessors

auto kestrel::ui::layout::positioning_frame::set_axis_origin(enum axis_origin origin) -> void
{
    m_axis_origin = origin;
}

auto kestrel::ui::layout::positioning_frame::axis_origin() const -> enum axis_origin
{
    return m_axis_origin;
}

auto kestrel::ui::layout::positioning_frame::set_lua_axis_origin(std::int32_t origin) -> void
{
    set_axis_origin(static_cast<enum axis_origin>(origin));
}

auto kestrel::ui::layout::positioning_frame::lua_axis_origin() const -> std::int32_t
{
    return static_cast<std::int32_t>(axis_origin());
}

auto kestrel::ui::layout::positioning_frame::set_axis_displacement(const math::point &displacement) -> void
{
    m_axis_displacement = displacement;
}

auto kestrel::ui::layout::positioning_frame::axis_displacement() const -> math::point
{
    return m_axis_displacement;
}

auto kestrel::ui::layout::positioning_frame::set_axis_direction(const math::point &direction) -> void
{
    m_axis_direction = direction;
}

auto kestrel::ui::layout::positioning_frame::axis_direction() const -> math::point
{
    return m_axis_direction;
}

auto kestrel::ui::layout::positioning_frame::set_scaling_factor(double factor) -> void
{
    m_scaling_factor = factor;
}

auto kestrel::ui::layout::positioning_frame::scaling_factor() const -> double
{
    return m_scaling_factor;
}

// MARK: - Calculations

auto kestrel::ui::layout::positioning_frame::position_entity_with_offset(const luabridge::LuaRef& entity, const math::point& offset) const -> void
{
    if (lua::ref_isa<scene_entity>(entity)) {
        position_scene_entity_with_offset(entity.cast<scene_entity::lua_reference>(), offset);
    }
    else if (lua::ref_isa<text_entity>(entity)) {
        position_text_entity_with_offset(entity.cast<text_entity::lua_reference>(), offset);
    }
    else if (lua::ref_isa<line_entity>(entity)) {
        position_line_entity_with_offset(entity.cast<line_entity::lua_reference>(), offset);
    }
}

auto kestrel::ui::layout::positioning_frame::position_entity(const luabridge::LuaRef& entity) const -> void
{
    if (lua::ref_isa<scene_entity>(entity)) {
        position_scene_entity_with_offset(entity.cast<scene_entity::lua_reference>(), {});
    }
    else if (lua::ref_isa<text_entity>(entity)) {
        position_text_entity_with_offset(entity.cast<text_entity::lua_reference>(), {});
    }
    else if (lua::ref_isa<line_entity>(entity)) {
        position_line_entity_with_offset(entity.cast<line_entity::lua_reference>(), {});
    }
}

auto kestrel::ui::layout::positioning_frame::lua_entity_position(const luabridge::LuaRef& entity) const -> math::point
{
    if (lua::ref_isa<scene_entity>(entity)) {
        return lua_scene_entity_position(entity.cast<scene_entity::lua_reference>());
    }
    else if (lua::ref_isa<text_entity>(entity)) {
        return lua_text_entity_position(entity.cast<text_entity::lua_reference>());
    }
    return {};
}

auto kestrel::ui::layout::positioning_frame::lua_entity_size(const luabridge::LuaRef& entity) const -> math::size
{
    if (lua::ref_isa<scene_entity>(entity)) {
        return lua_scene_entity_size(entity.cast<scene_entity::lua_reference>());
    }
    else if (lua::ref_isa<text_entity>(entity)) {
        return lua_text_entity_size(entity.cast<text_entity::lua_reference>());
    }
    return {};
}

auto kestrel::ui::layout::positioning_frame::position_scene_entity_with_offset(const ui::scene_entity::lua_reference& entity, const math::point& offset) const -> void
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

auto kestrel::ui::layout::positioning_frame::position_scene_entity(const ui::scene_entity::lua_reference& entity) const -> void
{
    position_scene_entity_with_offset(entity, math::point(0, 0));
}

auto kestrel::ui::layout::positioning_frame::position_text_entity_with_offset(const ui::text_entity::lua_reference& entity, const math::point& offset) const -> void
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

auto kestrel::ui::layout::positioning_frame::position_text_entity(const ui::text_entity::lua_reference& entity) const -> void
{
    position_text_entity_with_offset(entity, math::point(0, 0));
}

auto kestrel::ui::layout::positioning_frame::position_line_entity_with_offset(const ui::line_entity::lua_reference& entity, const math::point& offset) const -> void
{
    // Scale the values of the entity first, before moving on to positioning them in the frame.
    auto start_position = this->line_entity_start_position(*entity.get());
    auto end_position = this->line_entity_end_position(*entity.get());

    // Start constructing the viewport relative position
    math::point adjusted_start_position = m_target.origin + (m_axis_placement * m_scaling_factor) + (m_axis_displacement * m_scaling_factor) + start_position + (offset * m_scaling_factor);
    math::point adjusted_end_position = m_target.origin + (m_axis_placement * m_scaling_factor) + (m_axis_displacement * m_scaling_factor) + end_position + (offset * m_scaling_factor);

    // Apply the drawing dimensions to the entity.
    entity->set_draw_start(adjusted_start_position);
    entity->set_draw_end(adjusted_end_position);
}

auto kestrel::ui::layout::positioning_frame::position_line_entity(const ui::line_entity::lua_reference& entity) const -> void
{
    position_line_entity_with_offset(entity, math::point(0, 0));
}

auto kestrel::ui::layout::positioning_frame::scene_entity_position(const ui::scene_entity &entity) const -> math::point
{
    auto position = entity.position() * m_scaling_factor;
    return position.round();
}

auto kestrel::ui::layout::positioning_frame::scene_entity_size(const ui::scene_entity &entity) const -> math::size
{
    auto entity_size = entity.render_size() * m_scaling_factor;
    return entity_size.round();
}

auto kestrel::ui::layout::positioning_frame::lua_scene_entity_position(const ui::scene_entity::lua_reference &entity) const -> math::point
{
    return scene_entity_position(*entity.get());
}

auto kestrel::ui::layout::positioning_frame::lua_scene_entity_size(const ui::scene_entity::lua_reference &entity) const -> math::size
{
    return scene_entity_size(*entity.get());
}


auto kestrel::ui::layout::positioning_frame::text_entity_position(const ui::text_entity& entity) const -> math::point
{
    auto position = entity.position() * m_scaling_factor;
    return position.round();
}

auto kestrel::ui::layout::positioning_frame::text_entity_size(const ui::text_entity& entity) const -> math::size
{
    auto entity_size = entity.render_size();
    return entity_size.round();
}

auto kestrel::ui::layout::positioning_frame::lua_text_entity_position(const ui::text_entity::lua_reference& entity) const -> math::point
{
    return text_entity_position(*entity.get());
}

auto kestrel::ui::layout::positioning_frame::lua_text_entity_size(const ui::text_entity::lua_reference& entity) const -> math::size
{
    return text_entity_size(*entity.get());
}


auto kestrel::ui::layout::positioning_frame::line_entity_start_position(const ui::line_entity& entity) const -> math::point
{
    auto position = entity.start() * m_scaling_factor;
    return position.round();
}

auto kestrel::ui::layout::positioning_frame::line_entity_end_position(const ui::line_entity& entity) const -> math::point
{
    auto position = entity.end() * m_scaling_factor;
    return position.round();
}

auto kestrel::ui::layout::positioning_frame::lua_line_entity_start_position(const ui::line_entity::lua_reference &entity) const -> math::point
{
    return line_entity_start_position(*entity.get());
}

auto kestrel::ui::layout::positioning_frame::lua_line_entity_end_position(const ui::line_entity::lua_reference &entity) const -> math::point
{
    return line_entity_end_position(*entity.get());
}


auto kestrel::ui::layout::positioning_frame::translate_point_to(const math::point &point) const -> math::point
{
    return point + (m_target.origin + (m_axis_placement * m_scaling_factor) + (m_axis_displacement * m_scaling_factor));
}

auto kestrel::ui::layout::positioning_frame::translate_point_from(const math::point &point) const -> math::point
{
    return point - (m_target.origin + (m_axis_placement * m_scaling_factor) + (m_axis_displacement * m_scaling_factor));
}
