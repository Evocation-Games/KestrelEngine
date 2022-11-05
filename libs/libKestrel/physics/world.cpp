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

#include <libKestrel/physics/world.hpp>

// MARK: - Physics Bodies Management

auto kestrel::physics::world::create_physics_body() -> body::lua_reference
{
    auto body = body::lua_reference(new physics::body(this));
    m_bodies.emplace_back(body);
    return body;
}

auto kestrel::physics::world::add_physics_body(body::lua_reference ref) -> void
{
    m_bodies.emplace_back(ref);
}

auto kestrel::physics::world::destroy_physics_body(body *ref) -> void
{
    for (auto it = m_bodies.begin(); it != m_bodies.end(); ++it) {
        if (it->get() == ref) {
            m_bodies.erase(it);
            return;
        }
    }
}

auto kestrel::physics::world::get_physics_body(body *ref) -> body::lua_reference
{
    for (auto it = m_bodies.begin(); it != m_bodies.end(); ++it) {
        if (it->get() == ref) {
            return *it;
        }
    }
    return { nullptr };
}

// MARK: - Updates

auto kestrel::physics::world::update() -> void
{
    for (auto& body : m_bodies) {
        body->update();
    }
}