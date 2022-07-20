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
#include "util/hint.hpp"
#include "math/rect.hpp"
#include "math/line.hpp"
#include "scripting/state.hpp"
#include "core/asset/static_image.hpp"
#include "core/graphics/common/font.hpp"
#include "core/graphics/common/color.hpp"
#include "core/graphics/common/entity.hpp"
#include "core/graphics/common/rgba_buffer.hpp"
#include "core/asset/legacy/macintosh/picture.hpp"
#include "core/graphics/common/text/typesetter.hpp"
#include "core/asset/legacy/macintosh/color_icon.hpp"
#include "core/ui/font/font.hpp"

namespace graphics
{

    class canvas: public std::enable_shared_from_this<graphics::canvas>, public scripting::lua::object
    {
    public:
        typedef luabridge::RefCountedPtr<graphics::canvas> lua_reference;
        static auto enroll_object_api_in_state(const std::shared_ptr<scripting::lua::state>& lua) -> void;

    public:
        lua_api explicit canvas(const math::size& size);
        ~canvas();

        lua_api auto get_name() const -> std::string;
        lua_api auto set_name(const std::string& name) -> void;

        lua_api auto get_pen_color() const -> graphics::color;
        lua_api auto set_pen_color(const graphics::color& color) -> void;

        lua_api auto get_bounds() const -> math::rect;

        lua_api auto set_font(const ui::font::reference::lua_reference& font) -> void;
        [[nodiscard]] auto font() const -> std::shared_ptr<graphics::font>;

        lua_api auto clear() -> void;

        lua_api auto set_clipping_rect(const math::rect& r) -> void;
        lua_api auto clear_clipping_rect() -> void;

        lua_api auto draw_line(const math::point& p, const math::point& q, const double& thickness) -> void;

        lua_api auto draw_rect(const math::rect& r) -> void;
        lua_api auto fill_rect(const math::rect& r) -> void;

        lua_api auto draw_circle(const math::point& p, const double& r) -> void;
        lua_api auto fill_circle(const math::point& p, const double& r) -> void;

        lua_api auto layout_text(const std::string& text) -> math::size;
        lua_api auto layout_text_in_bounds(const std::string& text, const math::size& bounds) -> math::size;
        lua_api auto character_point_in_text(const int& position) const -> math::point;
        lua_api auto draw_text(const math::point& point) -> void;

        lua_api auto draw_picture(const asset::legacy::macintosh::quickdraw::picture::lua_reference& pict, const math::rect& rect) -> void;
        lua_api auto draw_image(const asset::legacy::macintosh::quickdraw::picture::lua_reference& image, const math::point& point, const math::size& sz) -> void;
        lua_api auto draw_color_icon(const asset::legacy::macintosh::quickdraw::color_icon::lua_reference& image, const math::point& point, const math::size& sz) -> void;
        lua_api auto draw_static_image(const asset::static_image::lua_reference& image, const math::rect& rect) -> void;

        lua_api auto apply_mask(const graphics::canvas::lua_reference& c) -> void;
        lua_api auto draw_mask(const luabridge::LuaRef& mask_function) -> void;

        lua_api auto spawn_entity(const math::point& position) ->std::shared_ptr<graphics::entity>;
        lua_api auto entity() -> std::shared_ptr<graphics::entity>;
        lua_api auto rebuild_texture() -> void;

    private:
        bool m_dirty { true };
        double m_scale { 2.0 };
        std::string m_name;
        math::size m_size;
        math::size m_scaled_size;
        graphics::rgba_buffer m_rgba_buffer;
        graphics::color m_pen_color;
        std::shared_ptr<graphics::entity> m_entity { nullptr };
        graphics::typesetter m_typesetter;
        std::shared_ptr<texture> m_linked_tex;
        std::optional<math::rect> m_clipping_rect;

        auto raw() const -> uint8_t *;
        auto data() const -> graphite::data::block;
        auto draw_picture_at_point(const asset::legacy::macintosh::quickdraw::picture::lua_reference& pict, const math::point &point) -> void;
    };

}

