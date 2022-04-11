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

// MARK: - Lua

auto ui::layout::positioning_frame::enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void
{
    lua->global_namespace()
        .beginClass<positioning_frame>("PositioningFrame")
            .addConstructor<auto(*)(math::rect, math::point, math::point)->void, positioning_frame::lua_reference>()
            .addProperty("frame", &positioning_frame::m_frame, false)
            .addProperty("anchorPoint", &positioning_frame::m_anchor_point, false)
            .addFunction("positionForEntity", &positioning_frame::lua_position_for_base_entity)
            .addFunction("translatePoint", &positioning_frame::translate_point)
            .addFunction("convertPoint", &positioning_frame::convert_point)
        .endClass();
}

// MARK: - Construction

ui::layout::positioning_frame::positioning_frame(math::rect frame, math::point anchor, math::point offset)
    : m_frame(frame), m_anchor_point(anchor)
{
    math::point base_origin { frame.size.width * anchor.x, frame.size.height * anchor.y };
    base_origin = base_origin + frame.origin;
    base_origin = base_origin + offset;
    m_base_origin = base_origin;
}

// MARK: - Methods

auto ui::layout::positioning_frame::position_for_entity(const ui::scene_entity& entity) const -> math::point
{
    math::size entity_size = entity.size();
    math::point entity_origin_offset = math::point(entity_size.width * m_anchor_point.x, entity_size.height * m_anchor_point.y) * -1;
    math::point entity_origin = entity.position() + entity_origin_offset;
    return m_base_origin + entity_origin;
}

auto ui::layout::positioning_frame::lua_position_for_base_entity(const ui::scene_entity::lua_reference& entity) const -> math::point
{
    return position_for_entity(*entity.get());
}

auto ui::layout::positioning_frame::translate_point(math::point p) const -> math::point
{
    return m_base_origin + p;
}

auto ui::layout::positioning_frame::convert_point(math::point p) const -> math::point
{
    return p - m_base_origin;
}