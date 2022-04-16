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

#include "renderer/common/draw_buffer.hpp"

// MARK: - Construction

renderer::draw_buffer::draw_buffer(std::size_t vertex_count, std::size_t texture_slots)
    : m_max(vertex_count), m_max_textures(texture_slots)
{
    m_vertices = new vertex[vertex_count];
    m_texture_slots = new std::shared_ptr<graphics::texture>[texture_slots];

    m_vertex_ptr = m_vertices;
    m_next_empty_texture_slot = m_texture_slots;
}

// MARK: - Destruction

renderer::draw_buffer::~draw_buffer()
{
    delete m_vertices;
    delete m_texture_slots;
}

// MARK: - Vertex Management

auto renderer::draw_buffer::clear() -> void
{
    m_vertex_ptr = m_vertices;
    m_next_empty_texture_slot = m_texture_slots;

    m_texture_count = 0;
    m_count = 0;
    m_blend = blending::normal;
}

auto renderer::draw_buffer::can_accept_texture(const std::shared_ptr<graphics::texture> &texture) const -> bool
{
    if (!m_texture_slots) {
        return false;
    }

    for (auto i = 0; i < m_texture_count; ++i) {
        if (m_texture_slots[i] == texture) {
            return true;
        }
    }

    return (m_texture_count < m_max_textures);
}

auto renderer::draw_buffer::push_texture(const std::shared_ptr<graphics::texture> &texture) -> float
{
    if (!m_texture_slots || !m_next_empty_texture_slot) {
        return -1;
    }

    for (auto i = 0; i < m_texture_count; ++i) {
        if (m_texture_slots[i] == texture) {
            return static_cast<float>(i);
        }
    }

    *m_next_empty_texture_slot = texture;
    m_next_empty_texture_slot++;

    auto slot = m_texture_count++;
    return static_cast<float>(slot);
}

auto renderer::draw_buffer::push_vertex(const math::point &point, const math::point &tex_coord, float alpha, float texture) -> void
{
    m_vertex_ptr->position.x = static_cast<float>(point.x);
    m_vertex_ptr->position.y = static_cast<float>(point.y);

    m_vertex_ptr->tex_coord.x = static_cast<float>(tex_coord.x);
    m_vertex_ptr->tex_coord.y = static_cast<float>(tex_coord.y);

    m_vertex_ptr->color.x = 1.f;
    m_vertex_ptr->color.y = 1.f;
    m_vertex_ptr->color.z = 1.f;
    m_vertex_ptr->color.w = alpha;

    m_vertex_ptr->texture = texture;

    m_vertex_ptr++;
    m_count++;
}