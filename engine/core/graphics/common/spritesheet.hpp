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

#if !defined(KESTREL_SPRITESHEET_HPP)
#define KESTREL_SPRITESHEET_HPP

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
            sprite(const math::rect& frame);

            auto frame() const -> math::rect;
            auto point() const -> math::point;
            auto size() const -> math::size;

        };

    private:
        std::shared_ptr<graphics::texture> m_backing_texture;
        std::vector<spritesheet::sprite> m_sprites;
        math::size m_sprite_base_size;

        auto layout_sprites() -> void;

    public:
        spritesheet(std::shared_ptr<graphics::texture> tex, const double& sprite_width, const double& sprite_height);
        spritesheet(std::shared_ptr<graphics::texture> tex, const math::size& sprite_size);

        auto texture() const -> std::shared_ptr<graphics::texture>;

        auto sprite_count() const -> int;
        auto at(const int& n) const -> spritesheet::sprite;
        auto sprite_size() const -> math::size;

        auto layout_sprites(const math::size& sprite_size) -> void;
        auto layout_sprites(const std::vector<math::rect>& sprite_frames) -> void;
    };

}

#endif //KESTREL_SPRITESHEET_HPP
