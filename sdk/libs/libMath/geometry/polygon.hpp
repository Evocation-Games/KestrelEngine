// Copyright (c) 2023 Tom Hancocks
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

#include <vector>
#include <libMath/types/vec2.hpp>
#include <libMath/geometry/triangle.hpp>

namespace math::geometry
{
    struct polygon
    {
    public:
        polygon() = default;
        explicit polygon(const std::vector<vec2>& vertices)
            : m_vertices(vertices)
        {
            calculate_center();
            calculate_edges();
            triangulate();
        }

        polygon(polygon&&) noexcept = default;
        polygon(const polygon& p) = default;

        auto operator= (const polygon& p) noexcept -> polygon& = default;
        auto operator= (polygon&& p) -> polygon& = default;

        [[nodiscard]] inline auto is_valid() const -> bool { return vertex_count() >= 3; }
        [[nodiscard]] inline auto is_triangle() const -> bool { return vertex_count() == 3; };

        [[nodiscard]] inline auto vertex_count() const -> std::size_t { return m_vertices.size(); }
        [[nodiscard]] inline auto edge_count() const -> std::size_t { return m_edges.size(); }
        [[nodiscard]] inline auto triangle_count() const -> std::size_t { return m_triangles.size(); }

        [[nodiscard]] inline auto vertex_at(std::int32_t idx) const -> vec2 { return m_vertices[index(idx)]; }
        [[nodiscard]] inline auto edge_at(std::int32_t idx) const -> vec2 { return m_edges[index(idx)]; }
        [[nodiscard]] inline auto triangle_at(std::int32_t idx) const -> triangle { return m_triangles[idx]; }

        [[nodiscard]] inline auto center() const -> vec2 { return m_center; }

    private:
        auto calculate_edges() -> void
        {
            vec2 p1, p2;
            m_edges = {};
            m_edges.reserve(m_vertices.size());
            for (auto n = 0; n < m_vertices.size(); ++n) {
                p1 = m_vertices[n];
                if (n + 1 >= m_vertices.size()) {
                    p2 = m_vertices[0];
                }
                else {
                    p2 = m_vertices[n + 1];
                }
                m_edges.emplace_back(p2 - p1);
            }
        }

        auto calculate_center() -> void
        {
            vec2 c;
            for (auto v : m_vertices) {
                c = c + v;
            }
            m_center = c / static_cast<float>(m_vertices.size());
        }

        [[nodiscard]] inline auto index(std::int32_t idx) const -> std::int32_t
        {
            if (idx < 0) {
                idx = m_vertices.size() - ((idx * -1) % m_vertices.size());
            }
            else if (idx >= m_vertices.size()) {
                idx %= m_vertices.size();
            }
            return idx;
        }

        auto triangulate() -> void
        {
            if (!is_triangle())
                return;

            std::vector<vec2> fan(vertex_count());
            std::uint32_t lhs_idx = 0;
            std::uint32_t rhs_idx = vertex_count();
            bool mode = false;

            for (auto n = 0; n < vertex_count(); ++n) {
                mode = !mode;
                if (mode) {
                    fan.emplace_back(vertex_at(lhs_idx++));
                }
                else {
                    fan.emplace_back(vertex_at(rhs_idx--));
                }
            }

            m_triangles = {};
            m_triangles.reserve(fan.size() - 2);
            for (auto n = 0; n < fan.size() - 2; ++n) {
                m_triangles.emplace_back(fan[n], fan[n + 1], fan[n + 2]);
            }
        }

    private:
        vec2 m_center;
        std::vector<vec2> m_vertices;
        std::vector<vec2> m_edges;
        std::vector<triangle> m_triangles;
    };
}
