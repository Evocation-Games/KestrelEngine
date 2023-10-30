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

#include <array>
#include <vector>
#include <libRenderCore/buffer/buffer.hpp>
#include <libOpenGLRenderer/opengl.hpp>
#include <libOpenGLRenderer/resource/shader/shader_program.hpp>
#include <libOpenGLRenderer/render/vertex_buffer.hpp>

#if !defined(OPENGL_MAX_FRAGMENT_TEXTURES)
#   define OPENGL_MAX_FRAGMENT_TEXTURES  32
#endif

namespace renderer::opengl
{
    class render_operation
    {
    public:
        struct command {
            const vertex_buffer *vertex_buffer { nullptr };
            const resource::shader::program *shader { nullptr };
            std::size_t data_size { 0 };
            std::size_t offset { 0 };
            std::uint8_t *ptr { nullptr };
            std::array<GLuint, OPENGL_MAX_FRAGMENT_TEXTURES> textures { 0 };
            std::size_t texture_count { 0 };
            std::size_t vertex_count { 0 };
        };

    public:
        auto initialize() -> void;

        [[nodiscard]] inline auto command_count() const -> std::size_t { return m_commands.size(); }
        [[nodiscard]] inline auto commands() const -> const std::vector<command>& { return m_commands; }

        auto clear() -> void;
        auto submit(const renderer::buffer& source, const std::vector<GLuint>& textures, const resource::shader::program& shader) -> void;

    private:
        vertex_buffer *m_buffer;
        std::vector<command> m_commands;
        std::size_t m_buffer_offset { 0 };
    };
}
