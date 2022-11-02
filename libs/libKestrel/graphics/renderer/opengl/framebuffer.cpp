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

#include <memory>
#include <libKestrel/graphics/renderer/opengl/framebuffer.hpp>
#include <libKestrel/graphics/renderer/opengl/constants.hpp>
#include <libKestrel/graphics/renderer/common/vertex.hpp>
#include <libKestrel/graphics/renderer/common/draw_buffer.hpp>
#include <libKestrel/graphics/renderer/common/renderer.hpp>
#include <libKestrel/graphics/renderer/opengl/texture.hpp>

// MARK: - Construction

kestrel::renderer::opengl::framebuffer::framebuffer(std::uint32_t width, std::uint32_t height, const GLint *textures, std::uint32_t slot_count, std::uint32_t vao, std::uint32_t vbo)
    : renderer::framebuffer(width, height),
      m_width(width),
      m_height(height),
      m_textures(textures),
      m_slot_count(slot_count),
      m_vao(vao),
      m_vbo(vbo)
{
    // Acquire a new framebuffer for OpenGL.
    invalidate();
}

// MARK: - Destruction

kestrel::renderer::opengl::framebuffer::~framebuffer()
{
    glDeleteFramebuffers(1, &m_id);
}

// MARK: - Setup and Invalidation

auto kestrel::renderer::opengl::framebuffer::invalidate() -> void
{
    glGenFramebuffers(1, &m_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);

    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

    GLenum draw_buffers[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, draw_buffers);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("OpenGL Framebuffer Error");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

// MARK: - Frame Lifecycle

auto kestrel::renderer::opengl::framebuffer::start() -> void
{
    glGetIntegerv(GL_VIEWPORT, m_viewport);

    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    glViewport(0, 0, static_cast<GLsizei>(m_width), static_cast<GLsizei>(m_height));
    m_projection = glm::ortho(0.0, static_cast<double>(m_width), 0.0, static_cast<double>(m_height), 1.0, -1.0);

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

auto kestrel::renderer::opengl::framebuffer::finalize(const std::function<auto() -> void> &callback) -> void
{
    glBindBuffer(GL_FRAMEBUFFER, 0);
    glViewport(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3]);
    callback();
}

auto kestrel::renderer::opengl::framebuffer::draw(const draw_buffer *buffer) -> void
{
    auto shader = buffer->shader();
    auto shader_id = shader->get<GLuint>();

    glUseProgram(shader_id);

    switch (buffer->blend()) {
        case blending::normal:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
        case blending::light:
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;
    }

    auto textures_location = glGetUniformLocation(shader_id, "u_textures");
    glUniform1iv(textures_location, m_slot_count, m_textures);

    auto projection_location = glGetUniformLocation(shader_id, "u_projection");
    glUniformMatrix4fv(projection_location, 1, false, glm::value_ptr(m_projection));

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizei>(buffer->data_size()), buffer->data());

    auto slots = buffer->texture_slots();
    for (auto i = 0; i < slots; ++i) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, buffer->texture(i).get()->handle());
    }

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, buffer->count());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// MARK: - Texture

auto kestrel::renderer::opengl::framebuffer::texture() -> std::shared_ptr<graphics::texture>
{
    if (!m_texture_ref && m_texture > 0) {
        auto tex = std::make_shared<opengl::texture>(m_texture, math::size(m_width, m_height));
        m_texture_ref = std::static_pointer_cast<graphics::texture>(tex);
    }
    return m_texture_ref;
}
