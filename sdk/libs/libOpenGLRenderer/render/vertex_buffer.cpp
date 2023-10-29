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

#include <thread>
#include <libOpenGLRenderer/render/vertex_buffer.hpp>
#include <libRenderCore/buffer/vertex.hpp>

// MARK: - Construction

auto renderer::opengl::vertex_buffer::initialize(std::size_t size) -> void
{
    glGenVertexArrays(1, &m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)size, nullptr, GL_DYNAMIC_DRAW);

    glBindVertexArray(m_vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (const void *)offsetof(vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (const void *)offsetof(vertex, color));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (const void *)offsetof(vertex, tex_coord));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(vertex), (const void *)offsetof(vertex, texture));

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    m_buffer = new std::uint8_t[size];

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(10us);
}

// MARK: - Destruction

renderer::opengl::vertex_buffer::~vertex_buffer()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
    delete m_buffer;
}

// MARK: - Binding

auto renderer::opengl::vertex_buffer::bind(std::size_t offset, std::size_t size) const -> void
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, (GLsizei)offset, (GLsizei)size, m_buffer);
    glBindVertexArray(m_vao);
}

auto renderer::opengl::vertex_buffer::unbind() const -> void
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// MARK: - Data Management

auto renderer::opengl::vertex_buffer::copy_data(std::size_t offset, const void *data, std::size_t size) -> std::uint8_t *
{
    auto ptr = m_buffer + offset;
    ::memcpy(ptr, data, size);
    return ptr;
}