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

#include <libECS/storage/storage.hpp>

// MARK: - Construction

ecs::storage::storage(std::uint32_t capacity)
    : m_reference_pool(capacity), m_entity(capacity)
{
    m_reference_pool.set_on_expand([&] (auto new_capacity) {
        m_entity.resize(new_capacity);
    });
}

// MARK: - Entity Creation

auto ecs::storage::create_entity() -> link
{
    link ref(m_reference_pool.acquire(), this);
    ref->reset();
    return ref;
}

// MARK: - Entity Destruction

auto ecs::storage::abandon(reference r) -> void
{
    // TODO: Determine if an explicit path is needed here?
}

// MARK: - Entity Access

auto ecs::storage::get(reference ref) -> link
{
    return { ref, this };
}

auto ecs::storage::operator[](const ecs::reference& ref) -> link
{
    return { ref, this };
}

auto ecs::storage::ephemeral_get(ecs::reference ref) -> entity&
{
    return m_entity[ref.value()];
}