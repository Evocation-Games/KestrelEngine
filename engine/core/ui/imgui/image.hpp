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

#include <string>
#include <imgui/imgui.h>
#include "core/ui/imgui/widget.hpp"
#include "scripting/state.hpp"
#include "util/hint.hpp"
#include "core/graphics/common/spritesheet.hpp"

namespace ui::imgui
{
    class image : public widget, public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<image> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        lua_api explicit image(luabridge::LuaRef src);

        [[nodiscard]] lua_api auto frame() const -> uint32_t { return m_frame; }
        lua_api auto set_frame(uint32_t frame) -> void { m_frame = frame; }

        [[nodiscard]] lua_api auto frame_count() const -> uint32_t { return m_spritesheet->sprite_count(); }

        auto position() const -> math::point override { return widget::position(); }
        auto set_position(const math::point &position) -> void override { widget::set_position(position); }

        auto size() const -> math::size override { return widget::size(); }
        auto set_size(const math::size &size) -> void override { widget::set_size(size); }

        auto draw() -> void override;

    private:
        std::shared_ptr<graphics::spritesheet> m_spritesheet;
        uint32_t m_frame { 0 };

    };
}