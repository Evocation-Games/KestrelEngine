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

#include <typeinfo>
#include <typeindex>
#include <type_traits>
#include <cassert>
#include <unordered_map>
#include <memory>
#include <libECS/registry/type.hpp>
#include <libECS/registry/array.hpp>
#include <libECS/entity/entity.hpp>
#include <libFoundation/profile/profiler.hpp>

namespace ecs::component
{
    class registry
    {
    public:
        template<typename T>
        auto register_component() -> void
        {
            std::type_index t(typeid(T));
            m_type_vector.emplace_back(t);
            m_types.insert({ t, m_next_component_type++ });
            m_components.insert({ t, std::make_shared<array<T>>() });
        }

        template<typename T>
        [[nodiscard]] auto component_type() const -> type
        {
            auto it = m_types.find(std::type_index(typeid(T)));
            if (it == m_types.end()) {
                return type_not_found;
            }
            return it->second;
        }

        template<typename T>
        auto add_component(ecs::entity entity, T component) -> void
        {
            component_array<T>().insert(entity, component);
        }

        template<typename T>
        auto remove_component(ecs::entity entity) -> void
        {
            component_array<T>()->remove(entity);
        }

        template<typename T>
        auto component(ecs::entity entity) -> T*
        {
            return component_array<T>().get(entity);
        }

        auto entity_destroyed(ecs::entity entity) -> void
        {
            for (const auto& it : m_components) {
                it.second->entity_destroyed(entity);
            }
        }

    private:
        template<typename T>
        auto component_array() -> array<T>&
        {
            std::type_index t(typeid(T));
            return *std::static_pointer_cast<array<T>>(m_components[t]);
        }

    private:
        std::vector<std::type_index> m_type_vector;
        std::unordered_map<std::type_index, type> m_types;
        std::unordered_map<std::type_index, std::shared_ptr<any_array>> m_components;
        ecs::component::type m_next_component_type { 0 };
    };
}