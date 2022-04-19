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

#include "renderer/opengl/context.hpp"

#include "core/event/event.hpp"
#include "core/environment.hpp"
#include "renderer/common/vertex.hpp"
#include "renderer/opengl/default_shader.hpp"
#include "renderer/opengl/swap_chain.hpp"
#include "renderer/opengl/framebuffer.hpp"
#include "renderer/opengl/texture.hpp"
#include "util/uuid.hpp"

#include "imgui/backends/imgui_impl_opengl3.h"
#include "imgui/backends/imgui_impl_glfw.h"

// MARK: - Globals

static struct {
    renderer::opengl::context *context { nullptr };
} s_opengl;

// MARK: - Construction / Initialisation

renderer::opengl::context::context(const std::function<auto() -> void> &callback)
{
    s_opengl.context = this;

    if (!glfwInit()) {
        // TODO: Handle gracefully...
        exit(1);
    }
    glfwSetErrorCallback(&opengl::context::error_handler);

#if !TARGET_MACOS
    link_api();
#endif

    detect_display_configuration();
    configure_window();
    configure_vertex_buffer();
    bind_event_handlers();

    callback();
}

// MARK: - Deinitialisation

renderer::opengl::context::~context()
{
}

// MARK: - ImGUI

auto renderer::opengl::context::enable_imgui() -> void
{
    const char *glsl_version = "#version 330";
    ImGui_ImplGlfw_InitForOpenGL(m_screen.window, false)
        && ImGui_ImplOpenGL3_Init(glsl_version);
}

auto renderer::opengl::context::disable_imgui() -> void
{
    m_imgui.enabled = false;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
}

// MARK: - OpenGL Linking

auto renderer::opengl::context::link_api() -> void
{
#if !TARGET_MACOS
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        const char *str = (const char *)glewGetErrorString(err);
        printf("%s\n", str);
        exit(1);
    }
    if (!GLEW_VERSION_2_1) {
        const char *str = (const char *)glewGetErrorString(err);
        printf("%s\n", str);
        exit(1);
    }
#endif
}

// MARK: - Configuration

auto renderer::opengl::context::configure_vertex_buffer() -> void
{
    glGenVertexArrays(1, &m_opengl.vao);

    glGenBuffers(1, &m_opengl.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_opengl.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex) * constants::max_quads * 6, nullptr, GL_DYNAMIC_DRAW);

    glBindVertexArray(m_opengl.vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (const void *)offsetof(vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), (const void *)offsetof(vertex, color));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (const void *)offsetof(vertex, tex_coord));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(vertex), (const void *) offsetof(vertex, texture));
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    for (auto i = 0; i < constants::texture_slots; ++i) {
        m_opengl.texture_samplers[i] = i;
    }

    glEnable(GL_BLEND);

    for (auto i = 0; i < constants::swap_count; ++i) {
        m_swap.passes[i] = new opengl::swap_chain(m_screen.window, m_opengl.texture_samplers, constants::texture_slots, m_opengl.vao, m_opengl.vbo);
        reinterpret_cast<opengl::swap_chain *>(m_swap.passes[i])->set_projection(m_opengl.projection);
    }
}

auto renderer::opengl::context::bind_event_handlers() -> void
{
    glfwSetCharCallback(m_screen.window, &character_typed_event);
    glfwSetKeyCallback(m_screen.window, &key_event);
    glfwSetCursorPosCallback(m_screen.window, &mouse_moved_event);
    glfwSetMouseButtonCallback(m_screen.window, &mouse_button_event);
}

// MARK: - Display Configuration

auto renderer::opengl::context::detect_display_configuration() -> void
{
    m_screen.monitor = glfwGetPrimaryMonitor();

#if !TARGET_MACOS
    float x_scale = 1;
    float y_scale = 1;
    glfwGetMonitorContentScale(m_screen.monitor, &x_scale, &y_scale);
    if (x_scale > 1 || y_scale > 1) {
        m_screen.dpi_factor = x_scale;
    }
#endif
}

// MARK: - Window Configuration

