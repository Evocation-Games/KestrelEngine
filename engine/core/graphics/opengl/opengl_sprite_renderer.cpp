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

graphics::opengl::sprite_renderer::sprite_renderer(std::shared_ptr<opengl::shader> shader, const double& scale)
    : m_shader(std::move(shader)), m_vao(0), m_vbo(0), m_scale(scale)
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

auto graphics::opengl::sprite_renderer::draw(const graphics::entity::lua_reference& entity) const -> void
{
    auto texture = entity->texture();
    if (!texture) {
        return;
    }

    m_shader->use();

    auto size = (entity->render_size * m_scale);
    if (entity->has_clipping_area()) {
        size = entity->clipping_area() * m_scale;
    }

    auto position = (entity->position * m_scale);

    auto model = glm::mat4(1.0);
    model = glm::translate(model, glm::vec3(glm::vec2(position.x, position.y), 0.0));
    model = glm::scale(model, glm::vec3(glm::vec2(size.width, size.height), 1.0));

    auto sprite = entity->spritesheet()->at(entity->sprite_index);

    m_shader->set_mat4("model", model);

    if (entity->has_clipping_area()) {
        m_shader->set_vec2("texSize", static_cast<GLfloat>(entity->clipping_area_uv().width), static_cast<GLfloat>(entity->clipping_area_uv().height));
        m_shader->set_vec2("texOffset",
                           static_cast<GLfloat>(sprite.point().x + entity->clipping_offset_uv().x),
                           static_cast<GLfloat>(sprite.point().y + entity->clipping_offset_uv().y));
    }
    else {
        m_shader->set_vec2("texSize", static_cast<GLfloat>(sprite.size().width), static_cast<GLfloat>(sprite.size().height));
        m_shader->set_vec2("texOffset", static_cast<GLfloat>(sprite.point().x), static_cast<GLfloat>(sprite.point().y));
    }

    m_shader->set_vec4("spriteColor", 1.0, 1.0, 1.0, static_cast<GLfloat>(entity->get_alpha()));

    glActiveTexture(GL_TEXTURE0);
    texture->bind();

    glEnable(GL_BLEND);
    switch (entity->blend()) {
        case entity::normal: {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
        }
        case entity::light: {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);
            break;
        }
    }

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
