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

#include <libKestrel/physics/collision_map.hpp>

// MARK: - Cell Management

auto kestrel::physics::collision_map::add_point(double x, double y) -> void
{
    struct point p;
    p.x = x;
    p.y = y;
    m_points.emplace_back(p);
}

auto kestrel::physics::collision_map::empty() const -> bool
{
    return m_points.empty();
}

auto kestrel::physics::collision_map::size() const -> std::size_t
{
    return m_points.size();
}

auto kestrel::physics::collision_map::at(std::size_t idx) const -> point
{
    return m_points.at(idx);
}

// MARK: - Collision Testing

auto kestrel::physics::collision_map::test(double x, double y) const -> bool
{
    return false;
}