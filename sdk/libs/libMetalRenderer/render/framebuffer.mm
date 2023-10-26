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

#include <libMetalRenderer/render/framebuffer.h>
#include <libMetalRenderer/constants.hpp>
#include <libFoundation/profile/profiler.hpp>

#if !defined(RENDERER_MAX_VERTEX_COUNT)
#   define RENDERER_MAX_VERTEX_COUNT    6'000'000
#endif

// MARK: - Constructor

renderer::metal::framebuffer::framebuffer(
    id<MTLDevice> device,
    std::uint32_t width,
    std::uint32_t height,
    MTLPixelFormat format
) {
    KESTREL_PROFILE_FUNCTION();

    // Setup the viewport
    m_viewport.size = vector2(width, height);
    m_viewport.viewport.originX = 0;
    m_viewport.viewport.originY = 0;
    m_viewport.viewport.width = (double)width;
    m_viewport.viewport.height = (double)height;
    m_viewport.viewport.znear = 1.0;
    m_viewport.viewport.zfar = -1.0;

    // Create a texture to render into.
    m_output.descriptor = [[MTLTextureDescriptor alloc] init];
    m_output.descriptor.textureType = MTLTextureType2D;
    m_output.descriptor.width = width;
    m_output.descriptor.height = height;
    m_output.descriptor.pixelFormat = MTLPixelFormatBGRA8Unorm;
    m_output.descriptor.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;
    m_output.texture = [device newTextureWithDescriptor:m_output.descriptor];
}

// MARK: - Lifecycle

auto renderer::metal::framebuffer::render(
    id<MTLCommandBuffer> command_buffer,
    const renderer::metal::render_operation& job
) -> void {
    KESTREL_PROFILE_FUNCTION();

    MTLRenderPassDescriptor *descriptor = [[MTLRenderPassDescriptor alloc] init];
    descriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    descriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
    descriptor.colorAttachments[0].clearColor = MTLClearColorMake(0, 0, 0, 1);
    descriptor.colorAttachments[0].texture = m_output.texture;

    id<MTLRenderCommandEncoder> command_encoder = [command_buffer renderCommandEncoderWithDescriptor:descriptor];
    [command_encoder setViewport:m_viewport.viewport];

    [command_encoder setVertexBytes:&m_viewport.size
                             length:sizeof(m_viewport.size)
                            atIndex:constants::vertex_input_index::viewport_size];

    std::uint32_t vertex_count = 0;
    for (const auto& command : job.commands()) {
        [command_encoder setRenderPipelineState:command.shader->pipeline()];

        [command_encoder setVertexBuffer:job.buffer()
                                         offset:command.offset
                                        atIndex:constants::vertex_input_index::vertices];
        vertex_count += command.vertex_count;

        for (auto i = 0; i < command.texture_count; ++i) {
            [command_encoder setFragmentTexture:command.textures[i] atIndex:i];
        }

        [command_encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:command.vertex_count];
    }

    [command_encoder endEncoding];
    command_encoder = nil;

    dispatch_group_t g = dispatch_group_create();
    dispatch_group_enter(g);

    [command_buffer addCompletedHandler:^(id<MTLCommandBuffer> buffer) {
        dispatch_group_leave(g);
    }];

    [command_buffer commit];
    descriptor = nil;

    dispatch_group_wait(g, DISPATCH_TIME_FOREVER);
}
