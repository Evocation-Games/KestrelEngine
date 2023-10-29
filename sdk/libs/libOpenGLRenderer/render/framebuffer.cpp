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

#include <libOpenGLRenderer/render/framebuffer.hpp>
#include <libFoundation/profile/profiler.hpp>
#include <libOpenGLRenderer/render/vertex_buffer.hpp>

// MARK: - Construction

renderer::opengl::framebuffer::framebuffer(std::uint32_t width, std::uint32_t height)
{
    KESTREL_PROFILE_FUNCTION();

    // Viewport
    m_viewport.projection = glm::ortho(0.0, (double)width, (double)height, 0.0, 1.0, -1.0);
    m_viewport.width = width;
    m_viewport.height = height;

    // Setup the framebuffer
    prepare();
}

// MARK: - Framebuffer Preparation

auto renderer::opengl::framebuffer::prepare() -> void
{
    glGenFramebuffers(1, &m_buffer.id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_buffer.id);

    glGenTextures(1, &m_output.texture);
    glBindTexture(GL_TEXTURE_2D, m_output.texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)m_viewport.width, (GLsizei)m_viewport.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_output.texture, 0);

    GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, draw_buffers);

    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        GLenum error = glGetError();
        throw std::runtime_error("Error creating framebuffer: " + std::to_string(error));
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// MARK: - Lifecycle

auto renderer::opengl::framebuffer::render(const renderer::opengl::render_operation &job) -> void
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_buffer.id);
    glViewport(0, 0, (GLsizei)m_viewport.width, (GLsizei)m_viewport.height);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (const auto& command : job.commands()) {
        glUseProgram((GLuint)command.shader->uid());

        auto textures_location = glGetUniformLocation((GLuint)command.shader->uid(), "u_textures");
        auto projection_location = glGetUniformLocation((GLuint)command.shader->uid(), "u_projection");
        auto texture_count = glGetUniformLocation((GLuint)command.shader->uid(), "u_texture_count");
        glUniform1f(texture_count, (float)command.texture_count);
        glUniform1iv(textures_location, (GLsizei)command.texture_count, (GLint *)&command.textures[0]);
        glUniformMatrix4fv(projection_location, 1, false, glm::value_ptr(m_viewport.projection));

        for (auto i = 0; i < command.texture_count; ++i) {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, command.textures[i]);
        }

        command.vertex_buffer->bind(command.offset, command.data_size);
        glDrawArrays(GL_TRIANGLES, 0, (GLsizei)command.vertex_count);
        command.vertex_buffer->unbind();
    }

    glFinish();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}