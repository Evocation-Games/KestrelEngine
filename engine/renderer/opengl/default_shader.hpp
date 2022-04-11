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

#pragma once

namespace renderer::opengl
{

    static constexpr const char *s_default_opengl_vertex_shader {R"(
        #version 330 core
        layout(location = 0) in vec4 a_position;
        layout(location = 1) in vec4 a_color;
        layout(location = 2) in vec2 a_tex_coord;
        layout(location = 3) in float a_texture;

        out vec2 v_tex_coord;
        out vec4 v_color;
        out float v_tex_index;

        uniform mat4 u_projection;

        void main()
        {
            v_tex_coord = a_tex_coord;
            v_color = a_color;
            v_tex_index = a_texture;

            gl_Position = u_projection * vec4(a_position.xy, 0.0, 1.0);
        }
    )"};

    static constexpr const char *s_default_opengl_fragment_shader {R"(
        #version 330 core
        out vec4 color;

        in vec2 v_tex_coord;
        in vec4 v_color;
        in float v_tex_index;

        uniform sampler2D u_textures[16];

        void main()
        {
            int tex_index = int(v_tex_index);
            color = v_color * texture(u_textures[tex_index], v_tex_coord);
        }
    )"};

}