auto renderer::opengl::context::configure_window() -> void
{
#if TARGET_MACOS
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (m_screen.dpi_factor > 1) {
        glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
    }
#else
    // TODO: Investigate OpenGL 4 for Linux and Windows.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    if (m_screen.dpi_factor > 1) {
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    }
#endif
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    m_opengl.title = "Kestrel [OpenGL]";
    m_screen.window = glfwCreateWindow(m_opengl.viewport_width, m_opengl.viewport_height, m_opengl.title.c_str(), nullptr, nullptr);
    if (!m_screen.window) {
        // TODO: Handle...
        glfwTerminate();
        exit(1);
    }

    glfwSetFramebufferSizeCallback(m_screen.window, &opengl::context::window_resized);
    window_resized(m_screen.window, m_opengl.viewport_width, m_opengl.viewport_height);
    glfwMakeContextCurrent(m_screen.window);

    glfwSwapInterval(1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Setup the default shaders
    add_shader_program("basic", s_default_opengl_vertex_shader, s_default_opengl_fragment_shader);

    // Setup a default projection
    m_opengl.projection = glm::ortho(0.0, (double)m_opengl.viewport_width, (double)m_opengl.viewport_height, 0.0, 1.0, -1.0);
}

auto renderer::opengl::context::window_resized(GLFWwindow *window, GLint width, GLint height) -> void
{
    // TODO: Handle this better?
    glViewport(0, 0, width, height);
}

auto renderer::opengl::context::set_viewport_size(const math::size &viewport_size) -> void
{
    m_opengl.viewport_width = static_cast<uint32_t>(viewport_size.width);
    m_opengl.viewport_height = static_cast<uint32_t>(viewport_size.height);

    glfwSetWindowSize(m_screen.window, m_opengl.viewport_width, m_opengl.viewport_height);
}

auto renderer::opengl::context::viewport_size() const -> math::size
{
    return { m_opengl.viewport_width, m_opengl.viewport_height };
}

auto renderer::opengl::context::set_viewport_title(const std::string &title) -> void
{
    glfwSetWindowTitle(m_screen.window, title.c_str());
    m_opengl.title = title;
}

auto renderer::opengl::context::viewport_title() const -> std::string
{
    return m_opengl.title;
}


// MARK: - Events

auto renderer::opengl::context::character_typed_event(GLFWwindow *window, unsigned int c) -> void
{
    auto e = event::key(event::key_typed, hid::key::unknown, c);
    if (auto owner = environment::active_environment().lock()) {
        owner->post_event(e);
    }
}

auto renderer::opengl::context::key_event(GLFWwindow *window, int code, int scancode, int action, int mods) -> void
{
    auto key = map_keycode(code);
    auto modifier_flags = static_cast<uint32_t>(map_modifiers(mods));
    auto env = environment::active_environment().lock();

    switch (action) {
        case GLFW_PRESS: {
            auto e = event::key(static_cast<enum event::type>(event::key_down | modifier_flags), key);
            env->post_event(e);
            break;
        }
        case GLFW_RELEASE: {
            auto e = event::key(static_cast<enum event::type>(event::key_up | modifier_flags), key);
            env->post_event(e);
            break;
        }
        case GLFW_REPEAT: {
            auto e = event::key(static_cast<enum event::type>(event::key_repeat | modifier_flags), key);
            env->post_event(e);
            break;
        }
        default: break;
    }
}

auto renderer::opengl::context::mouse_moved_event(GLFWwindow *window, double x, double y) -> void
{
    auto e = event::mouse(event::mouse_move, { x, y });
    if (auto env = environment::active_environment().lock()) {
        env->post_event(e);
    }
}

