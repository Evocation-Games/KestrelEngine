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

#include <libKestrel/graphics/sprites/sprite_sheet.hpp>
#include <libKestrel/physics/constructors/hitbox_constructor.hpp>

// MARK: - Sprite Construction

kestrel::graphics::sprite_sheet::sprite::sprite(double x, double y, double width, double height)
    : m_frame(x, y, width, height)
{
}

kestrel::graphics::sprite_sheet::sprite::sprite(const math::point& origin, const math::size& size)
    : m_frame(origin, size)
{
}

kestrel::graphics::sprite_sheet::sprite::sprite(const math::rect& frame)
    : m_frame(frame)
{
}

// MARK: - Sprite Accessor

auto kestrel::graphics::sprite_sheet::sprite::frame() const -> math::rect
{
    return m_frame;
}

auto kestrel::graphics::sprite_sheet::sprite::point() const -> math::point
{
    return m_frame.origin();
}

auto kestrel::graphics::sprite_sheet::sprite::size() const -> math::size
{
    return m_frame.size();
}

auto kestrel::graphics::sprite_sheet::sprite::hitbox() const -> const physics::hitbox &
{
    return m_hitbox;
}

auto kestrel::graphics::sprite_sheet::sprite::set_hitbox(const physics::hitbox &hb) -> void
{
    m_hitbox = hb;
}

// MARK: - Spritesheet Construction

kestrel::graphics::sprite_sheet::sprite_sheet(std::shared_ptr<graphics::texture> tex, std::uint32_t sprite_width, std::uint32_t sprite_height)
    : m_backing_texture(tex), m_sprite_base_size(sprite_width, sprite_height)
{
    layout_sprites(false);
}

kestrel::graphics::sprite_sheet::sprite_sheet(std::shared_ptr<graphics::texture> tex, const math::size& sprite_size)
    : m_backing_texture(tex), m_sprite_base_size(sprite_size)
{
    layout_sprites(false);
}

// MARK: - Sprite Sheet Destruction

kestrel::graphics::sprite_sheet::~sprite_sheet()
{
    m_backing_texture = nullptr;
    m_sprites.clear();
}

// MARK: - Spritesheet Accessors

auto kestrel::graphics::sprite_sheet::texture() const -> std::shared_ptr<graphics::texture>
{
    return m_backing_texture;
}

auto kestrel::graphics::sprite_sheet::sprite_count() const -> std::size_t
{
    return m_sprites.size();
}

auto kestrel::graphics::sprite_sheet::at(std::uint32_t n) const -> sprite_sheet::sprite
{
    return m_sprites.at(n);
}

auto kestrel::graphics::sprite_sheet::sprite_size() const -> math::size
{
    return m_sprite_base_size;
}

// MARK: - Sprite Sheet Layout

auto kestrel::graphics::sprite_sheet::layout_sprites(bool flipped) -> void
{
    // Each sprite is providing the UV texture mapping information that will be used for drawing
    // the appropriate portion of a texture.
    auto uv = m_sprite_base_size / m_backing_texture->size();

    // Determine the number of sprites in the grid.
    auto count = m_backing_texture->size() / m_sprite_base_size;
    auto count_x = count.width();
    auto count_y = count.height();

    if (flipped) {
        for (auto y = 0; y < count_y; ++y) {
            for (auto x = 0; x < count_x; ++x) {
                sprite_sheet::sprite sprite(x * uv.width(), (y + 1) * uv.height(), uv.width(), -uv.height());
                m_sprites.emplace_back(sprite);
            }
        }
    }
    else {
        for (auto y = 0; y < count_y; ++y) {
            for (auto x = 0; x < count_x; ++x) {
                sprite_sheet::sprite sprite(x * uv.width(), y * uv.height(), uv.width(), uv.height());
                m_sprites.emplace_back(sprite);
            }
        }
    }
    m_flipped = flipped;

}

auto kestrel::graphics::sprite_sheet::layout_sprites(const math::size &sprite_size, bool flipped) -> void
{
    m_sprite_base_size = sprite_size;
    m_sprites.clear();
    layout_sprites(flipped);
}

auto kestrel::graphics::sprite_sheet::layout_sprites(const std::vector<math::rect> &sprite_frames, bool flipped) -> void
{
    m_sprites.clear();
    m_sprite_base_size = sprite_frames.front().size();
    m_flipped = flipped;

    auto width = m_backing_texture->size().width();
    auto height = m_backing_texture->size().height();

    if (flipped) {
        for (const auto& frame : sprite_frames) {
            auto x = frame.x();
            auto y = frame.y();
            auto w = frame.width();
            auto h = frame.height();

            y = height - y;

            m_sprites.emplace_back(x / width, (y + 1) / height, w / width, -(h / height));
        }
    }
    else {
        for (const auto& frame : sprite_frames) {
            auto x = frame.x();
            auto y = frame.y();
            auto w = frame.width();
            auto h = frame.height();

            y = height - y - h;

            m_sprites.emplace_back(x / width, y / height, w / width, h / height);
        }
    }

}

auto kestrel::graphics::sprite_sheet::flipped() const -> bool
{
    return m_flipped;
}

// MARK: - HitBoxes

auto kestrel::graphics::sprite_sheet::build_hitboxes() -> void
{
    // Loop through each of the sprites and build up a collision map for each.
    for (auto& sprite : m_sprites) {
        auto hb = std::move(physics::hitbox_constructor::hitbox(shared_from_this(), sprite.frame()));
        sprite.set_hitbox(hb);
    }
}