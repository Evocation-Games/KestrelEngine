//
// Copyright (c) %%YEAR %%NAME
//

#version 330 core

layout(location = 0) in vec4 vertex;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;
uniform vec2 texOffset;
uniform vec2 texSize;

void main()
{
    TexCoords = (vertex.zw * texSize) + texOffset;
    gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}