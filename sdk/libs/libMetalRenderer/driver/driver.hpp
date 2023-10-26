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

#include <libMath/types/vec2.hpp>
#include <libRenderCore/callback.hpp>
#include <libRenderCore/driver/api_info.hpp>
#include <libRenderCore/driver/display_configuration.hpp>
#include <libRenderCore/driver/api_bindings.hpp>
#include <libRenderCore/shader/function.hpp>
#include <libRenderCore/shader/library.hpp>
#include <libRenderCore/shader/program.hpp>
#include <libRenderCore/frame/frame.hpp>
#include <libData/block.hpp>

namespace renderer::metal
{
    struct context;

    class driver
    {
    public:
        explicit driver(display_configuration cfg);
        static auto api_info() -> renderer::api_info;

        [[nodiscard]] auto api_bindings() -> renderer::api::bindings;

    private:
        auto initialize() -> void;
        auto configure_device() -> void;
        auto start(renderer::callback run_loop) -> void;

        auto render_job() -> void;

        auto set_viewport_title(const std::string&) -> void;
        auto set_viewport_size(std::uint32_t, std::uint32_t) -> void;
        [[nodiscard]] auto viewport_title() const -> std::string;
        [[nodiscard]] auto viewport_size() const -> math::vec2;

        auto compile_shader_library(const shader::library& lib) -> shader::program;
        [[nodiscard]] auto shader_program_named(const std::string& name) const -> shader::program;
        auto install_default_shader() -> shader::program;

        auto start_frame(const frame& frame) -> void;
        auto end_frame(renderer::callback cb) -> void;

        auto draw(const buffer& buffer) -> void;

        auto create_texture(const data::block& data, math::vec2 size) -> texture::device_id;
        auto update_texture(texture::device_id id, const data::block& data) -> void;
        auto destroy_texture(texture::device_id id) -> void;

    private:
        context *m_context { nullptr };
        display_configuration m_config;
    };
}