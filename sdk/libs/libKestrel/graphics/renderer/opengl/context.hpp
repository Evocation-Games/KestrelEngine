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

#include <array>
#include <functional>
#include <unordered_map>
#include <libKestrel/util/availability.hpp>
#include <libKestrel/util/uid.hpp>
#include <libKestrel/event/event.hpp>
#include <libKestrel/graphics/renderer/opengl/opengl.hpp>
#include <libKestrel/graphics/renderer/opengl/constants.hpp>
#include <libKestrel/graphics/renderer/common/shader/program.hpp>
#include <libKestrel/graphics/renderer/common/context.hpp>
#include <libKestrel/graphics/renderer/common/render_pass.hpp>
#include <libData/block.hpp>

namespace kestrel::renderer::opengl
{
    class context : public renderer::context
    {
    public:
        explicit context(const std::function<auto()->void>& callback);
        ~context() = default;

        auto enable_imgui() -> void override;
        auto disable_imgui() -> void override;
        [[nodiscard]] inline auto is_imgui_enabled() const -> bool override { return m_imgui.enabled; }

        auto create_shader_library(const std::string& name, const std::string& source) -> void override;
        auto create_shader_library(const std::string& name, const std::string& vertex_function, const std::string& fragment_function) -> std::shared_ptr<renderer::shader::program> override;
        auto add_shader_program(const std::string& name, const std::string& vertex_function, const std::string& fragment_function) -> std::shared_ptr<shader::program> override;
        auto shader_program(const std::string& name) -> std::shared_ptr<shader::program> override;

        auto start_frame(const render_pass *pass = nullptr, bool imgui = false) -> void override;
        auto finalize_frame(const std::function<auto() -> void>& callback) -> void override;

        auto draw(const draw_buffer *buffer) -> void override;

        auto create_framebuffer(const math::size& size) -> renderer::framebuffer * override;
        auto create_texture(const data::block& data, const math::size& size) -> std::shared_ptr<graphics::texture> override;

        auto set_tick_function(const std::function<auto()->void>& callback) -> void override;
        auto tick() -> void override;

        auto set_viewport_size(const math::size& viewport_size) -> void override;
        [[nodiscard]] auto viewport_size() const -> math::size override;
        [[nodiscard]] auto scaled_viewport_size() const -> math::size override;

        auto set_viewport_title(const std::string& title) -> void override;
        [[nodiscard]] auto viewport_title() const -> std::string override;

        [[nodiscard]] auto native_screen_scale() const -> float override;
        [[nodiscard]] auto native_screen_size() const -> math::size override;
        auto set_fullscreen(bool f) -> void override;

    private:
        auto configure_device() -> void {};

        auto link_api() -> void;

        auto detect_display_configuration() -> void;
        auto configure_window() -> void;
        auto configure_vertex_buffer() -> void;
        auto bind_event_handlers() -> void;

        auto compile_vertex_shader(const char *source) -> GLuint;
        auto compile_fragment_shader(const char *source) -> GLuint;
        auto link_shader_program(GLuint vertex, GLuint fragment) -> GLuint;

        static auto error_handler(GLint code, const char *reason) -> void;
        static auto window_resized(GLFWwindow *window, GLint width, GLint height) -> void;
        static auto character_typed_event(GLFWwindow *window, unsigned int c) -> void;
        static auto key_event(GLFWwindow *window, int code, int scancode, int action, int mods) -> void;
        static auto mouse_moved_event(GLFWwindow *window, double x, double y) -> void;
        static auto mouse_button_event(GLFWwindow *window, int button, int action, int mods) -> void;

        static auto map_keycode(int scancode) -> ::ui::hid::key;
        static auto map_modifiers(int modifiers) -> enum ::ui::event::type;

    private:
        struct {
            GLFWmonitor *monitor { nullptr };
            GLFWwindow *window { nullptr };
            float dpi_factor { 1.f };
        } m_screen;

        struct {
            GLuint vao { 0 };
            GLuint vbo { 0 };
            std::int32_t viewport_width { 1440 };
            std::int32_t viewport_height { 900 };
            std::int32_t scaled_viewport_width { 1440 };
            std::int32_t scaled_viewport_height { 900 };
            glm::mat4 projection;
            GLint texture_samplers[constants::texture_slots] { 0 };
            std::unordered_map<util::uid, std::shared_ptr<renderer::shader::program>> shader_programs;
            std::function<auto()->void> tick;
            std::string title;
        } m_opengl;

        struct {
            std::array<renderer::swap_chain *, constants::swap_count> passes;
            renderer::render_pass *ptr { nullptr };
            std::uint8_t index { 0 };
        } m_swap;

        struct {
            bool enabled { false };
        } m_imgui;
    };
}