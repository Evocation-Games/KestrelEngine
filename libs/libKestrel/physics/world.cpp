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

#include <libKestrel/kestrel.hpp>
#include <libKestrel/physics/world.hpp>
#include <libKestrel/physics/quad_tree.hpp>

// MARK: - Construction

kestrel::physics::world::world()
    : m_collision_tree(0, math::rect(math::point(0), kestrel::session().size()))
{}

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
    if (m_bodies.empty()) {
        return;
    }

    // Iterate through all bodies, and add them to a quad tree in order to test collisions.
    m_collision_tree.clear();
    for (auto& body : m_bodies) {
        body->update();

        if (body->hitbox().is_valid()) {
            math::rect bounds(body->position(), body->hitbox().size());
            m_collision_tree.insert(bounds, const_cast<physics::body *>(body.get()));
        }
    }

    // Work through the tree and determine collisions.
    for (auto n = 0; n < m_bodies.size(); ++n) {
        auto body = m_bodies[n];
        if (!body->hitbox().is_valid()) {
            continue;
        }

        body->reset_collisions();

        math::rect bounds(body->position(), body->hitbox().size());
        auto objects = m_collision_tree.retrieve(bounds);
        for (const auto& collision_candidate : objects) {
            auto candidate_body = collision_candidate.second;
            if ((body.get() != candidate_body) && body->hitbox().collision_test(candidate_body->hitbox())) {
                body->add_detected_collision(candidate_body);
            }
        }
    }
}