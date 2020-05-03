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
#include <string>
#include "foundation/math/size.hpp"
#include "foundation/gl/sprite.hpp"
#include "libGraphite/quickdraw/internal/surface.hpp"
#include "libDiamane/graphics/texture.hpp"

namespace kestrel { namespace gl {

    struct sprite;

    class spritesheet
    {
    private:
        std::shared_ptr<diamane::gl::texture> m_texture;
        math::size m_sprite_size;
        int m_total_sprites;
        std::vector<std::shared_ptr<gl::sprite>> m_sprites;

    public:
        spritesheet(std::shared_ptr<diamane::gl::texture> texture, const math::size& sprite_size, int total_sprites);
        spritesheet(std::shared_ptr<graphite::qd::surface> surface, const math::size& sprite_size, int total_sprites);

        static auto create(std::shared_ptr<diamane::gl::texture> texture, const math::size& sprite_size, int total_sprites) -> std::shared_ptr<gl::spritesheet>;
        static auto create(std::shared_ptr<graphite::qd::surface> surface, const math::size& sprite_size, int total_sprites) -> std::shared_ptr<gl::spritesheet>;

        auto configure_sprites(const math::size& sprite_size, int total_sprites) -> void;

        auto sprite_count() const -> int;
        auto texture() const -> std::shared_ptr<diamane::gl::texture>;

        auto sprite(int frame) const -> std::weak_ptr<gl::sprite>;
    };

}};

#endif //KESTREL_SPRITESHEET_HPP
