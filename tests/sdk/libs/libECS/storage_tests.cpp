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
#include <libTesting/testing.hpp>

TEST(createEntity_returnsValidStorageLink)
{
    ecs::storage sut(5);

    auto entity = sut.create_entity();
    test::equal(entity.reference().value(), 0);
}

TEST(getEntity_returnsValidStorageLink)
{
    ecs::storage sut(5);

    // Create an entity in a subscope, and capture the reference for it.
    // Edit a value of the entity, and then destroy the entity link.
    ecs::reference ref;
    {
        auto entity = sut.create_entity();
        ref = entity.reference();
        entity->position.value = { 2, 3 };
    }

    // Request a new link to the entity from the ECS, using the reference,
    // and check that it has the correct value.
    auto new_entity_link = sut.get(ref);
    test::equal(new_entity_link->position.value.x(), 2);
    test::equal(new_entity_link->position.value.y(), 3);
}

TEST(subscript_returnsValidStorageLink)
{
    ecs::storage sut(5);

    // Create an entity in a subscope, and capture the reference for it.
    // Edit a value of the entity, and then destroy the entity link.
    ecs::reference ref;
    {
        auto entity = sut.create_entity();
        ref = entity.reference();
        entity->position.value = { 4, 7 };
    }

    // Request a new link to the entity from the ECS, using the reference,
    // and check that it has the correct value.
    auto new_entity_link = sut[ref];
    test::equal(new_entity_link->position.value.x(), 4);
    test::equal(new_entity_link->position.value.y(), 7);
}

TEST(ephemeralGet_returnsValidEntityReference)
{
    ecs::storage sut(5);

    // Create an entity in a subscope, and capture the reference for it.
    // Edit a value of the entity, and then destroy the entity link.
    ecs::reference ref;
    {
        auto entity = sut.create_entity();
        ref = entity.reference();
        entity->position.value = { 1, 4 };
    }

    // Request a C++ reference to the entity using the provided reference.
    auto entity = sut.ephemeral_get(ref);
    test::equal(entity.position.value.x(), 1);
    test::equal(entity.position.value.y(), 4);
}

TEST(reacquiringEntity_correctlyResetsPreviousValues)
{
    ecs::storage sut(5);

    // Create an entity in a subscope and edit a value of the entity,
    // and then destroy the entity link, which should also destroy the entity.
    {
        auto entity = sut.create_entity();
        test::equal(entity.reference().value(), 0);
        entity->position.value = { 10, 4 };
    }

    // Repeat the process, but ensure the position is 0.
    {
        auto entity = sut.create_entity();
        test::equal(entity.reference().value(), 0);
        test::equal(entity->position.value.x(), 0);
        test::equal(entity->position.value.y(), 0);
    }
}
