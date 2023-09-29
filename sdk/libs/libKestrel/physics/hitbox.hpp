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

#pragma once

#include <libKestrel/physics/lod.hpp>
#include <libKestrel/math/vec2.hpp>
#include <libKestrel/math/rect.hpp>
#include <libKestrel/math/polygon.hpp>
#include <libKestrel/math/triangulated_polygon.hpp>

namespace kestrel::physics
{
    struct hitbox
    {
        enum class type { none, rect, circle, polygon };

        hitbox() = default;
        hitbox(const math::vec2& origin, double radius);
        explicit hitbox(const math::rect& rect);
        explicit hitbox(const math::triangulated_polygon& poly);
        hitbox(hitbox&&) = default;
        hitbox(const hitbox&) noexcept = default;

        auto operator=(hitbox&&) -> hitbox& = default;
        auto operator=(const hitbox&) noexcept -> hitbox& = default;

        [[nodiscard]] auto is_valid() const -> bool;

        [[nodiscard]] auto offset() const -> math::point;
        [[nodiscard]] auto lod() const -> enum lod;
        [[nodiscard]] auto type() const -> enum type;
        [[nodiscard]] auto center() const -> math::point;
        [[nodiscard]] auto size() const -> math::size;
        [[nodiscard]] auto polygon() const -> math::triangulated_polygon;

        auto set_lod(enum lod lod) -> void;
        auto set_offset(const math::point& offset) -> void;

        [[nodiscard]] auto scale_factor() const -> math::size;
        auto set_scale_factor(const math::size& size) -> void;

        [[nodiscard]] auto collision_test(const hitbox& hb) const -> bool;

    private:
        [[nodiscard]] static auto collision_test(const math::triangulated_polygon& a, const math::point& offset_a, const math::triangulated_polygon& b, const math::point& offset_b) -> bool;

    private:
        enum lod m_lod { medium };
        enum type m_type { none };
        double m_radius { 0 };
        math::point m_offset;
        math::vec2 m_origin;
        math::vec2 m_size;
        math::size m_scale { 1 };
        math::triangulated_polygon m_polygon;

    };
}