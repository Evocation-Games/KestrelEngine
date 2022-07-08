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
#include "renderer/common/draw_buffer.hpp"
#include "renderer/metal/texture.h"
#include "renderer/metal/shader.h"
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_metal.h>

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
    m_viewport_width = width;
    m_viewport_height = height;

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

    m_buffer_ptr = reinterpret_cast<uint8_t *>(m_buffer.contents);
    m_buffer_offset = 0;
    m_buffer_next_vertex = 0;
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

    m_pass_descriptor = nil;
}

auto renderer::metal::swap_chain::draw(const draw_buffer *buffer) -> void
{
    auto shader = reinterpret_cast<metal::shader::program *>(buffer->shader().get());
    auto state = shader->get_state();
    [m_command_encoder setRenderPipelineState:state];

    memcpy(m_buffer_ptr, buffer->data(), buffer->data_size());
//    [m_buffer didModifyRange:NSMakeRange(m_buffer_offset, buffer->data_size())];

    [m_command_encoder setVertexBytes:&m_viewport_size
                               length:sizeof(m_viewport_size)
                              atIndex:constants::vertex_input_index::viewport_size];

    std::size_t slots = buffer->texture_slots();
    for (uint32_t i = 0; i < slots; ++i) {
        auto texture_container = reinterpret_cast<renderer::metal::texture *>(buffer->texture(i).get());
        auto texture = texture_container->handle_ptr();
        [m_command_encoder setFragmentTexture:texture atIndex:i];
    }

    [m_command_encoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:m_buffer_next_vertex vertexCount:buffer->count()];
    m_buffer_ptr += buffer->data_size();
    m_buffer_offset += buffer->data_size();
    m_buffer_next_vertex += buffer->count();
}

// MARK: - ImGUI

auto renderer::metal::swap_chain::start_imgui() -> void
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize.x = static_cast<float>(m_viewport_width);
    io.DisplaySize.y = static_cast<float>(m_viewport_height);
    io.DisplayFramebufferScale = ImVec2(1.f, 1.f);
    io.DeltaTime = 1.0 / 50.0;

    ImGui_ImplMetal_NewFrame(m_pass_descriptor);
    ImGui::NewFrame();
}

auto renderer::metal::swap_chain::finalize_imgui() -> void
{
    ImGui::Render();
    ImGui_ImplMetal_RenderDrawData(ImGui::GetDrawData(), m_command_buffer, m_command_encoder);
    ImGui::EndFrame();
}

// MARK: -

auto renderer::metal::swap_chain::start() -> void
{

}
