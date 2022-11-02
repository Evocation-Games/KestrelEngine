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

namespace kestrel::renderer::metal
{

    static constexpr const char *s_default_metal_shader_code {R"(
        #include <metal_stdlib>
        #include <simd/simd.h>

        using namespace metal;

        typedef enum
        {
        	vertices = 0,
        	viewport_size = 1,
        } vertex_input_index;

        typedef enum
        {
        	base_color = 0,
        } texture_index;

        typedef struct
        {
        	vector_float4 position;
        	vector_float4 color;
        	vector_float2 tex_coord;
        	float texture;
        } vertex_descriptor;

        typedef struct
        {
        	float4 position [[position]];
        	float4 color;
        	float2 tex_coord;
        	float texture;
        } raster_data;

        // Vertex Function
        vertex raster_data vertex_shader(
        	uint vertex_id [[vertex_id]],
        	constant vertex_descriptor *vertex_array [[buffer(vertex_input_index::vertices)]],
        	constant vector_uint2 *viewport_size_ptr [[buffer(vertex_input_index::viewport_size)]]
        ) {
        	raster_data out;

        	auto position = vertex_array[vertex_id].position;
        	auto tex_coord = vertex_array[vertex_id].tex_coord;
        	auto color = vertex_array[vertex_id].color;
        	auto texture = floor(vertex_array[vertex_id].texture);
            auto scale = 1.f;

        	float2 viewport_size = float2(*viewport_size_ptr);
            float2 pixel_space_position = floor(position.xy * scale);
            float2 inverse_size(1.0f / viewport_size.x, 1.0f / viewport_size.y);
            float clip_x = (2.0f * pixel_space_position.x * inverse_size.x) - 1.0f;
            float clip_y = (2.0f * -pixel_space_position.y * inverse_size.y) + 1.0f;

            out.position = vector_float4(clip_x, clip_y, position.z, 1.0);
            out.tex_coord = tex_coord;
            out.color = color;
            out.texture = texture;

            return out;
        }

        // Fragment Function
        fragment float4 fragment_shader(
        	raster_data in [[stage_in]],
        	array<texture2d<half>, 16> textures [[texture(0)]]
        ) {
            if (in.texture < 0) {
                return in.color;
            }
            else {
                constexpr sampler texture_sampler (mag_filter::linear, min_filter::linear);
                const float4 color_sample = in.color * float4(textures[in.texture].sample(texture_sampler, in.tex_coord));
                return color_sample;
            }
        }
    )"};

}