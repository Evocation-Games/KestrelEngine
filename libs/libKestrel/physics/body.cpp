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
#include <libKestrel/physics/body.hpp>
#include <libKestrel/math/angular_difference.hpp>

// MARK: - Construction

kestrel::physics::body::body(physics::world *world)
    : m_world(world)
{}

// MARK: - Inertia

auto kestrel::physics::body::has_inertia() const -> bool
{
    return m_has_inertia;
}

auto kestrel::physics::body::set_inertia(bool f) -> void
{
    m_has_inertia = f;
}

// MARK: - Current Heading

auto kestrel::physics::body::heading() const -> math::angle
{
    return m_velocity.angle();
}

auto kestrel::physics::body::counter_heading() const -> math::angle
{
    return m_velocity.angle().opposite();
}

auto kestrel::physics::body::on_counter_heading() const -> bool
{
    return m_rotation.is_opposing(m_velocity.angle(), m_rotation_speed);
}

// MARK: - Positioning

auto kestrel::physics::body::position() const -> math::point
{
    return m_position;
}

auto kestrel::physics::body::set_position(const math::point& position) -> void
{
    m_position = position;
}

// MARK: - Velocity

auto kestrel::physics::body::velocity() const -> math::point
{
    return m_velocity;
}

auto kestrel::physics::body::set_velocity(const math::point& velocity) -> void
{
    m_velocity = velocity;
}

// MARK: - Speed

auto kestrel::physics::body::speed() const -> double
{
    return m_current_speed;
}

auto kestrel::physics::body::maximum_speed() const -> double
{
    return m_maximum_speed;
}

auto kestrel::physics::body::set_maximum_speed(double speed) -> void
{
    m_maximum_speed = speed;
}

auto kestrel::physics::body::reduce_speed(double speed) -> void
{
    if (m_has_inertia) {
        auto velocity = m_velocity.angle().vector(m_current_speed - speed);
        if (velocity.magnitude() <= 0) {
            velocity = math::point(0);
        }
        m_velocity = velocity;
        m_current_speed = m_velocity.magnitude();
    }
    else {
        m_current_speed = std::max(m_current_speed - speed, 0.0);
        m_velocity = m_rotation.vector(m_current_speed);
    }
}

// MARK: - Collisions

auto kestrel::physics::body::is_solid() const -> bool
{
    return m_solid;
}

auto kestrel::physics::body::collision_mask() const -> std::uint32_t
{
    return m_collision_mask;
}

auto kestrel::physics::body::set_collision_mask(std::uint32_t mask) -> void
{
    m_solid = (mask != 0);
    m_collision_mask = mask;

    if (m_solid) {

    }
}

// MARK: - Forces

auto kestrel::physics::body::apply_force(const math::point& force, bool ignore_maximum) -> void
{
    if (m_has_inertia) {
        auto velocity = m_velocity + force;
        if (!ignore_maximum && (velocity.magnitude() > m_maximum_speed)) {
            velocity = velocity.angle().vector(m_maximum_speed);
        }
        m_velocity = velocity;
        m_current_speed = m_velocity.magnitude();
    }
    else if (!ignore_maximum) {
        m_current_speed = std::min(m_current_speed + force.magnitude(), m_maximum_speed);
        m_velocity = m_rotation.vector(m_current_speed);
    }
    else {
        m_current_speed += force.magnitude();
        m_velocity = m_rotation.vector(m_current_speed);
    }
}

auto kestrel::physics::body::inverse_force() const -> math::point
{
     return m_rotation.opposite().vector(m_acceleration);
}

auto kestrel::physics::body::force() const -> math::point
{
    return m_rotation.vector(m_acceleration);
}

auto kestrel::physics::body::force_value(double value) const -> math::point
{
    return m_rotation.vector(value);
}

// MARK: - Accelerationm

auto kestrel::physics::body::acceleration() const -> double
{
    return m_acceleration;
}

auto kestrel::physics::body::set_acceleration(double accel) -> void
{
    m_acceleration = accel;
}

// MARK: - Rotation

auto kestrel::physics::body::rotation() const -> math::angle
{
    return m_rotation;
}

auto kestrel::physics::body::set_rotation(const math::angle &rotation) -> void
{
    m_rotation = rotation;
}

auto kestrel::physics::body::rotation_speed() const -> math::angular_difference
{
    return m_rotation_speed;
}

auto kestrel::physics::body::set_rotation_speed(const math::angular_difference& speed) -> void
{
    m_rotation_speed = speed;
}

auto kestrel::physics::body::rotate_clockwise() -> void
{
    m_rotation = m_rotation + math::angular_difference(m_rotation_speed);
    if (!m_has_inertia) {
        m_velocity = m_rotation.vector(m_current_speed);
    }
}

auto kestrel::physics::body::rotate_counter_clockwise() -> void
{
    m_rotation = m_rotation - math::angular_difference(m_rotation_speed);
    if (!m_has_inertia) {
        m_velocity = m_rotation.vector(m_current_speed);
    }
}

// MARK: - Halting

auto kestrel::physics::body::halt() -> void
{
    m_current_speed = 0;
    m_velocity = math::point(0);
}

// MARK: - Updating / Tick

auto kestrel::physics::body::update() -> void
{
    m_position = m_position + m_velocity;
}

// MARK: - Destruction

auto kestrel::physics::body::destroy() -> void
{
    if (m_world) {
        m_world->destroy_physics_body(this);
    }
}

// MARK: - Migration

auto kestrel::physics::body::migrate_to_world(physics::world *new_world) -> void
{
    if (m_world) {
        auto ref = m_world->get_physics_body(this);
        m_world->destroy_physics_body(this);
        m_world = new_world;
        m_world->add_physics_body(ref);
    }
}