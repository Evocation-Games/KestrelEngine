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

#include <vector>
#include <MetalKit/MetalKit.h>
#include <libRenderCore/frame/frame.hpp>
#include <libMetalRenderer/render/framebuffer.h>
#include <libMetalRenderer/render/render_operation.h>

namespace renderer::metal
{
    class frame_generator
    {
    public:
        frame_generator() = default;
        explicit frame_generator(id<MTLDevice> device,
                                 std::uint32_t width,
                                 std::uint32_t height,
                                 std::size_t queue_size,
                                 MTLPixelFormat format);

        [[nodiscard]] auto latest_frame_texture() -> id<MTLTexture>;
        [[nodiscard]] inline auto current_operation() -> render_operation& { return m_operation; }

        auto wait_for_ready() -> void;
        auto produce_new_frame(id<MTLCommandQueue> command_queue, renderer::callback completion) -> void;

    private:
        id<MTLDevice> m_device;
        std::uint64_t m_index { 0 };
        std::size_t m_queue_size { 3 };
        std::vector<framebuffer> m_buffers;
        id<MTLTexture> m_last_frame_texture { nil };
        render_operation m_operation;
        renderer::callback m_completion;
    };
}