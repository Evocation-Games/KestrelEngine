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
#include <unordered_map>
#include <libOpenGLRenderer/driver/driver.hpp>
#include <libOpenGLRenderer/opengl.hpp>
#include <libOpenGLRenderer/resource/shader/compiler.hpp>
#include <libOpenGLRenderer/resource/shader/shader_program.hpp>
#include <libOpenGLRenderer/resource/shader/default_shader.hpp>
#include <libOpenGLRenderer/driver/frame_generator.hpp>
#include <libOpenGLRenderer/driver/context.hpp>
#include <libOpenGLRenderer/driver/output.hpp>
#include <libOpenGLRenderer/driver/event_handler.hpp>
#include <libOpenGLRenderer/resource/texture.hpp>
#include <libFoundation/system/task/spin.hpp>

#if TARGET_MACOS
#   include <libMacOS/cocoa/screen.h>
#endif

// MARK: - Context

struct {
    bool initialized { false };
    renderer::opengl::driver *current_driver { nullptr };
} s_opengl_instance;

namespace renderer::opengl
{
    struct state
    {
        struct {
            GLFWmonitor *monitor { nullptr };
            float native_scale_factor { 0.f };
        } screen;

        struct {
            struct {
                renderer::callback frame_request;
                renderer::callback completion;
                context main;
                output main_output;
                context background;
                std::thread runner;
                frame_generator generator;
                bool should_terminate { false };
                foundation::task::spin_lock resource_lock;
            } render;

            struct {
                std::uint32_t width { 0 };
                std::uint32_t height { 0 };
                glm::mat4 projection { 0 };
            } viewport;

            struct {
                resource::shader::compiler compiler;
                std::uint64_t default_id { 0 };
                std::unordered_map<std::uint64_t, resource::shader::program> programs;
            } shader;

            struct {
                std::uint64_t next_id { 0 };
                std::unordered_map<std::uint64_t, texture> map;
            } textures;
        } opengl;
    };
}

// MARK: - Construction

renderer::opengl::driver::driver(renderer::display_configuration cfg)
    : m_config(cfg), m_state(new state())
{
    // Attempt to initialize OpenGL.
    if (!s_opengl_instance.initialized && !initialize_opengl()) {
        // TODO:: Handle failures gracefully.
        exit(1);
    }
}

// MARK: - Configuration

auto renderer::opengl::driver::initialize_opengl() -> bool
{
    if (s_opengl_instance.initialized) {
        // TODO: Issue a warning here?
        return true;
    }

    if (!glfwInit()) {
        return false;
    }
    glfwSetErrorCallback(&driver::error_handler);
    s_opengl_instance.initialized = true;

    // Do the initial setup and preparation.
    detect_display_configuration();
    configure_main_window();

    return true;
}

auto renderer::opengl::driver::initialize() -> void
{

}

auto renderer::opengl::driver::detect_display_configuration() -> void
{
    m_state->screen.monitor = glfwGetPrimaryMonitor();

#if !TARGET_MACOS
    float x_scale = 1.f;
    float y_scale = 1.f;
    glfwGetMonitorContentScale(m_state->screen.monitor, &x_scale, &y_scale);
    m_state->screen.native_scale_factor = x_scale;
#else
    m_state->screen.native_scale_factor = macos::cocoa::screen::scale_factor();
#endif
}

