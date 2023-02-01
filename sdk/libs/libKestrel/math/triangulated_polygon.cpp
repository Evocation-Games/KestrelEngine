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

#include <libKestrel/math/SIMD/float4.hpp>
#include <libKestrel/math/triangulated_polygon.hpp>

// MARK: - Construction

kestrel::math::triangulated_polygon::triangulated_polygon(const polygon &poly)
    : m_center(poly.center())
{
    triangulate_polygon(poly);
}

kestrel::math::triangulated_polygon::triangulated_polygon(const std::vector<triangle> &triangles, const math::vec2 &center)
    : m_triangles(triangles), m_center(center)
{}

// MARK: - Accessors

auto kestrel::math::triangulated_polygon::is_valid() const -> bool
{
    return !m_triangles.empty();
}

auto kestrel::math::triangulated_polygon::triangle_count() const -> std::size_t
{
    return m_triangles.size();
}

auto kestrel::math::triangulated_polygon::triangle_at(std::uint32_t idx) const -> triangle
{
    return m_triangles.at(idx);
}

auto kestrel::math::triangulated_polygon::center() const -> vec2
{
    return m_center;
}

// MARK: - Operators

auto kestrel::math::triangulated_polygon::operator*(const math::size &s) const -> triangulated_polygon
{
    auto triangles = m_triangles;
    vec2 center;
    for (auto& it : triangles) {
        auto v = SIMD::float4(it.a.x(), it.a.y(), it.b.x(), it.b.y()) * s.m_value;
        auto w = SIMD::float4(it.c.x(), it.c.y(), m_center.x(), m_center.y()) * s.m_value;
        it.a = vec2(v[0], v[1]);
        it.b = vec2(v[2], v[3]);
        it.c = vec2(w[0], w[1]);
        center = vec2(w[2], w[3]);
    }
    return triangulated_polygon(triangles, center);
}

// MARK: - Triangulation

auto kestrel::math::triangulated_polygon::triangulate_polygon(const polygon &poly) -> void
{
    if (poly.vertex_count() < 3) {
        return;
    }

    // 1. Transform the vertices from the polygon into a list that can be easily triangulated.
    std::vector<vec2> fan;
    std::uint32_t lhs_index = 0;
    std::uint32_t rhs_index = poly.vertex_count() - 1;
    bool mode = false;
    for (auto n = 0; n < poly.vertex_count(); ++n) {
        mode = !mode;
        if (mode) {
            fan.emplace_back(poly.vertex_at(lhs_index++));
        }
        else {
            fan.emplace_back(poly.vertex_at(rhs_index--));
        }
    }

    // 2. Begin constructing each of the triangles.
    for (auto n = 0; n < fan.size() - 2; ++n) {
        triangle tri(fan[n], fan[n + 1], fan[n + 2]);
        m_triangles.emplace_back(tri);
    }
}
