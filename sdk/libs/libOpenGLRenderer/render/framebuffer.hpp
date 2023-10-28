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

#include <cstdint>
#include <array>
#include <functional>
#include <libOpenGLRenderer/opengl.hpp>
#include <libOpenGLRenderer/render/render_operation.hpp>
#include <libRenderCore/callback.hpp>

namespace renderer::opengl
{
    class framebuffer
    {
    public:
        framebuffer(std::uint32_t width, std::uint32_t height);

        [[nodiscard]] inline auto texture() const -> GLuint { return m_output.texture; }

        auto render(const render_operation& job) -> void;

    private:
        auto prepare() -> void;

    private:
        struct {
            GLuint id { 0 };
            GLuint rbo { 0 };
        } m_buffer;

        struct {
            glm::mat4 projection { 0 };
            std::uint32_t width { 0 };
            std::uint32_t height { 0 };
            GLint bounds[4] { 0 };
        } m_viewport;

        struct {
            GLuint texture { 0 };
        } m_output;
    };
}