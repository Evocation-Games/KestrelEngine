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
#include "util/hint.hpp"
#include "scripting/state.hpp"
#include "util/lua_vector.hpp"
#include "math/rect.hpp"
#include "core/ui/font/manager.hpp"
#include "core/graphics/common/entity.hpp"
#include "core/graphics/common/canvas.hpp"
#include "core/ui/alignment.hpp"

namespace ui
{
    struct text_entity: public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<text_entity> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        text_entity() = default;
        explicit text_entity(const std::string& text);

        [[nodiscard]] lua_api auto text() const -> std::string;
        [[nodiscard]] lua_api auto font() const -> font::reference::lua_reference;
        [[nodiscard]] lua_api auto text_color() const -> graphics::color::lua_reference;
        [[nodiscard]] lua_api auto background_color() const -> graphics::color::lua_reference;
        [[nodiscard]] lua_api auto position() const -> math::point;
        [[nodiscard]] lua_api auto draw_position() const -> math::point;
        [[nodiscard]] lua_api auto centered() const -> bool;
        [[nodiscard]] lua_api auto size() const -> math::size;
        [[nodiscard]] lua_api auto half_size() const -> math::size;
        [[nodiscard]] lua_api auto render_size() const -> math::size;
        [[nodiscard]] lua_api auto draw_size() const -> math::size;
        [[nodiscard]] lua_api auto alpha() const -> double;
        [[nodiscard]] lua_api auto blend_mode() const -> int;
        [[nodiscard]] lua_api auto clipping_area() const -> math::size;
        [[nodiscard]] lua_api auto clipping_offset() const -> math::point;
        [[nodiscard]] lua_api auto ignore_positioning_frame_scaler() const -> bool;
        [[nodiscard]] lua_api auto lua_safe_alignment() const -> int32_t;

        lua_api auto set_text(const std::string& v) -> void;
        lua_api auto set_font(const font::reference::lua_reference& v) -> void;
        lua_api auto set_text_color(const graphics::color::lua_reference& v) -> void;
        lua_api auto set_background_color(const graphics::color::lua_reference& v) -> void;
        lua_api auto set_position(const math::point& v) -> void;
        lua_api auto set_draw_position(const math::point& v) -> void;
        lua_api auto set_centered(bool v) -> void;
        lua_api auto set_size(const math::size& v) -> void;
        lua_api auto set_render_size(const math::size& v) -> void;
        lua_api auto set_draw_size(const math::size& v) -> void;
        lua_api auto set_alpha(double v) -> void;
        lua_api auto set_blend_mode(int v) -> void;
        lua_api auto set_clipping_area(const math::size& v) -> void;
        lua_api auto set_clipping_offset(const math::point& v) -> void;
        lua_api auto set_ignore_positioning_frame_scaler(bool f) -> void;
        lua_api auto set_lua_safe_alignment(int32_t v) -> void;

        lua_api auto layout() -> void;
        lua_api auto on_layout(const luabridge::LuaRef& callback) -> void;
        lua_api auto draw() -> void;

    private:
        bool m_dirty { true };
        std::string m_text;
        font::reference::lua_reference m_font { nullptr };
        graphics::color::lua_reference m_text_color { nullptr };
        graphics::color::lua_reference m_background_color { nullptr };
        math::point m_position { 0 };
        std::int16_t m_min_height { 0 };
        bool m_centered { false };
        std::shared_ptr<graphics::entity> m_entity;
        std::unique_ptr<graphics::canvas> m_canvas;
        bool m_ignore_positioning_frame_scaler { false };
        enum horizontal_alignment m_alignment { horizontal_alignment::left };

        auto redraw() -> void;
    };
}