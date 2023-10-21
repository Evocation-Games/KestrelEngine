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

TEST(iterate_onlyVisitsCreatedEntities)
{
    ecs::storage sut(10);

    auto a = sut.create_entity();
    auto b = sut.create_entity();
    auto c = sut.create_entity();

    a->position.value = { 1, 1 };
    b->position.value = { 2, 2 };
    c->position.value = { 3, 3 };

    std::uint32_t count = 0;
    for (auto& entity : sut) {
        count++;
        test::equal(entity.position.value.x(), count);
    }

    test::equal(count, 3);
}

TEST(iterate_skipDestroyedEntities)
{
    ecs::storage sut(10);
    ecs::reference a_ref;
    ecs::reference c_ref;
    ecs::reference d_ref;

    {
        auto a = sut.create_entity();
        auto b = sut.create_entity();
        auto c = sut.create_entity();
        auto d = sut.create_entity();

        a_ref = a.reference();
        c_ref = c.reference();
        d_ref = d.reference();

        b->position.value = { 10, 10 };
    }

    std::uint32_t count = 0;
    for (auto& entity : sut) {
        count++;
        test::not_equal(entity.position.value.x(), 10);
    }

    test::equal(count, 3);
}
