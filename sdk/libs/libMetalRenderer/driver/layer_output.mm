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

#include <array>
#include <libMetalRenderer/driver/layer_output.h>
#include <libFoundation/profile/profiler.hpp>
#include <libMetalRenderer/constants.hpp>
#include <libRenderCore/buffer/vertex.hpp>

// MARK: - Construction

auto renderer::metal::layer_output::initialize(
    CAMetalLayer *layer,
    math::vec2 viewport_size,
    const resource::shader::program& shader
) -> void {
    m_layer = layer;
    m_shader = &shader;

    m_viewport_size = vector2((std::uint32_t)viewport_size.x(), (std::uint32_t)viewport_size.y());
    m_viewport.originX = 0;
    m_viewport.originY = 0;
    m_viewport.width = viewport_size.x();
    m_viewport.height = viewport_size.y();
    m_viewport.znear = 1.0;
    m_viewport.zfar = -1.0;

    // Setup a buffer with the appropriate vertexes created to display frame textures.
    m_buffer = [layer.device newBufferWithLength:sizeof(vertex) * 6 options:MTLResourceStorageModeShared];

    std::array<vertex, 6> vertices;
    vertices[0].texture = 0.f;
    vertices[1].texture = 0.f;
    vertices[2].texture = 0.f;
    vertices[3].texture = 0.f;
    vertices[4].texture = 0.f;
    vertices[5].texture = 0.f;

    vertices[0].color = renderer::vec4(1, 1, 1, 1);
    vertices[1].color = renderer::vec4(1, 1, 1, 1);
    vertices[2].color = renderer::vec4(1, 1, 1, 1);
    vertices[3].color = renderer::vec4(1, 1, 1, 1);
    vertices[4].color = renderer::vec4(1, 1, 1, 1);
    vertices[5].color = renderer::vec4(1, 1, 1, 1);

    auto px = 0.f;
    auto py = 0.f;
    auto sw = viewport_size.x();
    auto sh = viewport_size.y();

    vertices[0].position = renderer::vec4(px, sh, 0, 0);
    vertices[1].position = renderer::vec4(sw, sh, 0, 0);
    vertices[2].position = renderer::vec4(sw, py, 0, 0);
    vertices[3].position = renderer::vec4(px, sh, 0, 0);
    vertices[4].position = renderer::vec4(px, py, 0, 0);
    vertices[5].position = renderer::vec4(sw, py, 0, 0);

    vertices[0].tex_coord = renderer::vec2(0.f, 1.f);
    vertices[1].tex_coord = renderer::vec2(1.f, 1.f);
    vertices[2].tex_coord = renderer::vec2(1.f, 0.f);
    vertices[3].tex_coord = renderer::vec2(0.f, 1.f);
    vertices[4].tex_coord = renderer::vec2(0.f, 0.f);
    vertices[5].tex_coord = renderer::vec2(1.f, 0.f);

    ::memcpy(m_buffer.contents, &vertices[0], sizeof(vertex) * 6);
}

auto renderer::metal::layer_output::update_viewport_size(std::uint32_t width, std::uint32_t height) -> void
{

}

// MARK: - Frame Submission

auto renderer::metal::layer_output::push_frame(id<MTLCommandQueue> command_queue, id<MTLTexture> frame_texture) -> void
{
    KESTREL_PROFILE_FUNCTION();

    id<MTLCommandBuffer> command_buffer = command_queue.commandBuffer;
    id<CAMetalDrawable> drawable = m_layer.nextDrawable;

    MTLRenderPassDescriptor *descriptor = [[MTLRenderPassDescriptor alloc] init];
    descriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    descriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
    descriptor.colorAttachments[0].clearColor = MTLClearColorMake(0, 0, 0, 1);
    descriptor.colorAttachments[0].texture = drawable.texture;

    id<MTLRenderCommandEncoder> command_encoder = [command_buffer renderCommandEncoderWithDescriptor:descriptor];
    [command_encoder setViewport:m_viewport];

    [command_encoder setVertexBytes:&m_viewport_size
                             length:sizeof(m_viewport_size)
                            atIndex:constants::vertex_input_index::viewport_size];

    [command_encoder setRenderPipelineState:m_shader->pipeline()];

    [command_encoder setVertexBuffer:m_buffer
                              offset:0
                             atIndex:constants::vertex_input_index::vertices];

    [command_encoder setFragmentTexture:frame_texture atIndex:0];
    [command_encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:6];
    [command_encoder endEncoding];
    [command_buffer presentDrawable:drawable];

    dispatch_group_t g = dispatch_group_create();
    dispatch_group_enter(g);

    [command_buffer addCompletedHandler:^(id<MTLCommandBuffer> buffer) {
        dispatch_group_leave(g);
    }];

    [command_buffer commit];
    descriptor = nil;

    dispatch_group_wait(g, DISPATCH_TIME_FOREVER);
}