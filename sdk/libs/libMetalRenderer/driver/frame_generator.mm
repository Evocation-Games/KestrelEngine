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

#include <libMetalRenderer/driver/frame_generator.h>

// MARK: - Construction

renderer::metal::frame_generator::frame_generator(
    id<MTLDevice> device,
    std::uint32_t width,
    std::uint32_t height,
    std::size_t queue_size,
    MTLPixelFormat format
)
    : m_device(device),
      m_queue_size(queue_size)
{
    m_operation.initialize(device);
    for (auto i = 0; i < queue_size; ++i) {
        m_buffers.emplace_back(device, width, height, format);
    }
}

// MARK: - Frame Requesting

auto renderer::metal::frame_generator::latest_frame_texture() -> id<MTLTexture>
{
    return m_last_frame_texture;
}

// MARK: - Frame Construction

auto renderer::metal::frame_generator::wait_for_ready() -> void
{
    KESTREL_PROFILE_FUNCTION();

    // Clear the previous operation so that we can work from a clean slate.
    m_operation.clear();
}

auto renderer::metal::frame_generator::produce_new_frame(
    id<MTLCommandQueue> command_queue,
    renderer::callback completion
) -> void {
    using namespace std::chrono_literals;

    KESTREL_PROFILE_FUNCTION();
    auto completion_handler = std::move(completion);

    // Get the next buffer.
    auto buffer_idx = m_index;
    m_index = (buffer_idx + 1) % m_queue_size;
    auto& framebuffer = m_buffers[buffer_idx];

    // Instruct the framebuffer to render the current operation.
    framebuffer.render(command_queue.commandBuffer, m_operation);

    // Drop the oldest frame from the completion, and insert a new one at the back.
    m_last_frame_texture = framebuffer.texture();

    // Indicate that we have finished.
    completion_handler();
}