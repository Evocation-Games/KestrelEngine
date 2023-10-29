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

#include <libRenderCore/buffer/vertex.hpp>
#include <libOpenGLRenderer/driver/output.hpp>
#include <libMath/types/vec4.hpp>

// MARK: - Construction

auto renderer::opengl::output::initialize(GLint width, GLint height, const resource::shader::program& shader, bool vsync) -> void
{
    m_width = width;
    m_height = height;
    m_shader = &shader;
    m_projection = glm::ortho(0.0, (double)width, (double)height, 0.0, 1.0, -1.0);
    m_buffer.initialize(sizeof(vertex) * 6);

    auto px = 0.f;
    auto py = 0.f;
    auto sw = (float)width;
    auto sh = (float)height;

    // Setup some default buffers for drawing the frame texture.
    m_vertices[0].texture = 0.f;
    m_vertices[1].texture = 0.f;
    m_vertices[2].texture = 0.f;
    m_vertices[3].texture = 0.f;
    m_vertices[4].texture = 0.f;
    m_vertices[5].texture = 0.f;

    m_vertices[0].color = renderer::vec4(1, 1, 1, 1);
    m_vertices[1].color = renderer::vec4(1, 1, 1, 1);
    m_vertices[2].color = renderer::vec4(1, 1, 1, 1);
    m_vertices[3].color = renderer::vec4(1, 1, 1, 1);
    m_vertices[4].color = renderer::vec4(1, 1, 1, 1);
    m_vertices[5].color = renderer::vec4(1, 1, 1, 1);

    m_vertices[0].position = renderer::vec4(px, sh, 0, 0);
    m_vertices[1].position = renderer::vec4(sw, sh, 0, 0);
    m_vertices[2].position = renderer::vec4(sw, py, 0, 0);
    m_vertices[3].position = renderer::vec4(px, sh, 0, 0);
    m_vertices[4].position = renderer::vec4(px, py, 0, 0);
    m_vertices[5].position = renderer::vec4(sw, py, 0, 0);

    m_vertices[0].tex_coord = renderer::vec2(0.f, 1.f);
    m_vertices[1].tex_coord = renderer::vec2(1.f, 1.f);
    m_vertices[2].tex_coord = renderer::vec2(1.f, 0.f);
    m_vertices[3].tex_coord = renderer::vec2(0.f, 1.f);
    m_vertices[4].tex_coord = renderer::vec2(0.f, 0.f);
    m_vertices[5].tex_coord = renderer::vec2(1.f, 0.f);

    std::size_t buffer_size = sizeof(vertex) * 6;
    m_buffer.copy_data(0, &m_vertices[0], buffer_size);
}

// MARK: - Viewport

auto renderer::opengl::output::update_viewport_size(GLint width, GLint height) -> void
{

}

// MARK: - Frame

auto renderer::opengl::output::push_frame(GLint frame_texture, GLFWwindow *window) -> void
{
    GLuint shader = m_shader->uid();

    GLint textures[1] = { 0 };
    auto textures_location = glGetUniformLocation(shader, "u_textures");
    glUniform1iv(textures_location, 1, textures);

    auto texture_count = glGetUniformLocation(shader, "u_texture_count");
    glUniform1f(texture_count, 1);

    auto projection_location = glGetUniformLocation(shader, "u_projection");
    glUniformMatrix4fv(projection_location, 1, false, glm::value_ptr(m_projection));

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shader);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, frame_texture);

    // Draw
    m_buffer.bind(0, sizeof(vertex) * 6);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    m_buffer.unbind();

    glfwSwapBuffers(window);
    glFinish();
}
