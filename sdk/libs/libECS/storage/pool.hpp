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
#include <functional>
#include <algorithm>
#include <unordered_set>
#include <libECS/storage/reference.hpp>

#if !defined(ECS_DEFAULT_POOL_CAPACITY)
#   define ECS_DEFAULT_POOL_CAPACITY 1000000    // 1 Million
#endif

namespace ecs
{
    struct pool
    {
        explicit pool(std::uint32_t capacity = ECS_DEFAULT_POOL_CAPACITY)
        {
            expand_pool(capacity);
        }

        [[nodiscard]] inline auto capacity() const -> std::size_t { return m_capacity; }

        [[nodiscard]] inline auto assigned(std::uint32_t value) const -> bool { return m_assigned.contains(value); }

        [[nodiscard]] inline auto acquire() -> reference
        {
            if (m_values.empty()) {
                expand_pool(std::max(capacity() >> 1, 1UL));
            }

            // Acquire the first item in the pool, and remove it from the vector.
            auto value = m_values.front();
            m_values.erase(m_values.begin());
            m_assigned.insert(value);

            // Produce a reference for it, and return it to the caller.
            return reference(value, [&, value] {
                release_reference(value);
            });
        }

        auto set_on_expand(std::function<auto(std::uint32_t)->void> callback) -> void
        {
            m_on_expand = callback;
        }

    private:
        auto expand_pool(std::uint32_t amount) -> void
        {
            auto old_capacity = m_capacity;
            m_capacity += amount;
            m_values.resize(amount);
            for (auto n = 0; n < amount; ++n) {
                m_values[n] = old_capacity + n;
            }
            m_on_expand(m_capacity);
        }

        inline auto release_reference(std::uint32_t value) -> void
        {
            m_values.insert(m_values.begin(), value);
            m_assigned.erase(value);
        }

    private:
        std::uint32_t m_capacity { 0 };
        std::vector<std::uint32_t> m_values;
        std::unordered_set<std::uint32_t> m_assigned;
        std::function<auto(std::uint32_t)->void> m_on_expand { [] (auto) {} };
    };
}