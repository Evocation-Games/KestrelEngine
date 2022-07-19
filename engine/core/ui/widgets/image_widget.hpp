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
#include "util/hint.hpp"
#include "math/size.hpp"
#include "math/rect.hpp"
#include "math/point.hpp"
#include "scripting/state.hpp"
#include "core/graphics/common/color.hpp"
#include "core/ui/entity/scene_entity.hpp"

namespace ui::widgets
{
    struct image_widget: public scripting::lua::object
    {
    public:
        enum class content_alignment : int { center, fit, stretch };

        typedef luabridge::RefCountedPtr<image_widget> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;


    public:
        explicit image_widget(const luabridge::LuaRef& image);

        [[nodiscard]] auto entity() const -> ui::scene_entity::lua_reference;
        auto draw() -> void;

        [[nodiscard]] lua_api auto frame() const -> math::rect;
        [[nodiscard]] lua_api auto image() const -> luabridge::LuaRef;

        lua_api auto set_frame(const math::rect& frame) -> void;
        lua_api auto set_image(const luabridge::LuaRef& image) -> void;

    private:
        math::rect m_frame { 0, 0, 100, 100 };
        luabridge::LuaRef m_image_ref { nullptr };
        scene_entity::lua_reference m_entity { nullptr };
        content_alignment m_alignment { content_alignment::center };

        auto resize(bool reload = false) -> void;
    };
}

