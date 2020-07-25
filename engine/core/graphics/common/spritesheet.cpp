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

#include "core/graphics/common/spritesheet.hpp"

// MARK: - Sprite Construction

graphics::spritesheet::sprite::sprite(const double& x, const double& y, const double& width, const double& height)
    : m_frame(x, y, width, height)
{

}

graphics::spritesheet::sprite::sprite(const math::point& origin, const math::size& size)
    : m_frame(origin, size)
{

}

graphics::spritesheet::sprite::sprite(const math::rect& frame)
    : m_frame(frame)
{

}

// MARK: - Sprite Accessor

auto graphics::spritesheet::sprite::frame() const -> math::rect
{
    return m_frame;
}

auto graphics::spritesheet::sprite::point() const -> math::point
{
    return m_frame.origin;
}

auto graphics::spritesheet::sprite::size() const -> math::size
{
    return m_frame.size;
}

// MARK: - Spritesheet Construction

graphics::spritesheet::spritesheet(std::shared_ptr<graphics::texture> tex, const double& sprite_width, const double& sprite_height)
    : m_backing_texture(std::move(tex)), m_sprite_base_size(sprite_width, sprite_height)
{
    layout_sprites();
}

graphics::spritesheet::spritesheet(std::shared_ptr<graphics::texture> tex, const math::size& sprite_size)
    : m_backing_texture(std::move(tex)), m_sprite_base_size(sprite_size)
{
    layout_sprites();
}

// MARK: - Spritesheet Accessors

auto graphics::spritesheet::texture() const -> std::shared_ptr<graphics::texture>
{
    return m_backing_texture;
}

auto graphics::spritesheet::sprite_count() const -> int
{
    return m_sprites.size();
}

auto graphics::spritesheet::at(const int& n) const -> spritesheet::sprite
{
    return m_sprites.at(n);
}

auto graphics::spritesheet::sprite_size() const -> math::size
{
    return m_sprite_base_size;
}

// MARK: - Spritesheet Layout

auto graphics::spritesheet::layout_sprites() -> void
{
    // Each sprite is providing the UV texture mapping information that will be used for drawing
    // the appropriate portion of a texture.
    auto sprite_uv_width = m_sprite_base_size.width / m_backing_texture->size().width;
    auto sprite_uv_height = m_sprite_base_size.height / m_backing_texture->size().height;

    // Determine the number of sprites in the grid.
    auto count_x = static_cast<int>(m_backing_texture->size().width / m_sprite_base_size.width);
    auto count_y = static_cast<int>(m_backing_texture->size().height / m_sprite_base_size.height);

    for (auto y = 0; y < count_y; ++y) {
        for (auto x = 0; x < count_x; ++x) {
            spritesheet::sprite sprite(x * sprite_uv_width, y * sprite_uv_height, sprite_uv_width, sprite_uv_height);
            m_sprites.emplace_back(sprite);
        }
    }
}

auto graphics::spritesheet::layout_sprites(const math::size &sprite_size) -> void
{
    m_sprite_base_size = sprite_size;
    m_sprites.clear();
    layout_sprites();
}

auto graphics::spritesheet::layout_sprites(const std::vector<math::rect> &sprite_frames) -> void
{
    m_sprites.clear();
    m_sprite_base_size = sprite_frames.front().size;

    auto width = m_backing_texture->size().width;
    auto height = m_backing_texture->size().height;
    for (const auto& frame : sprite_frames) {
        auto x = frame.get_x();
        auto y = frame.get_y();
        auto w = frame.get_width();
        auto h = frame.get_height();

        y = height - y - h;

        m_sprites.emplace_back(spritesheet::sprite(x / width, y / height, w / width, h / height));
    }
}
