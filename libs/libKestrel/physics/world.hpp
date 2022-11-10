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

#include <vector>
#include <libKestrel/physics/body.hpp>
#include <libKestrel/physics/quad_tree.hpp>

namespace kestrel::physics
{
    class world
    {
    public:
        world();

        [[nodiscard]] auto create_physics_body() -> body::lua_reference;
        [[nodiscard]] auto get_physics_body(body *ref) -> body::lua_reference;
        auto add_physics_body(body::lua_reference ref) -> void;
        auto destroy_physics_body(body *ref) -> void;

        auto update() -> void;

    private:
        physics::quad_tree<body *, 10, 10> m_collision_tree;
        std::vector<body::lua_reference> m_bodies;
    };
}