auto renderer::opengl::context::mouse_button_event(GLFWwindow *window, int button, int action, int mods) -> void
{
    auto env = environment::active_environment().lock();
    double x, y;

    glfwGetCursorPos(window, &x, &y);

    switch (action) {
        case GLFW_PRESS: {
            switch (button) {
                case GLFW_MOUSE_BUTTON_LEFT: {
                    auto e = event::mouse(event::lmb_down, { x, y });
                    env->post_event(e);
                    break;
                }
                case GLFW_MOUSE_BUTTON_MIDDLE: {
                    auto e = event::mouse(event::mmb_down, { x, y });
                    env->post_event(e);
                    break;
                }
                case GLFW_MOUSE_BUTTON_RIGHT: {
                    auto e = event::mouse(event::rmb_down, { x, y });
                    env->post_event(e);
                    break;
                }
                default: break;
            }
            break;
        }
        case GLFW_RELEASE: {
            switch (button) {
                case GLFW_MOUSE_BUTTON_LEFT: {
                    auto e = event::mouse(event::lmb_up, { x, y });
                    env->post_event(e);
                    break;
                }
                case GLFW_MOUSE_BUTTON_MIDDLE: {
                    auto e = event::mouse(event::mmb_up, { x, y });
                    env->post_event(e);
                    break;
                }
                case GLFW_MOUSE_BUTTON_RIGHT: {
                    auto e = event::mouse(event::rmb_up, { x, y });
                    env->post_event(e);
                    break;
                }
                default: break;
            }
            break;
        }
        default: break;
    }
}

// MARK: - Error

auto renderer::opengl::context::error_handler(GLint code, const char *reason) -> void
{
    // TODO: Handle this better?
    throw std::runtime_error(reason);
}

// MARK: - Frame Life Cycle

auto renderer::opengl::context::start_frame(const render_pass *pass, bool imgui) -> void
{
    auto active_pass = const_cast<render_pass *>(pass);

    if (!pass) {
        auto pass_index = m_swap.index;
        m_swap.index = (pass_index + 1) % constants::swap_count;

        auto& render_pass = m_swap.passes[pass_index];
        render_pass->start();

        m_swap.ptr = render_pass;
        active_pass = const_cast<renderer::render_pass *>(reinterpret_cast<renderer::render_pass *>(render_pass));
    }

    if (imgui) {
        active_pass->start_imgui();
    }

    m_imgui.enabled = imgui;
}

auto renderer::opengl::context::finalize_frame(const std::function<auto() -> void> &callback) -> void
{
    renderer::render_pass *render_pass = m_swap.ptr ?: m_swap.passes[m_swap.index];

    if (m_imgui.enabled) {
        render_pass->finalize_imgui();
        m_imgui.enabled = false;
    }

    render_pass->finalize([] {
        glfwPollEvents();
    });

    m_swap.ptr = nullptr;
}

auto renderer::opengl::context::draw(const draw_buffer *buffer) -> void
{
    renderer::render_pass *render_pass = m_swap.ptr ?: m_swap.passes[m_swap.index];
    render_pass->draw(buffer);
}

// MARK: - Shaders

auto renderer::opengl::context::create_shader_library(const std::string &source) -> void
{
    // TODO: Investigate shader libraries in OpenGL
}

auto renderer::opengl::context::add_shader_program(const std::string &name, const std::string &vertex_function, const std::string &fragment_function) -> std::shared_ptr<shader::program>
{
    auto vertex = compile_vertex_shader(vertex_function.c_str());
    auto fragment = compile_fragment_shader(fragment_function.c_str());
    auto program_handle = link_shader_program(vertex, fragment);

    util::uuid id { name };
    auto program = std::make_shared<shader::program>(program_handle);
    m_opengl.shader_programs.insert(std::pair(id, program));

    return shader_program(name);
}

auto renderer::opengl::context::shader_program(const std::string &name) -> std::shared_ptr<shader::program>
{
    util::uuid id { name };

    auto it = m_opengl.shader_programs.find(id);
    if (it == m_opengl.shader_programs.end()) {
        // TODO: Handle this error
        abort();
    }

    return it->second;
}

auto renderer::opengl::context::compile_vertex_shader(const char *source) -> GLuint
{
    GLuint shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success = 0;
    char log[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 1024, nullptr, log);
        throw std::runtime_error(log);
    }

    return shader;
}

auto renderer::opengl::context::compile_fragment_shader(const char *source) -> GLuint
{
    GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success = 0;
    char log[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 1024, nullptr, log);
        throw std::runtime_error(log);
    }

    return shader;
}

auto renderer::opengl::context::link_shader_program(GLuint vertex, GLuint fragment) -> GLuint
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);

    GLint success = 0;
    char log[1024];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 1024, nullptr, log);
        throw std::runtime_error(log);
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return program;
}

