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

#include <cstdint>
#include <cstdlib>
#include <libRenderCore/buffer/vertex.hpp>
#include <libRenderCore/texture/id.hpp>
#include <libRenderCore/texture/store.hpp>
#include <libRenderCore/shader/program.hpp>

#if !defined(RENDERER_MAX_VERTEX_COUNT)
#   define RENDERER_MAX_VERTEX_COUNT    6'000'000
#endif

#if !defined(DEFAULT_MAX_TEXTURE_SLOTS)
#   define DEFAULT_MAX_TEXTURE_SLOTS    32
#endif

namespace renderer
{
    struct buffer
    {
        using texture_slot = std::uint32_t;

        buffer() = default;
        ~buffer();

        auto initialize(std::size_t vertex_count = RENDERER_MAX_VERTEX_COUNT, std::size_t texture_slots = DEFAULT_MAX_TEXTURE_SLOTS) -> void;

        [[nodiscard]] inline auto is_full() const -> bool { return m_vertices.count >= m_vertices.max; }
        [[nodiscard]] inline auto is_empty() const -> bool { return m_vertices.count == 0; }
        [[nodiscard]] inline auto count() const -> std::size_t { return m_vertices.count; }
        [[nodiscard]] inline auto locked() const -> bool { return m_locked; }

        [[nodiscard]] inline auto data() const -> void * { return reinterpret_cast<void *>(m_vertices.base); }
        [[nodiscard]] inline auto data_size() const -> std::size_t { return (m_vertices.count * sizeof(vertex)); }
        [[nodiscard]] inline auto max_textures() const -> std::size_t { return m_texture_slots.max_texture_slots; }
        [[nodiscard]] inline auto texture_count() const -> std::size_t { return m_texture_slots.device_ids.size(); }
        [[nodiscard]] inline auto shader_program() const -> shader::program { return m_shader; }

        auto lock() -> void;
        auto unlock() -> void;
        auto reset() -> void;

        inline auto current_vertex() -> vertex& { return *m_vertices.current; }
        auto commit_vertex() -> void;
        auto next_vertex() -> vertex&;

        [[nodiscard]] auto can_accept_texture(texture::id id) const -> bool;
        [[nodiscard]] auto texture_for_slot(buffer::texture_slot slot) const -> texture::id;
        [[nodiscard]] auto device_texture_for_slot(buffer::texture_slot slot) const -> texture::device_id;
        auto bind_texture(texture::id id, texture::device_id dev) -> buffer::texture_slot;

        auto set_shader_program(shader::program program) -> void;

    private:
        auto configure_vertex_buffer() -> void;

    private:
        bool m_locked { false };

        struct {
            vertex *base { nullptr };
            vertex *current { nullptr };
            std::size_t count { 0 };
            std::size_t max { 0 };
        } m_vertices;

        struct {
            std::size_t max_texture_slots { 0 };
            std::vector<texture::id> ids;
            std::vector<texture::device_id> device_ids;
        } m_texture_slots;

        shader::program m_shader;
    };
}