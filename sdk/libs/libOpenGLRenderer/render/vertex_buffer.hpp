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

#include <libOpenGLRenderer/opengl.hpp>

namespace renderer::opengl
{
    struct vertex_buffer
    {
        vertex_buffer() = default;
        ~vertex_buffer();

        auto initialize(std::size_t size) -> void;

        auto bind(std::size_t offset, std::size_t size) const -> void;
        auto unbind() const -> void;

        auto copy_data(std::size_t offset, const void *data, std::size_t size) -> std::uint8_t *;

    private:
        GLuint m_vao { 0 };
        GLuint m_vbo { 0 };
        std::uint8_t *m_buffer { nullptr };
        std::size_t m_size { 0 };
    };
}