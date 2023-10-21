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

#include <libKestrel/math/polygon.hpp>

// MARK: - Construction

kestrel::math::polygon::polygon(const std::vector<point>& vertices)
{
    for (const auto& p : vertices) {
        m_vertices.emplace_back(p);
    }
    calculate_center();
    calculate_edges();
}

kestrel::math::polygon::polygon(std::vector<vec2>& vertices)
    : m_vertices(vertices)
{
    calculate_center();
    calculate_edges();
}

// MARK: - Accessors

auto kestrel::math::polygon::is_valid() const -> bool
{
    return m_vertices.size() >= 3;
}

auto kestrel::math::polygon::is_triangle() const -> bool
{
    return m_vertices.size() == 3;
}

auto kestrel::math::polygon::is_convex() const -> bool
{
    return false;
}

auto kestrel::math::polygon::vertex_count() const -> std::size_t
{
    return m_vertices.size();
}

auto kestrel::math::polygon::edge_count() const -> std::size_t
{
    return m_edges.size();
}

auto kestrel::math::polygon::center() const -> vec2
{
    return m_center_point;
}

// MARK: - Vertex / Edge Management

auto kestrel::math::polygon::vertex_at(std::int32_t idx) const -> vec2
{
    return m_vertices.at(corrected_index(idx));
}

auto kestrel::math::polygon::edge_at(std::int32_t idx) const -> vec2
{
    return m_edges.at(corrected_index(idx));
}

auto kestrel::math::polygon::calculate_edges() -> void
{
    vec2 p1, p2;
    m_edges.clear();
    for (auto n = 0; n < m_vertices.size(); ++n) {
        p1 = m_vertices.at(n);
        if (n + 1 >= m_vertices.size()) {
            p2 = m_vertices.at(0);
        }
        else {
            p2 = m_vertices.at(n + 1);
        }
        m_edges.emplace_back(p2 - p1);
    }
}

auto kestrel::math::polygon::calculate_center() -> void
{
    vec2 c;
    for (const auto& v : m_vertices) {
        c = c + v;
    }
    m_center_point = c / static_cast<float>(m_vertices.size());
}

auto kestrel::math::polygon::corrected_index(std::int32_t idx) const -> std::int32_t
{
    if (idx < 0) {
        idx = static_cast<std::int32_t>(m_vertices.size()) - ((idx * -1) % m_vertices.size());
    }
    else if (idx >= m_vertices.size()) {
        idx %= static_cast<std::int32_t>(m_vertices.size());
    }
    return idx;
}

