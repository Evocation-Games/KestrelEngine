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

#include <libECS/storage/pool.hpp>
#include <libTesting/testing.hpp>

TEST(acquire_returnsReferencesWithExpectedValues)
{
    // Setup a test pool with a capacity of 10.
    ecs::pool sut(10);

    auto a = sut.acquire();
    auto b = sut.acquire();
    auto c = sut.acquire();

    test::equal(a.value(), 0);
    test::equal(b.value(), 1);
    test::equal(c.value(), 2);
}

TEST(destroyReference_releasesAcquiredValueBackToPool)
{
    ecs::pool sut(10);

    // Create a new scope to get RAII behaviour.
    {
        auto a = sut.acquire();
        test::equal(a.value(), 0);
    }

    // 'a' should have been released to the pool, and as such should be
    // the first available again.
    auto b = sut.acquire();
    test::equal(b.value(), 0);
}

TEST(acquire_expandsPoolWhenDepleted)
{
    ecs::pool sut(2);

    // Setup an expansion callback to listen to when the pool expands.
    bool expectation = false;
    sut.set_on_expand([&] (auto new_capacity) {
        expectation = true;
        test::equal(new_capacity, 3);
    });

    // Acquire from the pool 3 times, and validate each value.
    auto a = sut.acquire();
    auto b = sut.acquire();
    auto c = sut.acquire();

    test::equal(a.value(), 0);
    test::equal(b.value(), 1);
    test::equal(c.value(), 2);

    // Finally, validate that the expansion callback was executed.
    test::is_true(expectation);
}

TEST(capacity_reportsExpectedValue)
{
    ecs::pool sut(2);

    test::equal(sut.capacity(), 2);

    // Acquire from the pool 3 times
    auto a = sut.acquire();
    auto b = sut.acquire();
    auto c = sut.acquire();

    test::equal(sut.capacity(), 3);
}