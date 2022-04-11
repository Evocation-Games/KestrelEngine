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

#include "util/hint.hpp"

#if TARGET_MACOS

#include "renderer/metal/constants.h"

#include <array>
#include <unordered_map>
#include "renderer/common/shader.hpp"
#include "renderer/common/context.hpp"
#include "renderer/common/render_pass.hpp"
#include "util/uuid.hpp"

#if __OBJC__ || __OBJC2__
#   include <Cocoa/Cocoa.h>
#   include <MetalKit/MetalKit.h>
#   define MTLDeviceRef                     id<MTLDevice>
#   define MTLLibraryRef                    id<MTLLibrary>
#   define MTLCommandQueueRef               id<MTLCommandQueue>
#else
#   define NSWindow                         void
#   define CAMetalLayer                     void
#   define MTLDeviceRef                     void *
#   define MTLLibraryRef                    void *
#   define MTLCommandQueueRef               void *
#   define dispatch_semaphore_t             void *
#   define dispatch_source_t                void *
#   define CVDisplayLinkRef                 void *
#   define MTLCreateSystemDefaultDevice()   nullptr
#   define dispatch_semaphore_create(...)   nullptr
#endif

namespace renderer::metal
{
    class context : public renderer::context
    {
    public:
        static auto start_application(const std::function<auto(metal::context *)->void>& callback) -> void;
        static auto supports_metal() -> bool;

        explicit context(const std::function<auto()->void>& callback);
        ~context();

        auto enable_imgui() -> void override;
        auto disable_imgui() -> void override;
        [[nodiscard]] inline auto is_imgui_enabled() const -> bool override { return m_imgui.enabled; }

        auto create_shader_library(const std::string& source) -> void override;
        auto add_shader_program(const std::string& name, const std::string& vertex_function_name, const std::string& fragment_function_name) -> std::shared_ptr<shader::program> override;
        auto shader_program(const std::string& name) -> std::shared_ptr<shader::program> override;

        auto start_frame(const render_pass *pass = nullptr, bool imgui = false) -> void override;
        auto finalize_frame(const std::function<auto() -> void>& callback) -> void override;

        auto draw(const draw_buffer *buffer) -> void override;

        auto create_framebuffer(const math::size& size) -> renderer::framebuffer * override;

        auto create_texture(uint64_t handle, const math::size& size) -> std::shared_ptr<graphics::texture> override;
        auto create_texture(const std::vector<uint32_t>& data, const math::size& size) -> std::shared_ptr<graphics::texture> override;
        auto create_texture(void *data, const math::size& size) -> std::shared_ptr<graphics::texture> override;

        auto set_tick_function(const std::function<auto()->void>& callback) -> void override;

    private:
        auto configure_device() -> void;
        auto tick() -> void;

    private:
        NSWindow *m_window { nullptr };
        CAMetalLayer *m_output_layer { nullptr };

        struct {
            MTLDeviceRef device { MTLCreateSystemDefaultDevice() };
            MTLLibraryRef library { nullptr };
            MTLCommandQueueRef command_queue { nullptr };
            std::unordered_map<util::uuid, std::shared_ptr<shader::program>> shader_programs;
            uint32_t viewport_width { 1280 };
            uint32_t viewport_height { 720 };
        } m_metal;

        struct {
            std::array<renderer::swap_chain *, constants::swap_count> passes;
            renderer::swap_chain *ptr { nullptr };
            dispatch_semaphore_t sema { dispatch_semaphore_create(constants::swap_count) };
            uint8_t index { 0 };
        } m_swap;

        struct {
            dispatch_source_t source;
            CVDisplayLinkRef link_out;
            std::function<auto()->void> tick;
        } m_display;

        struct {
            bool enabled { false };
        } m_imgui;

    };
}

#endif
