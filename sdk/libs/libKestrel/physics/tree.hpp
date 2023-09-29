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

#include <libKestrel/math/rect.hpp>
#include <libKestrel/memory/slab.hpp>
/*
namespace kestrel::physics
{
    template<typename ID, std::uint32_t D, std::uint32_t M>
    struct quad_tree
    {
        static constexpr std::uint64_t top_left_idx = 0;
        static constexpr std::uint64_t top_right_idx = 1;
        static constexpr std::uint64_t bottom_left_idx = 2;
        static constexpr std::uint64_t bottom_right_idx = 3;

        typedef std::int64_t index;

        struct element
        {
            ID id;
            index idx;
            math::rect bb;
        };

        struct quadrants
        {
            math::rect bounds;
            math::rect tl;
            math::rect tr;
            math::rect bl;
            math::rect br;
        };

        struct node
        {
            std::int32_t level { 0 };
            struct quadrants quadrants;
            index next[4] { 0 };
            index element[M] { 0 };
            std::int32_t element_count { 0 };
        };

        quad_tree() = default;

        explicit quad_tree(const math::rect& bounds)
        {
            auto r = m_nodes.request();
            auto& root = m_nodes.get(r);
            root.element = -1;
            root.quadrants = quadrants(bounds);
            root.level = 0;
        };

        quad_tree(quad_tree&&) noexcept = default;
        quad_tree(const quad_tree&) noexcept = default;

        auto clear() -> void
        {
            m_elements.purge();
            m_nodes.purge();
        }

        auto insert(ID object, const math::rect& r) -> void
        {
            auto idx = m_elements.request();
            auto& ref = m_elements.get(idx);
            ref.idx = idx;
            ref.id = object;
            ref.bb = r;

            auto& node = m_nodes.first();
            recursive_insert(ref, node, 0);
        }

    private:
        memory::slab<element, 50'000> m_elements;
        memory::slab<node, 50'000> m_nodes;
        math::rect m_bounds;

        [[nodiscard]] inline auto quadrants(const math::rect& r) const -> quadrants
        {
            auto s = r.size() / 2;
            auto tl = math::rect(r.origin(), s);
            return {
                .tl = tl,
                .tr = tl + math::point(s.width(), 0),
                .bl = tl + math::point(0, s.height()),
                .br = tl + math::point(s.width(), s.height())
            };
        }

        inline auto is_divided(const node& n) const -> bool
        {
            return (n.next[0] & n.next[1] & n.next[2] & n.next[3]) != 0;
        }

        inline auto divide(node& n) -> void
        {
            // Request the new nodes for the division
            n.next[0] = m_nodes.request();
            n.next[1] = m_nodes.request();
            n.next[2] = m_nodes.request();
            n.next[3] = m_nodes.request();

            // Configure the new nodes.
            m_nodes.get(n.next[0]).quadrants = quadrants(n.quadrants.tl);
            m_nodes.get(n.next[0]).level = n.level + 1;

            m_nodes.get(n.next[1]).quadrants = quadrants(n.quadrants.tr);
            m_nodes.get(n.next[1]).level = n.level + 1;

            m_nodes.get(n.next[2]).quadrants = quadrants(n.quadrants.bl);
            m_nodes.get(n.next[2]).level = n.level + 1;

            m_nodes.get(n.next[3]).quadrants = quadrants(n.quadrants.br);
            m_nodes.get(n.next[3]).level = n.level + 1;

            // Move elements in to their new nodes.
        }

        inline auto recursive_insert(element& element, node& parent, std::int32_t level) -> void
        {
            if (!parent.quadrants.bounds.contains_rect(element.bb)) {
                return;
            }

            if (!is_divided(parent)) {
                if (parent.element_count < M) {
                    parent.element[parent.element_count++] = element.idx;
                    return;
                }
                else {
                    divide(parent);
                }
            }

            if (parent.quadrants.tl.contains_rect(element.bb)) {
                recursive_insert(element, m_nodes.get(parent.next[0]), level + 1);
                return;
            }
            else if (parent.quadrants.tl.intersects(element.bb)) {
                recursive_insert(element, m_nodes.get(parent.next[0]), level + 1);
            }

            if (parent.quadrants.tr.contains_rect(element.bb)) {
                recursive_insert(element, m_nodes.get(parent.next[1]), level + 1);
                return;
            }
            else if (parent.quadrants.tr.intersects(element.bb)) {
                recursive_insert(element, m_nodes.get(parent.next[1]), level + 1);

            }

            if (parent.quadrants.bl.contains_rect(element.bb)) {
                recursive_insert(element, m_nodes.get(parent.next[2]), level + 1);
                return;
            }
            else if (parent.quadrants.bl.intersects(element.bb)) {
                recursive_insert(element, m_nodes.get(parent.next[2]), level + 1);
            }

            if (parent.quadrants.br.contains_rect(element.bb)) {
                recursive_insert(element, m_nodes.get(parent.next[3]), level + 1);
                return;
            }
            else if (parent.quadrants.br.intersects(element.bb)) {
                recursive_insert(element, m_nodes.get(parent.next[3]), level + 1);
            }
        }
    };

}
 //*/