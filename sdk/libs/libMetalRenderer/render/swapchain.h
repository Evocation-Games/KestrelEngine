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

#include <array>
#include <functional>
#include <libRenderCore/buffer/buffer.hpp>
#include <libRenderCore/buffer/vertex.hpp>
#include <libMetalRenderer/driver/shader_program.h>
#include <libMetalRenderer/render/render_operation.h>
#include <libRenderCore/callback.hpp>
#include <MetalKit/MetalKit.h>

#if !defined(METAL_SWAPCHAIN_BUFFER_COUNT)
#   define METAL_SWAPCHAIN_BUFFER_COUNT 1
#endif

namespace renderer::metal
{
    class swapchain
    {
    public:
        auto prepare(id<CAMetalDrawable> drawable, id<MTLCommandBuffer> command_buffer, std::uint32_t width, std::uint32_t height) -> void;
        auto commit(render_operation job, renderer::callback completion) -> void;

    private:
        render_operation m_operation;

        struct {
            vector_uint2 size;
            std::uint32_t width { 0 };
            std::uint32_t height { 0 };
        } m_viewport;

        struct {
            MTLRenderPassDescriptor *descriptor;
            id<MTLCommandQueue> command_queue;
            id<MTLRenderCommandEncoder> command_encoder;
            id<MTLCommandBuffer> command_buffer;
        } m_pass;

        struct {
            id<MTLDrawable> drawable { nil };
            id<MTLTexture> texture { nil };
        } m_output;
    };
}