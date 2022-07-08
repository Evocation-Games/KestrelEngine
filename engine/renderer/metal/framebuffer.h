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

#include "util/hint.hpp"
#include "renderer/common/render_pass.hpp"

#if TARGET_MACOS

#include <MetalKit/MetalKit.h>

namespace renderer::metal
{
    class framebuffer : public renderer::framebuffer
    {
    public:
        framebuffer(uint32_t width, uint32_t height);
        ~framebuffer();

        auto invalidate() -> void override;

        auto start() -> void override;
        auto finalize(const std::function<auto()->void>& callback) -> void override;

        auto draw(const draw_buffer *buffer) -> void override;

        [[nodiscard]] auto texture() -> std::shared_ptr<graphics::texture> override;

    private:
        auto start_imgui() -> void override {};
        auto finalize_imgui() -> void override {};

    private:
        MTLRenderPassDescriptor *m_pass_descriptor { nullptr };
        vector_uint2 m_viewport_size;
        std::shared_ptr<graphics::texture> m_texture_ref;
        id<MTLDevice> m_device { nullptr };
        id<MTLTexture> m_texture { nullptr };
        id<MTLBuffer> m_buffer { nullptr };
        id<MTLCommandQueue> m_command_queue { nullptr };
        id<MTLRenderCommandEncoder> m_command_encoder { nullptr };
        id<MTLCommandBuffer> m_command_buffer { nullptr };
    };
}

#endif
