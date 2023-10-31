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

#include <libRenderCore/buffer/buffer.hpp>

// MARK: - Construction

auto renderer::buffer::initialize(std::size_t vertex_count, std::size_t texture_slots) -> void
{
    m_vertices.max = vertex_count;
    m_texture_slots.max_texture_slots = texture_slots;
    configure_vertex_buffer();
}

// MARK: - Destruction

renderer::buffer::~buffer()
{
    delete m_vertices.base;
}

// MARK: - Configuration

auto renderer::buffer::configure_vertex_buffer() -> void
{
    m_vertices.base = new vertex[m_vertices.max];
    m_vertices.current = m_vertices.base;
}

// MARK: - Management

auto renderer::buffer::lock() -> void
{
    m_locked = true;
}

auto renderer::buffer::unlock() -> void
{
    m_locked = false;
}

auto renderer::buffer::reset() -> void
{
    m_vertices.current = m_vertices.base;
    m_vertices.count = 0;
    m_texture_slots.ids.clear();
    m_texture_slots.device_ids.clear();
    unlock();
}

auto renderer::buffer::commit_vertex() -> void
{
    m_vertices.current++;
    m_vertices.count++;
}

auto renderer::buffer::next_vertex() -> vertex&
{
    m_vertices.count++;
    return *m_vertices.current++;
}

// MARK: - Texture Slots

auto renderer::buffer::can_accept_texture(texture::id id) const -> bool
{
    if (m_texture_slots.ids.size() >= m_texture_slots.max_texture_slots) {
        return false;
    }
    return true;
}

auto renderer::buffer::can_accept_texture(const component::texturing *texture) const -> bool
{
    if (texture && (m_texture_slots.ids.size() >= m_texture_slots.max_texture_slots)) {
        return false;
    }
    return true;
}

auto renderer::buffer::bind_texture(texture::id id, texture::device_id dev) -> buffer::texture_slot
{
    auto it = std::find(m_texture_slots.ids.begin(), m_texture_slots.ids.end(), id);
    if (it == m_texture_slots.ids.end()) {
        m_texture_slots.ids.emplace_back(id);
        m_texture_slots.device_ids.emplace_back(dev);
        return m_texture_slots.ids.size() - 1;
    }
    else {
        auto idx = it - m_texture_slots.ids.begin();
        return idx;
    }
}

auto renderer::buffer::texture_for_slot(buffer::texture_slot slot) const -> texture::id
{
    return m_texture_slots.ids[slot];
}

auto renderer::buffer::device_texture_for_slot(buffer::texture_slot slot) const -> texture::device_id
{
    return m_texture_slots.device_ids[slot];
}

// MARK: - Shaders

auto renderer::buffer::set_shader_program(shader::program program) -> void
{
    m_shader = program;
}