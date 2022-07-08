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

#include <memory>
#include <functional>
#include "core/graphics/common/texture.hpp"

namespace renderer
{
    class draw_buffer;

    // MARK: - Abstract Render Pass Base

    struct render_pass
    {
    public:
        render_pass() = default;

        virtual auto start() -> void = 0;
        virtual auto finalize(const std::function<auto()->void>& callback) -> void = 0;

        virtual auto start_imgui() -> void = 0;
        virtual auto finalize_imgui() -> void = 0;

        virtual auto draw(const draw_buffer *buffer) -> void = 0;

        [[nodiscard]] virtual auto outputs_to_texture() const -> bool = 0;
    };

    // MARK: - Abstract Framebuffer

    struct framebuffer : public render_pass
    {
    public:

        framebuffer(uint32_t width, uint32_t height) : m_width(width), m_height(height) {};

        [[nodiscard]] inline auto width() const -> uint32_t { return m_width; }
        [[nodiscard]] inline auto height() const -> uint32_t { return m_height; }

        [[nodiscard]] auto outputs_to_texture() const -> bool override { return true; }

        [[nodiscard]] virtual auto texture() -> std::shared_ptr<graphics::texture> = 0;

        virtual auto invalidate() -> void = 0;

    protected:
        uint32_t m_width { 0 };
        uint32_t m_height { 0 };
    };

    // MARK: - Abstract Swap Chain

    struct swap_chain : public render_pass
    {
    public:

        [[nodiscard]] auto outputs_to_texture() const -> bool override { return false; }

    };

}