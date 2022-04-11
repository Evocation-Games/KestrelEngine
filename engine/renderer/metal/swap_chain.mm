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

#include "renderer/metal/swap_chain.h"
#include "renderer/metal/constants.h"
#include "renderer/common/vertex.hpp"
#include "renderer/common/draw_buffer.hpp"

// MARK: - Construction

renderer::metal::swap_chain::swap_chain()
    : renderer::swap_chain(),
      m_device(MTLCreateSystemDefaultDevice())
{
    m_buffer = [m_device newBufferWithLength:sizeof(vertex) * constants::max_quads options:MTLResourceStorageModeShared];
}

// MARK: - Destruction

renderer::metal::swap_chain::~swap_chain()
{

}

// MARK: - Frame Lifecycle

auto renderer::metal::swap_chain::start(id<CAMetalDrawable> drawable, id<MTLCommandBuffer> command_buffer, uint32_t width, uint32_t height) -> void
{
    m_drawable = drawable;
    m_texture = drawable.texture;
    m_command_buffer = command_buffer;
    m_viewport_size = vector2(width, height);

    MTLViewport viewport;
    viewport.originX = 0.0;
    viewport.originY = 0.0;
    viewport.width = static_cast<double>(width);
    viewport.height = static_cast<double>(height);
    viewport.znear = 1.0;
    viewport.zfar = -1.0;

    m_pass_descriptor = [[MTLRenderPassDescriptor alloc] init];
    m_pass_descriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    m_pass_descriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
    m_pass_descriptor.colorAttachments[0].clearColor = MTLClearColorMake(0, 0, 0, 1);
    m_pass_descriptor.colorAttachments[0].texture = m_texture;

    m_command_encoder = [m_command_buffer renderCommandEncoderWithDescriptor:m_pass_descriptor];
    [m_command_encoder setViewport:viewport];
    [m_command_encoder setVertexBuffer:m_buffer
                                     offset:0
                                    atIndex:constants::vertex_input_index::vertices];
}

auto renderer::metal::swap_chain::finalize(const std::function<auto() -> void>& callback) -> void
{
    auto completion = callback;

    [m_command_encoder endEncoding];
    [m_command_buffer presentDrawable:m_drawable];
    m_command_encoder = nil;

    [m_command_buffer addCompletedHandler:^(id<MTLCommandBuffer> buffer) {
        completion();
    }];

    [m_command_buffer commit];
}

auto renderer::metal::swap_chain::draw(const draw_buffer *buffer) -> void
{
    auto shader = buffer->shader();
    id<MTLRenderPipelineState> state = reinterpret_cast<id<MTLRenderPipelineState>>(shader->get<void *>());
    [m_command_encoder setRenderPipelineState:state];

    memcpy(m_buffer.contents, buffer->data(), buffer->data_size());

    [m_command_encoder setVertexBytes:&m_viewport_size
                               length:sizeof(m_viewport_size)
                              atIndex:constants::vertex_input_index::viewport_size];

    std::size_t slots = buffer->texture_slots();
    for (uint32_t i = 0; i < slots; ++i) {
        auto texture = reinterpret_cast<id<MTLTexture>>(buffer->texture(i)->handle());
        [m_command_encoder setFragmentTexture:texture atIndex:i];
    }

    [m_command_encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:buffer->count()];
}

// MARK: - ImGUI

auto renderer::metal::swap_chain::start_imgui() -> void
{

}

auto renderer::metal::swap_chain::finalize_imgui() -> void
{

}

// MARK: -

auto renderer::metal::swap_chain::start() -> void
{

}