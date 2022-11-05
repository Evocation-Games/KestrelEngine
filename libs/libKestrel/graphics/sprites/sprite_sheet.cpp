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
#include <libKestrel/physics/collision_map.hpp>

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
    return m_frame.origin;
}

auto kestrel::graphics::sprite_sheet::sprite::size() const -> math::size
{
    return m_frame.size;
}

auto kestrel::graphics::sprite_sheet::sprite::collision_map() const -> const physics::collision_map &
{
    return m_map;
}

auto kestrel::graphics::sprite_sheet::sprite::set_collision_map(physics::collision_map map) -> void
{
    m_map = std::move(map);
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
    auto sprite_uv_width = m_sprite_base_size.width / m_backing_texture->size().width;
    auto sprite_uv_height = m_sprite_base_size.height / m_backing_texture->size().height;

    // Determine the number of sprites in the grid.
    auto count_x = static_cast<int>(m_backing_texture->size().width / m_sprite_base_size.width);
    auto count_y = static_cast<int>(m_backing_texture->size().height / m_sprite_base_size.height);

    if (flipped) {
        for (auto y = 0; y < count_y; ++y) {
            for (auto x = 0; x < count_x; ++x) {
                sprite_sheet::sprite sprite(x * sprite_uv_width, (y + 1) * sprite_uv_height, sprite_uv_width, -sprite_uv_height);
                m_sprites.emplace_back(sprite);
            }
        }
    }
    else {
        for (auto y = 0; y < count_y; ++y) {
            for (auto x = 0; x < count_x; ++x) {
                sprite_sheet::sprite sprite(x * sprite_uv_width, y * sprite_uv_height, sprite_uv_width, sprite_uv_height);
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
    m_sprite_base_size = sprite_frames.front().size;
    m_flipped = flipped;

    auto width = m_backing_texture->size().width;
    auto height = m_backing_texture->size().height;

    if (flipped) {
        for (const auto& frame : sprite_frames) {
            auto x = frame.get_x();
            auto y = frame.get_y();
            auto w = frame.get_width();
            auto h = frame.get_height();

            y = height - y;

            m_sprites.emplace_back(sprite_sheet::sprite(x / width, (y + 1) / height, w / width, -(h / height)));
        }
    }
    else {
        for (const auto& frame : sprite_frames) {
            auto x = frame.get_x();
            auto y = frame.get_y();
            auto w = frame.get_width();
            auto h = frame.get_height();

            y = height - y - h;

            m_sprites.emplace_back(sprite_sheet::sprite(x / width, y / height, w / width, h / height));
        }
    }

}

auto kestrel::graphics::sprite_sheet::flipped() const -> bool
{
    return m_flipped;
}

// MARK: - Collision Maps

auto kestrel::graphics::sprite_sheet::build_collision_maps() -> void
{
    auto width = m_backing_texture->size().width;
    auto height = m_backing_texture->size().height;

    // Loop through each of the sprites and build up a collision map for each.
    for (auto& sprite : m_sprites) {
        auto x_step = std::max(1.0, (sprite.size().width * width) / physics::collision_map::granularity);
        auto y_step = std::max(1.0, (sprite.size().height * height) / physics::collision_map::granularity);

        auto sprite_width = (sprite.size().width * width);
        auto sprite_height = (sprite.size().height * height);

        // Divide the sprite into "chunks" and determine if there is an edge within the cell.
        // if there is then mark the cell as solid.
        physics::collision_map map;

        std::vector<std::pair<double, double>> left;
        std::vector<std::pair<double, double>> right;

        for (auto y = 0; y < (sprite.size().height * height); y += y_step) {
            auto real_y = y + (sprite.point().y * height);

            // Work from the left side and find the left edge (if it exists)
            auto found_left_edge = false;
            for (auto x = 0; x < (sprite.size().width * width); x += x_step) {
                auto real_x = x + (sprite.point().x * width);
                auto alpha = m_backing_texture->color(real_x, real_y).get_alpha();

                if (alpha >= 10) {
                    left.emplace_back(x, y);
                    found_left_edge = true;
                    break;
                }
            }

            if (!found_left_edge) {
                continue;
            }

            for (auto x = 0; x < (sprite.size().width * width); x += x_step) {
                auto real_x = ((sprite.point().x + sprite.size().width) * width) - 1 - x;
                auto alpha = m_backing_texture->color(real_x, real_y).get_alpha();
                if (alpha >= 10) {
                    right.emplace_back(sprite_width - x - 1, y);
                    break;
                }
            }
        }

        for (const auto& p : left) {
            map.add_point(p.first, p.second);
        }
        for (auto it = right.rbegin(); it != right.rend(); ++it) {
            map.add_point(it->first, it->second);
        }

        // Store the collision map for this frame.
        sprite.set_collision_map(std::move(map));
    }
}