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

#include "renderer/metal/framebuffer.h"
#include "renderer/common/vertex.hpp"
#include "renderer/metal/constants.h"
#include "renderer/common/draw_buffer.hpp"
#include "renderer/common/renderer.hpp"
#include "renderer/metal/texture.h"
#include "renderer/metal/shader.h"
#include "renderer/metal/context.h"

// MARK: - Construction

renderer::metal::framebuffer::framebuffer(uint32_t width, uint32_t height)
    : renderer::framebuffer(width, height),
      m_device(MTLCreateSystemDefaultDevice())
{
    invalidate();
}

// MARK: - Destruction

renderer::metal::framebuffer::~framebuffer()
{

}

// MARK: - Texture

auto renderer::metal::framebuffer::texture() -> std::shared_ptr<graphics::texture>
{
    if (!m_texture_ref && !m_texture) {
        auto tex = std::make_shared<metal::texture>(m_texture, math::size(m_width, m_height));
        m_texture_ref = std::static_pointer_cast<graphics::texture>(tex);
    }
    return m_texture_ref;
}

// MARK: - Setup and Invalidation

auto renderer::metal::framebuffer::invalidate() -> void
{
    m_command_queue = [m_device newCommandQueue];

    // Setup the rendering pipeline for the framebuffer
    MTLTextureDescriptor *texture_descriptor = [[MTLTextureDescriptor alloc] init];
    texture_descriptor.textureType = MTLTextureType2D;
    texture_descriptor.width = m_width;
    texture_descriptor.height = m_height;
    texture_descriptor.pixelFormat = MTLPixelFormatBGRA8Unorm;
    texture_descriptor.usage = MTLTextureUsageRenderTarget | MTLTextureUsageShaderRead;

    m_texture = [m_device newTextureWithDescriptor:texture_descriptor];

    // Setup the render pass
    m_pass_descriptor = [[MTLRenderPassDescriptor alloc] init];
    m_pass_descriptor.colorAttachments[0].texture = m_texture;
    m_pass_descriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
    m_pass_descriptor.colorAttachments[0].clearColor = MTLClearColorMake(0, 0, 0, 1);
    m_pass_descriptor.colorAttachments[0].storeAction = MTLStoreActionStore;

    // Setup the vertex buffer
    m_buffer = [m_device newBufferWithLength:constants::max_quads * 6 * sizeof(vertex) options:MTLResourceStorageModeShared];
}

// MARK: - Frame Lifecycle

auto renderer::metal::framebuffer::start() -> void
{
    m_command_buffer = [m_command_queue commandBuffer];

    MTLViewport viewport;
    viewport.originX = 0;
    viewport.originY = 0;
    viewport.width = m_width;
    viewport.height = m_height;
    viewport.znear = 1.0;
    viewport.zfar = -1.0;

    m_command_encoder = [m_command_buffer renderCommandEncoderWithDescriptor:m_pass_descriptor];
    [m_command_encoder setLabel:@"com.kestrel.metal.framebuffer"];
    [m_command_encoder setViewport:viewport];
    [m_command_encoder setVertexBuffer:m_buffer offset:0 atIndex:constants::vertex_input_index::vertices];
}

auto renderer::metal::framebuffer::finalize(const std::function<auto() -> void> &callback) -> void
{
    [m_command_encoder endEncoding];
    m_command_encoder = nil;

    [m_command_buffer addCompletedHandler:^(id<MTLCommandBuffer> buffer) {
        callback();
    }];

    [m_command_buffer commit];
}

auto renderer::metal::framebuffer::draw(const draw_buffer *buffer) -> void
{
    auto shader = reinterpret_cast<metal::shader::program *>(buffer->shader().get());
    auto state = shader->get_state();
    [m_command_encoder setRenderPipelineState:state];

    memcpy(m_buffer.contents, buffer->data(), buffer->data_size());

    [m_command_encoder setVertexBytes:&m_viewport_size
                               length:sizeof(m_viewport_size)
                              atIndex:constants::vertex_input_index::viewport_size];

    std::size_t slots = buffer->texture_slots();
    for (uint32_t i = 0; i < slots; ++i) {
        auto texture_container = reinterpret_cast<renderer::metal::texture *>(buffer->texture(i).get());
        auto texture = texture_container->handle_ptr();
        [m_command_encoder setFragmentTexture:texture atIndex:i];
    }

    [m_command_encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:buffer->count()];
}
