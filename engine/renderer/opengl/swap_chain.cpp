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

#include "renderer/opengl/swap_chain.hpp"
#include "renderer/common/draw_buffer.hpp"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

// MARK: - Construction

renderer::opengl::swap_chain::swap_chain(GLFWwindow *window, const GLint *textures, uint32_t slot_count, uint32_t vao, uint32_t vbo)
    : renderer::swap_chain(),
      m_window(window),
      m_slot_count(slot_count),
      m_vao(vao),
      m_vbo(vbo),
      m_textures(textures)
{

}

// MARK: - Destruction

renderer::opengl::swap_chain::~swap_chain()
{

}

// MARK: - Frame Life Cycle

auto renderer::opengl::swap_chain::start() -> void
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

auto renderer::opengl::swap_chain::draw(const draw_buffer *buffer) -> void
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

auto renderer::opengl::swap_chain::finalize(const std::function<auto() -> void> &callback) -> void
{
    glfwSwapBuffers(m_window);
    callback();
}

// MARK: - ImGUI

auto renderer::opengl::swap_chain::start_imgui() -> void
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

auto renderer::opengl::swap_chain::finalize_imgui() -> void
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// MARK: - Projection

auto renderer::opengl::swap_chain::set_projection(const glm::mat4 &projection) -> void
{
    m_projection = projection;
}