// MARK: - Framebuffer

auto renderer::opengl::context::create_framebuffer(const math::size &size) -> renderer::framebuffer *
{
    auto fb = new renderer::opengl::framebuffer(m_opengl.viewport_width, m_opengl.viewport_height, m_opengl.texture_samplers, constants::texture_slots, m_opengl.vao, m_opengl.vbo);

    return fb;
}

// MARK: - Textures

auto renderer::opengl::context::create_texture(uint64_t handle, const math::size &size) -> std::shared_ptr<graphics::texture>
{
    return std::make_shared<opengl::texture>(handle, size);
}

auto renderer::opengl::context::create_texture(const std::vector<uint32_t> &data, const math::size &size) -> std::shared_ptr<graphics::texture>
{
    auto texture =  create_texture(const_cast<void *>(reinterpret_cast<const void *>(&data[0])), size);
    texture->set_data(data);
    return texture;
}

auto renderer::opengl::context::create_texture(void *data, const math::size &size) -> std::shared_ptr<graphics::texture>
{
    GLuint texture { 0 };
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(size.width), static_cast<GLsizei>(size.height), 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    auto out = create_texture(static_cast<uint64_t>(texture), size);
    out->set_raw_data_ptr(reinterpret_cast<const uint8_t *>(data));
    return out;
}

// MARK: - Tick Function

auto renderer::opengl::context::set_tick_function(const std::function<auto()->void>& callback) -> void
{
    m_opengl.tick = callback;
}

auto renderer::opengl::context::tick() -> void
{
    m_opengl.tick();
}

// MARK: - Key Mappings

