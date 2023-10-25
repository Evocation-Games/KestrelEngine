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
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <libECS/system/signature.hpp>
#include <libECS/system/system.hpp>

namespace ecs
{
    class system_manager
    {
    public:
        template<typename T>
        auto register_system() -> std::shared_ptr<T>
        {
            auto system = std::make_shared<T>();
            m_systems.insert({ std::type_index(typeid(T)), system });
            return system;
        }

        template<typename T>
        auto set_signature(ecs::signature signature) -> void
        {
            m_signatures.insert({ std::type_index(typeid(T)), signature });
        }

        auto entity_destroyed(ecs::entity entity) -> void
        {
            for (const auto& it : m_systems) {
                it.second->entities.erase(entity);
            }
        }

        auto entity_signature_changed(ecs::entity entity, ecs::signature signature) -> void
        {
            for (const auto& it : m_systems) {
                const auto& type = it.first;
                const auto& system = it.second;
                const auto& system_signature = m_signatures[type];
                if ((signature & system_signature) == system_signature) {
                    system->entities.insert(entity);
                }
                else {
                    system->entities.erase(entity);
                }
            }
        }

    private:
        std::unordered_map<std::type_index, signature> m_signatures;
        std::unordered_map<std::type_index, std::shared_ptr<system>> m_systems;
    };
}