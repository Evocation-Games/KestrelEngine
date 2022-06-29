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
#include "math/rect.hpp"
#include "core/graphics/common/texture.hpp"

namespace graphics
{

    class spritesheet: public std::enable_shared_from_this<graphics::spritesheet>
    {
    public:
        struct sprite
        {
        private:
            math::rect m_frame;

        public:
            sprite(const double& x, const double& y, const double& width, const double& height);
            sprite(const math::point& origin, const math::size& size);
            explicit sprite(const math::rect& frame);

            [[nodiscard]] auto frame() const -> math::rect;
            [[nodiscard]] auto point() const -> math::point;
            [[nodiscard]] auto size() const -> math::size;

        };

    public:
        spritesheet(std::shared_ptr<graphics::texture> tex, uint32_t sprite_width, uint32_t sprite_height);
        spritesheet(std::shared_ptr<graphics::texture> tex, const math::size& sprite_size);

        auto texture() const -> std::shared_ptr<graphics::texture>;

        auto sprite_count() const -> std::size_t;
        auto at(const int& n) const -> spritesheet::sprite;
        auto sprite_size() const -> math::size;
        auto flipped() const -> bool;

        auto layout_sprites(const math::size& sprite_size, bool flipped = false) -> void;
        auto layout_sprites(const std::vector<math::rect>& sprite_frames, bool flipped = false) -> void;

    private:
        std::shared_ptr<graphics::texture> m_backing_texture;
        std::vector<spritesheet::sprite> m_sprites;
        math::size m_sprite_base_size;
        bool m_flipped { false };

        auto layout_sprites(bool flipped) -> void;
    };

}