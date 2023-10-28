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

#include <iostream>
#include <stdexcept>
#include <libOpenGLRenderer/driver/context.hpp>
#include <libFoundation/availability.hpp>

// MARK: - Construction

auto renderer::opengl::context::initialize(GLint width, GLint height, float scale, bool visible, GLFWwindow *share) -> void
{
#if TARGET_MACOS
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    if (m_native_scale_factor > 1) {
        glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
    }
#else
    // TODO: Investigate OpenGL 4 for Linux and Windows.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    if (m_native_scale_factor > 1) {
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    }
#endif
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_VISIBLE, visible ? GL_TRUE : GL_FALSE);

    auto window = glfwCreateWindow(width, height, "OpenGL", nullptr, share);
    if (!window) {
        throw std::runtime_error("Failed to create window");
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, &context::resized);
    resized(window, width, height);

    m_context_window = window;
}

// MARK: - Destruction

renderer::opengl::context::~context()
{
    if (m_context_window) {
        glfwDestroyWindow(m_context_window);
    }
}

// MARK: - Interaction

auto renderer::opengl::context::make_current() -> void
{
    glfwMakeContextCurrent(m_context_window);
}

// MARK: - Callbacks

auto renderer::opengl::context::resized(GLFWwindow *window, GLint width, GLint height) -> void
{
    glViewport(0, 0, width, height);
}