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

#include <memory>
#include <libECS/entity/manager.hpp>
#include <libECS/registry/registry.hpp>
#include <libECS/system/manager.hpp>
#include <libFoundation/profile/profiler.hpp>

#if !defined(ECS_MAX_ENTITY_COUNT)
#   define ECS_MAX_ENTITY_COUNT     1'000'000
#endif

namespace ecs
{
    class world
    {
    public:
        auto prepare(std::size_t capacity = ECS_MAX_ENTITY_COUNT) -> void
        {
            m_registry = std::make_unique<ecs::component::registry>();
            m_entities = std::make_unique<ecs::entity_manager<ECS_MAX_ENTITY_COUNT>>();
            m_systems = std::make_unique<ecs::system_manager>();
        }

        auto create_entity() -> ecs::entity
        {
            return m_entities->create_entity();
        }

        auto destroy_entity(ecs::entity entity) -> void
        {
            m_entities->destroy_entity(entity);
            m_registry->entity_destroyed(entity);
            m_systems->entity_destroyed(entity);
        }

        template<typename T>
        auto register_component() -> void
        {
            m_registry->register_component<T>();
        }

        template<typename T>
        auto add_component(ecs::entity entity, T component) -> void
        {
            m_registry->add_component<T>(entity, component);
            auto signature = m_entities->signature(entity);
            signature.set(m_registry->component_type<T>(), true);
            m_entities->set_signature(entity, signature);
            m_systems->entity_signature_changed(entity, signature);
        }

        template<typename T>
        auto remove_component(ecs::entity entity) -> void
        {
            m_registry->remove_component<T>(entity);
            auto signature = m_entities->signature(entity);
            signature.set(m_registry->component_type<T>(), false);
            m_entities->set_signature(entity, signature);
            m_systems->entity_signature_changed(entity, signature);
        }

        template<typename T>
        auto component(ecs::entity entity) -> T*
        {
            return m_registry->component<T>(entity);
        }

        template<typename T>
        [[nodiscard]] auto component(ecs::entity entity) const -> const T*
        {
            return m_registry->component<T>(entity);
        }

        template<typename T>
        auto component_type() -> ecs::component::type
        {
            return m_registry->component_type<T>();
        }

        template<typename T>
        auto register_system() -> std::shared_ptr<T>
        {
            return m_systems->register_system<T>();
        }

        template<typename T>
        auto set_system_signature(ecs::signature signature) -> void
        {
            m_systems->set_signature<T>(signature);
        }

    private:
        std::unique_ptr<ecs::component::registry> m_registry;
        std::unique_ptr<ecs::system_manager> m_systems;
        std::unique_ptr<ecs::entity_manager<ECS_MAX_ENTITY_COUNT>> m_entities;
    };
}