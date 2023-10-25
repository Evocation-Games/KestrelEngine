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
#include <array>
#include <MetalKit/MetalKit.h>
#include <libRenderCore/buffer/buffer.hpp>
#include <libMetalRenderer/driver/shader_program.h>

#if !defined(METAL_MAX_FRAGMENT_TEXTURES)
#   define METAL_MAX_FRAGMENT_TEXTURES  32
#endif

namespace renderer::metal
{
    class render_operation
    {
    public:
        struct command
        {
            const metal::shader_program *shader { nullptr };
            std::size_t data_size { 0 };
            std::uintptr_t offset { 0 };
            std::uint8_t *ptr { nullptr };
            std::array<id<MTLTexture>, METAL_MAX_FRAGMENT_TEXTURES> textures;
            std::size_t texture_count { 0 };
            std::size_t vertex_count { 0 };
        };

    public:
        auto initialize(id<MTLDevice> device) -> void;

        [[nodiscard]] inline auto buffer() const -> id<MTLBuffer> { return m_buffer; }
        [[nodiscard]] inline auto command_count() const -> std::size_t { return m_commands.size(); }
        [[nodiscard]] inline auto commands() const -> const std::vector<command>& { return m_commands; }

        auto clear() -> void;
        auto submit(const renderer::buffer& source, const std::vector<id<MTLTexture>>& textures, const metal::shader_program& shader) -> void;

    private:
        id<MTLBuffer> m_buffer;
        std::vector<command> m_commands;
        std::uint8_t *m_buffer_ptr { nullptr };
        std::size_t m_buffer_offset { 0 };
    };
}