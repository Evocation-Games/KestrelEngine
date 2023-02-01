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

#include <array>
#include <libKestrel/math/point.hpp>
#include <libKestrel/math/size.hpp>
#include <libKestrel/math/rect.hpp>
#include <libKestrel/graphics/types/color.hpp>
#include <libKestrel/graphics/texture/texture.hpp>
#include <libKestrel/graphics/renderer/common/camera.hpp>
#include <libKestrel/graphics/renderer/common/shader/program.hpp>
#include <libKestrel/graphics/renderer/common/vertex.hpp>
#include <libKestrel/graphics/renderer/common/blending.hpp>

namespace kestrel::renderer
{
    class draw_buffer
    {
    public:
        explicit draw_buffer(std::size_t vertex_count = 60'000, std::size_t texture_slots = 8);
        ~draw_buffer();

        auto clear() -> void;
        auto reset() -> void;

        [[nodiscard]] inline auto is_full() const -> bool { return m_count >= m_max; }
        [[nodiscard]] inline auto is_empty() const -> bool { return m_count == 0; }
        [[nodiscard]] inline auto count() const -> std::size_t { return m_count; }

        auto set_camera(const camera& camera) -> void { m_camera = camera; }
        [[nodiscard]] inline auto camera() const -> const camera& { return m_camera; }

        auto set_shader(const std::shared_ptr<shader::program>& shader) -> void { m_shader = shader; }
        [[nodiscard]] inline auto shader() const -> std::shared_ptr<shader::program> { return m_shader; }

        auto set_blend(enum renderer::blending mode) -> void { m_blend = mode; }
        [[nodiscard]] auto blend() const -> enum renderer::blending { return m_blend; };

        [[nodiscard]] auto can_accept_texture(const std::shared_ptr<graphics::texture>& texture) const -> bool;
        auto push_texture(const std::shared_ptr<graphics::texture>& texture) -> float;
        auto push_vertex(const math::vec2& v, const math::point& tex_coord, float alpha, float texture, const std::array<math::vec4, 8>& shader_info) -> void;
        auto push_vertex(const math::vec2 &v, const graphics::color& color, const std::array<math::vec4, 8>& shader_info) -> void;

        [[nodiscard]] inline auto data() const -> void * { return reinterpret_cast<void *>(m_vertices); }
        [[nodiscard]] inline auto data_size() const -> std::size_t { return (m_count * sizeof(vertex)); }

        [[nodiscard]] inline auto texture(std::uint8_t idx) const -> std::shared_ptr<graphics::texture> { return m_texture_slots[idx]; }
        [[nodiscard]] inline auto texture_slots() const -> std::size_t { return m_texture_count; }

    private:
        renderer::camera m_camera;
        std::shared_ptr<shader::program> m_shader;
        enum renderer::blending m_blend { renderer::blending::normal };

        std::shared_ptr<graphics::texture> *m_texture_slots { nullptr };
        std::shared_ptr<graphics::texture> *m_next_empty_texture_slot { nullptr };
        std::size_t m_max_textures { 0 };
        std::size_t m_texture_count { 0 };

        std::size_t m_max { 0 };
        std::size_t m_count { 0 };

        struct vertex *m_vertices { nullptr };
        struct vertex *m_vertex_ptr { nullptr };

    };
}