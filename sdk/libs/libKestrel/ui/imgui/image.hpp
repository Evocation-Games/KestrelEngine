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
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/ui/imgui/widget.hpp>
#include <libKestrel/graphics/sprites/sprite_sheet.hpp>

namespace kestrel::ui::imgui
{
    class lua_api(ImGui.Image, Available_0_8) image : public widget
    {
    public:
        has_constructable_lua_api(image);

        luatool_type_fix(luabridge::LuaRef, src)
        lua_constructor(Available_0_8) explicit image(luabridge::LuaRef src);

        lua_getter(frame, Available_0_8) [[nodiscard]] auto frame() const -> std::uint32_t { return m_frame; }
        lua_setter(frame, Available_0_8) auto set_frame(std::uint32_t frame) -> void { m_frame = frame; }

        lua_getter(frameCount, Available_0_8) [[nodiscard]] auto frame_count() const -> std::uint32_t { return m_sprite_sheet->sprite_count(); }

        lua_getter(position, Available_0_8) auto position() const -> math::point override { return widget::position(); }
        lua_setter(position, Available_0_8) auto set_position(const math::point &position) -> void override { widget::set_position(position); }

        lua_getter(size, Available_0_8) auto size() const -> math::size override { return widget::size(); }
        lua_setter(size, Available_0_8) auto set_size(const math::size &size) -> void override { widget::set_size(size); }

        auto draw() -> void override;

    private:
        std::shared_ptr<graphics::sprite_sheet> m_sprite_sheet;
        std::uint32_t m_frame { 0 };

    };
}