// Copyright (c) 2022 Tom Hancocks
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

#include <libKestrel/physics/constructors/hitbox_constructor.hpp>

// MARK: - Polygon Constructors

auto kestrel::physics::hitbox_constructor::polygon(std::shared_ptr<graphics::sprite_sheet> sheet, const math::rect& sprite_frame, std::uint32_t accuracy) -> math::polygon
{
    // Find the backing texture and get basic information required for the processing.
    auto texture = sheet->texture();
    auto width = texture->size().width;
    auto height = texture->size().height;

    // Determine the accuracy so that we can step through the sprite.
    math::rect sprite(sprite_frame.get_x() * width, sprite_frame.get_y() * height, sprite_frame.get_width() * width, sprite_frame.get_height() * height);
    auto step_x = static_cast<std::int32_t>(std::max(1.0, sprite.get_width() / accuracy));
    auto step_y = static_cast<std::int32_t>(std::max(1.0, sprite.get_height() / accuracy));

    std::vector<math::vec2> lhs;
    std::vector<math::vec2> rhs;
    for (auto y = 0; y < static_cast<std::int32_t>(sprite.get_height()); y += step_y) {
        auto real_y = y + sprite.get_y();

        auto found_left_edge = false;
        for (auto x = 0; x < static_cast<std::int32_t>(sprite.get_width()); x += step_x) {
            auto real_x = x + sprite.get_x();
            auto alpha = texture->color(real_x, real_y).get_alpha();
            if (alpha >= alpha_threshold) {
                lhs.emplace_back(x, y);
                found_left_edge = true;
                break;
            }
        }

        if (!found_left_edge) {
            continue;
        }

        for (auto x = 0; x < static_cast<std::int32_t>(sprite.get_width()); ++x) {
            auto real_x = (sprite.get_x() + sprite.get_width() - 1) - x;
            auto alpha = texture->color(real_x, real_y).get_alpha();
            if (alpha >= alpha_threshold) {
                rhs.emplace_back(sprite.get_width() - x - 1, y);
                break;
            }
        }
    }

    lhs.insert(lhs.end(), rhs.rbegin(), rhs.rend());
    math::polygon poly(lhs);
    return std::move(poly);
}

auto kestrel::physics::hitbox_constructor::triangulated_polygon(std::shared_ptr<graphics::sprite_sheet> sheet, const math::rect& sprite_frame, std::uint32_t accuracy) -> math::triangulated_polygon
{
    auto poly = std::move(polygon(sheet, sprite_frame, accuracy));
    math::triangulated_polygon tri_poly(poly);
    return std::move(tri_poly);
}

// MARK: - HitBox Construction

auto kestrel::physics::hitbox_constructor::hitbox(std::shared_ptr<graphics::sprite_sheet> sheet, const math::rect &sprite_frame, std::uint32_t accuracy) -> physics::hitbox
{
    auto poly = std::move(triangulated_polygon(sheet, sprite_frame, accuracy));
    physics::hitbox hb(poly);
    hb.set_lod(physics::calculate_lod(accuracy));
    return std::move(hb);
}