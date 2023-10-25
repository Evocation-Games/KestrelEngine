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

#include <libMetalRenderer/render/render_operation.h>
#include <libFoundation/profile/profiler.hpp>

#if !defined(RENDERER_MAX_VERTEX_COUNT)
#   define RENDERER_MAX_VERTEX_COUNT    3'000'000
#endif

// MARK: - Construction

auto renderer::metal::render_operation::initialize(id<MTLDevice> device) -> void
{
    // Reserve space for 128 commands.
    m_commands.reserve(128);

    // Acquire a suitable buffer
    m_buffer = [device newBufferWithLength:sizeof(vertex) * RENDERER_MAX_VERTEX_COUNT
                                   options:MTLResourceStorageModeShared];
    m_buffer_ptr = (std::uint8_t *)m_buffer.contents;
    m_buffer_offset = 0;
}

// MARK: - Operations

auto renderer::metal::render_operation::clear() -> void
{
    KESTREL_PROFILE_FUNCTION();
    m_commands.clear();
    m_buffer_ptr = (std::uint8_t *)m_buffer.contents;
    m_buffer_offset = 0;
}

// MARK: - Drawing

auto renderer::metal::render_operation::submit(
    const renderer::buffer &source,
    const std::vector<id<MTLTexture>>& textures,
    const metal::shader_program& shader
) -> void {
    KESTREL_PROFILE_FUNCTION();
    command cmd;
    cmd.shader = &shader;
    cmd.texture_count = textures.size();
    for (auto i = 0; i < cmd.texture_count; ++i) {
        cmd.textures[i] = textures[i];
    }
    cmd.offset = m_buffer_offset;
    cmd.ptr = m_buffer_ptr;
    cmd.data_size = source.data_size();
    cmd.vertex_count = source.count();

    // Copy the data into the buffer.
    ::memcpy(m_buffer_ptr, source.data(), cmd.data_size);
    m_buffer_ptr += cmd.data_size;
    m_buffer_offset += cmd.data_size;

    // Store the command
    m_commands.emplace_back(std::move(cmd));
}
