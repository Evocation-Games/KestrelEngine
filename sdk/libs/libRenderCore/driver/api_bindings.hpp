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
#include <libMath/types/vec2.hpp>
#include <libRenderCore/callback.hpp>
#include <libRenderCore/buffer/buffer.hpp>
#include <libRenderCore/frame/frame.hpp>
#include <libRenderCore/texture/store.hpp>
#include <libRenderCore/event/event_controller.hpp>
#include <libData/block.hpp>

namespace renderer::api
{
    struct bindings
    {
        struct {
            std::function<auto(renderer::callback)->void> start;
        } core;

        struct {
            std::function<auto(const std::string&)->void> set_viewport_title;
            std::function<auto()->std::string> viewport_title;
            std::function<auto(std::uint32_t, std::uint32_t)->void> set_viewport_size;
            std::function<auto()->math::vec2> viewport_size;
        } configuration;

        struct {
            std::function<auto(const buffer&)->void> submit_draw_buffer;
            std::function<auto(renderer::callback)->void> finish;
        } frame_generation;

        struct {
            std::function<auto(event::controller&)->void> attach_event_controller;
        } delegate;

        struct {
            std::function<auto(const data::block&, math::vec2)->texture::device_id> create;
            std::function<auto(texture::device_id, const data::block&)->void> update;
            std::function<auto(texture::device_id)->void> destroy;
        } texture;
    };
}
