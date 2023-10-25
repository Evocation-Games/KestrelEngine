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

#include <libMetalRenderer/render/swapchain.h>
#include <libMetalRenderer/constants.hpp>
#include <libMetalRenderer/driver/shader_program.h>
#include <libFoundation/profile/profiler.hpp>

#if !defined(RENDERER_MAX_VERTEX_COUNT)
#   define RENDERER_MAX_VERTEX_COUNT    6'000'000
#endif

// MARK: - Lifecycle

auto renderer::metal::swapchain::prepare(
    id <CAMetalDrawable> drawable,
    id <MTLCommandBuffer> command_buffer,
    std::uint32_t width,
    std::uint32_t height
) -> void {
    KESTREL_PROFILE_FUNCTION();
    m_output.drawable = drawable;
    m_output.texture = drawable.texture;
    m_pass.command_buffer = command_buffer;
    m_viewport.size = vector2(width, height);
    m_viewport.width = width;
    m_viewport.height = height;

    MTLViewport viewport;
    viewport.originX = viewport.originY = 0;
    viewport.width = (double)width;
    viewport.height = (double)height;
    viewport.znear = 1.0;
    viewport.zfar = -1.0;

    m_pass.descriptor = [[MTLRenderPassDescriptor alloc] init];
    m_pass.descriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    m_pass.descriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
    m_pass.descriptor.colorAttachments[0].clearColor = MTLClearColorMake(0, 0, 0, 1);
    m_pass.descriptor.colorAttachments[0].texture = m_output.texture;

    m_pass.command_encoder = [m_pass.command_buffer renderCommandEncoderWithDescriptor:m_pass.descriptor];
    [m_pass.command_encoder setViewport:viewport];
}

auto renderer::metal::swapchain::commit(renderer::metal::render_operation job, renderer::callback completion) -> void
{
    KESTREL_PROFILE_FUNCTION()
    auto completion_handler = std::move(completion);

    [m_pass.command_encoder setVertexBytes:&m_viewport.size
                                    length:sizeof(m_viewport.size)
                                   atIndex:constants::vertex_input_index::viewport_size];

    for (const auto& command : job.commands()) {
        [m_pass.command_encoder setRenderPipelineState:command.shader->pipeline()];

        [m_pass.command_encoder setVertexBuffer:job.buffer()
                                         offset:command.offset
                                        atIndex:constants::vertex_input_index::vertices];

        for (auto i = 0; i < command.texture_count; ++i) {
            [m_pass.command_encoder setFragmentTexture:command.textures[i] atIndex:i];
        }

        [m_pass.command_encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:command.vertex_count];
    }

    [m_pass.command_encoder endEncoding];
    [m_pass.command_buffer presentDrawable:m_output.drawable];
    m_pass.command_encoder = nil;

    [m_pass.command_buffer addCompletedHandler:^(id<MTLCommandBuffer> buffer) {
        completion_handler();
    }];

    [m_pass.command_buffer commit];
    m_pass.descriptor = nil;
}
