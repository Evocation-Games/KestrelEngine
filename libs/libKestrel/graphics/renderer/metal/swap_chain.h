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

#include <libKestrel/util/availability.hpp>
#include <libKestrel/graphics/renderer/common/render_pass.hpp>

#if TARGET_MACOS
#include <MetalKit/MetalKit.h>

namespace kestrel::renderer::metal
{
    class swap_chain : public renderer::swap_chain
    {
    public:
        swap_chain();
        ~swap_chain() = default;

        auto start(id<CAMetalDrawable> drawable, id<MTLCommandBuffer> command_buffer, std::uint32_t width, std::uint32_t height) -> void;
        auto finalize(const std::function<auto() -> void>& callback) -> void override;

        auto draw(const draw_buffer *buffer) -> void override;

        auto start_imgui() -> void override;
        auto finalize_imgui() -> void override;

    private:
        auto start() -> void override;

    private:
        MTLRenderPassDescriptor *m_pass_descriptor { nullptr };
        vector_uint2 m_viewport_size;
        std::uint32_t m_viewport_width { 0 };
        std::uint32_t m_viewport_height { 0 };
        std::shared_ptr<graphics::texture> m_texture_ref;
        id<CAMetalDrawable> m_drawable { nullptr };
        id<MTLTexture> m_texture { nullptr };
        id<MTLDevice> m_device { nullptr };
        id<MTLBuffer> m_buffer { nullptr };
        std::uint8_t *m_buffer_ptr { nullptr };
        std::size_t m_buffer_offset { 0 };
        std::size_t m_buffer_next_vertex { 0 };
        id<MTLCommandQueue> m_command_queue { nullptr };
        id<MTLRenderCommandEncoder> m_command_encoder { nullptr };
        id<MTLCommandBuffer> m_command_buffer { nullptr };
    };
}

#endif