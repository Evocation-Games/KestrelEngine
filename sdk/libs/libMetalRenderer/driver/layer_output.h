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

#include <MetalKit/MetalKit.h>
#include <libMath/types/vec2.hpp>
#include <libMetalRenderer/resource/shader/shader_program.h>

namespace renderer::metal
{
    struct layer_output
    {
        auto initialize(CAMetalLayer *layer, math::vec2 viewport_size, const resource::shader::program& shader) -> void;
        auto update_viewport_size(std::uint32_t width, std::uint32_t height) -> void;

        auto push_frame(id<MTLCommandQueue> commandQueue, id<MTLTexture> frame_texture) -> void;

    private:
        CAMetalLayer *m_layer { nil };
        vector_uint2 m_viewport_size;
        MTLViewport m_viewport;
        const resource::shader::program *m_shader;
        id<MTLBuffer> m_buffer;
    };
}