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

// MARK: - Construction

kestrel::physics::hitbox::hitbox(const math::vec2 &origin, double radius)
    : m_type(type::circle), m_origin(origin), m_radius(radius)
{}

kestrel::physics::hitbox::hitbox(const math::rect &rect)
    : m_type(type::rect), m_origin(rect.get_origin()), m_size(rect.get_size())
{}

kestrel::physics::hitbox::hitbox(const math::triangulated_polygon &poly)
    : m_type(type::polygon), m_origin(poly.center()), m_polygon(poly)
{}

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

// MARK: - Collision Checking

auto kestrel::physics::hitbox::collision_test(const hitbox &hb) const -> bool
{
    // TODO: Handle the varying LODs...
    if (m_type == type::polygon && hb.m_type == type::polygon) {
        return collision_test(m_polygon, m_offset, hb.m_polygon, hb.m_offset);
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
    for (auto i = 0; i < a.triangle_count(); ++i) {
        auto t1 = a.triangle_at(i) + offset_a;
        for (auto j = 0; j < b.triangle_count(); ++j) {
            auto t2 = b.triangle_at(j) + offset_b;
            if (t1.check_collision(t2)) {
                return true;
            }
        }
    }
    return false;
}