auto renderer::opengl::driver::configure_main_window() -> void
{
    m_state->opengl.viewport.width = (GLint)m_config.output_width();
    m_state->opengl.viewport.height = (GLint)m_config.output_height();
    m_state->opengl.render.main.initialize(
        m_state->opengl.viewport.width,
        m_state->opengl.viewport.height,
        m_state->screen.native_scale_factor
    );

    if (!m_state->opengl.render.main.is_valid()) {
        // TODO: Handle...
        glfwTerminate();
        exit(1);
    }

    m_state->opengl.render.main.make_current();

    glfwSwapInterval(1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#if !TARGET_MACOS
    GLenum err = glewInit();
    if (err != GLEW_OK || !GLEW_VERSION_2_1) {
        const char *str = (const char *)glewGetErrorString(err);
        throw std::runtime_error(str);
    }
#endif

    m_state->opengl.viewport.projection = glm::ortho(0.0, (double)m_state->opengl.viewport.width, 0.0, (double)m_state->opengl.viewport.height, 1.0, -1.0);
}

auto renderer::opengl::driver::configure_device() -> void
{
    // Setup event handlers
    event::receiver::initialize(m_state->opengl.render.main.window());

    // Shaders
    install_default_shader();
}

// MARK: - API

auto renderer::opengl::driver::api_info() -> renderer::api_info
{
    const GLubyte *renderer = glGetString(GL_RENDERER);
    return { "OpenGL", (const char *)renderer ?: "Unknown" };
}

auto renderer::opengl::driver::api_bindings() -> renderer::api::bindings
{
    api::bindings bindings;
    // Core
    bindings.core.initialize = [&] { initialize(); };
    bindings.core.start = [&] (auto frame_request) { start(std::move(frame_request)); };

    // Configuration

    // Frame Generation
    bindings.frame_generation.finish = [&] (auto callback) { end_frame(std::move(callback)); };
    bindings.frame_generation.submit_draw_buffer = [&] (const auto& buffer) { draw(buffer); };

    // Textures
    bindings.texture.create = [&] (const auto& data, auto size) { return create_texture(data, size); };
    bindings.texture.update = [&] (auto id, const auto& data) { update_texture(id, data); };
    bindings.texture.destroy = [&] (auto id) { destroy_texture(id); };

    // Delegation
    bindings.delegate.attach_event_receiver = [&] (auto *receiver) { event::receiver::attach_receiver(receiver); };
    return bindings;
}

// MARK: - OpenGL Callbacks

auto renderer::opengl::driver::error_handler(std::int32_t code, const char *reason) -> void
{
    throw std::runtime_error("GLFW Error: " + std::string(reason));
}

// MARK: - Application Management

#define USE_OFFSCREEN_RENDERING true

auto renderer::opengl::driver::start(renderer::callback frame_request_callback) -> void
{
    m_state->opengl.render.frame_request = std::move(frame_request_callback);

    // Finish configuring the context
    configure_device();
    dispatch_render_thread();

    // Fetch the default shader program.
    m_state->opengl.render.main.make_current();
    auto default_shader = m_state->opengl.shader.programs.at(m_state->opengl.shader.default_id);
    m_state->opengl.render.main_output.initialize(
        m_state->opengl.viewport.width,
        m_state->opengl.viewport.height,
        default_shader
    );

    // Enter a running loop to keep the session alive.
    while (true) {
#if USE_OFFSCREEN_RENDERING
        // Render the latest texture from the frame generator to the main viewport.
        m_state->opengl.render.resource_lock.lock();
        m_state->opengl.render.main.make_current();
        m_state->opengl.render.main_output.push_frame(
            m_state->opengl.render.generator.latest_frame_texture(),
            m_state->opengl.render.main.window()
        );
        m_state->opengl.render.resource_lock.unlock();
#else
        m_state->opengl.render.main.make_current();
        m_state->opengl.render.generator.wait_for_ready();
        m_state->opengl.render.frame_request();
        glfwSwapBuffers(m_state->opengl.render.main.window());
#endif
        glfwPollEvents();
    };
}

// MARK: - Threading

auto renderer::opengl::driver::dispatch_render_thread() -> void
{
#if USE_OFFSCREEN_RENDERING
    // Create a new context based on the main context for use in the background runner thread.
    m_state->opengl.render.background.initialize(
        m_state->opengl.viewport.width,
        m_state->opengl.viewport.height,
        m_state->screen.native_scale_factor,
        false,
        m_state->opengl.render.main.window()
    );
    m_state->opengl.render.background.make_current();
#endif

    // Configure the frame generator
    m_state->opengl.render.generator = frame_generator(
        m_state->opengl.viewport.width,
        m_state->opengl.viewport.height,
        m_config.swap_chain_count()
    );

#if USE_OFFSCREEN_RENDERING
    m_state->opengl.render.runner = std::thread([&] {
        render_job();
    });

    // TODO: Check how thread scheduling needs to be done on Windows.
    sched_param sch { 0 };
    sch.sched_priority = 45;
    pthread_setschedparam(m_state->opengl.render.runner.native_handle(), SCHED_RR, &sch);

    m_state->opengl.render.runner.detach();
#endif
}

auto renderer::opengl::driver::render_job() -> void
{
    while (!m_state->opengl.render.should_terminate) {
        m_state->opengl.render.generator.wait_for_ready();
        m_state->opengl.render.resource_lock.lock();
        m_state->opengl.render.background.make_current();
        m_state->opengl.render.frame_request();
        m_state->opengl.render.resource_lock.unlock();
    }
}

// MARK: - Shaders

auto renderer::opengl::driver::compile_shader_library(const shader::library& lib) -> shader::program
{
    auto program = m_state->opengl.shader.compiler.compile(lib);
    auto program_uid = program.uid();
    m_state->opengl.shader.programs.insert({ program_uid, std::move(program) });

    // Finally return a program reference for the shader...
    return shader::program(program_uid);
}

auto renderer::opengl::driver::shader_program_named(const std::string& name) const -> shader::program
{
    for (const auto& it : m_state->opengl.shader.programs) {
        if (it.second.name() == name) {
            return shader::program(it.first);
        }
    }
    throw std::runtime_error("Failed to find shader program '" + name + ".");
}

auto renderer::opengl::driver::install_default_shader() -> shader::program
{
    shader::library lib(
        "lib",
        shader::function(resource::shader::default_vertex_function, shader::function::type::VERTEX, "vertex_shader"),
        shader::function(resource::shader::default_fragment_function, shader::function::type::FRAGMENT, "fragment_shader")
    );
    auto program = compile_shader_library(lib);
    m_state->opengl.shader.default_id = program.id();
    return program;
}

// MARK: - Frame Management

auto renderer::opengl::driver::end_frame(renderer::callback completion) -> void
{
    m_state->opengl.render.background.make_current();
    m_state->opengl.render.completion = std::move(completion);
    m_state->opengl.render.generator.produce_new_frame([&] {
        m_state->opengl.render.completion();
    });
}

auto renderer::opengl::driver::draw(const renderer::buffer &buffer) -> void
{
    // The first action to take is constructing a vector of textures so that we can correctly
    // bind the correct texture handles to each vertex.
    std::vector<GLuint> textures(buffer.texture_count());
    for (auto i = 0; i < buffer.texture_count(); ++i) {
        auto it = m_state->opengl.textures.map.find(buffer.device_texture_for_slot(i));
        if (it != m_state->opengl.textures.map.end()) {
            textures[i] = it->second.handle;
        }
        else {
            // TODO: Throw an error here??
            textures[i] = 0;
        }
    }

    // Next find the correct shader.
    const auto& shader = m_state->opengl.shader.programs.at(m_state->opengl.shader.default_id);

    // Forward to the swapchain.
    m_state->opengl.render.generator.current_operation().submit(buffer, textures, shader);
}

// MARK: - Texture Management

auto renderer::opengl::driver::create_texture(const data::block &data, math::vec2 size) -> renderer::texture::device_id
{
    opengl::texture texture;
    texture.device_id = m_state->opengl.textures.next_id++;
    texture.width = (GLsizei)size.x();
    texture.height = (GLsizei)size.y();

    // Acquire an ID for the texture...
    glGenTextures(1, &texture.handle);
    std::cout << "Create texture (" << texture.handle << ")" << std::endl;

    // Configure the texture
    glBindTexture(GL_TEXTURE_2D, texture.handle);
    glTexImage2D(GL_TEXTURE_2D, 0, texture.internal_format, texture.width, texture.height, 0, texture.image_format, GL_UNSIGNED_BYTE, data.get<void *>());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture.wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture.wrap_t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, texture.filter_min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture.filter_mag);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFinish();

    texture.uploaded = true;

    // Store the texture, and get a fixed ID.
    m_state->opengl.textures.map.insert({ texture.device_id, texture });
    return texture.device_id;
}

auto renderer::opengl::driver::update_texture(renderer::texture::device_id id, const data::block &data) -> void
{
    auto it = m_state->opengl.textures.map.find(id);
    if (it != m_state->opengl.textures.map.end()) {
        auto& texture = it->second;
        glBindTexture(GL_TEXTURE_2D, texture.handle);
        glTexImage2D(GL_TEXTURE_2D, 0, texture.internal_format, texture.width, texture.height, 0, texture.image_format, GL_UNSIGNED_BYTE, data.get<void *>());
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

auto renderer::opengl::driver::destroy_texture(renderer::texture::device_id id) -> void
{
    auto it = m_state->opengl.textures.map.find(id);
    if (it != m_state->opengl.textures.map.end()) {
        GLuint textures[] = { it->second.handle };
        glDeleteTextures(1, textures);
        m_state->opengl.textures.map.erase(it);
    }
}