auto renderer::opengl::context::map_keycode(int scancode) -> hid::key
{
    switch (scancode) {
        // Special Keys
        case GLFW_KEY_ESCAPE:               return hid::key::escape;
        case GLFW_KEY_ENTER:                return hid::key::enter;
        case GLFW_KEY_BACKSPACE:            return hid::key::backspace;
        case GLFW_KEY_TAB:                  return hid::key::tab;

        // Cursor Keys
        case GLFW_KEY_LEFT:                 return hid::key::left;
        case GLFW_KEY_RIGHT:                return hid::key::right;
        case GLFW_KEY_UP:                   return hid::key::up;
        case GLFW_KEY_DOWN:                 return hid::key::down;

        // Letters
        case GLFW_KEY_A:                    return hid::key::a;
        case GLFW_KEY_B:                    return hid::key::b;
        case GLFW_KEY_C:                    return hid::key::c;
        case GLFW_KEY_D:                    return hid::key::d;
        case GLFW_KEY_E:                    return hid::key::e;
        case GLFW_KEY_F:                    return hid::key::f;
        case GLFW_KEY_G:                    return hid::key::g;
        case GLFW_KEY_H:                    return hid::key::h;
        case GLFW_KEY_I:                    return hid::key::i;
        case GLFW_KEY_J:                    return hid::key::j;
        case GLFW_KEY_K:                    return hid::key::k;
        case GLFW_KEY_L:                    return hid::key::l;
        case GLFW_KEY_M:                    return hid::key::m;
        case GLFW_KEY_N:                    return hid::key::n;
        case GLFW_KEY_O:                    return hid::key::o;
        case GLFW_KEY_P:                    return hid::key::p;
        case GLFW_KEY_Q:                    return hid::key::q;
        case GLFW_KEY_R:                    return hid::key::r;
        case GLFW_KEY_S:                    return hid::key::s;
        case GLFW_KEY_T:                    return hid::key::t;
        case GLFW_KEY_U:                    return hid::key::u;
        case GLFW_KEY_V:                    return hid::key::v;
        case GLFW_KEY_W:                    return hid::key::w;
        case GLFW_KEY_X:                    return hid::key::x;
        case GLFW_KEY_Y:                    return hid::key::y;
        case GLFW_KEY_Z:                    return hid::key::z;

        // Numbers
        case GLFW_KEY_0:
        case GLFW_KEY_KP_0:                 return hid::key::kp_0;
        case GLFW_KEY_1:
        case GLFW_KEY_KP_1:                 return hid::key::kp_1;
        case GLFW_KEY_2:
        case GLFW_KEY_KP_2:                 return hid::key::kp_2;
        case GLFW_KEY_3:
        case GLFW_KEY_KP_3:                 return hid::key::kp_3;
        case GLFW_KEY_4:
        case GLFW_KEY_KP_4:                 return hid::key::kp_4;
        case GLFW_KEY_5:
        case GLFW_KEY_KP_5:                 return hid::key::kp_5;
        case GLFW_KEY_6:
        case GLFW_KEY_KP_6:                 return hid::key::kp_6;
        case GLFW_KEY_7:
        case GLFW_KEY_KP_7:                 return hid::key::kp_7;
        case GLFW_KEY_8:
        case GLFW_KEY_KP_8:                 return hid::key::kp_8;
        case GLFW_KEY_9:
        case GLFW_KEY_KP_9:                 return hid::key::kp_9;

        // Function Keys
        case GLFW_KEY_F1:                   return hid::key::f1;
        case GLFW_KEY_F2:                   return hid::key::f2;
        case GLFW_KEY_F3:                   return hid::key::f3;
        case GLFW_KEY_F4:                   return hid::key::f4;
        case GLFW_KEY_F5:                   return hid::key::f5;
        case GLFW_KEY_F6:                   return hid::key::f6;
        case GLFW_KEY_F7:                   return hid::key::f7;
        case GLFW_KEY_F8:                   return hid::key::f8;
        case GLFW_KEY_F9:                   return hid::key::f9;
        case GLFW_KEY_F10:                  return hid::key::f10;
        case GLFW_KEY_F11:                  return hid::key::f11;
        case GLFW_KEY_F12:                  return hid::key::f12;

        // Misc
        case GLFW_KEY_SPACE:                return hid::key::space;
        case GLFW_KEY_LEFT_BRACKET:         return hid::key::left_bracket;
        case GLFW_KEY_RIGHT_BRACKET:        return hid::key::right_bracket;
        case GLFW_KEY_APOSTROPHE:           return hid::key::apostrophe;
        case GLFW_KEY_PERIOD:               return hid::key::period;
        case GLFW_KEY_COMMA:                return hid::key::comma;
        case GLFW_KEY_MINUS:                return hid::key::minus;
        case GLFW_KEY_EQUAL:                return hid::key::equal;

        // Modifiers
        case GLFW_KEY_LEFT_SHIFT:           return hid::key::left_shift;
        case GLFW_KEY_RIGHT_SHIFT:          return hid::key::right_shift;
        case GLFW_KEY_LEFT_ALT:             return hid::key::left_alt;
        case GLFW_KEY_RIGHT_ALT:            return hid::key::right_alt;
        case GLFW_KEY_LEFT_CONTROL:         return hid::key::left_control;
        case GLFW_KEY_RIGHT_CONTROL:        return hid::key::right_control;
        case GLFW_KEY_LEFT_SUPER:           return hid::key::left_super;
        case GLFW_KEY_RIGHT_SUPER:          return hid::key::right_super;

        // Unknown
        default:                            return hid::key::unknown;
    }
}

auto renderer::opengl::context::map_modifiers(int modifiers) -> enum event::type
{
    uint32_t modifier_flags = 0;

    if (modifiers & GLFW_MOD_SHIFT) {
        modifier_flags |= static_cast<uint32_t>(event::type::has_shift_modifier);
    }
    else if (modifiers & GLFW_MOD_ALT) {
        modifier_flags |= static_cast<uint32_t>(event::type::has_alt_modifier);
    }
    else if (modifiers & GLFW_MOD_CAPS_LOCK) {
        modifier_flags |= static_cast<uint32_t>(event::type::has_caps_lock_modifier);
    }
    else if (modifiers & GLFW_MOD_CONTROL) {
        modifier_flags |= static_cast<uint32_t>(event::type::has_control_modifier);
    }
    else if (modifiers & GLFW_MOD_SUPER) {
        modifier_flags |= static_cast<uint32_t>(event::type::has_super_modifier);
    }

    return static_cast<enum event::type>(modifier_flags);
}