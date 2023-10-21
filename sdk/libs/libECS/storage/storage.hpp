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

#include <iterator>
#include <libECS/storage/pool.hpp>
#include <libECS/entity/entity.hpp>

#if !defined(ECS_DEFAULT_STORAGE_CAPACITY)
#   define ECS_DEFAULT_STORAGE_CAPACITY 1000000    // 1 Million
#endif

namespace ecs
{
    class storage
    {
    public:
        struct link;

    public:
        explicit storage(std::uint32_t capacity = ECS_DEFAULT_STORAGE_CAPACITY);

        [[nodiscard]] auto create_entity() -> link;
        [[nodiscard]] auto get(reference) -> link;
        auto abandon(reference) -> void;

        auto operator[] (const reference&) -> link;

        [[nodiscard]] auto ephemeral_get(reference ref) -> entity&;

    public:
#pragma region Entity Link
        struct link
        {
            link() = default;
            link(const reference& ref, storage *owner) : m_reference(ref), m_owner(owner) {};

            [[nodiscard]] auto reference() const -> reference { return m_reference; }

            [[nodiscard]] auto operator->() const -> entity *
            {
                if (!m_owner) return nullptr;
                return &(m_owner->ephemeral_get(m_reference));
            }

            auto destroy() -> void
            {
                if (m_owner) m_owner->abandon(m_reference);
            }

        private:
            ecs::reference m_reference;
            storage *m_owner { nullptr };
        };
#pragma endregion

    public:
#pragma region Iterator
    struct iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = entity;
        using pointer = entity*;
        using reference = entity&;

        explicit iterator(pointer ptr, std::uint32_t value, pool *pool) : m_ptr(ptr), m_pool(pool), m_value(value) {}

        [[nodiscard]] auto operator*() const -> reference { return *m_ptr; }
        auto operator->() -> pointer { return m_ptr; }

        auto operator++() -> iterator&
        {
            do {
                m_ptr++;
                m_value++;
                if (m_pool->assigned(m_value)) break;
            }
            while (m_value < m_pool->capacity());
            return *this;
        }

        auto operator++(int) -> iterator { iterator tmp = *this; ++(*this); return tmp; }

        friend auto operator== (const iterator& a, const iterator& b) -> bool { return a.m_value == b.m_value; }
        friend auto operator!= (const iterator& a, const iterator& b) -> bool { return a.m_value != b.m_value; }

    private:
        std::uint32_t m_value { 0 };
        pointer m_ptr;
        pool *m_pool;
    };

        auto begin() -> iterator { return iterator(&m_entity[0], 0, &m_reference_pool); }
        auto end() -> iterator { return iterator(nullptr, m_reference_pool.capacity(), &m_reference_pool); }
#pragma endregion

    private:
        pool m_reference_pool;
        std::vector<entity> m_entity;
    };
}