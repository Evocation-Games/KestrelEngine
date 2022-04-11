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

namespace ui::layout
{
    struct positioning_frame : public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<ui::layout::positioning_frame> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        lua_api positioning_frame(math::rect frame, math::point anchor, math::point offset);

        [[nodiscard]] auto position_for_entity(const scene_entity& entity) const -> math::point;

        [[nodiscard]] lua_api auto lua_position_for_base_entity(const ui::scene_entity::lua_reference& entity) const -> math::point;
        [[nodiscard]] lua_api auto translate_point(math::point p) const -> math::point;
        [[nodiscard]] lua_api auto convert_point(math::point p0) const -> math::point;

    private:
        math::point m_anchor_point;
        math::point m_base_origin;
        math::rect m_frame;
    };
}