// Copyright (c) 2020 Tom Hancocks
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

#include <type_traits>
#include <memory>
#include <vector>
#include <optional>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/math/rect.hpp>
#include <libKestrel/math/line.hpp>
#include <libKestrel/graphics/image/static_image.hpp>
#include <libKestrel/graphics/types/color.hpp>
#include <libKestrel/entity/entity.hpp>
#include <libKestrel/graphics/canvas/rgba_buffer.hpp>
#include <libKestrel/font/typesetter.hpp>
#include <libKestrel/graphics/legacy/macintosh/picture.hpp>
#include <libKestrel/graphics/legacy/macintosh/color_icon.hpp>
#include <libKestrel/font/font.hpp>

namespace kestrel::graphics
{
    class lua_api(Canvas, Available_0_8) canvas: public std::enable_shared_from_this<graphics::canvas>
    {
    public:
        has_constructable_lua_api(canvas);

        lua_constructor(Available_0_8) explicit canvas(const math::size& size);
        ~canvas();

        lua_getter(name, Available_0_8) auto get_name() const -> std::string;
        lua_setter(name, Available_0_8) auto set_name(const std::string& name) -> void;

        lua_getter(penColor, Available_0_8) auto get_pen_color() const -> color::lua_reference;
        lua_setter(penColor, Available_0_8) auto set_pen_color(const color::lua_reference& color) -> void;

        lua_getter(bounds, Available_0_8) auto get_bounds() const -> math::rect;

        lua_setter(font, Available_0_8) auto set_font(const kestrel::font::reference::lua_reference& font) -> void;
        lua_getter(font, Available_0_8) [[nodiscard]] auto font() const -> std::shared_ptr<font>;

        lua_function(clear, Available_0_8) auto clear() -> void;

        lua_function(setClippingRect, Available_0_8) auto set_clipping_rect(const math::rect& r) -> void;
        lua_function(clearClippingRect, Available_0_8) auto clear_clipping_rect() -> void;

        lua_function(drawLine, Available_0_8) auto draw_line(const math::point& p, const math::point& q, double thickness) -> void;

        lua_function(drawRect, Available_0_8) auto draw_rect(const math::rect& r) -> void;
        lua_function(fillRect, Available_0_8) auto fill_rect(const math::rect& r) -> void;

        lua_function(drawCircle, Available_0_8) auto draw_circle(const math::point& p, double r) -> void;
        lua_function(fillCircle, Available_0_8) auto fill_circle(const math::point& p, double r) -> void;

        lua_function(layoutText, Available_0_8) auto layout_text(const std::string& text) -> math::size;
        lua_function(layoutTextInBounds, Available_0_8) auto layout_text_in_bounds(const std::string& text, const math::size& bounds) -> math::size;
        lua_function(characterPointInText, Available_0_8) auto character_point_in_text(std::uint32_t position) const -> math::point;
        lua_function(drawText, Available_0_8) auto draw_text(const math::point& point) -> void;

        lua_function(drawMacintoshPicture, Available_0_8) auto draw_picture(const image::legacy::macintosh::quickdraw::picture::lua_reference& pict, const math::rect& rect) -> void;
        lua_function(drawImage, Available_0_8) auto draw_image(const image::legacy::macintosh::quickdraw::picture::lua_reference& image, const math::point& point, const math::size& sz) -> void;
        lua_function(drawColorIcon, Available_0_8) auto draw_color_icon(const image::legacy::macintosh::quickdraw::color_icon::lua_reference& image, const math::point& point, const math::size& sz) -> void;
        lua_function(drawStaticImage, Available_0_8) auto draw_static_image(const image::static_image::lua_reference& image, const math::rect& rect) -> void;

        lua_function(applyMask, Available_0_8) auto apply_mask(const lua_reference& c) -> void;
        lua_function(drawMask, Available_0_8) auto draw_mask(const luabridge::LuaRef& mask_function) -> void;

        lua_function(spawnEntity, Available_0_8) auto spawn_entity(const math::point& position) ->std::shared_ptr<ecs::entity>;
        lua_function(entity, Available_0_8) auto entity() -> std::shared_ptr<ecs::entity>;
        lua_function(rebuildEntityTexture, Available_0_8) auto rebuild_texture() -> void;

    private:
        bool m_dirty { true };
        double m_scale { 2.0 };
        std::string m_name;
        math::size m_size;
        math::size m_scaled_size;
        rgba_buffer m_rgba_buffer;
        color m_pen_color;
        std::shared_ptr<ecs::entity> m_entity { nullptr };
        kestrel::font::typesetter m_typesetter;
        std::shared_ptr<texture> m_linked_tex;
        std::optional<math::rect> m_clipping_rect;

        auto raw() const -> std::uint8_t *;
        auto data() const -> graphite::data::block;
        auto draw_picture_at_point(const image::legacy::macintosh::quickdraw::picture::lua_reference& pict, const math::point &point) -> void;
    };

}

