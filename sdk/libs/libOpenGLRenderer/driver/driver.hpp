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

#include <functional>
#include <libRenderCore/callback.hpp>
#include <libRenderCore/driver/api_info.hpp>
#include <libRenderCore/driver/display_configuration.hpp>
#include <libRenderCore/driver/api_bindings.hpp>
#include <libRenderCore/shader/library.hpp>
#include <libRenderCore/shader/program.hpp>

struct GLFWwindow;

namespace renderer::opengl
{
    struct state;

    class driver
    {
    public:
        explicit driver(display_configuration cfg);
        static auto api_info() -> renderer::api_info;

        [[nodiscard]] auto api_bindings() -> renderer::api::bindings;

    private:
        auto initialize_opengl() -> bool;
        auto start(renderer::callback frame_request_callback) -> void;

        auto detect_display_configuration() -> void;
        auto configure_main_window() -> void;
        auto configure_device() -> void;
        auto dispatch_render_thread() -> void;
        auto render_job() -> void;

        static auto error_handler(std::int32_t code, const char *reason) -> void;

        auto compile_shader_library(const shader::library& lib) -> shader::program;
        [[nodiscard]] auto shader_program_named(const std::string& name) const -> shader::program;
        auto install_default_shader() -> shader::program;

        auto end_frame(renderer::callback cb) -> void;
        auto draw(const buffer& buffer) -> void;

    private:
        state *m_state { nullptr };
        display_configuration m_config;
    };
}