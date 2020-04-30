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

#include "foundation/gl/spritesheet.hpp"

// MARK: - Constructors

kestrel::gl::spritesheet::spritesheet(std::shared_ptr<graphite::qd::surface> surface, const math::size& sprite_size, int total_sprites)
    : m_sprite_size(sprite_size), m_total_sprites(total_sprites)
{
    // Create a new texture from the surface and register it.
    auto size = math::size(surface->size());
    m_texture = diamane::gl::texture::from_raw(surface->raw(), size.diamane());
    m_texture->clear_uvs();

    if (!m_texture->is_registered()) {
        m_texture->register_texture();
    }

    // Setup an initial pool of sprites.
    configure_sprites(sprite_size, total_sprites);
}

auto kestrel::gl::spritesheet::create(std::shared_ptr<graphite::qd::surface> surface, const math::size& sprite_size, int total_sprites) -> std::shared_ptr<kestrel::gl::spritesheet>
{
    return std::make_shared<kestrel::gl::spritesheet>(surface, sprite_size, total_sprites);
}

// MARK: - Accessors

auto kestrel::gl::spritesheet::sprite_count() const -> int
{
    return m_sprites.size();
}

auto kestrel::gl::spritesheet::texture() const -> std::shared_ptr<diamane::gl::texture>
{
    return m_texture;
}

auto kestrel::gl::spritesheet::sprite(int frame) const -> std::weak_ptr<gl::sprite>
{
    return m_sprites[frame];
}

auto kestrel::gl::spritesheet::configure_sprites(const math::size& sprite_size, int total_sprites) -> void
{
    m_sprites.clear();
    m_total_sprites = total_sprites;
    m_texture->clear_uvs();

    auto size = m_texture->size();

    // Build the vector of sprites available in the surface. To do this we need to calculate the
    // geometry of the sprite sheet (what is the grid of sprites) and then traverse it and construct
    // each sprite as we get to it.
    auto grid = math::size(size.width() / sprite_size.width(), size.height() / sprite_size.height());
    for (auto y = 0; y < static_cast<int>(grid.height()); ++y) {
        for (auto x = 0; x < static_cast<int>(grid.width()); ++x) {
            auto sprite = gl::sprite::create(*this, math::point(x, y), sprite_size);
            m_sprites.emplace_back(sprite);
            sprite->set_uv_index(m_texture->add_uv(sprite->uv().diamane()));
        }
    }
}
