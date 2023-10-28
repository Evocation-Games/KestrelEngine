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

#include <iostream>
#include <stdexcept>
#include <libOpenGLRenderer/resource/shader/compiler.hpp>
#include <libOpenGLRenderer/resource/shader/default_shader.hpp>
#include <libOpenGLRenderer/opengl.hpp>
#include <libRenderCore/shader/function.hpp>

// MARK: - Compilation

auto renderer::opengl::resource::shader::compiler::compile(const renderer::shader::library& library) -> shader::program
{
    auto vertex_shader = compile_function(library.vertex_function());
    auto fragment_shader = compile_function(library.fragment_function());
    auto program_id = link_program(vertex_shader, fragment_shader);

    shader::program program(program_id, library.name());
    return std::move(program);
}

auto renderer::opengl::resource::shader::compiler::compile_function(const renderer::shader::function &function) -> std::uint32_t
{
    GLint function_type = 0;
    std::string shader_template;
    std::string token;

    switch (function.type()) {
        case renderer::shader::function::VERTEX: {
            function_type = GL_VERTEX_SHADER;
            shader_template = vertex_shader_template;
            token = "@@VERTEX_FUNCTION@@";
            break;
        }
        case renderer::shader::function::FRAGMENT: {
            function_type = GL_FRAGMENT_SHADER;
            shader_template = fragment_shader_template;
            token = "@@FRAGMENT_FUNCTION@@";
            break;
        }
        default: {
            // TODO: Throw an error here?
            return 0;
        }
    }

    auto function_it = shader_template.find(token);
    if (function_it != std::string::npos) {
        shader_template.replace(function_it, token.length(), function.code());
    }

    auto version = (const char *)glGetString(GL_VERSION);

    auto source = shader_template.c_str();
    std::cout << source << std::endl;
    GLuint shader = glCreateShader(function_type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        GLint log_length;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_length);
        if (log_length > 0) {
            std::string log(log_length, ' ');
            GLsizei written;
            glGetShaderInfoLog(shader, 1024, &written, &log[0]);
            throw std::runtime_error("Shader Error: " + log);
        }
        throw std::runtime_error("Unknown shader compilation error!");
    }

    return shader;
}

auto renderer::opengl::resource::shader::compiler::link_program(
    std::uint32_t vertex,
    std::uint32_t fragment
) -> std::uint32_t {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    GLint success = 0;
    char log[1024];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 1024, nullptr, log);
        throw std::runtime_error(log);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}
