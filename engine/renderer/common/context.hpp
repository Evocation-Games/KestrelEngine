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

#include <string>
#include <functional>
#include "util/uuid.hpp"
#include "renderer/common/shader.hpp"
#include "renderer/common/render_pass.hpp"
#include <libGraphite/data/data.hpp>

namespace renderer
{
    class context
    {
    public:

        virtual auto enable_imgui() -> void = 0;
        virtual auto disable_imgui() -> void = 0;
        [[nodiscard]] virtual auto is_imgui_enabled() const -> bool = 0;

        virtual auto create_shader_library(const std::string& source) -> void = 0;
        virtual auto add_shader_program(const std::string& name, const std::string& vertex, const std::string& fragment) -> std::shared_ptr<shader::program> = 0;
        virtual auto shader_program(const std::string& name) -> std::shared_ptr<shader::program> = 0;

        virtual auto start_frame(const render_pass *pass = nullptr, bool imgui = false) -> void = 0;
        virtual auto finalize_frame(const std::function<auto()->void>& callback) -> void = 0;

        virtual auto draw(const draw_buffer *buffer) -> void = 0;

        virtual auto create_framebuffer(const math::size& size) -> renderer::framebuffer * = 0;

        virtual auto create_texture(uint64_t handle, const math::size& size) -> std::shared_ptr<graphics::texture> = 0;
        virtual auto create_texture(const graphite::data::block& data, const math::size& size) -> std::shared_ptr<graphics::texture> = 0;
        virtual auto create_texture(void *data, const math::size& size) -> std::shared_ptr<graphics::texture> = 0;

        virtual auto set_tick_function(const std::function<auto()->void>& callback) -> void = 0;
        virtual auto tick() -> void = 0;

        virtual auto set_viewport_size(const math::size& viewport_size) -> void = 0;
        virtual auto viewport_size() const -> math::size = 0;

        virtual auto set_viewport_title(const std::string& title) -> void = 0;
        virtual auto viewport_title() const -> std::string = 0;
    };
}