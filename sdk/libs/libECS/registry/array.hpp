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
#include <unordered_map>
#include <libECS/entity/entity.hpp>
#include <libFoundation/profile/profiler.hpp>

namespace ecs::component
{
    struct any_array
    {
        virtual auto entity_destroyed(ecs::entity entity) -> void = 0;
    };

    template<typename T>
    struct array : any_array
    {
        explicit array(std::size_t capacity = 2'000'000) { expand(capacity); }

        auto insert(ecs::entity entity, T component) -> void
        {
            std::size_t idx = m_size++;
            m_entity_map[entity] = idx;
            m_entry_map[idx] = entity;
            m_entries[idx] = component;
        }

        auto remove(ecs::entity entity) -> void
        {
            std::size_t removed_idx = m_entity_map[entity];
            std::size_t last_idx = m_size - 1;
            m_entries[removed_idx] = m_entries[last_idx];

            auto last_entity = m_entry_map[last_idx];
            m_entity_map[last_entity] = removed_idx;
            m_entity_map[removed_idx] = last_entity;

            m_entity_map.erase(entity);
            m_entry_map.erase(last_idx);
            --m_size;
        }

        auto get(ecs::entity entity) -> T*
        {
            auto it = m_entity_map.find(entity);
            if (it == m_entity_map.end()) {
                return nullptr;
            }
            return &m_entries.at(it->second);
        }

        auto entity_destroyed(ecs::entity entity) -> void override
        {
            if (m_entity_map.find(entity) != m_entity_map.end()) {
                remove(entity);
            }
        }

    private:
        auto expand(std::size_t delta) -> void
        {
            auto new_capacity = m_entries.capacity() + delta;
            m_entries.resize(new_capacity);
        }

    private:
        std::size_t m_size { 0 };
        std::vector<T> m_entries;
        std::unordered_map<entity, std::size_t> m_entity_map;
        std::unordered_map<std::size_t, entity> m_entry_map;
    };
}
