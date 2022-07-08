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
#include "scripting/state.hpp"
#include "util/hint.hpp"
#include "core/graphics/common/canvas.hpp"
#include "core/graphics/common/color.hpp"
#include "math/point.hpp"
#include "math/size.hpp"
#include "math/rect.hpp"

namespace ui
{
    struct scene_entity;
}

namespace ui::widgets
{
    struct textarea_widget : public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<textarea_widget> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        explicit lua_api textarea_widget(const std::string& text);
        textarea_widget(const math::rect& frame, const std::string& text);

        auto draw() -> void;

        [[nodiscard]] auto entity() const -> std::shared_ptr<ui::scene_entity>;

        [[nodiscard]] lua_api auto text() const -> std::string;
        [[nodiscard]] lua_api auto font() const -> std::string;
        [[nodiscard]] auto font_size() const -> int16_t;
        [[nodiscard]] lua_api auto color() const -> graphics::color::lua_reference;
        [[nodiscard]] lua_api auto background_color() const -> graphics::color::lua_reference;
        [[nodiscard]] lua_api auto offset() const -> math::size;
        [[nodiscard]] lua_api auto position() const -> math::point;
        [[nodiscard]] lua_api auto size() const -> math::size;
        [[nodiscard]] lua_api auto frame() const -> math::rect;
        [[nodiscard]] lua_api auto scroll_offset() const -> int32_t;

        lua_api auto set_text(const std::string& v) -> void;
        lua_api auto set_font(const std::string& v) -> void;
        auto set_font_size(int16_t v) -> void;
        lua_api auto set_color(const graphics::color::lua_reference& v) -> void;
        lua_api auto set_background_color(const graphics::color::lua_reference& v) -> void;
        lua_api auto set_offset(const math::size& v) -> void;
        lua_api auto set_position(const math::point& v) -> void;
        lua_api auto set_size(const math::size& v) -> void;
        lua_api auto set_frame(const math::rect& v) -> void;
        lua_api auto set_scroll_offset(int32_t offset) -> void;

        [[nodiscard]] lua_api auto can_scroll_up() const -> bool;
        [[nodiscard]] lua_api auto can_scroll_down() const -> bool;

    private:
        bool m_dirty { true };
        std::string m_text;
        std::string m_font_face { "Geneva" };
        int16_t m_font_size { 12 };
        graphics::color m_color { graphics::color::white_color() };
        graphics::color m_background { graphics::color::clear_color() };
        math::size m_offset { 0 };
        std::unique_ptr<graphics::canvas> m_canvas;
        std::shared_ptr<ui::scene_entity> m_entity;
        int32_t m_scroll_offset { 0 };
        bool m_can_scroll_up { false };
        bool m_can_scroll_down { false };

        auto redraw_entity() -> void;

        [[nodiscard]] lua_api auto lua_safe_font_size() const -> int;
        lua_api auto set_lua_safe_font_size(int v) -> void;
    };
}
