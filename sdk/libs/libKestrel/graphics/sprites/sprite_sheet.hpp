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

#include <memory>
#include <vector>
#include <libKestrel/math/rect.hpp>
#include <libKestrel/graphics/texture/texture.hpp>
#include <libKestrel/physics/hitbox.hpp>

namespace kestrel::graphics
{
    class sprite_sheet: public std::enable_shared_from_this<sprite_sheet>
    {
    public:
        struct sprite
        {
            sprite(double x, double y, double width, double height);
            sprite(const math::point& origin, const math::size& size);
            explicit sprite(const math::rect& frame);

            [[nodiscard]] auto frame() const -> math::rect;
            [[nodiscard]] auto point() const -> math::point;
            [[nodiscard]] auto size() const -> math::size;

            [[nodiscard]] auto hitbox() const -> const physics::hitbox&;
            auto set_hitbox(const physics::hitbox& hb) -> void;

        private:
            math::rect m_frame;
            physics::hitbox m_hitbox;
        };

    public:
        sprite_sheet(std::shared_ptr<graphics::texture> tex, std::uint32_t sprite_width, std::uint32_t sprite_height);
        sprite_sheet(std::shared_ptr<graphics::texture> tex, const math::size& sprite_size);

        ~sprite_sheet();

        auto texture() const -> std::shared_ptr<graphics::texture>;

        auto sprite_count() const -> std::size_t;
        auto at(std::uint32_t n) const -> sprite_sheet::sprite;
        auto sprite_size() const -> math::size;
        auto flipped() const -> bool;

        auto layout_sprites(const math::size& sprite_size, bool flipped = false) -> void;
        auto layout_sprites(const std::vector<math::rect>& sprite_frames, bool flipped = false) -> void;

        auto build_hitboxes() -> void;

    private:
        std::shared_ptr<graphics::texture> m_backing_texture;
        std::vector<sprite_sheet::sprite> m_sprites;
        math::size m_sprite_base_size;
        bool m_flipped { false };

        auto layout_sprites(bool flipped) -> void;

    };
}