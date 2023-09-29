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

#include <cstdint>
#include <type_traits>
#include <array>
#include <libKestrel/math/rect.hpp>

namespace kestrel::physics
{
    template<typename T, std::uint32_t M, std::uint32_t L>
    struct quad_tree
    {
        quad_tree() = default;
        quad_tree(quad_tree&&) noexcept = default;
        quad_tree(const quad_tree&) noexcept = default;

        quad_tree(std::uint32_t level, const math::rect& bounds)
            : m_level(level), m_bounds(bounds)
        {}

        ~quad_tree()
        {
            clear();
        }

        auto clear() -> void
        {
            m_objects.clear();

            for (auto n = 0; n < m_nodes.size(); ++n) {
                if (m_nodes[n]) {
                    m_nodes[n]->clear();
                    delete m_nodes[n];
                    m_nodes[n] = nullptr;
                }
            }
        }

        auto insert(const math::rect& rect, T object) -> void
        {
            if (m_nodes[0]) {
                auto idx = index(rect);
                if (idx != -1) {
                    m_nodes[idx]->insert(rect, object);
                    return;
                }
            }

            m_objects.template emplace_back(std::make_pair(rect, object));

            if (m_objects.size() > M && m_level < L) {
                if (!m_nodes[0]) {
                    split();
                }

                std::uint32_t i = 0;
                while (i < m_objects.size()) {
                    auto idx = index(m_objects[i].first);
                    if (idx != -1) {
                        m_nodes[idx]->insert(m_objects[i].first, m_objects[i].second);
                        m_objects.erase(m_objects.begin() + i);
                    }
                    else {
                        i++;
                    }
                }
            }
        }

        auto retrieve(const math::rect& rect) const -> std::vector<std::pair<math::rect, T>>
        {
            std::vector<std::pair<math::rect, T>> result;
            auto idx = index(rect);
            if (idx != -1 && m_nodes[0]) {
                auto node_result = m_nodes[idx]->retrieve(rect);
                result.insert(result.end(), node_result.begin(), node_result.end());
            }
            result.insert(result.end(), m_objects.begin(), m_objects.end());
            return result;
        }

    private:
        auto split() -> void
        {
            auto size = m_bounds.size() / 2;
            auto origin = m_bounds.origin();

            m_nodes[0] = new quad_tree<T, M, L>(m_level + 1, math::rect(origin, size));
            m_nodes[1] = new quad_tree<T, M, L>(m_level + 1, math::rect(origin + math::point(size.width(), 0), size));
            m_nodes[2] = new quad_tree<T, M, L>(m_level + 1, math::rect(origin + math::point(0, size.height()), size));
            m_nodes[3] = new quad_tree<T, M, L>(m_level + 1, math::rect(origin + math::point(size.width(), size.height()), size));
        }

        [[nodiscard]] auto index(const math::rect& rect) const -> std::int64_t
        {
            std::int64_t idx = -1;
            auto half_size = m_bounds.size() / 2;
            auto midpoint = m_bounds.origin() + math::point(half_size.width(), half_size.height());

            auto top_quadrant = (rect.y() < midpoint.y() && rect.max_y() < midpoint.y());
            auto bottom_quadrant = (rect.y() > midpoint.y());

            if (rect.x() < midpoint.x() && rect.max_x() < midpoint.x()) {
                if (top_quadrant) {
                    idx = 0;
                }
                else if (bottom_quadrant) {
                    idx = 2;
                }
            }
            else if (rect.x() > midpoint.x()) {
                if (top_quadrant) {
                    idx = 1;
                }
                else if (bottom_quadrant) {
                    idx = 3;
                }
            }

            return idx;
        }

    private:
        std::uint32_t m_level { 0 };
        std::vector<std::pair<math::rect, T>> m_objects;
        math::rect m_bounds;
        std::array<quad_tree<T, M, L> *, 4> m_nodes { nullptr };
    };
}