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

#pragma once

#include <unordered_set>
#include <unordered_map>
#include <libKestrel/math/point.hpp>
#include <libKestrel/math/angle.hpp>
#include <libKestrel/math/angular_difference.hpp>
#include <libKestrel/lua/runtime/runtime.hpp>
#include <libKestrel/lua/scripting.hpp>
#include <libKestrel/lua/support/vector.hpp>
#include <libKestrel/physics/hitbox.hpp>

namespace kestrel::physics
{
    // TODO: This PhysicsBody is currently very rudimentary and needs to be expanded.

    struct world;

    struct lua_api(Physics.Body, Available_0_8) body
    {
    public:
        has_constructable_lua_api(body);

        typedef std::uint64_t identifier;

        explicit body(physics::world *world = nullptr, identifier id = 0);

        lua_getter(id, Available_0_8) [[nodiscard]] auto id() const -> std::uint64_t;
        auto force_id_change(identifier id) -> void;

        lua_getter(hasInertia, Available_0_8) [[nodiscard]] auto has_inertia() const -> bool;
        lua_setter(hasInertia, Available_0_8) auto set_inertia(bool f) -> void;

        lua_getter(heading, Available_0_8) [[nodiscard]] auto heading() const -> math::angle;
        lua_getter(counterHeading, Available_0_8) [[nodiscard]] auto counter_heading() const -> math::angle;
        lua_getter(onCounterHeading, Available_0_8) [[nodiscard]] auto on_counter_heading() const -> bool;

        lua_getter(position, Available_0_8) [[nodiscard]] auto position() const -> math::point;
        lua_setter(position, Available_0_8) auto set_position(const math::point& position) -> void;

        lua_getter(velocity, Available_0_8) [[nodiscard]] auto velocity() const -> math::point;
        lua_setter(velocity, Available_0_8) auto set_velocity(const math::point& velocity) -> void;

        lua_getter(speed, Available_0_8) [[nodiscard]] auto speed() const -> double;
        lua_getter(maximumSpeed, Available_0_8) [[nodiscard]] auto maximum_speed() const -> double;
        lua_setter(maximumSpeed, Available_0_8) auto set_maximum_speed(double speed) -> void;
        lua_function(reduceSpeed, Available_0_8) auto reduce_speed(double speed) -> void;

        auto set_hitbox(const physics::hitbox& hb) -> void;
        [[nodiscard]] auto hitbox() const -> const hitbox&;
        lua_getter(collisionType, Available_0_8) [[nodiscard]] auto collision_type() const -> std::uint32_t;
        lua_setter(collisionType, Available_0_8) auto set_collision_type(std::uint32_t type) -> void;
        [[nodiscard]] auto allows_collisions_of_type(std::uint32_t type) const -> bool;
        [[nodiscard]] auto allows_collisions_from_body(const lua_reference& ref) const -> bool;
        [[nodiscard]] auto allows_collisions_from_body(body *ref) const -> bool;
        lua_function(rejectCollisionsFromType, Available_0_8) auto reject_collisions_from_type(std::uint32_t type) -> void;
        lua_function(rejectCollisionsFromBody, Available_0_8) auto reject_collisions_from_body(const lua_reference& ref) -> void;
        lua_function(hasCollisionFromBody, Available_0_8) [[nodiscard]] auto has_collision_from_body(const lua_reference& ref) const -> bool;
        lua_getter(hasCollisions, Available_0_8) [[nodiscard]] auto has_collisions() const -> bool;
        lua_getter(allCollisions, Available_0_8) [[nodiscard]] auto all_collisions() const -> lua::vector<lua_reference>;

        lua_function(simulateForce, Available_0_9) auto simulate_force(const math::point& velocity, const math::point& force, bool ignore_maximum) -> math::point;
        lua_function(applyForce, Available_0_8) auto apply_force(const math::point& force, bool ignore_maximum) -> void;
        lua_function(forceVectorValue, Available_0_8) [[nodiscard]] auto force_value(double value) const -> math::point;
        lua_getter(inverseForce, Available_0_8) [[nodiscard]] auto inverse_force() const -> math::point;
        lua_getter(accelerationForce, Available_0_8) [[nodiscard]] auto force() const -> math::point;
        lua_getter(acceleration, Available_0_8) [[nodiscard]] auto acceleration() const -> double;
        lua_setter(acceleration, Available_0_8) auto set_acceleration(double accel) -> void;

        lua_getter(rotation, Available_0_8) [[nodiscard]] auto rotation() const -> math::angle;
        lua_setter(rotation, Available_0_8) auto set_rotation(const math::angle& rotation) -> void;
        lua_getter(rotationSpeed, Available_0_8) [[nodiscard]] auto rotation_speed() const -> math::angular_difference;
        lua_setter(rotationSpeed, Available_0_8) auto set_rotation_speed(const math::angular_difference& speed) -> void;
        lua_function(rotateClockwise, Available_0_8) auto rotate_clockwise() -> void;
        lua_function(rotateCounterClockwise, Available_0_8) auto rotate_counter_clockwise() -> void;

        lua_function(halt, Available_0_8) auto halt() -> void;

        lua_getter(info, Available_0_8) [[nodiscard]] auto info() const -> luabridge::LuaRef;
        lua_setter(info, Available_0_8) auto set_info(luabridge::LuaRef ref) -> void;

        auto update() -> void;

        auto destroy() -> void;
        auto migrate_to_world(physics::world *new_world) -> void;

        auto reset_collisions() -> void;
        auto add_detected_collision(lua_reference collided) -> void;

        [[nodiscard]] auto scaling_factor() const -> math::size;
        auto set_scaling_factor(const math::size& factor) -> void;

    private:
        identifier m_id { 0 };
        physics::world *m_world { nullptr };
        std::unordered_map<identifier, lua_reference> m_collisions;
        std::uint32_t m_collision_type { 0 };
        std::unordered_set<std::uint32_t> m_rejected_collision_types;
        std::unordered_set<identifier> m_rejected_collisions;
        physics::hitbox m_hitbox;
        luabridge::LuaRef m_info { nullptr };
        bool m_has_inertia { true };
        math::point m_position;
        math::point m_velocity;
        math::angle m_rotation;
        double m_current_speed { 0 };
        math::angular_difference m_rotation_speed;
        double m_maximum_speed { 0 };
        double m_acceleration { 0 };
    };
}