// Copyright (c) 2022 Tom Hancocks
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

#include <libTesting/testing.hpp>
#include <libKestrel/physics/collisions.hpp>
#include <libKestrel/math/triangle.hpp>

using namespace kestrel;

// MARK: - Triangle Triangle Collisions

TEST(triangle_triangle_hasCollision_whenOverlapping)
{
    math::triangle a({ 0, 0 }, { 5, 0 }, { 2.5, 5 });
    math::triangle b({ 0, 4 }, { 2.5, -1 }, { 5, 4 });

    test::measure([a, b] {
        test::is_true(physics::collisions::test(a, b));
    });
}

TEST(triangle_triangle_hasCollision_whenNotOverlapping)
{
    math::triangle a({ 10, 10 }, { 10, 40 }, { 40, 10 });
    math::triangle b({ 100, 0 }, { 130, 0 }, { 100, 30 });

    test::measure([a, b] {
        test::is_false(physics::collisions::test(a, b));
    });
}