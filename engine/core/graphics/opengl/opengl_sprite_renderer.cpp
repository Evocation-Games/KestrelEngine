// Copyright (c) 2020 Tom Hancocks
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

#include "core/graphics/opengl/opengl_sprite_renderer.hpp"

#include <utility>

// MARK: - Construction

graphics::opengl::sprite_renderer::sprite_renderer(std::shared_ptr<opengl::shader> shader)
    : m_shader(std::move(shader)), m_vao(0), m_vbo(0)
{
    m_shader->use();

    // Setup vertex information and arrays
    float vertices[] = {
        // pos      // tex
        0.0, 1.0,   0.0, 1.0,
        1.0, 0.0,   1.0, 0.0,
        0.0, 0.0,   0.0, 0.0,

        0.0, 1.0,   0.0, 1.0,
        1.0, 1.0,   1.0, 1.0,
        1.0, 0.0,   1.0, 0.0
    };

    glGenVertexArrays(1, &m_vao);

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(m_vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, false, 4 * sizeof(float), nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// MARK: - Drawing

auto graphics::opengl::sprite_renderer::draw(const std::shared_ptr<graphics::entity>& entity) const -> void
{

}
