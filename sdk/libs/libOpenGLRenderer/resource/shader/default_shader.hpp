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

#pragma once

namespace renderer::opengl::resource::shader
{
#pragma region Shader Library Template Code
    static constexpr const char *vertex_shader_template {R"(
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
    @@VERTEX_FUNCTION@@
}
    )"};

    static constexpr const char *fragment_shader_template {R"(
#version 330 core
out vec4 color;

in vec2 v_tex_coord;
in vec4 v_color;
in float v_tex_index;

uniform sampler2D u_textures[16];
uniform float u_texture_count;

void main()
{
    @@FRAGMENT_FUNCTION@@
}
    )"};
#pragma endregion

#pragma region Default Shader Code
    static constexpr const char *default_vertex_function {R"(
    v_tex_coord = a_tex_coord;
    v_color = a_color;
    v_tex_index = a_texture;

    gl_Position = u_projection * vec4(a_position.xy, 0.0, 1.0);
    )"};

    static constexpr const char *default_fragment_function {R"(
    if (u_texture_count == 0) {
        color = vec4(v_tex_coord, 0.0, 1.0);
        return;
    }

    int tex_index = int(v_tex_index);
    switch (tex_index) {
    case 0: color = v_color * texture(u_textures[0], v_tex_coord); break;
    case 1: color = v_color * texture(u_textures[1], v_tex_coord); break;
    case 2: color = v_color * texture(u_textures[2], v_tex_coord); break;
    case 3: color = v_color * texture(u_textures[3], v_tex_coord); break;
    case 4: color = v_color * texture(u_textures[4], v_tex_coord); break;
    case 5: color = v_color * texture(u_textures[5], v_tex_coord); break;
    case 6: color = v_color * texture(u_textures[6], v_tex_coord); break;
    case 7: color = v_color * texture(u_textures[7], v_tex_coord); break;
    case 8: color = v_color * texture(u_textures[8], v_tex_coord); break;
    case 9: color = v_color * texture(u_textures[9], v_tex_coord); break;
    case 10: color = v_color * texture(u_textures[10], v_tex_coord); break;
    case 11: color = v_color * texture(u_textures[11], v_tex_coord); break;
    case 12: color = v_color * texture(u_textures[12], v_tex_coord); break;
    case 13: color = v_color * texture(u_textures[13], v_tex_coord); break;
    case 14: color = v_color * texture(u_textures[14], v_tex_coord); break;
    case 15: color = v_color * texture(u_textures[15], v_tex_coord); break;
    default: color = v_color; break;
    }
    )"};
#pragma endregion
}