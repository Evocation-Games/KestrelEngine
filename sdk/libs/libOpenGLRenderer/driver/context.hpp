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
    class context
    {
    public:
        context() = default;
        ~context();

        auto initialize(GLint width, GLint height, float scale = 1.f, bool visible = true, GLFWwindow *share = nullptr) -> void;

        [[nodiscard]] inline auto is_valid() const -> bool { return m_context_window != nullptr; }
        [[nodiscard]] inline auto window() const -> GLFWwindow * { return m_context_window; }

        auto make_current() -> void;

    private:
        static auto resized(GLFWwindow *window, GLint width, GLint height) -> void;

    private:
        GLint m_width { 0 };
        GLint m_height { 0 };
        GLFWwindow *m_context_window { nullptr };
        float m_native_scale_factor { 1.f };
    };
}