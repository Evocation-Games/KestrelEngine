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

#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

#include "core/graphics/metal/metal_shader.h"

typedef struct
{
    float4 position [[position]];
    float4 color;
    float2 texture_coord;
} rasterizer_data;

// Vertex Function
vertex rasterizer_data vertexShader(
    uint vertex_id [[ vertex_id ]],
    constant graphics::metal::vertex_descriptor *vertex_array [[ buffer(graphics::metal::vertex_input_index::vertices) ]],
    constant vector_uint2 *viewport_size_pointer [[ buffer(graphics::metal::vertex_input_index::viewport_size) ]]
) {
    rasterizer_data out;

    float2 pixel_space_position = vertex_array[vertex_id].position.xy;
    float2 viewport_size = float2(*viewport_size_pointer);

    out.position = vector_float4(0.0, 0.0, 0.0, 1.0);
    out.position.xy = (pixel_space_position / (viewport_size / vertex_array[vertex_id].scale)) * 2;
    out.texture_coord = vertex_array[vertex_id].texture_coord;
    out.color = vertex_array[vertex_id].color;

    return out;
}

// Fragment function
fragment float4 samplingShader(
    rasterizer_data in [[stage_in]],
    texture2d<half> color_texture [[ texture(graphics::metal::texture_index::base_color) ]]
) {
    constexpr sampler texture_sampler (mag_filter::linear, min_filter::linear);
    const float4 color_sample = in.color * float4(color_texture.sample(texture_sampler, in.texture_coord));
    return color_sample;
}
