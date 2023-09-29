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
#include <libKestrel/graphics/renderer/opengl/opengl.hpp>

namespace kestrel::renderer::opengl
{
    class swap_chain : public renderer::swap_chain
    {
    public:
        swap_chain() = default;
        swap_chain(GLFWwindow *window, const GLint *textures, std::uint32_t slot_count, std::uint32_t vao, std::uint32_t vbo);
        ~swap_chain() = default;

        auto start() -> void override;
        auto finalize(const std::function<auto() -> void>& callback) -> void override;
        auto draw(const draw_buffer *buffer) -> void override;

        auto start_imgui() -> void override;
        auto finalize_imgui() -> void override;

        auto set_projection(const glm::mat4& projection) -> void;

    private:
        GLFWwindow *m_window { nullptr };
        std::uint32_t m_slot_count { 0 };
        std::uint32_t m_vao { 0 };
        std::uint32_t m_vbo { 0 };
        const GLint *m_textures { nullptr };
        glm::mat4 m_projection;
    };
}