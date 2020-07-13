// Copyright (c) 2020 Tom Hancocks
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

#include <stdexcept>
#include <utility>
#include <iostream>
#include "core/graphics/opengl/opengl_session_window.hpp"
#include "core/graphics/opengl/opengl_scene.hpp"
#include "core/graphics/opengl/opengl.hpp"
#include <libGraphite/rsrc/manager.hpp>
#include <libGraphite/data/reader.hpp>

// MARK: - Construction

graphics::opengl::session_window::session_window(std::shared_ptr<environment> env)
    : graphics::session_window(std::move(env))
{
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialise OpenGL. Unable to proceed.");
    }

    // Configure the main window for the session.
#if __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    glfwSetErrorCallback([](GLint code, const char *reason) {
        throw std::runtime_error("Failed to create a window for OpenGL session: " + std::string(reason));
    });

    m_window = glfwCreateWindow(800, 600, "Kestrel", nullptr, nullptr);
    if (!m_window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create a window for OpenGL session.");
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Load the default/basic sprite shader from 'GLSL #0 and #1'
    m_sprite_shader = std::make_shared<opengl::shader>(0, 1);
    m_sprite_renderer = opengl::sprite_renderer(std::dynamic_pointer_cast<opengl::shader>(m_sprite_shader));

    m_alive = true;

}

// MARK: - Configuration

auto graphics::opengl::session_window::configure_viewport(GLdouble width, GLdouble height) -> void
{
    glViewport(0, 0, width, height);
    glm::mat4 projection = glm::ortho(0.0, width, height, 0.0, -1.0, 0.0);

}

auto graphics::opengl::session_window::set_title(const std::string& title) -> void
{
    glfwSetWindowTitle(m_window, title.c_str());
}

auto graphics::opengl::session_window::set_size(const math::size& size) -> void
{
    glfwSetWindowSize(m_window, static_cast<int>(size.width), static_cast<int>(size.height));
}

// MARK: - Rendering

auto graphics::opengl::session_window::render() -> void
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    graphics::session_window::render();

    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

// MARK: - Scene Management

auto graphics::opengl::session_window::new_scene(const scripting::lua::script &script) -> std::shared_ptr<graphics::scene>
{
    return std::make_shared<graphics::opengl::scene>(shared_from_this(), script);
}

// MARK: - Helpers

auto graphics::opengl::session_window::create_texture(const math::size &size,
                                                      std::vector<uint32_t> data) const -> std::shared_ptr<graphics::texture>
{
    return std::make_shared<graphics::texture>(size, std::move(data));
}