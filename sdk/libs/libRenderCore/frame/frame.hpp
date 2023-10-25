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

#pragma once

#include <array>
#include <thread>
#include <functional>
#include <libMath/types/vec2.hpp>
#include <libMath/types/vec3.hpp>
#include <libMath/types/vec4.hpp>
#include <libMath/geometry/rect.hpp>
#include <libRenderCore/buffer/buffer.hpp>
#include <libRenderCore/submission/submission.hpp>
#include <libRenderCore/components/drawable.hpp>
#include <libRenderCore/components/texturing.hpp>

namespace renderer
{
    class driver;

    struct frame
    {
    public:
        explicit frame(const driver *driver);

        auto initialize() -> void;

        auto begin() -> frame&;
        auto finalize(std::function<auto(std::function<auto()->void>)->void> callback) -> void;

        auto draw(ecs::entity entity, const ecs::world *world) -> void;

    private:
        struct quad
        {
            quad() = default;
            std::array<vertex*, 6> vertices {};
        };

        static auto get_quad(buffer& buffer) -> quad;
        static auto layout_quad(quad& q, const component::drawable *drawable) -> void;
        static auto color_quad(quad& q, const component::drawable *drawable) -> void;
        static auto texture_quad(quad& q, buffer::texture_slot slot, const component::texturing *texturing) -> void;

    private:
        auto reset() -> void;

        auto current_buffer() -> buffer&;
        auto submit_buffer() -> buffer&;

    private:
        enum state { READY, PREPARING, SUBMITTED, WORKING, TERMINATED };

        struct {
            const renderer::driver *driver;
            math::geometry::rect bounds;
        } m_renderer;

        struct {
            state state { READY };
            std::function<auto(std::function<auto()->void>)->void> completion { [] (auto cb) {} };
        } m_task;

        struct {
            std::array<buffer, 5> all;
            std::size_t current { 0 };
        } m_buffers;
    };
}