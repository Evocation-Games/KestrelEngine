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

#include <libKestrel/util/availability.hpp>

#if TARGET_MACOS

#include <array>
#include <unordered_map>
#include <libKestrel/graphics/renderer/metal/constants.h>
#include <libKestrel/graphics/renderer/common/shader/program.hpp>
#include <libKestrel/graphics/renderer/common/context.hpp>
#include <libKestrel/graphics/renderer/common/render_pass.hpp>
#include <libKestrel/util/uid.hpp>

#if USING_OBJC
#   include <Cocoa/Cocoa.h>
#   include <MetalKit/MetalKit.h>
#   define MTLDeviceRef                     id<MTLDevice>
#   define MTLLibraryRef                    id<MTLLibrary>
#   define MTLCommandQueueRef               id<MTLCommandQueue>
#   define MTLTextureRef                    id<MTLTexture>
#else
#   define NSWindow                         void
#   define CAMetalLayer                     void
#   define MTLDeviceRef                     void *
#   define MTLLibraryRef                    void *
#   define MTLCommandQueueRef               void *
#   define dispatch_semaphore_t             void *
#   define dispatch_source_t                void *
#   define CVDisplayLinkRef                 void *
#   define MTLTextureRef                    std::uintptr_t
#   define MTLCreateSystemDefaultDevice()   nullptr
#   define dispatch_semaphore_create(...)   nullptr
#endif

namespace kestrel::renderer::metal
{
    class context : public renderer::context
    {
    public:
        static auto start_application(const std::function<auto(metal::context *)->void>& callback) -> void;
        static auto supports_metal() -> bool;

        context() = default;
        explicit context(const std::function<auto()->void>& callback);
        ~context() = default;

        [[nodiscard]] inline auto device() const -> MTLDeviceRef { return m_metal.device; };

        auto enable_imgui() -> void override;
        auto disable_imgui() -> void override;
        [[nodiscard]] inline auto is_imgui_enabled() const -> bool override { return m_imgui.enabled; }

        auto create_shader_library(const std::string& name, const std::string& source) -> void override;
        auto create_shader_library(const std::string& name, const std::string& vertex_function, const std::string& fragment_function) -> std::shared_ptr<renderer::shader::program> override;
        auto add_shader_program(const std::string& name, const std::string& vertex_function_name, const std::string& fragment_function_name) -> std::shared_ptr<renderer::shader::program> override;
        auto shader_program(const std::string& name) -> std::shared_ptr<renderer::shader::program> override;

        auto start_frame(const render_pass *pass = nullptr, bool imgui = false) -> void override;
        auto finalize_frame(const std::function<auto() -> void>& callback) -> void override;

        auto draw(const draw_buffer *buffer) -> void override;

        auto create_framebuffer(const math::size& size) -> renderer::framebuffer * override;
        auto create_texture(const graphite::data::block& data, const math::size& size) -> std::shared_ptr<graphics::texture> override;

        auto set_tick_function(const std::function<auto()->void>& callback) -> void override;
        auto tick() -> void override;

        auto set_viewport_size(const math::size& viewport_size) -> void override;
        [[nodiscard]] auto viewport_size() const -> math::size override;

        auto set_viewport_title(const std::string& title) -> void override;
        [[nodiscard]] auto viewport_title() const -> std::string override;

        [[nodiscard]] auto native_screen_size() const -> math::size override;
        auto set_fullscreen(bool f) -> void override;

    private:
        auto configure_device() -> void;

    private:
        NSWindow *m_window { nullptr };
        CAMetalLayer *m_output_layer { nullptr };

        struct {
            MTLDeviceRef device { MTLCreateSystemDefaultDevice() };
            std::unordered_map<std::string, MTLLibraryRef> libraries;
            MTLCommandQueueRef command_queue { nullptr };
            std::unordered_map<util::uid, std::shared_ptr<renderer::shader::program>> shader_programs;
            std::uint32_t viewport_width { 1280 };
            std::uint32_t viewport_height { 720 };
            bool fullscreen { false };
        } m_metal;

        struct {
            std::array<renderer::swap_chain *, constants::swap_count> passes;
            renderer::swap_chain *ptr { nullptr };
            dispatch_semaphore_t sema { dispatch_semaphore_create(constants::swap_count) };
            std::uint8_t index { 0 };
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
