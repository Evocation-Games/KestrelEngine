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
    static constexpr const char *s_default_vertex_function {R"(
        out.position = float4(clip_x, clip_y, position.z, 1.0);
        out.tex_coord = tex_coord;
        out.color = color;
        out.texture = texture;
        return out;
    )"};

    static constexpr const char *s_default_fragment_function {R"(
        if (in.texture < 0) {
            return in.color;
        }
        else {
            constexpr sampler texture_sampler (mag_filter::linear, min_filter::linear);
            const float4 color_sample = in.color * float4(textures[in.texture].sample(texture_sampler, in.tex_coord));
            return color_sample;
        }
    )"};

    static constexpr const char *s_vertex_function_name = "vertex_shader";
    static constexpr const char *s_fragment_function_name = "fragment_shader";

    static constexpr const char *s_shader_code_template {R"(
        #include <metal_stdlib>
        #include <simd/simd.h>

        using namespace metal;

        // Generate a random float in the range [0.0f, 1.0f] using x, y, and z (based on the xor128 algorithm)
        float rand(float3 v)
        {
            int seed = v.x + v.y * 57 + v.z * 241;
            seed= (seed<< 13) ^ seed;
            return (( 1.0 - ( (seed * (seed * seed * 15731 + 789221) + 1376312589) & 2147483647) / 1073741824.0f) + 1.0f) / 2.0f;
        }

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
        	float4 position;
        	float4 color;
            float4 attachment_0;
            float4 attachment_1;
            float4 attachment_2;
            float4 attachment_3;
            float4 attachment_4;
            float4 attachment_5;
            float4 attachment_6;
            float4 attachment_7;
            float4 attachment_8;
            float4 attachment_9;
            float4 attachment_10;
            float4 attachment_11;
            float4 attachment_12;
        	float2 tex_coord;
        	float texture;
        } vertex_descriptor;

        typedef struct
        {
        	float4 position [[position]];
        	float4 color;
            float4 attachment_0;
            float4 attachment_1;
            float4 attachment_2;
            float4 attachment_3;
            float4 attachment_4;
            float4 attachment_5;
            float4 attachment_6;
            float4 attachment_7;
            float4 attachment_8;
            float4 attachment_9;
            float4 attachment_10;
            float4 attachment_11;
            float4 attachment_12;
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

            out.attachment_0 = vertex_array[vertex_id].attachment_0;
            out.attachment_1 = vertex_array[vertex_id].attachment_1;
            out.attachment_2 = vertex_array[vertex_id].attachment_2;
            out.attachment_3 = vertex_array[vertex_id].attachment_3;
            out.attachment_4 = vertex_array[vertex_id].attachment_4;
            out.attachment_5 = vertex_array[vertex_id].attachment_5;
            out.attachment_6 = vertex_array[vertex_id].attachment_6;
            out.attachment_7 = vertex_array[vertex_id].attachment_7;
            out.attachment_8 = vertex_array[vertex_id].attachment_8;
            out.attachment_9 = vertex_array[vertex_id].attachment_9;
            out.attachment_10 = vertex_array[vertex_id].attachment_10;
            out.attachment_11 = vertex_array[vertex_id].attachment_11;
            out.attachment_12 = vertex_array[vertex_id].attachment_12;

        	float2 viewport_size = float2(*viewport_size_ptr);
            float2 pixel_space_position = floor(position.xy * scale);
            float2 inverse_size(1.0f / viewport_size.x, 1.0f / viewport_size.y);
            float clip_x = (2.0f * pixel_space_position.x * inverse_size.x) - 1.0f;
            float clip_y = (2.0f * -pixel_space_position.y * inverse_size.y) + 1.0f;

            @@VERTEX_FUNCTION@@
        }

        // Fragment Function
        fragment float4 fragment_shader(
        	raster_data in [[stage_in]],
        	array<texture2d<half>, 31> textures [[texture(0)]]
        ) {
            @@FRAGMENT_FUNCTION@@
        }
    )"};

}