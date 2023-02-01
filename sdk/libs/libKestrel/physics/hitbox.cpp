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

#include <stdexcept>
#include <libKestrel/physics/hitbox.hpp>
#include <libKestrel/physics/collisions.hpp>

// MARK: - Construction

kestrel::physics::hitbox::hitbox(const math::vec2 &origin, double radius)
    : m_type(type::circle), m_origin(origin), m_radius(radius)
{}

kestrel::physics::hitbox::hitbox(const math::rect &rect)
    : m_type(type::rect), m_origin(rect.origin()), m_size(rect.size())
{}

kestrel::physics::hitbox::hitbox(const math::triangulated_polygon &poly)
    : m_type(type::polygon), m_origin(poly.center()), m_polygon(poly)
{
    // Workout the effective radius of the polygon.
    float radius = 0;
    for (auto n = 0; n < poly.triangle_count(); ++n) {
        auto tri = poly.triangle_at(n);
        radius = std::max(radius, tri.a.magnitude());
        radius = std::max(radius, tri.b.magnitude());
        radius = std::max(radius, tri.c.magnitude());
    }
    m_radius = radius;
}

// MARK: - Accessors

auto kestrel::physics::hitbox::offset() const -> math::point
{
    return m_offset;
}

auto kestrel::physics::hitbox::is_valid() const -> bool
{
    return (m_type != type::none);
}

auto kestrel::physics::hitbox::lod() const -> enum lod
{
    return m_lod;
}

auto kestrel::physics::hitbox::type() const -> enum type
{
    return m_type;
}

auto kestrel::physics::hitbox::center() const -> math::point
{
    return m_origin.to_point();
}

auto kestrel::physics::hitbox::size() const -> math::size
{
    return m_size.to_size();
}

auto kestrel::physics::hitbox::polygon() const -> math::triangulated_polygon
{
    return m_polygon;
}

auto kestrel::physics::hitbox::set_lod(enum lod lod) -> void
{
    m_lod = lod;
}

auto kestrel::physics::hitbox::set_offset(const math::point &offset) -> void
{
    m_offset = offset;
}

auto kestrel::physics::hitbox::scale_factor() const -> math::size
{
    return m_scale;
}

auto kestrel::physics::hitbox::set_scale_factor(const math::size &size) -> void
{
    m_scale = size;
}

// MARK: - Collision Checking

auto kestrel::physics::hitbox::collision_test(const hitbox &hb) const -> bool
{
    // TODO: Handle the varying LODs...
    if (m_type == type::polygon && hb.m_type == type::polygon) {
        auto distance = m_offset.distance_to(hb.m_offset);
        if (distance < m_radius + hb.m_radius) {
            return collision_test(m_polygon * m_scale, m_offset, hb.m_polygon * hb.m_scale, hb.m_offset);
        }
        return false;
    }
    else if (m_type == type::none || hb.m_type == type::none) {
        return false;
    }
    else {
        throw std::runtime_error("Unhandled collision test scenario.");
    }
}

auto kestrel::physics::hitbox::collision_test(const math::triangulated_polygon &a, const math::point& offset_a, const math::triangulated_polygon &b, const math::point& offset_b) -> bool
{
    // TODO: This is a major bottleneck currently.
    for (auto i = 0; i < a.triangle_count(); ++i) {
        auto t1 = a.triangle_at(i) - a.center() + offset_a;
        for (auto j = 0; j < b.triangle_count(); ++j) {
            auto t2 = b.triangle_at(j) - b.center() + offset_b;
            if (collisions::test(t1, t2)) {
                return true;
            }
        }
    }
    return false;
}