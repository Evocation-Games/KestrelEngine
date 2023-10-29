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

#include <libOpenGLRenderer/render/render_operation.hpp>
#include <libFoundation/profile/profiler.hpp>

#if !defined(RENDERER_MAX_VERTEX_COUNT)
#   define RENDERER_MAX_VERTEX_COUNT                3'000'000
#endif
#if !defined(RENDERER_DEFAULT_COMMAND_QUEUE_SIZE)
#   define RENDERER_DEFAULT_COMMAND_QUEUE_SIZE      128
#endif

// MARK: - Construction

auto renderer::opengl::render_operation::initialize() -> void
{
    // Reserve space for 128 commands.
    m_commands.reserve(RENDERER_DEFAULT_COMMAND_QUEUE_SIZE);

    // Setup a vertex buffer...
    m_buffer = new vertex_buffer();
    m_buffer->initialize(sizeof(vertex) * 6);
}

// MARK: - Operations

auto renderer::opengl::render_operation::clear() -> void
{
    KESTREL_PROFILE_FUNCTION();
    m_commands.clear();
    m_buffer_offset = 0;
}

auto renderer::opengl::render_operation::submit(
    const renderer::buffer &source,
    const std::vector<GLuint> &textures,
    const resource::shader::program &shader
) -> void {
    KESTREL_PROFILE_FUNCTION();
    command cmd;
    cmd.shader = &shader;
    cmd.vertex_buffer = m_buffer;
    cmd.texture_count = textures.size();
    for (auto i = 0; i < cmd.texture_count; ++i) {
        cmd.textures[i] = textures[i];
    }
    cmd.offset = m_buffer_offset;
    cmd.data_size = source.data_size();
    cmd.vertex_count = source.count();

    // Copy the data into the buffer.
    cmd.ptr = m_buffer->copy_data(m_buffer_offset, source.data(), source.data_size());
    m_buffer_offset += cmd.data_size;

    // Store the command
    m_commands.emplace_back(cmd);
}