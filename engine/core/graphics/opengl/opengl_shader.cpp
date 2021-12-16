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

#if __x86_64__

#include <iostream>
#include "core/graphics/opengl/opengl_shader.hpp"
#include <libGraphite/rsrc/manager.hpp>
#include <libGraphite/data/reader.hpp>

// MARK: - Construction

graphics::opengl::shader::shader(const int64_t &vertex_id, const int64_t &fragment_id)
    : graphics::shader("glsl", vertex_id, fragment_id)
{
    GLuint vertex = 0;
    GLuint fragment = 0;
    GLuint shader = 0;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    auto vertex_source = m_vertex_code.c_str();
    glShaderSource(vertex, 1, &vertex_source, nullptr);
    glCompileShader(vertex);
    check(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    auto fragment_source = m_fragment_code.c_str();
    glShaderSource(fragment, 1, &fragment_source, nullptr);
    glCompileShader(fragment);
    check(fragment, "FRAGMENT");

    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);
    glLinkProgram(m_id);
    check(m_id, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

// MARK: - Shader Compilation

auto graphics::opengl::shader::check(GLuint object, const std::string &type) -> void
{
    GLint success = 0;
    char info_log[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(object, 1024, nullptr, info_log);
            std::cerr << "SHADER ERROR: [" << type << "] " << info_log << std::endl;
        }
    }
    else {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(object, 1024, nullptr, info_log);
            std::cerr << "LINKER ERROR: [" << type << "] " << info_log << std::endl;
        }
    }
}

// MARK: - Shader Interaction

auto graphics::opengl::shader::use() -> std::shared_ptr<graphics::shader>
{
    glUseProgram(m_id);
    return graphics::shader::use();
}

auto graphics::opengl::shader::set_float(const std::string& name, float value, bool use_shader) -> void
{
    if (use_shader) {
        use();
    }
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
}

auto graphics::opengl::shader::set_integer(const std::string& name, float value, bool use_shader) -> void
{
    if (use_shader) {
        use();
    }
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
}

auto graphics::opengl::shader::set_vec2(const std::string& name, float x, float y, bool use_shader) -> void
{
    if (use_shader) {
        use();
    }
    glUniform2f(glGetUniformLocation(m_id, name.c_str()), x, y);
}

auto graphics::opengl::shader::set_vec2(const std::string& name, glm::vec2 &value, bool use_shader) -> void
{
    if (use_shader) {
        use();
    }
    glUniform2f(glGetUniformLocation(m_id, name.c_str()), value.x, value.y);
}

auto graphics::opengl::shader::set_vec3(const std::string& name, float x, float y, float z, bool use_shader) -> void
{
    if (use_shader) {
        use();
    }
    glUniform3f(glGetUniformLocation(m_id, name.c_str()), x, y, z);
}

auto graphics::opengl::shader::set_vec3(const std::string& name, glm::vec3 &value, bool use_shader) -> void
{
    if (use_shader) {
        use();
    }
    glUniform3f(glGetUniformLocation(m_id, name.c_str()), value.x, value.y, value.z);
}

auto graphics::opengl::shader::set_vec4(const std::string& name, float x, float y, float z, float w, bool use_shader) -> void
{
    if (use_shader) {
        use();
    }
    glUniform4f(glGetUniformLocation(m_id, name.c_str()), x, y, z, w);
}

auto graphics::opengl::shader::set_vec4(const std::string& name, glm::vec4 &value, bool use_shader) -> void
{
    if (use_shader) {
        use();
    }
    glUniform4f(glGetUniformLocation(m_id, name.c_str()), value.x, value.y, value.z, value.w);
}

auto graphics::opengl::shader::set_mat4(const std::string& name, glm::mat4 &matrix, bool use_shader) -> void
{
    if (use_shader) {
        use();
    }
    glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, false, glm::value_ptr(matrix));
}

#endif