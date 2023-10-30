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

#include <string>
#include <libRenderCore/driver/api_info.hpp>
#include <libRenderCore/driver/api_bindings.hpp>
#include <libRenderCore/driver/display_configuration.hpp>
#include <libRenderCore/frame/frame.hpp>
#include <libRenderCore/texture/store.hpp>
#include <libEvent/receiver.hpp>
#include <libData/block.hpp>
#include <libMath/types/vec2.hpp>
#include <libECS/world/world.hpp>

namespace renderer
{
    class driver
    {
    public:
        using frame_request_callback = std::function<auto(frame&)->void>;

        driver();

        [[nodiscard]] inline auto driver_name() const -> std::string { return m_api.info.name(); }
        [[nodiscard]] inline auto gpu_name() const -> std::string { return m_api.info.gpu_name(); }

        auto start(frame_request_callback frame_request) -> void;

        auto set_event_receiver(::event::receiver *receiver) -> void;

        auto create_texture(texture::id id, const data::block& data, math::vec2 size) -> bool;
        auto update_texture(texture::id id, const data::block& data, math::vec2 size) -> bool;
        auto lock_texture(texture::id id) -> void;
        auto unlock_texture(texture::id id) -> void;
        auto upload_texture(texture::id id) -> void;
        [[nodiscard]] auto device_texture_id(texture::id id) const -> texture::device_id;

        auto draw(const buffer& buffer) const -> void;

    private:
        auto initialize_backend_driver() -> void;
        auto start_driver(frame_request_callback frame_request) -> void;

        auto start_frame() -> void;
        auto end_frame() -> void;

    private:
        struct {
            void *backend { nullptr };
            api_info info;
            api::bindings bindings;
            ::event::receiver *event_receiver { nullptr };
        } m_api;

        struct {
            display_configuration display;
        } m_config;

        struct {
            texture::store texture;
        } m_store;

        struct {
            frame frame { nullptr };
            frame_request_callback frame_request;
        } m_render;

    };
}