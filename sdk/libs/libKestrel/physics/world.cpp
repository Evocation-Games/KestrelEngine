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

// MARK: - Destruction

kestrel::physics::world::~world()
{
    m_destroyed =  true;
}

// MARK: - Physics Bodies Management

auto kestrel::physics::world::create_physics_body() -> body::lua_reference
{
    auto id = m_bodies.request();
    auto& body = m_bodies[id];
    body.ref = { new physics::body(shared_from_this(), id) };
    return body.ref;
}

auto kestrel::physics::world::add_physics_body(const body::lua_reference& ref) -> void
{
    if (ref.get() && ref->collision_type() > 0) {
        auto id = m_bodies.request();
        m_bodies[id].ref = ref;
        ref->force_id_change(id);
    }
}

auto kestrel::physics::world::destroy_physics_body(body *ref) -> void
{
    if (ref == nullptr || !m_bodies.available()) {
        return;
    }

    auto& body = m_bodies[ref->id()];
    if (body.ref.use_count() > 0 && body.ref.get() == ref) {
        // This is actually the correct reference, and not chance.
        body.ref = { nullptr };
        m_bodies.release(ref->id());
    }
}

auto kestrel::physics::world::get_physics_body(body *ref) -> body::lua_reference
{
    if (ref != nullptr) {
        auto& body = m_bodies[ref->id()];
        if (body.ref.get() == ref) {
            // This is actually the correct reference, and not chance.
            return body.ref;
        }
    }
    return { nullptr };
}

auto kestrel::physics::world::purge_all_bodies() -> void
{
    if (m_bodies.allocated() > 0) {
        for (auto ref = m_bodies.begin(); ref <= m_bodies.end(); ++ref) {
            auto body = m_bodies.get_allocated(ref);
            if (body.ref.get()) {
                body.ref->migrate_to_world({});
            }
            m_bodies.release(ref);
        }
    }
}

// MARK: - Updates

auto kestrel::physics::world::update() -> void
{
    if (m_bodies.allocated() == 0) {
        return;
    }

    // Iterate through all bodies, and add them to a quad tree in order to test collisions.
    m_collision_tree.clear();
    for (std::uint64_t n = 0; n < m_bodies.allocated(); ++n) {
        auto& body = m_bodies.get_allocated(n);
        if (body.ref.get() == nullptr || body.ref->collision_type() == 0) {
            continue;
        }

        body.ref->update();
        if (body.ref->hitbox().is_valid()) {
            math::rect bounds(body.ref->position(), body.ref->hitbox().size());
            m_collision_tree.insert(bounds, body.ref->id());
        }
    }

    // Work through the tree and determine collisions.
    for (std::uint64_t n = 0; n < m_bodies.allocated(); ++n) {
        auto& body = m_bodies.get_allocated(n);
        if (body.ref.get() == nullptr || body.ref->collision_type() == 0 || !body.ref->hitbox().is_valid()) {
            continue;
        }


        body.ref->reset_collisions();

        math::rect bounds(body.ref->position(), body.ref->hitbox().size());
        auto objects = m_collision_tree.retrieve(bounds);
        for (const auto& collision_candidate : objects) {
            auto candidate_body = m_bodies[collision_candidate.second];
            if ((body.ref.get() != candidate_body.ref.get()) && body.ref->hitbox().collision_test(candidate_body.ref->hitbox())) {
                body.ref->add_detected_collision(candidate_body.ref);
            }
        }
    }
}