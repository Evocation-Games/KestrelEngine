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
#include "core/graphics/common/canvas.hpp"

namespace ui
{
    struct scene_entity;
}

namespace ui::widgets
{
    struct custom_widget: public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<custom_widget> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    private:
        bool m_dirty { true };
        luabridge::LuaRef m_drawing_function;
        luabridge::LuaRef m_user_info;
        graphics::canvas::lua_reference m_canvas;
        std::shared_ptr<scene_entity> m_entity;
        math::rect m_frame { 0, 0, 100, 100 };

        auto resize() -> void;
        auto redraw() -> void;

    public:
        explicit custom_widget(const luabridge::LuaRef& drawing_function);

        [[nodiscard]] auto entity() const -> std::shared_ptr<ui::scene_entity>;
        lua_api auto draw() -> void;

        [[nodiscard]] lua_api auto frame() const -> math::rect;
        [[nodiscard]] lua_api auto user_info() const -> luabridge::LuaRef;

        lua_api auto set_frame(const math::rect& frame) -> void;
        lua_api auto set_user_info(const luabridge::LuaRef& info) -> void;

    };
}