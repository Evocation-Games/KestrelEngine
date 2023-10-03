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
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/lua/support/vector.hpp>
#include <libKestrel/math/rect.hpp>
#include <libKestrel/font/manager.hpp>
#include <libKestrel/graphics/canvas/canvas.hpp>
#include <libKestrel/graphics/types/color.hpp>
#include <libKestrel/ui/alignment.hpp>
#include <libKestrel/ui/layout/axis_origin.hpp>

namespace kestrel::ui
{
    struct lua_api(TextEntity, Available_0_8) text_entity
    {
    public:
        has_constructable_lua_api(text_entity);

        text_entity() = default;
        lua_constructor(Available_0_8) explicit text_entity(const std::string& text);

        // MARK: - Text Properties
        lua_getter(text, Available_0_8) [[nodiscard]] auto text() const -> std::string;
        lua_setter(text, Available_0_8) auto set_text(const std::string& v) -> void;
        lua_getter(font, Available_0_8) [[nodiscard]] auto font() const -> font::reference::lua_reference;
        lua_setter(font, Available_0_8) auto set_font(const font::reference::lua_reference& v) -> void;
        lua_getter(textColor, Available_0_8) [[nodiscard]] auto text_color() const -> graphics::color::lua_reference;
        lua_setter(textColor, Available_0_8) auto set_text_color(const graphics::color::lua_reference& v) -> void;
        lua_getter(backgroundColor, Available_0_8) [[nodiscard]] auto background_color() const -> graphics::color::lua_reference;
        lua_setter(backgroundColor, Available_0_8) auto set_background_color(const graphics::color::lua_reference& v) -> void;
        lua_getter(alignment, Available_0_8) [[nodiscard]] auto lua_safe_alignment() const -> int32_t;
        lua_setter(alignment, Available_0_8) auto set_lua_safe_alignment(std::int32_t v) -> void;

        // MARK: - Positioning
        lua_getter(position, Available_0_8) [[nodiscard]] auto position() const -> math::point;
        lua_setter(position, Available_0_8) auto set_position(const math::point& v) -> void;
        auto update_position() -> void;

        [[nodiscard]] auto anchor_point() const -> enum layout::axis_origin;
        auto set_anchor_point(enum layout::axis_origin v) -> void;
        lua_getter(anchorPoint, Available_0_8) [[nodiscard]] auto lua_anchor_point() const -> std::int32_t;
        lua_setter(anchorPoint, Available_0_8) auto set_lua_anchor_point(std::int32_t v) -> void;

        // MARK: - Sizing
        lua_getter(size, Available_0_8) [[nodiscard]] auto size() const -> math::size;
        lua_setter(size, Available_0_8) auto set_size(const math::size& v) -> void;
        lua_getter(halfSize, Available_0_8) [[nodiscard]] auto half_size() const -> math::size;

        // MARK: - Layout
        lua_getter(ignorePositioningFrameScaler, Available_0_8) [[nodiscard]] auto ignore_positioning_frame_scaler() const -> bool;
        lua_setter(ignorePositioningFrameScaler, Available_0_8) auto set_ignore_positioning_frame_scaler(bool f) -> void;
        lua_function(layout, Available_0_8) auto layout() -> void;
        lua_function(onLayout, Available_0_8) auto on_layout(const luabridge::LuaRef& callback) -> void;

        // MARK: - Drawing
        lua_getter(alpha, Available_0_8) [[nodiscard]] auto alpha() const -> double;
        lua_setter(alpha, Available_0_8) auto set_alpha(double v) -> void;
        lua_getter(blend, Available_0_8) [[nodiscard]] auto blend_mode() const -> std::int32_t;
        lua_setter(blend, Available_0_8) auto set_blend_mode(std::int32_t v) -> void;
        lua_function(draw, Available_0_8) auto draw() -> void;

        // MARK: - Clipping
        lua_getter(clippingArea, Available_0_8) [[nodiscard]] auto clipping_area() const -> math::size;
        lua_setter(clippingArea, Available_0_8) auto set_clipping_area(const math::size& v) -> void;
        lua_getter(clippingOffset, Available_0_8) [[nodiscard]] auto clipping_offset() const -> math::point;
        lua_setter(clippingOffset, Available_0_8) auto set_clipping_offset(const math::point& v) -> void;

    private:
        bool m_dirty { true };
        std::string m_text;
        font::reference::lua_reference m_font { nullptr };
        graphics::color::lua_reference m_text_color { nullptr };
        graphics::color::lua_reference m_background_color { nullptr };
        math::point m_position { 0 };
        std::int16_t m_min_height { 0 };
        enum layout::axis_origin m_anchor { layout::axis_origin::center };
        std::shared_ptr<ecs::entity> m_entity;
        std::unique_ptr<graphics::canvas> m_canvas;
        bool m_ignore_positioning_frame_scaler { false };
        enum horizontal_alignment m_alignment { horizontal_alignment::left };

        auto redraw() -> void;
    };
}