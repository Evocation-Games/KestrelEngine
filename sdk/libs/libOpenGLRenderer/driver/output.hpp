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

#include <chrono>
#include <array>
#include <libOpenGLRenderer/opengl.hpp>
#include <libOpenGLRenderer/resource/shader/shader_program.hpp>
#include <libOpenGLRenderer/render/vertex_buffer.hpp>
#include <libRenderCore/buffer/vertex.hpp>

#pragma once

namespace renderer::opengl
{
    struct output
    {
        auto initialize(GLint width, GLint height, const resource::shader::program& shader, bool vsync = true) -> void;
        auto update_viewport_size(GLint width, GLint height) -> void;

        auto push_frame(GLint frame_texture, GLFWwindow *window) -> void;

    private:
        bool m_vsync { false };
        GLint m_width { 0 };
        GLint m_height { 0 };
        glm::mat4 m_projection { 0 };
        const resource::shader::program *m_shader;
        std::array<vertex, 6> m_vertices;
        vertex_buffer m_buffer;
    };
}